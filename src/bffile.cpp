#include "bffile.hpp"
#include "db.hpp"

#include <cstddef>
#include <unistd.h>
namespace bfdb {
    bffile:: ~bffile() {
        close(fd);
    };

    int bffile::open(const std::string file) {
        fd = ::open(file.c_str(), 0);
        if (fd == -1) {
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

    int bffile::fsync() {
        return ::fsync(fd);
    }
}