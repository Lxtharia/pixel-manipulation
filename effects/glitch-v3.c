#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define RMAX 100
#define RMIN 20
typedef struct color {
	char b;
	char g;
	char r;
	char a;
} color;


void glitchSwap(col_wrap* x, col_wrap* y){
	//swapped the wrapper structs
	col_wrap temp = *x;
	*x = *y;
	*y = temp;
	// also swap the colors in the data array
	//This copies memory
	color tempc = *(x->col);
	*(x->col) = *(y->col);
	*(y->col) = tempc;
	// x is pointing to the pixel/color cx, y to cy
	// we now swap x and y, because we sort these and they have to pull the actual color struct themselves
	// what we do here is. We swap the pixels cx and cy. This puts these in the right order. 
	// But x is still pointing to the previous address where cy is now located.
	// So not only is the hue inaccurate to the pixel, if x were to swap with z, x would swap cy cause thats the pixel it points to 
	// I don't know how to write efficient comments, but this will help me understand when i forget again
}


static void glitchInterval(col_wrap* a, int size){
	// sorting algorithm i copied from stackoverflow haha, classic;
/*	
	int ran = (rand()*0xffffff)/0x111111;
	char r = (ran & 0xff0000) >> 16;
	char g = (ran & 0x00ff00) >> 8;
	char b =  ran & 0x0000ff;
	for (int i = 0; i < size; i++) {
		//printf("g was %x, is now %x\n", a[i].g, g);
		a[i].r = 0;
		a[i].g = g;
		a[i].b = b;
	}
	return;*/
	int gap = size;
    char swapped = 0;
    while ((gap > 1) || swapped) {
        if (gap > 1) gap = (int)(gap/1.247330950103979);
        swapped = 0;
        for (int i = 0; gap + i < size; i++){
		    if (isLess(a+i+gap, a+i)) {
			   glitchSwap(a+i+gap, a+i);
				swapped = 1;
			}
		}
    }
}

void glitch(uint32_t data[], int width, int height) {
	color* pixelData = (color*) data;
	col_wrap* pixels = calloc(width*height, sizeof(col_wrap));
	printf("V2-glitching\n");

	int hue_threshold = 180;
	int bright_threshold = 128;

	#pragma omp parallel for
		for (int y = 0; y < height; ++y) {

			int k = 0;
			int ran = RMIN + (rand() % RMAX);
			
			

			for (int x = 0; x < width; ++x) {
				int index = y * width + x;
				// create the pixel_wrapper and put the hue in there
				color* current = pixelData + index;
				int hue = getHue(current);
				int brightness = getBrightness(current);;
				pixels[index].col=current;
				pixels[index].hue=hue;
				pixels[index].brightness=brightness;

				//if (k >= ran) {
				if (brightness > bright_threshold || k>=width-1) {	
					//printf("hue: %d\n", hue);
					// we give it an offset :)
					// example: section from 0d40 to 0d52: index is d52, k is 12, so we start at d52-12 = d40 (hopefully)
				//	printf("Sorting from %x to +%d = %x\n", pixels + index -k, k, pixels + index);
					
					glitchInterval(pixels + index - k, k);
					//debugIntervals(pixels + index -k, k);
					// choose how long our next section should be
					ran = RMIN + (rand() % RMAX);
					// and take care that it doent go over the width of the image
					if(index%width+ran >= width-1) ran = width-1 - index%width;
					// im commenting every line
					k = 0;
					//DEBUG lmao
					//return;
				}
				k++;
				
				/* int luma = 0.2989 * r + 0.5870 * g + 0.1140 * b;
				if (luma < 0) luma = 0;
				if (luma > 255) luma = 255;
				luma &= 0xFF;
				data[index] = luma << 16 | luma << 8 | luma;*/
			}
		}
	//pragma end
}

void swaylock_effect(uint32_t data[], int width, int height){
	glitch(data, width, height);
}
