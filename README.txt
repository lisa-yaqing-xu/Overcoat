Yaqing(Lisa) Xu
108059610
lisa.yaqing.xu@gmail.com
yaqing.xu@stonybrook.edu

How to run the project:

I setup all the libraries and dependencies right in this project as relative paths(I only really used glm and glut anyway) so it should run from the getgo. I tested it on my virtual machine with basically nothing but VS and Chrome installed on it and it ran. If it doesn't run, please let me know.

I included 2 mesh files, sphere.obj, which is a generic uv sphere generated in Blender, and bunny.obj, which is a simplified stanford bunny mesh, for testing purposes. At the start the window will prompt you to load a obj file. Either is fine. You can also supply your own, though it's a bit volatile with highly complex meshes; the complex version of the stanford bunny caused it to crash, for example. Overcoat was meant to be used with a rough proxy mesh instead of a highly detailed one, anyway.

to switch between tools, you need to be on the opengl window and not the command window. Use the command window only to type in level/initial level changes.

============================
SOURCES AND REFERENCES
============================
Libraries:
I used glut32 for rendering and glm for doing the vector math.

Snippets of code I referenced (the specific methods are in the comments in the code):
https://nccastaff.bournemouth.ac.uk/jmacey/RobTheBloke/www/opengl_programming.html
I used the Simple Glut Example (first one, with the static wireframe teapot) to help set up the skeleton structure of the glut rendering environment. 

https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Load_OBJ
I followed this tutorial to write an obj loader

Furthermore, I included an assignment2.pdf, which is my 328 homework 2. I used the algorithm Professor Gu gave (which meant repurposing some code I wrote for homework based on that from about 3 semesters ago) for the raycasting part.