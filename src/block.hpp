#ifndef __BLOCK__H__
#define __BLOCK__H__

#include "db.hpp"

namespace bfdb {
    namespace sstable {
        static const size_t BLOCK_TRAILER_SIZE = 
            1  //compression type 
            + 4; //crc

        enum compression_type {
            NO_COMPRESSION,
            SNAPPY_COMPRESSION,
            ZSTD_COMPRESSION,
        };


        class block {
            struct compression_block {
                typedef int (*compression_pt)(std::string &in);
                enum compression_type type;
                compression_pt handler;
            };
        
            int static compression_no(std::string &in) {
                return BFDB_OK;
            }

            int static compression_snappy(std::string &in) {
                //FIXME: support
                return BFDB_ERR;
            }

            int static compression_zstd(std::string &in) {
                //FIXME: support
                return BFDB_ERR;
            }

            static constexpr compression_block cb_table[] = {
                {
                    .type = NO_COMPRESSION,
                    .handler = compression_no,
                },
                {
                    .type = SNAPPY_COMPRESSION,
                    .handler = compression_snappy,
                },
                {
                    .type = ZSTD_COMPRESSION,
                    .handler = compression_zstd,
                },
            };
        public:
            int block_flush_restart_pointer();
            int block_compress(compression_type &type);
            int block_append_trailer(compression_type type);
            
            int prepare_flush();
            int put(std::string &key, std::string &value);
            size_t block_size();
            int reset() {
                buffer.clear();
                restarts.clear();
                //do we need this ??? FIXME: remove this
                restarts.push_back(0);
                return BFDB_OK;
            }
        private:
            uint32_t crc;
            //compress type
            enum compression_type ctype = NO_COMPRESSION;
        
        protected:
            //shared ptr
            std::string buffer;
            std::vector<uint32_t> restarts;
        };

    }
}


#endif  /*__BLOCK__H__*/