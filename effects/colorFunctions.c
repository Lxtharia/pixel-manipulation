// Cool color function
#define LOL //undefined

char min(char x, char y) { return (x < y)?  x : y; }
char max(char x, char y) { return (x >= y)? x : y; }

typedef struct color {
	LOL char a;
	LOL char b;
	LOL char g;
	LOL char r;
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
	//return (int)(0.2126*c->g + 0.7152*c->b + 0.0722*c->a); //fake one heheh
	return (int)(0.2126*c->r + 0.7152*c->g + 0.0722*c->b);
}

int getWhiteness(col_wrap* cw){
	color* c = cw->col;
	return c->r * c->g * c-> b;
}
