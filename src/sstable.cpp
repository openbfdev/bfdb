#include "sstable.hpp"
#include "db.hpp"
#include "serializer.hpp"
#include <utility>

namespace bfdb {
    namespace sstable {


        int data_block::put(std::string &key, std::string &value) {
            //save the key to max key
            max_key = key;
            return block::put(key, value);
        }

        size_t data_block::block_size() {
            return block::block_size();
        }

        int data_block::prepare_flush() {
            //TODO: somthing other
            return block::prepare_flush();
        }

        int sstable::flush_data_block() {
            if (data_block.prepare_flush() != BFDB_OK) {
                return BFDB_ERR;
            }

            //FIXME: remove data_block_offset
            dbs.push_back(
                std::pair<std::string, block_range>(
                    data_block.get_max_key(), 
                    block_range(data_block_offset, data_block.get_buffer().size())
                )
            );
            
            data_block_offset += data_block.get_buffer().size();

            //for next data block use..
            return data_block.reset();

        }

        int sstable::flush_index_block() {
            
            return BFDB_OK;
        }

        int sstable::flush_meta_index_block() {
            
            return BFDB_OK;
        }

        int sstable::flush_filter_block() {
            
            return BFDB_OK;
        }

        int sstable::flush() {
            if (flush_data_block() != BFDB_OK) {
                return BFDB_ERR;
            }

            if (flush_filter_block() != BFDB_OK) {
                return BFDB_ERR;
            }

            if (flush_meta_index_block() != BFDB_OK) {
                return BFDB_ERR;
            }

            if (flush_index_block() != BFDB_OK) {
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
           
            if (data_block.put(key, value) != BFDB_OK) {
                return BFDB_ERR;
            }
            
            if (!need_flush_data_block()) {
                return BFDB_OK;
            }
            
            if (flush_data_block() != BFDB_OK) {
                return BFDB_ERR;
            }

            return BFDB_OK;
        }

    }
}