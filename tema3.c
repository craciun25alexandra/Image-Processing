#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "bmp_header.h"
#define NR1_FOR_BITS 31
#define NR2_FOR_BITS 24
#define NR3_FOR_BITS 32
#define DIMSTRING 50
#define DM_ALLOC 20

struct RGB{
    unsigned char blue;
    unsigned char green;
    unsigned char red;
};

struct Image{
    int16_t height;
    int16_t width;
    struct RGB **rgb;
};

struct Image readImage(FILE *in, int16_t height, int16_t width) {
    struct Image image;
    image.rgb = malloc(height * sizeof(void *));
    if (!image.rgb) {
    printf("Alocarea matricei a esuat");
    } else {
    image.height = height;
    image.width = width;
    unsigned int allbits = ((NR1_FOR_BITS + NR2_FOR_BITS * width)
    /NR3_FOR_BITS) * 4;
    unsigned int nrRgb = allbits / sizeof(struct RGB) + 1;
    for (int i = height - 1; i >= 0; i--) {
        image.rgb[i] = malloc(nrRgb * sizeof(struct RGB));
        if (!image.rgb[i]) {
        printf("Alocarea matricei a esuat");
        for (int k = height - 1; k >= i; k--)
        free(image.rgb[k]);
        free(image.rgb);
        } else {
        fread(image.rgb[i], 1, allbits, in);
    }}}
    return image;
}
struct Image MatrixPic(int16_t x, struct RGB *pallet) {
    struct Image pic;
    pic.height = x;
    pic.width = x;
    pic.rgb = malloc(x * sizeof(void *));
    if (!pic.rgb) {
    printf("Alocarea matricei a esuat");
    } else {
    for (int i = x - 1; i >= 0; i--) {
        pic.rgb[i] = malloc(x * sizeof(struct RGB));
        if (!pic.rgb[i]) {
        printf("Alocarea matricei a esuat");
        for (int k = x - 1; k >= i; k--)
        free(pic.rgb[k]);
        free(pic.rgb);
        } else {
        for (int j = x - 1; j >= 0; j--) {
            pic.rgb[i][j].blue = pallet->blue;
            pic.rgb[i][j].green = pallet->green;
            pic.rgb[i][j].red = pallet->red;
        }}
    }}
    return pic;
}

