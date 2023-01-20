#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define RMAX 100
#define RMIN 20

char min(int x, char y){
	return (x < y)? x : y;
}
char max(char x, char y) { return (x >= y)? x:y; }

typedef struct {
	char b;
	char g;
	char r;
	char a;
} color;

void swap(color* x, color* y){
	color temp = *x;
	*x = *y;
	*y = temp;
}

float getHue(int red, int green, int blue) {

    char cmin = min(min(red, green), blue);
    char cmax = max(max(red, green), blue);

    if (cmin == cmax) {
        return 0;
    }

    float hue = 0.0;
    if (cmax == red) {
        hue = (green - blue) / (cmax - cmin);

    } else if (cmax == green) {
        hue = 2.0 + (blue - red) / (cmax - cmin);

    } else {
        hue = 4.0 + (red - green) / (cmax - cmin);
    }

//    hue = hue * 60;
//    if (hue < 0) hue = hue + 360;

    return hue;
}




char isLess(color* x, color* y){
	//return getHue(x->r, x->g, x->b) < getHue(y->r, y->g, y->b); 
	return (x->r*x->g*x->b < y->r*y->g*y->b);
}

void sortInterval(color* a, int size){
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
			   swap(a+i+gap, a+i);
				swapped = 1;
			}
		}
    }
}



void swaylock_effect(uint32_t *data, int width, int height) {
	color* pixels = (color*) data;
	printf("LMaoo\n");
	#pragma omp parallel for
		for (int y = 0; y < height; ++y) {

			int k = 0;
			int ran = RMIN + (rand() % RMAX);

			for (int x = 0; x < width; ++x) {
				int index = y * width + x;
				char r = (data[index] & 0xff0000) >> 16;
				char g = (data[index] & 0x00ff00) >> 8;
				char b = (data[index] & 0x0000ff);
				
				float hue = getHue(r, g, b);

				if (k >= ran) {
				//if (hue < 0) {	
					//printf("hue: %d\n", hue);
					// we give it an offset :)
					// example: section from 0d40 to 0d52: index is d52, k is 12, so we start at d52-12 = d40 (hopefully)
				//	printf("Sorting from %x to +%d = %x\n", pixels + index -k, k, pixels + index);
					sortInterval(pixels + index - k, k);
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


