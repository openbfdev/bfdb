#ifndef __MEMTABLE__H__
#define __MEMTABLE__H__

#include <string>
#include "db.hpp"
#include "bfdev/skiplist.h"

namespace bfdb {
    class memtable {
        public:
            memtable();
            ~memtable();
            int get(const std::string &key, std::string &value, uint64_t sequence);
            int put(const std::string &key, const std::string &value);
            int put(const std::string &key, const std::string &value, uint64_t sequence, uint8_t type);
        private:
            typedef struct mtable_node_s {
                uint32_t key_size;
                char    *key;
                uint64_t sequence;
                uint8_t  type;

                uint32_t value_size;
                char    *value;
            }mtable_node_t;
            int mtable_node_build(mtable_node_t* &node, const std::string &key, const std::string &value, uint64_t sequence, uint8_t type);
            static long mtable_node_cmp(const void *ap, const void *bp);
            static void mtable_node_free(void *p);

            struct bfdev_skip_head *table;
    };
}

#endif  /*__MEMTABLE__H__*/
