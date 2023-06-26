#ifndef __DB__H__
#define __DB__H__
#include <string>
#include "dblog.hpp"
#include "memtable.hpp"
#include <cstdint>

#define BFDB_OK 0
#define BFDB_ERR -1

namespace bfdb {
    class bfdb {
        public:
            bfdb(const std::string &name);
            int open();
            int put(const std::string &key, const std::string &value);
            int get(const std::string &key);
            ~bfdb();
        private:
            class bfdb *db;
            class dblog log;
            const std::string db_name;
            class memtable mtable;
            uint64_t sequence = 0;
    };
}

#endif  /*__DB__H__*/
