#include "serializer.hpp"
#include "db.hpp"

namespace bfdb {
    
        int serializer::serialize(std::string &dst, uint32_t i) {
            //FIXME: use Major disjunction sequence
            dst.append((char *)&i, sizeof(uint32_t));
            return BFDB_OK;
        }

        int serializer::serialize(std::string &dst, uint64_t i) {
            //FIXME: use Major disjunction sequence 
            dst.append((char *)&i, sizeof(uint64_t));
            return BFDB_OK;
        }

        int serializer::serialize(std::string &dst, const char *data) {
            //do nothing, just append data
            dst.append(data);
            return BFDB_OK;
        }
}
