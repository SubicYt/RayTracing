#include "geometry.h"
#include<iostream>
#include <vector>
#include <fstream>
#include <cmath>

struct Light {
	float intensity; 
	Vec3f position;
	
	Light(const Vec3f& p, const float& i) {
		intensity = i;
		position = p;
	}
/*
check the angle between a normal vector in this point and the vector describing a direction of light. 
The smaller the angle, the better the surface is illuminated. 
the scalar product between two vectors a and b is 
equal to product of norms of vectors times the cosine of the angle between the vectors: 
a*b = |a| |b| cos(alpha(a,b)). 
If we take vectors of unit length, the dot product will give us the intensity of surface illumination.
*/
};

struct Object_Material {
	Vec3f diffuse_color;
	float shininess_exponent; 
	Vec2f light_constants;

	Object_Material(){
		diffuse_color = Vec3f();
		shininess_exponent = 1;
		light_constants = Vec2f(1, 0);
	}

	Object_Material(const Vec3f& color, const float& e, const Vec2f& constants) {
		diffuse_color = color;
		shininess_exponent = e;
		light_constants = constants;
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
	const std::vector<Sphere> spheres,
	Vec3f& hit, Vec3f& normal, Object_Material& material) {

	float sphere_distance = std::numeric_limits<float>::max();

	//implementing each sphere within the sphere vector
	for (size_t i = 0; i < spheres.size(); i++) {
		float distance_i = 0;
		//check to see if there is an intersection occuring at the sphere
		if (spheres[i].ray_intersections(origin, ray_direction, distance_i) && distance_i < sphere_distance) {
			sphere_distance = distance_i;
			//vector that hit the sphere. 
			hit = ray_direction * distance_i + origin; 
			normal = (hit - spheres[i].center).normalize();
			material = spheres[i].material; 
		}
	}
	return sphere_distance < 1000; // returns true if hit anything. 
}
	 
/*
Within the cast rays function it checks for intersections. where there is an intersection we use Color 1, if not, use color 2.
*/
Vec3f reflection(Vec3f light_direction, Vec3f normal) {
	Vec3f reflect = normal-light_direction*2.f * (light_direction * normal); //ensure to pass light_dir as a negative value
	return reflect;
}


Vec3f cast_rays(const Vec3f& origin, const Vec3f& ray_direction,
	const std::vector<Sphere> spheres,
	const std::vector<Light>& lights) {
	
	Vec3f Hit; 
	Vec3f Point_Normal;
	Object_Material material; 
	float diffusion_light_intensity = 0;
	float specular_light_intenstiy = 0; 

	if (!Object_intersections(origin, ray_direction, spheres, Hit, Point_Normal, material)) {
		return Vec3f(0.1, 0.1, 0.4);
	}
	for (size_t i = 0; i < lights.size(); i++) {
		Vec3f Light_direction = (lights[i].position - Hit).normalize(); // direction of the light points towards hit object
		//light_direction scaled to 1
		float intensity = std::max(0.f, Light_direction * Point_Normal); //clamp to zero. Ensure no char wrap around. 
		//light intensity determined by the size of angle made by the light hitting the object to it's normal point. 
		diffusion_light_intensity += lights[i].intensity * intensity; // Add to the diffusion instensity 
		//refer to phong reflection model for speculat light intensity computation. 

        Vec3f neg_light_dir = -Light_direction;
        Vec3f reflection_dir = reflection(neg_light_dir, Point_Normal); // this is crazy, there needs to be a way to clean this up. 
		Vec3f negative_reflection_dir = reflection_dir * -1; 

		float reflection_clamped_zero = std::max(0.f, negative_reflection_dir * ray_direction);
		specular_light_intenstiy += powf(reflection_clamped_zero,
			material.shininess_exponent) * lights[i].intensity; 
	}
	return material.diffuse_color * diffusion_light_intensity * material.light_constants[0] + 
		Vec3f(1., 1., 1.) * specular_light_intenstiy * material.light_constants[1]; 
	//Vec3f(1, 1, 1) just returns 255 when multiplied, the color is really bright, like almost white. 
}


	void render(const std::vector<Sphere>& spheres, const std::vector<Light>& lights) {
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

				frameBuffer[i + j * width] = cast_rays(Vec3f(0, 0, 0), dir, spheres, lights);
			}
		}
		std::ofstream outFrameBuffer("./ObjectsWithLight.ppm", std::ios::binary);
		outFrameBuffer << "P6\n" // P6 format for binary RGB 
			<< width << " " << height << "\n" << "255\n"; // Max color value

		//write pixel data
		for (size_t i = 0; i < width * height; ++i) {
			Vec3f& c = frameBuffer[i];
			float max = std::max(c[0], std::max(c[1], c[2]));
			if (max > 1) c = c * (1. / max);
			for (size_t j = 0; j < 3; j++) {
				outFrameBuffer << (char)(255 * frameBuffer[i][j]); // Scale to 0-255 range
			}
		}
		outFrameBuffer.close(); // Close the file after writing
	}

int main() {
	Object_Material ivory = Object_Material(Vec3f(0.2, 0.5, 0.7), 50, Vec2f(0.6, 0.3));
	Object_Material apple = Object_Material(Vec3f(0.3, 0.1, 0.1), 10, Vec2f(0.9, 0.1));

	std::vector<Sphere> spheres;
	spheres.push_back(Sphere(Vec3f(-3, 0, -16), 2, ivory));
	spheres.push_back(Sphere(Vec3f(-1.0, -1.5, -12), 2, apple));
	spheres.push_back(Sphere(Vec3f(1.5, -0.5, -18), 3, apple));
	spheres.push_back(Sphere(Vec3f(7, 5, -18), 4, ivory));


	std::vector<Light> lights;
	lights.push_back(Light(Vec3f(-20, 30, 20), 1.5));
	lights.push_back(Light(Vec3f(30, 50, -25), 1.8));
	lights.push_back(Light(Vec3f(30, 20, 30), 1.7));

	render(spheres, lights);

	std::cout << "Spheres rendered successfully" << std::endl;
	return 0;
}