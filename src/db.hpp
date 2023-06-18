#ifndef __DB__H__
#define __DB__H__
#include <string>
#include "dblog.hpp"

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
    };
}

#endif  /*__DB__H__*/