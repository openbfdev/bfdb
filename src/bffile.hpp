#ifndef __BFFILE__H__
#define __BFFILE__H__
#include <string>
#include <fcntl.h>
#include <cstdint>

namespace bfdb {
    class bffile {
        public:
            int open(const std::string file);
            int write(const std::string content);
            int write(const char *data, size_t size);
            int write(uint32_t i);
            int write(uint8_t b);
            int read(char *buf, size_t size);
            int read(char *buf, size_t size, off_t offset);
            int fsync();
            ~bffile();
        private:
            const std::string name;
            int fd;
            off_t sys_offset;
    };
}

#endif  /*__BFFILE__H__*/
