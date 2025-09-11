// refactored from https://github.com/ggml-org/ggml/blob/master/examples/yolo/yolo-image.cpp
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "image_proc.h"

typedef struct image_params {
    const char *fname_in;
    const char *fname_out;
} image_params;

void resize_image(image_t *img, int w, int h)
{
    image_t resized;
    image_init(&resized, w, h, img->channels);

    image_t part;
    image_init(&part, w, img->height, img->channels);

    float w_scale = (float)(img->width - 1) / (w - 1);
    float h_scale = (float)(img->height - 1) / (h - 1);
    for (int k = 0; k < img->channels; ++k){
        for (int r = 0; r < img->height; ++r) {
            for (int c = 0; c < w; ++c) {
                float val = 0;
                if (c == w-1 || img->width == 1){
                    val = get_pixel(img, img->width-1, r, k);
                } else {
                    float sx = c*w_scale;
                    int ix = (int) sx;
                    float dx = sx - ix;
                    val = (1 - dx) * get_pixel(img, ix, r, k) + dx * get_pixel(img, ix+1, r, k);
                }
                set_pixel(&part, c, r, k, val);
            }
        }
    }
    for (int k = 0; k < img->channels; ++k){
        for (int r = 0; r < h; ++r){
            float sy = r*h_scale;
            int iy = (int) sy;
            float dy = sy - iy;
            for (int c = 0; c < w; ++c){
                float val = (1-dy) * get_pixel(&part, c, iy, k);
                set_pixel(&resized, c, r, k, val);
            }
            if (r == h-1 || img->height == 1) continue;
            for (int c = 0; c < w; ++c){
                float val = dy * get_pixel(&part, c, iy+1, k);
                add_pixel(&resized, c, r, k, val);
            }
        }
    }

    image_t *garbage = img;

    img = &resized;

    free_image(garbage);
    free_image(&part);
}

void embed_image(image_t *source, image_t *dest, int dx, int dy)
{
    for (int k = 0; k < source->channels; ++k) {
        for (int y = 0; y < source->height; ++y) {
            for (int x = 0; x < source->width; ++x) {
                float val = get_pixel(source, x, y, k);
                set_pixel(dest, dx+x, dy+y, k, val);
            }
        }
    }
}


void letterbox_image(image_t *img, int w, int h){
    int new_w = img->width;
    int new_h = img->height;

    if (((float)w/img->width) < ((float)h/img->height)) {
        new_w = w;
        new_h = (img->height * w)/img->width;
    } else {
        new_h = h;
        new_w = (img->width * h)/img->height;
    }

    resize_image(img, new_w, new_h);

    image_t boxed;
    image_init(&boxed, w, h, img->channels); 

    embed_image(img, &boxed, (w-new_w)/2, (h-new_h)/2);

    image_t *garbage = img;
    img = &boxed;

    free_image(garbage);
}

int load_image(const char *filename_in, image_t *img){
    int w, h, c;
    uint8_t * data = stbi_load(filename_in, &w, &h, &c, 3);
    if (!data) {
        return 0;
    }
    image_init(img, w, h, c);
    for (int k = 0; k < c; ++k){
        for (int j = 0; j < h; ++j){
            for (int i = 0; i < w; ++i){
                int dst_index = i + w*j + w*h*k;
                int src_index = k + c*i + c*w*j;
                img->data[dst_index] = (float)data[src_index]/255.;
            }
        }
    }
    stbi_image_free(data);
    return 1;
}

int output_image(const char *filename_out, image_t *img, int quality){
    
    uint8_t *data = (uint8_t*)calloc(img->width * img->height * img->channels, sizeof(uint8_t));
    for(int c = 0; c < img->channels; c++){
        for(int i = 0; i < img->width*img->height; i++){
            data[i*img->channels+c] = (uint8_t) (255*img->data[c*img->width*img->height + i]);
        }
    }
    int success = stbi_write_jpg(filename_out, img->width, img->height, img->channels, data, quality);
    free(data);
    if(!success){
        return 0;
    }
    return 1;
}

int main(int argc, char *argv[]){

    image_params *params;
    params->fname_in = "../input.jpg";
    params->fname_out = "output.jpg";
    image_t img;
    if (!load_image(params->fname_in, &img)) {
        fprintf(stderr, "%s: failed to load image from '%s'\n", __func__, params->fname_in);
        return 1;
    }

    letterbox_image(&img, 256, 256); 

    if (!output_image(params->fname_out, &img, 80)) {
        fprintf(stderr, "%s: failed to save image to '%s'\n", __func__, params->fname_out);
        return 1;
    }
    free_image(&img);

}