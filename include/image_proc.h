#ifndef IMAGE_H
#define IMAGE_H



typedef struct image {
    int width;
    int height;
    int channels;
    float *data;
    unsigned int size;

} image_t;

typedef struct image_params {
    const char *fname_in;
    const char *fname_out;
} image_params_t;

image_params_t *params_init(image_params_t *params, char *file_in, char *file_out);

void image_init(image_t *img, int w, int h, int c);

float get_pixel(image_t *img, int x, int y, int c);

void set_pixel(image_t *img, int x, int y, int c, float val);

void add_pixel(image_t *img, int x, int y, int c, float val);

void fill(image_t *img, float val);

void free_image(image_t *img);

void copy_image(image_t *src, image_t *dest);

int load_image(const char *filename, image_t *img);

void letterbox_image(image_t *img, int w, int h);

void resize_image(image_t *img, int w, int h);

#endif