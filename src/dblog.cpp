#include "dblog.hpp"
#include "db.hpp"
#include <cstddef>
#include "bffile.hpp"
#include "serializer.hpp"

namespace bfdb {

    int dblog::open(std::string db_name, uint64_t log_number) {
        if (log.open(db_name + "/" + std::to_string(log_number) + ".log") != BFDB_OK) {
            return BFDB_ERR;
        }
        return BFDB_OK;
    }

    //append record
    int dblog::append_record(enum log_record_type type, const char *data, size_t size) {
        uint32_t crc = 0;

        if (log.write(crc) != BFDB_OK) {
            return BFDB_ERR;
        }

        if (log.write((uint8_t)(size & 0xff)) != BFDB_OK) {
            return BFDB_ERR;
        }

        if (log.write((uint8_t)(size >> 8)) != BFDB_OK) {
            return BFDB_ERR;
        }

        if (log.write((uint8_t)(type)) != BFDB_OK) {
            return BFDB_ERR;
        }

        if (log.write(data, size) != BFDB_OK) {
            return BFDB_ERR;
        }

        log.fsync();

        block_offset += DBLOG_HEADER_SIZE + size;
        return BFDB_OK;
    }

    int dblog::append_records(const std::string &data) {
        const char *p = data.data();
        size_t size = data.length();
        size_t n, cap;
        log_record_type type;

        cap = DBLOG_BLOCK_SIZE - block_offset - DBLOG_HEADER_SIZE;
        if (cap >= size) {
            type = DBLOG_RECORD_FULL;
            n = size;
            return append_record(type, p, n);
        }

        type = DBLOG_RECORD_FIRST;

        for (;;) {
            if (size == 0) {
                break;
            }

            if ((DBLOG_BLOCK_SIZE - block_offset) < DBLOG_MINIMAL_DATA_SIZE) {
                //append zero
                block_offset = 0;
            }

            cap = DBLOG_BLOCK_SIZE - block_offset - DBLOG_HEADER_SIZE;
            n = std::min(cap, size);

            if (append_record(type, p, n) != BFDB_OK) {
                return BFDB_ERR;
            }

            if (n == cap) {
                type = DBLOG_RECORD_MIDDLE;
            }

            if (n == size) {
                type = DBLOG_RECORD_LAST;
            }

            p += n;
            size -= n;
        }

        return BFDB_OK;
    }

    int dblog::put(const std::string &key, const std::string &value) {
        std::string dst;

        serializer::serialize(dst, (uint32_t)key.length());
        serializer::serialize(dst, key.data());
        serializer::serialize(dst, (uint32_t)value.length());
        serializer::serialize(dst, value.data());

        return append_records(dst);
    }

}
