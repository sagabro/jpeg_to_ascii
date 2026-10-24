#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const char SCALE[] = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";
const int SCALE_LEN = 69;

int Kernel[3][3] = {
    {0,-1,0},
    {-1,5,-1},
    {0,-1,0},
};


int applykernelsharpen(unsigned char* data, int img_width, int img_height, int x, int y, int channel) {
    int sum = 0;

    for (int k_row = 0; k_row < 3; k_row++) {
        for (int k_col = 0; k_col < 3; k_col++){

            int neighbor_x = x + (k_col - 1);
            int neighbor_y = y + (k_row - 1);

            if (neighbor_x < 0) neighbor_x = 0;
            if (neighbor_y < 0) neighbor_y = 0;
            if (neighbor_x >= img_width) neighbor_x = img_width - 1;
            if (neighbor_y >= img_height) neighbor_y = img_height - 1;

            int neighbor_index = (neighbor_y * img_width + neighbor_x) * 3 + channel;

            sum += Kernel[k_col][k_row] * data[neighbor_index];
        }
    }
    if (sum > 255) sum = 255;
    if (sum < 0) sum = 0;

    return sum;
}

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


            int r = applykernelsharpen(data, img_width, img_height, orig_x, orig_y, 0);
            int g = applykernelsharpen(data, img_width, img_height, orig_x, orig_y, 1);
            int b = applykernelsharpen(data, img_width, img_height, orig_x, orig_y, 2);

            int brightness = (int)(0.299 * r + 0.587 * g + 0.114 * b);

            int char_index = brightness * (SCALE_LEN - 1) / 255;

            putchar(SCALE[char_index]);
        }
        putchar('\n');
    }

    stbi_image_free(data);
    return 0;
}
