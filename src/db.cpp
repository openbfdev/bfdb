#include "db.hpp"

namespace bfdb {
    bfdb::bfdb(const std::string &name): db_name(name){

    }

    bfdb::~bfdb() {

    }

    int bfdb::open() {
        if (db_name.size() == 0) {
            return BFDB_ERR;
        }

        
        return BFDB_OK;
    }
}