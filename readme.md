# Mandelbrot Set Optimizing

*** Programm optimizing calculations using intinsics in Mandelbrot set construction (as an example of their usage); ***

![](/mandelbrot_set.png?raw=true "Optional Title")

## Work time
There were 4 version of Mandelbrot set construction: 
- not optimized version ([not_optimized_version_link](/mandelbrot.cpp)) where calculation is used without using any optimizing
- first pseudo optimized version  ([pseudo_optimized_link](/mandelbrot_pseudo_optmzd_1.cpp)) where it is tried to parallel calculation processes using for-cycles to work with 4 pixels at the same time
- second pseudo optimized version ([pseudo_optimized_link](/mandelbrot_pseudo_optmzd_2.cpp)) where first version calculation split into functions to help the proccessor parallel them
- pseudo optimizing functions have such name because they do no paralleling at reality
- optimized version ([optimized_version_link](/mandelbrot_optmzd_3.cpp)) with usage of intrinsics function at almost all calculations