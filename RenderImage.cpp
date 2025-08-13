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
	for (size_t j = 0; j < height; j++) {
		for (size_t i = 0; i < width; i++){
			
			frameBuffer[j * width + i] = Vec3f(
			j / float(height), //Red Channel
			i / float(width), // Green channel
			0 // Blue channel set to 0 for simplicity's sake
			);
		}
	}

	std::ofstream outFrameBuffer("./out.ppm", std::ios::binary);
	outFrameBuffer << "P6\n" // P6 format for binary RGB 
		<< width << " " << height << "\n" << "255\n"; // Max color value

	//write pixel data
	for (size_t i = 0; i < width * height; ++i) {
		for (size_t j = 0; j < 3; j++) {
			outFrameBuffer << (char)(255 * frameBuffer[i][j]); // Scale to 0-255 range
		}
	}

	outFrameBuffer.close(); // Close the file after writing
}

int main() {
	render();
	std::cout << "Image rendered successfully!" << std::endl;
	return 0;
}
