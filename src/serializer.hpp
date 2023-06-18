#ifndef __SERIALIZE__H__
#define __SERIALIZE__H__

#include <string>
#include "db.hpp"

namespace bfdb {
    class serializer {
        public:
            int serialize(std::string &dst, uint32_t i);
            int serialize(std::string &dst, uint64_t i);

    };
}


#endif  /*__SERIALIZE__H__*/