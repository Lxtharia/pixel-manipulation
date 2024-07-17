#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "gilbert2d.c"

static void swap(col_wrap* x, col_wrap* y){
	//swapped the wrapper structs
	col_wrap temp = *x;
	*x = *y;
	*y = temp;
	// also swap the colors in the data array
	color tempc = *(x->col);
	*(x->col) = *(y->col);
	*(y->col) = tempc;
	// THE SORT/GLITCH DIFFERENCE: (fixing the error ha ha)
	// swap the pointers
	color* tempp = x->col;
	x->col = y->col;
	y->col = tempp;
}


static void sortInterval(col_wrap* a, int start, int end){
    // sorting algorithm i copied from stackoverflow haha, classic;
    int gap = end-start;
    char swapped = 0;
    while ((gap > 1) || swapped) {
        if (gap > 1) gap = (int)(gap/1.247330950103979);
        swapped = 0;
        for (int i = start; gap + i <= end; i++){
		    if (isLess(a+i+gap, a+i)) {
			   swap(a+i+gap, a+i);
				swapped = 1;
			}
		}
    }
}

void debugIntervals(col_wrap* a, int start, int end){
	// just color each section in a random color
	for (int i = start; i <= end; ++i){
		a[i].col->r = (rand() % 0xff);
		a[i].col->g = (rand() % 0xff);
		a[i].col->b = (rand() % 0xff);
	}
}

//#define RMAX 400
#define RMIN 100
#define THRESH_BRIGHT 128
#define THRESH_HUE 180
int RMAX = 1200;
int ran_section_len = 0;

enum selec_mode {RANDOM, HUE, BRIGHT, FULL_LINE};

static char condition (col_wrap c, int section_offset_counter){
    switch(RANDOM){
	case RANDOM:
		section_offset_counter++;
		if(ran_section_len <= 0 || section_offset_counter >= ran_section_len){
			ran_section_len = RMIN + (rand() % RMAX);
			return 1;
		}
		return 0;
	case HUE:
		return c.hue > THRESH_HUE;
	case BRIGHT:
		return c.brightness > THRESH_BRIGHT;
	default:
		return 0;
    }
}

void hilbertsort(uint32_t data[], int width, int height) {
	color* origData = (color*) data;
	col_wrap* pixels = calloc(width*height, sizeof(col_wrap));

	vector_t* coords = gilbert2d(width, height);
	color* pixelData = calloc(width*height, sizeof(col_wrap));
	printf("> Hilbert sorting\n");
	for (int i = 0; i < width*height; i++){
		int ind = (coords[i].x + width * coords[i].y);
		pixelData[i] = origData[ind];
	}


	#pragma omp parallel for
	for (int y = 0; y < height; ++y) {
		int k = 0;
		for (int x = 0; x < width; ++x) {

			int index = y * width + x;
			// create the pixel_wrapper and put the hue in there
			color* current = pixelData + index;
			pixels[index].col = current;
			pixels[index].hue = getHue(current);
			pixels[index].brightness = getBrightness(current);

			if (condition(pixels[index], k) || x+1>=width){

				sortInterval(pixels, index - k, index);
				//debugIntervals(pixels, index - k, index);

				ran_section_len = RMIN + (rand() % RMAX);
				k = 0;
			}
			k++;
		}
	}
	
	for (int i = 0; i < width*height; i++){
		int ind = (coords[i].x + width * coords[i].y);
		origData[ind] = pixelData[i];
	}
//pragma end
}

void swaylock_effect(uint32_t data[], int width, int height) {
	RMAX = 200 + rand() % 1200;
	printf("Sorting with max section length of %d\n", RMAX);
	hilbertsort(data, width, height);
}

