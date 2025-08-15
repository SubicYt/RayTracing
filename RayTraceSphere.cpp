#include "geometry.h"
#include<iostream>
#include <vector>
#include <fstream>
#include <cmath>

struct light_source {
	float intensity; 
	Vec3f position;
	
	light_source(const Vec3f& p, const float& i) {
		intensity = i;
		position = p;
	}
};

struct Object_Material {
	Vec3f diffuse_color;

	Object_Material(){
		diffuse_color = Vec3f();
	}

	Object_Material(const Vec3f& color) {
		diffuse_color = color;
	}

};

struct Sphere {

	Vec3f center;
	float radius;
	Object_Material material;

	Sphere(const Vec3f& c, const float& rad, const Object_Material& m) { 
		//Center is vec3f used to describe center of circle. 
		//one scalar value to define the radius 
		center = c;
		radius = rad;
		material = m;
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
		float Intersection_distance = sqrt(radius * radius - center_to_ray); // Note that center_to_ray is already squared value

		//the two points of intersection are trivial, if explanation is needed check README.
		distance = projection_point - Intersection_distance;
		float distance_far_intersection = projection_point + Intersection_distance;

		if (distance < 0) {
			distance = distance_far_intersection; //Only one point of contact with circle
		}
		if (distance < 0) {
			return false; //If still zero, circle is completely behind the ray
		}
		return true;
	}
};

bool Object_intersections(const Vec3f& origin, const Vec3f& ray_direction,
	const std::vector<Sphere> spheres, Vec3f& hit, Vec3f& normal, Object_Material& material) {
	
	float sphere_distance = std::numeric_limits<float>::max(); // start with a distance of infinity.
	
	//Loop through each sphere in the spheres vector 
	for (size_t i = 0; i < spheres.size(); i++) {
		float dist_i;
		if (spheres[i].ray_intersections(origin, ray_direction, dist_i) && dist_i < sphere_distance) {
			sphere_distance = dist_i; // update sphere_distance to current circle in vector. 
			hit = origin + ray_direction * dist_i; // Vector to the 3d sphere itself.
			normal = (hit - spheres[i].center).normalize(); // Point normal to the hit, scaled to 1. 
			material = spheres[i].material; 
		}
	}
	return sphere_distance < 1000; 
	//returns true if we actually hit anything.
}
	 
/*
Within the cast rays function it checks for intersections. where there is an intersection we use Color 1, if not, use color 2.
*/
	Vec3f cast_rays(const Vec3f& origin, const Vec3f& ray_direction, const std::vector<Sphere> spheres) {
		Vec3f Hit;
		Vec3f Normal;
		Object_Material material; 

		float distance = std::numeric_limits<float>::max();
		if (!Object_intersections(origin, ray_direction, spheres, Hit, Normal, material)) {
			return Vec3f(0.3, 0.3, 0.7);
		}
		return material.diffuse_color; 
	};

	void render(const std::vector<Sphere> spheres) {
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

				frameBuffer[i + j * width] = cast_rays(Vec3f(0, 0, 0), dir, spheres);
			}
		}
		std::ofstream outFrameBuffer("./multiple_objects.ppm", std::ios::binary);
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
	Object_Material ivory = Vec3f(0.2, 0.5, 0.7);
	Object_Material apple = Vec3f(0.3, 0.1, 0.1);

	std::vector<Sphere> spheres;
	spheres.push_back(Sphere(Vec3f(-3, 0, -16), 2, ivory));
	spheres.push_back(Sphere(Vec3f(-1.0, -1.5, -12), 2, apple));
	spheres.push_back(Sphere(Vec3f(1.5, -0.5, -18), 3, apple));
	spheres.push_back(Sphere(Vec3f(7, 5, -18), 4, ivory));

	render(spheres);

	std::cout << "Spheres rendered successfully" << std::endl;
	return 0;
}
