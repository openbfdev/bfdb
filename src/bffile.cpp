#include "bffile.hpp"
#include "db.hpp"

#include <cassert>
#include <cstddef>
#include <cstdio>
#include <unistd.h>

namespace bfdb {
    bffile:: ~bffile() {
        close(fd);
    };

    int bffile::open(const std::string file) {
        fd = ::open(file.c_str(), (O_WRONLY|O_APPEND)|O_CREAT, 0644);
        if (fd == -1) {
            perror("");
            return BFDB_ERR;
        }
        return BFDB_OK;
    }

    int bffile::write(const std::string content) {
        return write(content.data(), content.length());
    }

    int bffile::write(uint8_t b) {
        return write((const char *)&b, sizeof(uint8_t));
    }

    int bffile::write(uint32_t i) {
        return write((const char *)&i, sizeof(uint32_t));
    }

    int bffile::write(const char *data, size_t size) {
        size_t n;
        for (;;) {
            if (size == 0) {
                break;
            }
            
            n = ::write(fd, data, size);
            if (n < 0) {
                if (errno == EINTR) {
                    continue;
                }
                return BFDB_ERR;
            }

            data += n;
            size -= n;
        }

        return BFDB_OK;
    }

    int bffile::read(char *buf, size_t size) {
        size_t n;
        n = ::read(fd, buf, size);
        if (n == -1) {
            return BFDB_ERR;
        }

        assert(size >= n);

        sys_offset += n;
        return BFDB_OK;
    }


    int bffile::read(char *buf, size_t size, off_t offset) {
        if (sys_offset != offset) {
            if (::lseek(fd, offset, SEEK_SET) == -1) {
                return BFDB_ERR;
            }
            
            sys_offset = offset;
        }

        return read(buf, size);
    }

    int bffile::fsync() {
        return ::fsync(fd);
    }
}