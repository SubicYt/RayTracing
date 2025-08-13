Step 1.) Render and Write image to Disk using a "render()" function.
 - The logic is included in the description of Render.cpp, I will attach the "geometry.h" header file which was made by developer ssloy.
 - Geometry.h just allows you with a class of multidimensional vectors which allows for manipulation of said vectors.
 - I used a Vect3f (vector with 3 respective elements to R, G, B) to store RGB values for each pixel to be then read to a .PPM formated file.

NOTE: I was confused about this but to make this clear, and clear for others reading... the pixels need to be taken from their (x,y) coordinates and put into a 1 dimension data structure in order to
be properly read and represented by .PPM files. File formates like .PPM expect all pixels to be laid out in a single continuous block.
 * hence why the framebuffer declaration is made as such ---> std::vector<Vect3f> frameBuffer(width*height); //only 1 dimension. 
