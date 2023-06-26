#include "db.hpp"
#include <cstdio>
#include <string>
#include "dblog.hpp"
#include <sys/stat.h>

namespace bfdb {
    bfdb::bfdb(const std::string &name): db_name(name){

    }

    bfdb::~bfdb() {

    }

    int bfdb::open() {
        if (db_name.size() == 0) {
            return BFDB_ERR;
        }

        if (::mkdir(db_name.c_str(), 0755) != 0) {
            // perror("");
            //ignore error
        }

        if (log.open(db_name, 0) != BFDB_OK) {
            return BFDB_ERR;
        }

        return BFDB_OK;
    }


    int bfdb::put(const std::string &key, const std::string &value) {

        if (log.put(key, value) != BFDB_OK) {
            return BFDB_ERR;
        }

        return BFDB_OK;
    }
}
