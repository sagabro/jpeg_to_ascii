#include <stdio.h>
#include <stdlib.h>

// This macro tells stb_image to create the actual implementation code here
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Define our brightness-to-character ramp (from darkest/densest to lightest)
// Feel free to tweak this string to change the texture of your art
const char SCALE[] = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";
const int SCALE_LEN = 69; // Number of characters in the scale above

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: %s <path_to_jpeg> [target_width]\n", argv[0]);
        return 1;
    }

    char* filename = argv[1];
    int target_width = 100; // Default output width in characters
    if (argc >= 3) {
        target_width = atoi(argv[2]);
    }

    int img_width, img_height, img_channels;

    // Load the image. Forcing 3 channels ensures we get a predictable RGB array
    unsigned char* data = stbi_load(filename, &img_width, &img_height, &img_channels, 3);
    if (data == NULL) {
        printf("Error: Could not load or find image file '%s'.\n", filename);
        return 1;
    }

    // Characters are taller than they are wide. To prevent the ASCII art from
    // looking vertically stretched, we apply an aspect ratio correction factor (~0.45).
    double aspect_ratio = (double)img_height / (double)img_width;
    int target_height = (int)(target_width * aspect_ratio * 0.45);
    if (target_height < 1) target_height = 1;

    // Loop through every character row and column in the output grid
    for (int y = 0; y < target_height; y++) {
        for (int x = 0; x < target_width; x++) {

            // Map the current text coordinate (x, y) back to original pixel coordinates
            int orig_x = (int)((double)x / target_width * img_width);
            int orig_y = (int)((double)y / target_height * img_height);

            // Calculate the 1D array index for the top-left byte of this pixel's RGB data
            int pixel_index = (orig_y * img_width + orig_x) * 3;

            unsigned char r = data[pixel_index];
            unsigned char g = data[pixel_index + 1];
            unsigned char b = data[pixel_index + 2];

            // Standard NTSC formula to convert RGB to Grayscale brightness (0 - 255)
            int brightness = (int)(0.299 * r + 0.587 * g + 0.114 * b);

            // Map the 0-255 brightness to our character scale index
            int char_index = (brightness * (SCALE_LEN - 1)) / 255;

            // Print the corresponding character
            putchar(SCALE[char_index]);
        }
        // Move to the next line of text
        putchar('\n');
    }

    // Clean up allocated memory
    stbi_image_free(data);
    return 0;
}
