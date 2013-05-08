correspondence_problem_demo
===========================

A 3d graphics engine that displays the barberpole illusion and some rotating spirals to illustrate the correspondence problem.

texture.xcf, texture.bmp, texture.c are all the same image, GIMP was used to convert the bmp into the c source (in which the struct type was manually given the name 'GIMP_IMAGE'.
demo.bat calls the executable with a number of different command line parameters.
correspondence_problem_demo.dev is the project file for Dev-C++, which contains compiler flags, etc.
correspondence_problem_demo_main.cpp is the main source file.

In order to compile this, the FLTK library is needed. I downloaded the library via Dev-C++'s DevPak system, from http://devpaks.org/details.php?devpak=318
