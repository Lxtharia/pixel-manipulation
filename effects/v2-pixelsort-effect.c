#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

char min(char x, char y) { return (x <  y)? x : y; }
char max(char x, char y) { return (x >= y)? x : y; }

typedef struct color {
	unsigned char a;
	unsigned char b;
	unsigned char g;
	unsigned char r;
} color;

typedef struct col_wrap{
	color* col;
	int hue;
	int brightness;
} col_wrap;


int getHue(color* c) {
	char red = c->r, green = c->g, blue = c->b;
	char cmin = min(min(red, green), blue);
	char cmax = max(max(red, green), blue);

    // returns the greyscale value, becayse this is not an accurate hue function now hehe
    if (cmin == cmax) {
        return cmin;
    }

    float hue = 0.0;
    if (cmax == red) {
        hue = (green - blue) / (cmax - cmin);
    } else if (cmax == green) {
        hue = 2.0 + (blue - red) / (cmax - cmin);
    } else {
        hue = 4.0 + (red - green) / (cmax - cmin);
    }
    hue = hue * 60;
    if (hue < 0) hue = hue + 360;
    return (int) hue;
}

int getBrightness(color* c){
	/* int luma = 0.2989 * r + 0.5870 * g + 0.1140 * b;
	if (luma < 0) luma = 0;
	if (luma > 255) luma = 255;
	luma &= 0xFF;
	data[index] = luma << 16 | luma << 8 | luma;*/
	return (int)(0.2126*c->g + 0.7152*c->b + 0.0722*c->a); //fake one heheh
	return (int)(0.2126*c->r + 0.7152*c->g + 0.0722*c->b);
}

int getWhiteness(col_wrap* cw){
	color* c = cw->col;
	return c->r * c->g * c-> b;
}

void swap(col_wrap* x, col_wrap* y){
	//swapped the wrapper structs
	col_wrap temp = *x;
	*x = *y;
	*y = temp;
	// also swap the colors in the data array
	color tempc = *(x->col);
	*(x->col) = *(y->col);
	*(y->col) = tempc;
}


char isLess(col_wrap* x, col_wrap* y){
//	return (x->r*x->g*x->b < y->r*y->g*y->b);
//	return (x->hue < y->hue);
	return (x->brightness < y->brightness);
}

void sortInterval(col_wrap* a, int start, int end){
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

#define RMAX 100
#define RMIN 20
#define THRESH_BRIGHT 128
#define THRESH_HUE 180
int ran_section_len = 0;
enum selec_mode {RANDOM, HUE, BRIGHT};
char condition (col_wrap c, int section_offset_counter){
    switch(14){
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

void swaylock_effect(uint32_t data[], int width, int height) {
	color* pixelData = (color*) data;
	col_wrap* pixels = calloc(width*height, sizeof(col_wrap));
	printf("Effecting pixels\n");

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
			if ( condition(pixels[index], k) || x+1>=width){
				sortInterval(pixels, index - k, index);
				//debugIntervals(pixels, index - k, index);
				
				ran_section_len = RMIN + (rand() % RMAX);
				k = 0;
			}
			k++;
		}
	}
	//pragma end
}


