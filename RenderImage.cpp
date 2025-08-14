#include "geometry.h"
#include<iostream>
#include <vector>
#include <fstream>
#include <cmath>
/*
an intuitive way to save a gradient image to disk
Steps go as follows:
 - Create FrameBuffer object
 - Fill each pixel with RGB value based on (x,y) coordinates
		*NOTE: (x,y) coordinates are in the range [0,1] for each pixel and are
		* mapped to a 1D array of size width * height
		* This is for file format .ppm expecting all pixels to be laid in a single continuous block.

- Write Image to file
		* Open file in binary mode
		* Write header information (P6, width, height, max color value)
		* Write pixel data as RGB values
		* Close file
*/
void render() {
	const int height = 1080; //my native screen and image resolution
	const int width = 1920;
	std::vector<Vec3f> frameBuffer(width * height); // declare 1d framebuffer vector (hole RGB respective to slots) 

	// Initialize the framebuffer with a gradient based on pixel coordinates
	for (size_t i = 0; i < width; i++) {
		for (size_t j = 0; j < height; j++) {
			frameBuffer[j * width + i] = Vec3f(
				i / float(width), // Red Channel
				j / float(height), // Green Channel
				0 // Blue channel 0 for simplicity sake. 
			);
		}
	}
	// P6 format for binary RGB 
	// 225 is written to ensure Max color value
	//write pixel data
	std::ofstream ImageOut; 
	ImageOut.open("./gradient.ppm", std::ios::binary);

	//P6 indicates that image data is stored in binary format, raw binary rep. of pixel colors. 
	ImageOut << "P6\n" << width << " " << height << "\n255\n";
	for (size_t i = 0; i < (height * width); ++i) {
		for (size_t j = 0; j < 3; j++) {
			ImageOut << (char)(255 * frameBuffer[i][j]); // Multiply by 255 to get an 8 bit value. Cast to char. 
	}
}
	outFrameBuffer.close(); // Close the file after writing
}

int main() {
	render();
	std::cout << "Image rendered successfully!" << std::endl;
	return 0;
}
