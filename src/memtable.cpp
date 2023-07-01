#include "memtable.hpp"
#include "bfdev/include/bfdev/skiplist.h"
#include "bfdev/skiplist.h"
#include "serializer.hpp"
#include <cstddef>
#include <cassert>
#include <cstring>
#include <stdint.h>

namespace bfdb {

    long memtable::mtable_node_insert_cmp(const void *a, const void *b) {
        mtable_node_t *nodea = (mtable_node_t *)a, *nodeb = (mtable_node_t *)b;
        int result;

        result = strcmp(nodea->key, nodeb->key);
        if (result)
            return result;

        return nodea->sequence - nodeb->sequence;
    }

    long memtable::mtable_node_find_cmp(const void *node, const void *key) {
        return strcmp(((mtable_node_t *)node)->key, (const char *)key);
    }

    void memtable::mtable_node_free(void *p) {
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

        node->key_size = key.size();

        node->key = (char *)::malloc(node->key_size + 1);
        if (node->key == NULL) {
            return BFDB_ERR;
        }

        ::memcpy(node->key, key.data(), key.size());
        node->key[node->key_size] = '\0';

        node->sequence = sequence;
        node->type = type;

        node->value_size = value.size();

        if (node->value_size == 0) {
            node->value = NULL;
            return BFDB_OK;
        }

        node->value = (char *)::malloc(node->value_size + 1);
        if (node->value == NULL) {
            return BFDB_ERR;
        }

        ::memcpy(node->value, value.data(), value.size());
        node->value[node->value_size] = '\0';

        return BFDB_OK;
    }

    int memtable::put(const std::string &key, const std::string &value, uint64_t sequence, uint8_t type) {
        mtable_node_t *mnode;

        if (mtable_node_build(mnode, key, value, sequence, type) != BFDB_OK) {
            return BFDB_ERR;
        }


        if (bfdev_skiplist_insert(table, (void *)mnode, mtable_node_insert_cmp) != BFDEV_ENOERR) {
            return BFDB_ERR;
        }

        return BFDB_OK;
    }

    struct bfdev_skip_node* memtable::skiplist_find_max(struct bfdev_skip_node *node, uint64_t sequence) {
        struct bfdev_skip_node *walk, *result = node;

        walk = node;
        bfdev_skiplist_for_each_continue(walk, table, 0) {
            if (strcmp(((mtable_node_t *)walk->pdata)->key,
                       ((mtable_node_t *)node->pdata)->key) != 0) {
                break;
            }

            if (((mtable_node_t *)walk->pdata)->sequence > sequence) {
                break;
            }

            // k: a b -> NULL
            // s: 2 1

            result = walk;
        }

        return result;
    }

    struct bfdev_skip_node* memtable::skiplist_prev_find_max(struct bfdev_skip_node *node, uint64_t sequence) {
        struct bfdev_skip_node *walk, *result = NULL;

        return NULL;

        walk = node;
        bfdev_skiplist_for_each_reverse_from(walk, table, 0) {
            if (strcmp(((mtable_node_t *)walk->pdata)->key,
                       ((mtable_node_t *)node->pdata)->key) != 0) {
                break;
            }

            if (((mtable_node_t *)walk->pdata)->sequence < sequence) {
                break;
            }

            if (((mtable_node_t *)walk->pdata)->sequence == sequence) {
                result = walk;
                break;
            }
        }

        return result;

    }

    int memtable::get(const std::string &key, std::string &value, uint64_t sequence) {
        struct bfdev_skip_node *node, *next_max, *prev_max, *max;
        mtable_node_t *result = NULL;
        uint64_t next_max_seq = 0, prev_max_seq = 0;
        value.clear();

        node = (struct bfdev_skip_node *)bfdev_skiplist_find(table, (void *)key.data(), mtable_node_find_cmp);
        if (node == NULL) {
            return BFDB_ERR;
        }

        if (((mtable_node_t *)node->pdata)->sequence == sequence) {
            max = node;
        }else {
            next_max = skiplist_find_max(node, sequence);
            if (next_max) {
                next_max_seq = ((mtable_node_t *)next_max->pdata)->sequence;
            }

            prev_max = skiplist_prev_find_max(node, sequence);
            if (prev_max) {
                prev_max_seq = ((mtable_node_t *)prev_max->pdata)->sequence;
            }

            max = next_max_seq > prev_max_seq ? next_max : prev_max;
        }

        result = (mtable_node_t *)max->pdata;

        if (result->type == MTABLE_PUT_DELETE) {
            assert(value.size() == 0);
            return BFDB_OK;
        }

        assert(result->type == MTABLE_PUT_INSERT);
        value.assign(result->value, (size_t)result->value_size);

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
