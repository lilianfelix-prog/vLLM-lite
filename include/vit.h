// refactored from https://github.com/staghado/vit.cpp/blob/main/vit.h
#include <stdlib.h> 

struct vit_hparams
{
    int32_t hidden_size = 768;
    int32_t num_hidden_layers = 12;
    int32_t num_attention_heads = 12;
    int32_t num_classes = 1000;
    int32_t patch_size = 8;
    int32_t img_size = 224;
    int32_t ftype = 1;
    float eps = 1e-6f;
    const char *interpolation = "bicubic";
    struct id2label_map {
        int key;
        char *id2label;
    };
    id2label_map *id2labels;

    int32_t n_enc_head_dim() const;
    int32_t n_img_size() const;
    int32_t n_patch_size() const;
    int32_t n_img_embd() const;
};

struct vit_block {
    struct ggml_tensor *norm1_w;
    struct ggml_tensor *norm1_b;
    struct ggml_tensor *qkv_w;
    struct ggml_tensor *qkv_b;
    struct ggml_tensor *proj_w;
    struct ggml_tensor *proj_b;
    struct ggml_tensor *norm2_w;
    struct ggml_tensor *norm2_b;
    struct ggml_tensor *mlp_lin1_w;
    struct ggml_tensor *mlp_lin1_b;
    struct ggml_tensor *mlp_lin2_w;
    struct ggml_tensor *mlp_lin2_b;
};

struct classifier_head
{
    struct ggml_tensor *norm_w;
    struct ggml_tensor *norm_b;
    struct ggml_tensor *head_w;
    struct ggml_tensor *head_b;
};

struct vit_image_encoder{
    struct ggml_tensor *pe;
    struct ggml_tensor *cls_token;
    struct ggml_tensor *proj_w;
    struct ggml_tensor *proj_b;
    u_int32_t num_block;
    struct vit_block *layers;
};

typedef struct vit_params {
    int32_t seed = -1;
    int32_t n_threads = 4;
    int32_t topk = 5;
    const char *model = "../ggml-model-f16.gguf";  // model path
    const char *fname_inp = "../assets/tench.jpg"; // image path
    float eps = 1e-6f;                             // epsilon used in LN
} vit_params_t;

struct vit_model {
    vit_hparams hparams;
    vit_image_encoder enc_img;
    classifier_head classifier;
    struct ggml_context *ctx;

    int32_t map_size;
    struct tensor_map {
        char *key;
        struct ggml_tensor *tensor;
    };
    struct tensor_map *tensors;

};

bool vit_model_load(const char *fname, vit_model *model);