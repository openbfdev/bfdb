#ifndef __SERIALIZE__H__
#define __SERIALIZE__H__

#include <string>
#include "db.hpp"

namespace bfdb {
    class serializer {
        public:
            static int serialize(std::string &dst, uint32_t i);
            static int serialize(std::string &dst, uint64_t i);
            static int serialize(std::string &dst, const char *data);
    };
}


#endif  /*__SERIALIZE__H__*/