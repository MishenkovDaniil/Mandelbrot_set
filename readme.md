# Mandelbrot Set Optimizing

***Programm optimizing calculations using intinsics in Mandelbrot set construction (as an example of their usage);***

![](/mandelbrot_set.png?raw=true "Optional Title")

## Versions
There were 4 version of Mandelbrot set construction: 
- not optimized version ([not_optimized_version_link](/mandelbrot.cpp)) where calculation is used without using any optimizing
- first pseudo optimized version  ([pseudo_optimized_link](/mandelbrot_pseudo_optmzd_1.cpp)) where it is tried to parallel calculation processes using for-cycles to work with 4 pixels at the same time
- second pseudo optimized version ([pseudo_optimized_link](/mandelbrot_pseudo_optmzd_2.cpp)) where first version calculation split into functions to help the proccessor parallel them
- pseudo optimizing functions have such name because they do no paralleling at reality
- optimized version ([optimized_version_link](/mandelbrot_optmzd_3.cpp)) with usage of intrinsics function at almost all calculations

## Work time 

| VERSION                   | FPS   | BOOST (%) |
| ------------------------- | ----- | --------- |
| not_optimized             | 1.65  |     0     |
| pseudo_optimized (first)  | 0.13  |    -92    |            
| pseudo_optimized (second) | 0.12  |    -93    |                
| not_optimized             | 2.83  |     72    |

Working fps was measured excluding rendering and calculated as 1/work_time (average work time in 100 work iteration)
All versions work with -o3 and -mavx2 flags and use SSE instructions

## Results
- it is not always possible to boost your programm by 'helping' inbuilt optimizer by building different auxilary constructions in your code
- using intrinsics may boost your programm and do it better than inbuilt optimizer 
- overheads does not allow to boost it absolutely (for example, if you treat 4 pixels at the same time (as in this programm) you will never boost it in 4 times)
