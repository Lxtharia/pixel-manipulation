#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int sgn(int x) {
    return x < 0 ? -1 : (x > 0 ? 1 : 0);
}

typedef struct vector_t {
	int x;
	int y;
} vector_t;

void generate2d(vector_t* coords, int* ind, int x, int y, int ax, int ay, int bx, int by) {
    int w = abs(ax + ay);
    int h = abs(bx + by);

    int dax = sgn(ax);
    int day = sgn(ay);
    int dbx = sgn(bx);
    int dby = sgn(by);

    if (h == 1) {
        // trivial row fill
        for (int i = 0; i < w; i++) {
//            printf("%d (%d,%d)\n",*ind, x, y);
			coords[*ind] = (vector_t) {x, y};
			*ind +=1;
            x += dax;
            y += day;
        }
        return;
    }

    if (w == 1) {
        // trivial column fill
        for (int i = 0; i < h; i++) {
//            printf("%d (%d,%d)\n",*ind, x, y);
			coords[*ind] = (vector_t) {x, y};
			*ind += 1;
            x += dbx;
            y += dby;
        }
        return;
    }

    int ax2 = ax / 2;
    int ay2 = ay / 2;
    int bx2 = bx / 2;
    int by2 = by / 2;

    int w2 = abs(ax2 + ay2);
    int h2 = abs(bx2 + by2);

    if (2 * w > 3 * h) {
        if (w2 % 2 && w > 2) {
            // prefer even steps
            ax2 += dax;
            ay2 += day;
        }

        // long case: split in two parts only
        generate2d(coords,ind, x, y, ax2, ay2, bx, by);
        generate2d(coords,ind, x + ax2, y + ay2, ax - ax2, ay - ay2, bx, by);
    } else {
        if (h2 % 2 && h > 2) {
            // prefer even steps
            bx2 += dbx;
            by2 += dby;
        }

        // standard case: one step up, one long horizontal, one step down
        generate2d(coords,ind, x, y, bx2, by2, ax2, ay2);
        generate2d(coords,ind, x + bx2, y + by2, ax, ay, bx - bx2, by - by2);
        generate2d(coords,ind, x + (ax - dax) + (bx2 - dbx), y + (ay - day) + (by2 - dby),
                   -bx2, -by2, -(ax - ax2), -(ay - ay2));
    }
}

vector_t* gilbert2d(int width, int height) {
	vector_t* coords = malloc(width * height * sizeof(vector_t));
	int i = 0;
    if (width >= height) {
        generate2d(coords,&i, 0, 0, width, 0, 0, height);
    } else {
        generate2d(coords,&i, 0, 0, 0, height, width, 0);
    }
	return coords;
}
/*
int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <width> <height>\n", argv[0]);
        return 1;
    }
    int width = atoi(argv[1]);
    int height = atoi(argv[2]);
	vector_t* c;
    c = gilbert2d(width, height);
	
//	for(int i=0; i<width*height; i++){
//		printf("%u>%d...%d\n",c,  c[i].x, c[i].y);
//	}

    return 0;
}
*/
