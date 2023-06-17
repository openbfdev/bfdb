#ifndef __DB__H__
#define __DB__H__
#include <string>

#define BFDB_OK 0
#define BFDB_ERR -1

namespace bfdb {
    class bfdb {
        public:
            bfdb(const std::string &name);
            int open();
            int put(std::string &key, std::string &value);
            int get(std::string &key);
            ~bfdb();
        private:
            class bfdb *db;
            const std::string db_name;
    };
}

#endif  /*__DB__H__*/