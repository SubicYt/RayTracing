PART 1: A MINI 3D RENDER WITH RAY TRACING! (an introduction blurb)
-

All math and logic will be uploaded shortly with screenshots of work and equations used.

(REVIEW THESE SOURCES PRIOR TO DIVING INTO CODE OR DOCUMENTATION)
--Seriously though, this isn't very intuitive at all unless you are cracked out of your mind... you will be lost. Like me lol. 
Just review it and work the stuff out as you go along, it doesn't take very long. 

To calculate sphere intersections refer to this -------> https://www.lighthouse3d.com/tutorials/maths/ray-sphere-intersection/

The Phong Reflection Model is responsible for specular light ---------> https://en.wikipedia.org/wiki/Phong_reflection_model
__________________________________________________________________________________________________________________________________

PART 2: RENDER GRADIENT - SAVE IMAGE TO DISK
-
Step 1.) Render and Write image to Disk using a "render()" function.
 - The logic is included in the description of Render.cpp, I will attach the "geometry.h" header file which was made by developer ssloy.
 - Geometry.h just allows you with a class of multidimensional vectors which allows for manipulation of said vectors.
 - I used a Vect3f (vector with 3 respective elements to R, G, B) to store RGB values for each pixel to be then read to a .PPM formated file.

NOTE: I was confused about this but to make this clear, and clear for others reading... the pixels need to be taken from their (x,y) coordinates and put into a 1 dimension data structure in order to
be properly read and represented by .PPM files. File formates like .PPM expect all pixels to be laid out in a single continuous block.
 * hence why the framebuffer declaration is made as such ---> std::vector<Vect3f> frameBuffer(width*height); //only 1 dimension. 

More proper documentation coming soon...
