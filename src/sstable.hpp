#ifndef __SSTABLE__H__
#define __SSTABLE__H__

#include <cstddef>
#include <memory>
#include <string>
#include <vector>
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
            int block_append_trailer(compression_type type);
            int prepare_flush();
            int block_compress(compression_type &type);
            int put(std::string &key, std::string &value);
            size_t block_size();
        private:
            uint32_t crc;
            //compress type
            enum compression_type ctype = NO_COMPRESSION;
        
        protected:
            //shared ptr
            std::string buffer;
            std::vector<uint32_t> restarts;
        };

        // data block
        class data_block :block {
        public:
            int put(std::string &key, std::string &value);
            size_t block_size();
        private:
        };


        class filter_block :block {

        };

        class index_block :block {

        };

        //meta index
        class mindex_block :block {

        };

        class sstable {
            public:
                int put(std::string &key, std::string &value);
                int flush_data_block();
                int flush();
                int flush_readable();
            private:
                std::string filename;
                //FIXME: use this 
                // std::shared_ptr<std::string> filename;
                bffile file;

                data_block dblock;
                filter_block filter_block;
                mindex_block mindex_block;
                index_block  index_block;

        };
    

    
    };
    
}

#endif  /*__SSTABLE__H__*/