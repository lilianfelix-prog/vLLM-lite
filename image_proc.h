#include <assert.h>

typedef struct image {
    int width;
    int height;
    int channels;
    float *data;
    unsigned int size;

} image_t;

void image_init(image_t *img, int w, int h, int c){
    img->width = w;
    img->height = h;
    img->channels = c;
    img->size = w * h * c;
}

float get_pixel(image_t *img, int x, int y, int c){
    assert(x >= 0 && x < img->width && y >= 0 && y < img->height && c >= 0 && c < img->channels);
    return img->data[img->channels * img->width * img->height + y * img->width + x];
}

void set_pixel(image_t *img, int x, int y, int c, float val){
    assert(x >= 0 && x < img->width && y >= 0 && y < img->height && c >= 0 && c < img->channels);
    img->data[img->channels * img->width * img->height + y * img->width + x] = val;
}

void add_pixel(image_t *img, int x, int y, int c, float val){
    assert(x >= 0 && x < img->width && y >= 0 && y < img->height && c >= 0 && c < img->channels);
    img->data[img->channels * img->width * img->height + y * img->width + x] += val;
}

void fill(image_t *img, float val){
    for(int i = 0; i < img->size; i++){
        img->data[i] = val;
    }
    // calloc the memory for img
}

int load_image(const char *filename, image_t *img);
void letterbox_image(image_t *img, int w, int h);
image_t *resize_image(image_t *img, int w, int h);