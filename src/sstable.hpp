#ifndef __SSTABLE__H__
#define __SSTABLE__H__

#include <_types/_uint32_t.h>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "db.hpp"
#include "block.hpp"

namespace bfdb {
    namespace sstable {
        struct block_range {
            block_range(uint32_t offset, uint32_t size): offset(offset), size(size) {

            }

            uint32_t offset;
            uint32_t size;
        };
        #define MAX_BLOCK_SIZE (4 * 1024)

        // data block
        class data_block :block {
        public:
            int put(std::string &key, std::string &value) {
                //save the key to max key
                max_key = key;
                return block::put(key, value);
            }
            
            int prepare_flush() {
                //TODO: somthing other
                return block::prepare_flush();
            }

            size_t block_size() {
                return block::block_size();
            }

            const std::string& get_max_key() {
                return max_key;
            }

            const std::string& get_buffer() {
                return buffer;
            }

            int reset() {
                max_key.clear();
                return block::reset();
            }

        private:
            std::string max_key;
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
                int flush_index_block();
                int flush_meta_index_block();
                int flush_filter_block();
                int flush();
                bool need_flush_data_block() {
                    
                    if (data_block.block_size() < MAX_BLOCK_SIZE) {
                        return false;
                    }

                    return true;
                }

                int flush_readable();
            private:
                std::string filename;
                //FIXME: use this 
                // std::shared_ptr<std::string> filename;
                bffile file;

                //each time just have one datablock for save memory
                data_block data_block;
                
                //each time just have one filter_block for save memory
                filter_block filter_block;

                mindex_block mindex_block;
                index_block  index_block;

                //FIXME: remove data_block_offset
                uint32_t data_block_offset = 0;
                std::vector<std::pair<std::string, block_range>> dbs;


        };
    

    
    };
    
}

#endif  /*__SSTABLE__H__*/