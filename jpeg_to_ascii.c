#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const char SCALE[] = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";
const int SCALE_LEN = 69;

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: %s <path_to_jpeg> [target_width]\n", argv[0]);
        return 1;
    }

    char* filename = argv[1];
    int target_width = 100;
    if (argc >= 3) {
        target_width = atoi(argv[2]);
    }

    int img_width, img_height, img_channels;

    unsigned char* data = stbi_load(filename, &img_width, &img_height, &img_channels, 3);
    if (data == NULL) {
        printf("Error: Could not load or find image file '%s'.\n", filename);
        return 1;
    }

    double aspect_ratio = (double)img_height / (double)img_width;
    int target_height = (int)(target_width * aspect_ratio * 0.45);
    if (target_height < 1) target_height = 1;

    for (int y = 0; y < target_height; y++) {
        for (int x = 0; x < target_width; x++) {

            int orig_x = (int)((double)x / target_width * img_width);
            int orig_y = (int)((double)y / target_height * img_height);

            int pixel_index = (orig_y * img_width + orig_x) * 3;

            unsigned char r = data[pixel_index];
            unsigned char g = data[pixel_index + 1];
            unsigned char b = data[pixel_index + 2];

            int brightness = (int)(0.299 * r + 0.587 * g + 0.114 * b);

            int char_index = brightness * (SCALE_LEN - 1) / 255;

            putchar(SCALE[char_index]);
        }
        putchar('\n');
    }

    stbi_image_free(data);
    return 0;
}
