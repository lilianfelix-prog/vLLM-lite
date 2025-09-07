// draft for PageAttention
// similar to llama-batch.h 
// planning memory management
#include <cstdint>
#include <vector>
#include <memory>
#include <unordered_map>
#include <deque>
#include <set>

struct block {
    uint32_t block_id;
    uint32_t ref_count;
    std::vector<uint8_t> hash;
    std::vector<uint32_t> token_ids;
    
    int init(uint32_t block_id) {
        hash = std::vector<uint8_t>();
        token_ids = std::vector<uint32_t>();
        block_id = block_id;
        ref_count = 0;
        return 0;
    };
    int update(std::vector<uint8_t> n_hash, std::vector<uint32_t> n_token_ids) {
        hash = n_hash;
        token_ids = n_token_ids;
        return 0;
    };
    int reset() {
        ref_count = 1;
        hash = std::vector<uint8_t>();
        token_ids = std::vector<uint32_t>();
        return 0;
    }
};

class sequence {

};

class block_manager {
    public:
        block_manager(uint32_t num_blocks, uint32_t block_size);
        
        static std::vector<uint8_t> compute_hash(std::vector<uint32_t> n_token_ids, uint8_t prefix);

        int allocate(sequence seq);
        int deallocate(sequence seq);
        bool can_allocate(sequence seq);
        bool can_append(sequence seq);
        int may_append(sequence seq);


    private:
        block allocate_block(uint32_t block_id);
        block deallocate_block(uint32_t block_id);

        using hash_t = std::vector<uint8_t>;

        uint32_t block_size;
        std::vector<block> blocks;
        std::unordered_map<hash_t, uint32_t> hash_to_block_ids;
        std::deque<uint32_t> free_block_ids;
        std::set<uint32_t> used_block_ids;

};

