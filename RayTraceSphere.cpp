#include "geometry.h"
#include<iostream>
#include <vector>
#include <fstream>
#include <cmath>


struct Sphere {

	Vec3f center;
	float radius;

	Sphere(const Vec3f& c, const float& rad) { //Pass These as a reference.
		//The sphere object is made up of 4 number components
		//There are 3 numbers stored within a 3d vector to define the "center"
		//one scalar value to define the radius 
		center = c;
		radius = rad;
	}

	bool ray_intersections(const Vec3f& origin, const Vec3f& ray_direction, float& t_hit) const {
		Vec3f VPC = center - origin;

		float projection_len = VPC * ray_direction;
		float center_to_ray = VPC * VPC - projection_len * projection_len;

		if (center_to_ray > radius * radius) {
			//there is no intersection
			return false;
		}

		//distance from projection point to intersection point along the ray
		float dist = sqrt(radius * radius - center_to_ray);

		//account for two intersection points along ray
		float near_point = projection_len - dist;
		float far_point = projection_len + dist;

		t_hit = near_point;

		if (t_hit < 0) {
			t_hit = far_point;
		}
		if (t_hit < 0) {
			return false;
		}
		return true;
	}
};


	Vec3f cast_rays(const Vec3f& origin, const Vec3f ray_direction, const Sphere& sphere) {
		float sphere_dist = std::numeric_limits<float>::max();
		if (!sphere.ray_intersections(origin, ray_direction, sphere_dist)) {
			return Vec3f(0.2, 0.7, 0.8);// background color
		}
		return Vec3f(0.2, 0.2, 0.3); //sphere color
	}


	void render(const Sphere & sphere) {
		const int height = 1080; //my native screen and image resolution
		const int width = 1920;
		const int fov = 3.14159265358979323846 / 2.;
		
		std::vector<Vec3f> frameBuffer(width * height); // declare 1d framebuffer vector (hole RGB respective to slots) 

		// Initialize the framebuffer with a gradient based on pixel coordinates
		for (size_t j = 0; j < height; j++) {
			for (size_t i = 0; i < width; i++) {
				float x = (2 * (i + 0.5) / (float)width - 1) * tan(fov / 2.) * width / (float)height;
				float y = -(2 * (j + 0.5) / (float)height - 1) * tan(fov / 2.);
				Vec3f dir = Vec3f(x, y, -1).normalize();
				frameBuffer[i + j * width] = cast_rays(Vec3f(0, 0, 0), dir, sphere);
			}
		}

		std::ofstream outFrameBuffer("./circle.ppm", std::ios::binary);
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
	Sphere sphere(Vec3f(-3, 0, -16), 3);
	render(sphere);
	std::cout << "Successful sphere render";
	return 0;
}
