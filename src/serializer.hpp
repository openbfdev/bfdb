#ifndef __SERIALIZE__H__
#define __SERIALIZE__H__

#include <string>
#include "db.hpp"

namespace bfdb {
    class serializer {
        public:
            static int serialize(std::string &dst, uint8_t b);
            static int serialize(std::string &dst, uint32_t i);
            static int serialize(std::string &dst, uint64_t l);
            static int serialize(std::string &dst, const char *data);

            static int serialize(char *p, uint8_t b);
            static int serialize(char *p, uint32_t i);
            static int serialize(char *p, uint64_t l);
            static int serialize(char *p, const char *data, size_t size);

            static int deserialize(const char *data, size_t size, uint8_t &b);
            static int deserialize(const char *data, size_t size, uint32_t &i);
            static int deserialize(const char *data, size_t size, uint64_t &l);
            static int deserialize(const char *data, size_t size, std::string &s);
            static int deserialize(const char *data, size_t size, char* &s);
    };
}


#endif  /*__SERIALIZE__H__*/