void freeImage(struct Image pic) {
    for (int i = pic.height - 1; i >= 0; i--)
    free(pic.rgb[i]);
    free(pic.rgb);
}
struct PartsImage{
    bmp_fileheader a;
    bmp_infoheader b;
    struct Image c;
};
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}
struct PartsImage openfile(char p[DIMSTRING]) {
    struct PartsImage x;
    FILE *in = fopen(p, "rb");
    bmp_fileheader image_file;
    bmp_infoheader image_info;
    fread(&image_file, sizeof(bmp_fileheader), 1, in);
    fread(&image_info, sizeof(bmp_infoheader), 1, in);
    fseek(in, image_file.imageDataOffset, SEEK_SET);
    struct Image image = readImage(in,
    (int16_t)image_info.height, (int16_t)image_info.width);
    fclose(in);
    x.a = image_file;
    x.b = image_info;
    x.c = image;
    return x;
}
void create(bmp_fileheader image_file, bmp_infoheader image_info,
struct Image pic, char y[DIMSTRING]) {
    FILE *in2 = fopen(y, "wp");
    fwrite(&image_file, sizeof(bmp_fileheader), 1, in2);
    fwrite(&image_info, sizeof(bmp_infoheader), 1, in2);
    for (int i = pic.height - 1; i >= 0; i--)
        fwrite(pic.rgb[i],  ((NR1_FOR_BITS + NR2_FOR_BITS
        * pic.width) /NR3_FOR_BITS) * 4, 1, in2);
    fclose(in2);
}
void draw(int y1, int x1, int y2, int x2, struct Image m2,
struct PartsImage imag1, int xw) {
    if (abs(x2 - x1) >= abs(y2 - y1)) {
        if (x1 > x2) {
            swap(&x1, &x2);
            swap(&y1, &y2);
        }
        for (int xm = x1; xm <= x2; xm++) {
            int ym = ((xm - x1) * (y2 - y1) + y1 * (x2 - x1)) / (x2 - x1);

            for (int i = 0; i < xw; i++)
                for (int j = 0; j < xw; j++) {
                    int t = i + xm - xw / 2;
                    int p = j + ym - xw / 2;
                    if (t >= 0 && p >= 0 && t < imag1.c.height &&
                    p < imag1.c.width)
                        imag1.c.rgb[imag1.c.height - t - 1][p] = m2.rgb[i][j];
                }
        }
    } else {
        if (y1 > y2) {
            swap(&x1, &x2);
            swap(&y1, &y2);
        }
        for (int ym = y1; ym <= y2; ym++) {
           int xm = ((ym - y1) * (x2 - x1) + x1 * (y2 - y1)) / (y2 - y1);

            for (int i = 0; i < xw; i++)
                for (int j = 0; j < xw; j++) {
                    int t = i + xm - xw / 2;
                    int p = j + ym - xw / 2;
                    if (t >= 0 && p >= 0 && t < imag1.c.height &&
                    p < imag1.c.width)
                        imag1.c.rgb[imag1.c.height - t - 1][p] = m2.rgb[i][j];
                }
        }
    }
}
struct lungimi{
    int16_t l1;
    int16_t l2;
};
void fill(int16_t x, int16_t y, struct RGB *pallet,
struct RGB **rgb, struct RGB *cobai, struct lungimi *dim) {
    rgb[x][y].red = pallet->red;
    rgb[x][y].green = pallet->green;
    rgb[x][y].blue = pallet->blue;
     if (y - 1 >= 0) {
            if ((cobai->red == rgb[x][y - 1].red) &&
            (cobai->green == rgb[x][y - 1].green) &&
            (cobai->blue == rgb[x][y - 1].blue))
                fill(x, (int16_t)(y - 1), pallet, rgb, cobai, dim);
        }
        if (x - 1 >= 0) {
            if ((cobai->red == rgb[x - 1][y].red) &&
            (cobai->green == rgb[x - 1][y].green) &&
            (cobai->blue == rgb[x - 1][y].blue))
                fill((int16_t)(x - 1), y, pallet, rgb, cobai, dim);
        }
        if (x + 1 < dim->l1) {
            if ((cobai->red == rgb[x + 1][y].red) &&
            (cobai->green == rgb[x + 1][y].green) &&
            (cobai->blue == rgb[x + 1][y].blue))
                fill((int16_t)(x + 1), y, pallet, rgb, cobai, dim);
        }
        if (y + 1 < dim->l2) {
            if ((cobai->red == rgb[x][y + 1].red) &&
            (cobai->green == rgb[x][y + 1].green) &&
            (cobai->blue == rgb[x][y + 1].blue))
                fill(x, (int16_t)(y + 1), pallet, rgb, cobai, dim);
        }
}
int main() {
    struct PartsImage imag1, imag2;
    struct lungimi *dimensiuni = malloc(sizeof(struct lungimi));
    struct RGB *pallet = malloc(sizeof(struct RGB)),
    *cobai = malloc(sizeof(struct RGB));
    struct Image MatDraw;
    int ok = 1, wd = 0, hg = 0, ok2 = 0;
    int16_t xw = 0;
    int R = 0, B = 0, G = 0;
    char *s = malloc(DM_ALLOC), *p = malloc(DM_ALLOC),
    *a1 = malloc(DM_ALLOC);
    if (!s || !p || !a1 || !pallet || !dimensiuni || !cobai) {
        printf("Alocarea memoriei a esuat");
        return -1;
    }
    while (ok == 1) {
        scanf("%s", s);
        if (strcmp(s, "edit") == 0) {
            scanf("%s", p);
            imag1 = openfile(p);
        } else if (strcmp(s, "save") == 0) {
            scanf("%s", a1);
            create(imag1.a, imag1.b, imag1.c, a1);
        } else if (strcmp(s, "insert") == 0) {
            scanf("%s", a1);
            int x = 0, y = 0;
            scanf("%u %u", &y, &x);
            imag2 = openfile(a1);
            int h1 = imag1.c.height;
            int w1 = imag1.c.width;
            int w2 = imag2.c.width;
            int h2 = imag2.c.height;
            int i1 = h1 - x - 1;
            int i2 = h2 - 1;
            while (i1 >= 0 && i2 >= 0) {
                int j1 = y;
                int j2 = 0;
                while (j1 < w1 && j2 < w2) {
                    imag1.c.rgb[i1][j1] = imag2.c.rgb[i2][j2];
                    j1++;
                    j2++;
                }
                i1--;
                i2--;
            }
            freeImage(imag2.c);
        } else if (strcmp(s, "set") == 0) {
            scanf("%s", a1);
            if (strcmp(a1, "draw_color") == 0) {
                scanf("%d%d%d", &R, &G, &B);
                pallet->blue = B;
                pallet->green = G;
                pallet->red = R;
                if (ok2)
                MatDraw = MatrixPic(xw, pallet);
            } else {
                scanf("%hd", &xw);
                MatDraw = MatrixPic(xw, pallet);
                ok2 = 1;
            }
        } else if (strcmp(s, "draw") == 0) {
            scanf("%s", a1);
            int x1 = 0, x2 = 0, x3 = 0, y1 = 0, y2 = 0, y3 = 0;
            if (strcmp(a1, "line") == 0) {
                scanf("%d %d %d %d", &y1, &x1, &y2, &x2);
                draw(y1, x1, y2, x2, MatDraw, imag1, xw);
            } else if (strcmp(a1, "rectangle") == 0) {
                scanf("%d %d %d %d", &y1, &x1, &wd, &hg);
                draw(y1, x1, y1 + wd, x1, MatDraw, imag1, xw);
                draw(y1, x1, y1, x1 + hg, MatDraw, imag1, xw);
                draw(y1, x1 + hg, y1 + wd, x1 + hg, MatDraw, imag1, xw);
                draw(y1 + wd, x1, y1 + wd, x1 + hg, MatDraw, imag1, xw);
            } else if (strcmp(a1, "triangle") == 0) {
                scanf("%d %d %d %d %d %d", &y1, &x1, &y2, &x2, &y3, &x3);
                draw(y1, x1, y2, x2, MatDraw, imag1, xw);
                draw(y2, x2, y3, x3, MatDraw, imag1, xw);
                draw(y1, x1, y3, x3, MatDraw, imag1, xw);
            }
        } else if (strcmp(s, "fill") == 0) {
            int16_t x = 0, y = 0;
            scanf("%hd %hd", &y, &x);
            cobai->blue = imag1.c.rgb[imag1.c.height - x - 1][y].blue;
            cobai->green = imag1.c.rgb[imag1.c.height - x - 1][y].green;
            cobai->red = imag1.c.rgb[imag1.c.height - x - 1][y].red;
            dimensiuni->l1 = imag1.c.height;
            dimensiuni->l2 = imag1.c.width;
            fill((int16_t)(imag1.c.height - x - 1), y, pallet, imag1.c.rgb,
            cobai, dimensiuni);
        } else if (strcmp(s, "quit") == 0) {
            freeImage(imag1.c);
            ok = 0;
        }
    }
    free(s);
    free(p);
    free(a1);
    free(pallet);
    free(cobai);
    free(dimensiuni);
    return 0;
}
