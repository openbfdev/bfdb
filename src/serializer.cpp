#include "serializer.hpp"
#include "db.hpp"
#include <cassert>
#include <cstddef>
#include <string.h>

namespace bfdb {

        int serializer::serialize(std::string &dst, uint8_t b) {
            //FIXME: use Major disjunction sequence
            dst.append((char *)&b, sizeof(uint8_t));
            return BFDB_OK;
        }

        int serializer::serialize(std::string &dst, uint32_t i) {
            //FIXME: use Major disjunction sequence
            dst.append((char *)&i, sizeof(uint32_t));
            return BFDB_OK;
        }

        int serializer::serialize(std::string &dst, uint64_t l) {
            //FIXME: use Major disjunction sequence 
            dst.append((char *)&l, sizeof(uint64_t));
            return BFDB_OK;
        }

        int serializer::serialize(std::string &dst, const char *data) {
            //do nothing, just append data
            dst.append(data);
            return BFDB_OK;
        }

        int serializer::serialize(char *p, uint8_t b) {
            //FIXME: use Major disjunction sequence
            memcpy(p, &b, sizeof(b));
            return BFDB_OK;
        }

        int serializer::serialize(char *p, uint32_t i) {
            //FIXME: use Major disjunction sequence
            memcpy(p, &i, sizeof(i));
            return BFDB_OK;
        }

        int serializer::serialize(char *p, uint64_t l) {
            //FIXME: use Major disjunction sequence 
            memcpy(p, &l, sizeof(l));
            return BFDB_OK;
        }

        int serializer::serialize(char *p, const char *data, size_t size) {
            memcpy((void *)p, (const void *)data, size);
            return BFDB_OK;
        }

        int serializer::deserialize(const char *data, size_t size, uint8_t &b) {
            assert(size == 1);
            memcpy(&b, data, size);
            return BFDB_OK;
        }

        int serializer::deserialize(const char *data, size_t size, uint32_t &i) {
            assert(size == 4);
            memcpy(&i, data, size);
            return BFDB_OK;
        }

        int serializer::deserialize(const char *data, size_t size, uint64_t &l) {
            assert(size == 8);
            memcpy(&l, data, size);
            return BFDB_OK;
        }

        int serializer::deserialize(const char *data, size_t size, std::string &s) {
            s = {data, size};
            return BFDB_OK;
        }

        int serializer::deserialize(const char *data, size_t size, char* &s) {
            s = (char *)data;
            return BFDB_OK;
        }
}
