#include "serializer.hpp"
#include "db.hpp"

namespace bfdb {
    
        int serializer::serialize(std::string &dst, uint32_t i) {
            //FIXME: 
            dst.append((char *)&i, sizeof(uint32_t));
            return BFDB_OK;
        }

        int serializer::serialize(std::string &dst, uint64_t i) {
            //FIXME: 
            dst.append((char *)&i, sizeof(uint64_t));
            return BFDB_OK;
        }
}
