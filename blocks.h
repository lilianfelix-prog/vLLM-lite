// kv cache memory management as blocks
#pragma once

#include <stdint.h>
#include <stddef.h>

// TODO: manage sequences
struct sequence;

struct block {
    uint32_t block_id;
    uint32_t ref_count;

    uint8_t  *hash;       
    size_t    hash_size;

    uint32_t *token_ids;  
    size_t    token_count;
};


int  block_init(struct block *b, uint32_t id);
int  block_update(struct block *b, const uint8_t *hash, size_t hash_size, const uint32_t *token_ids, size_t token_count);
int  block_reset(struct block *b);
void block_free(struct block *b);

// TODO: 
struct block_manager {
    // make dynamic lists for blocks
    uint32_t block_size;
    uint32_t num_blocks;
    struct block *blocks; 

    // hash to block id
    struct hash_to_block_id { uint8_t *hash; uint32_t block_id; };
    struct hash_to_block_id *hash_to_block_id;
    size_t hash_to_block_id_count;

    // queue 
    uint32_t *free_block_ids;   
    size_t    free_count;

    // set 
    uint32_t *used_block_ids;   
    size_t    used_count;

};

struct block_manager *block_manager_create(uint32_t num_blocks, uint32_t block_size);
void block_manager_destroy(struct block_manager *mgr);

int  block_manager_allocate(struct block_manager *mgr, struct sequence *seq);
static block _allocate_block(struct block_manager *mgr, uint32_t block_id); 

int  block_manager_deallocate(struct block_manager *mgr, struct sequence *seq);
static block _deallocate_block(struct block_manager *mgr, uint32_t block_id);

int  block_manager_can_allocate(struct block_manager *mgr, const struct sequence *seq);
int  block_manager_can_append(struct block_manager *mgr, const struct sequence *seq);
int  block_manager_may_append(struct block_manager *mgr, struct sequence *seq);


static int block_manager_compute_hash(const uint32_t *token_ids, size_t token_count,
                                uint8_t prefix, uint8_t **out_hash, size_t *out_size);
