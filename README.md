# RiverSim
A sperimental river simulator implementing the Preissmann scheme using OpenGL, C++ and the FLTK, CMINPACK libraries.

To make this project properly work the following libraries are necessary:
- FLTK 1.3.3 (http://www.fltk.org/software.php) - We don't know if the newest versions are compatibles.
- cminpack 1.3.4 (http://devernay.free.fr/hacks/cminpack/) - We don't know if the newest versions are compatibles.

NOTE:
The FLTK libraries needs to be compiled with the OpenGL and threads flags enabled during the configure command.

HOW TO COMPILE:
- LINUX: Just run a "make all" command in the root folder of the project. Use make clean to delete all .o .cpp~ .h~ .txt~ files.
