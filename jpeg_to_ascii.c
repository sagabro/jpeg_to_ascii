    #include <stdio.h>
    #include <stdlib.h>

    #define STB_IMAGE_IMPLEMENTATION
    #include "stb_image.h"

    const char SCALE[] = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";
    const int scale_length = 69;


    int main(int argc, char *argv[]) {
        if (argc < 2) {
            printf("Usage: %s <image_file> <target_width>\n", argv[0]);
            return 1;
        }
        char *file = argv[1];
        int target_width = 100;
        if (argc >= 3) {
            target_width = atoi(argv[2]);
        }

        int img_height,img_width, img_channels;

        unsigned char *img_data = stbi_load(file, &img_width, &img_height, &img_channels, 3);
        if (!img_data) {
            printf("Failed to load image\n");
            return 1;
        }

        double aspect_ratio = (double)img_height / img_width;
        int target_height = (int)(target_width * aspect_ratio * .45);
        if (target_height < 1)  target_height = 1;

        for (int y = 0; y < target_height; y++) {
            for (int x = 0; x < target_width; x++) {

                int orig_x = (int)((double)x * img_width / target_width);
                int orig_y = (int)((double)y * img_height / target_height);
                if (orig_x >= img_width) orig_x = img_width - 1;
                if (orig_y >= img_height) orig_y = img_height - 1;

                int pixel_index = (orig_y * img_width + orig_x) * 3;

                unsigned char r = img_data[pixel_index];
                unsigned char g = img_data[pixel_index + 1];
                unsigned char b = img_data[pixel_index + 2];

                int brightness = (.299 * r + .587 * g + .114 * b);
                int char_index = brightness * scale_length/255;

                putchar(SCALE[char_index]);
            }
        }
        stbi_image_free(img_data);
        return 0;
    };
