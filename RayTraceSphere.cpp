#include "geometry.h"
#include<iostream>
#include <vector>
#include <fstream>
#include <cmath>

struct Sphere {

	Vec3f center;
	float radius;

	Sphere(const Vec3f& c, const float& rad) { 
		//Center is vec3f used to describe center of circle. 
		//one scalar value to define the radius 
		center = c;
		radius = rad;
	}

	bool ray_intersections(const Vec3f& origin, const Vec3f& ray_direction, float& distance) const {
		Vec3f Origin_to_center = center - origin; // origin respective of ray.
		float projection_point = ray_direction * Origin_to_center; // orientation of the ray multiplied by vector to center of ray. 
		float center_to_ray = Origin_to_center * Origin_to_center - projection_point * projection_point; // clostest point from circle center to ray. 

		if (center_to_ray > radius * radius) {
			return false; // No intersection
		}
		//distance from projection point to intersection is needed. 
		//Look for two points of intersection, near and far. 
		float distance = sqrt(radius * radius - center_to_ray); // Note that center_to_ray is already squared value

		//the two points of intersection are trivial, if explanation is needed check README.
		distance = projection_point - distance;
		float distance_far_intersection = projection_point + distance;

		if (distance < 0) {
			distance = distance_far_intersection; //Only one point of contact with circle
		}
		if (distance < 0) {
			return false; //If still zero, circle is completely behind the ray
		}
		return true;
	}
};

/*
Within the cast rays function it checks for intersections. where there is an intersection we use Color 1, if not, use color 2.
*/
	Vec3f cast_rays(const Vec3f& origin, const Vec3f& ray_direction, const Sphere & sphere) {
		float distance = std::numeric_limits<float>::max();
		if (!sphere.ray_intersections(origin, ray_direction, distance)) {
			return Vec3f(0.3, 0.3, 0.7); // background color due to !intersections
		}
			return Vec3f(0.4, 0.6, 0.7); // circles
	};

	void render(const Sphere& sphere) {
		const int height = 1080; //my native screen and image resolution
		const int width = 1920;
		const int fov = 3.14159265358979323846 / 2.;
		
		std::vector<Vec3f> frameBuffer(width * height); // declare 1d framebuffer vector (holds RGB values respective to slots) 

		for (size_t j = 0; j < height; j++) {
			for (size_t i = 0; i < width; i++) {
				/*
				*Variables x and y below build the ray_direction.
				* For each pixel you build a ray_direction as such.
				*/
				
				float x = ((2 * (i + 0.5) / (float)width) - 1)
					* tan(fov / 2.)
					* width / (float)height;
				
				float y = -(2 * (j + 0.5) / (float)height - 1) 
					* tan(fov / 2.);

				Vec3f dir = Vec3f(x, y, -1).normalize(); // .normalize()  Scales the vector to a lenth 1. 
				// We scale to len. 1 because we are looking for the orientation of the vector only.
				//The direction * distance is what gives us a ray used for computation within the ray_intersections function.

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
