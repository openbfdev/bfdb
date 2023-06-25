#include "memtable.hpp"
#include "bfdev/include/bfdev/skiplist.h"
#include "bfdev/skiplist.h"
#include "serializer.hpp"
#include <cstddef>

namespace bfdb {

    long memtable::mtable_node_cmp(const void *a, const void *b) {
        mtable_node_t *nodea = (mtable_node_t *)a , *nodeb = (mtable_node_t *)b;

        if (nodea->key > nodeb->key) {
            return 1;
        }

        if (nodea->key < nodeb->key) {
            return -1;
        }

        //akey == bkey
        if (nodea->sequence > nodeb->sequence) {
            return -1;
        }

        if (nodea->sequence < nodeb->sequence) {
            return 1;
        }

        return 0;
    }

    void memtable::mtable_node_free(const void *p) {
        mtable_node_t *node;
        node = (mtable_node_t *)p;

        ::free(node->key);
        ::free(node->value);
        ::free(node);
    }

    int memtable::mtable_node_build(mtable_node_t* &node, const std::string &key, const std::string &value, uint64_t sequence, uint8_t type) {
        node = (mtable_node_t *)malloc(sizeof(mtable_node_t));
        if (node == NULL) {
            return BFDB_ERR;
        }

        node->key_size = key.length();

        node->key = (char *)malloc(node->key_size);
        if (node->key == NULL) {
            return BFDB_ERR;
        }

        node->sequence = sequence;
        node->type = type;

        node->value_size = value.length();

        if (node->value_size == 0) {
            node->value = NULL;
            return BFDB_OK;
        }

        node->value = (char *)malloc(node->value_size);
        if (node->value == NULL) {
            return BFDB_ERR;
        }

        return BFDB_OK;
    }

    int memtable::put(const std::string &key, const std::string &value, uint64_t sequence, uint8_t type) {
        mtable_node_t *mnode;

        if (mtable_node_build(mnode, key, value, sequence, type) != BFDB_OK) {
            return BFDB_ERR;
        }


        if (bfdev_skiplist_insert(table, (void *)mnode, mtable_node_cmp) != BFDEV_ENOERR) {
            return BFDB_ERR;
        }

        return BFDB_OK;
    }

    int memtable::get(const std::string &key, std::string &value, uint64_t sequence) {
        bfdev_skip_node *node;
        const std::string dummy_value;
        uint8_t dummpy_type = 0;
        mtable_node_t *mnode, *vnode;

        if (mtable_node_build(mnode, key, dummy_value, sequence, dummpy_type) != BFDB_OK) {
            return BFDB_ERR;
        }

        node = bfdev_skiplist_find(table, (void *)mnode, mtable_node_cmp);
        if (node == NULL) {
            return BFDB_ERR;
        }
        
        vnode = (mtable_node_t *)node->pdata;

        value.assign(vnode->value, (size_t)vnode->value_size);

        return BFDB_OK;
    }

    memtable::memtable() {
        table = bfdev_skiplist_create(NULL, 6);
        assert(table);
    }

    memtable::~memtable() {
        bfdev_skiplist_destroy(table, mtable_node_free);
    }
}

