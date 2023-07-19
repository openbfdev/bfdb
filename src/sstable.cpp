#include "sstable.hpp"
#include "bfdev/crc.h"
#include "db.hpp"
#include "serializer.hpp"
#include <_types/_uint32_t.h>
#include <_types/_uint8_t.h>

namespace bfdb {
    namespace sstable {
        //this is a estimated
        size_t block::block_size() {
            return buffer.size() + BLOCK_TRAILER_SIZE + 
                    restarts.size() * sizeof(uint32_t) //all restarts
                    + sizeof(uint32_t); //restart's number
        }

        int block::block_append_trailer(compression_type type) {
            char trailer[BLOCK_TRAILER_SIZE];
            
            uint32_t crc;
            trailer[0] = type;

            crc = bfdev_crc32((const uint8_t *)buffer.data(), buffer.size(), 0);
            crc = bfdev_crc32((const uint8_t *)trailer, 1, crc);

            serializer::serialize(&trailer[1], crc);

            buffer.append(trailer);
            return BFDB_OK;
        }

        int block::block_compress(compression_type &type) {
            size_t i;
            for (i = 0; i < sizeof(cb_table) / sizeof(cb_table[0]); i++) {
                if (ctype != cb_table[i].type) {
                    continue;
                }

                if (cb_table[i].handler(buffer) != BFDB_OK) {
                    type = NO_COMPRESSION;
                    break;
                }

                type = cb_table[i].type;
                break;
            }

            return BFDB_OK;
        }

        int block::prepare_flush() {
            size_t i;
            compression_type type;

            for (i = 0; i < restarts.size(); i++) {
                serializer::serialize(buffer, restarts[i]);
            }

            serializer::serialize(buffer, (uint32_t)restarts.size());

            if (block_compress(type) != BFDB_OK) {
                return BFDB_ERR;
            }

            if (block_append_trailer(type) != BFDB_OK) {
                return BFDB_ERR;
            }

            return BFDB_OK;
        }

        int block::put(std::string &key, std::string &value) {
            size_t shared, no_shared;

            restarts.push_back(buffer.size());
            
            //TODO: set shared and no_shared by key shared for save disk usage
            shared = 0;
            no_shared = key.size() - shared;
            
            serializer::serialize(buffer, (uint32_t)shared);
            serializer::serialize(buffer, (uint32_t)no_shared);
            serializer::serialize(buffer, (uint32_t)value.size());

            serializer::serialize(buffer, key.data() + shared, no_shared);
            serializer::serialize(buffer, value.data(), value.size());
            return BFDB_OK;
        }


        int data_block::put(std::string &key, std::string &value) {
            return block::put(key, value);
        }

        size_t data_block::block_size() {
            return block::block_size();
        }


        int sstable::flush_data_block() {
            
            return BFDB_OK;
        }


        int sstable::flush() {
            if (flush_data_block() != BFDB_OK) {
                return BFDB_ERR;
            }


            return BFDB_OK;
        }

        int sstable::flush_readable() {
            // TODO:
            return BFDB_OK;
        }

        //just put them to memory, or just flash data block
        int sstable::put(std::string &key, std::string &value) {
           
            if (dblock.put(key, value) != BFDB_OK) {
                return BFDB_ERR;
            }
            
            
            if (flush_data_block() != BFDB_OK) {
                return BFDB_ERR;
            }

            return BFDB_OK;
        }



    }
}