#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

int height, width, MAXCOLOR;
pthread_t p1, p2;
pthread_mutex_t lock;
clock_t start, end;

struct pixel
{
    int r, g, b;
};

void grey_scale(struct pixel image[height][width])
{
    pthread_mutex_lock(&lock);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int grey = (image[i][j].r * 0.299) + (image[i][j].g * 0.587) + (image[i][j].b * 0.114);
            image[i][j].r = grey;
            image[i][j].g = grey;
            image[i][j].b = grey;
        }
    }
    pthread_mutex_unlock(&lock);
}

void contrast(struct pixel image[height][width])
{
    pthread_mutex_lock(&lock);
    float F = 0.5;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].r = (int)(image[i][j].r + 50) * F;
            image[i][j].g = (int)(image[i][j].g + 50) * F;
            image[i][j].b = (int)(image[i][j].b + 50) * F;

            if (image[i][j].r > 255)
                image[i][j].r = 255;
            if (image[i][j].g > 255)
                image[i][j].g = 255;
            if (image[i][j].b > 255)
                image[i][j].b = 255;
        }
    }
    pthread_mutex_unlock(&lock);
}

int main(int argc, char **argv)
{   
    start = clock();
    // Reading from input
    FILE *fi = fopen(argv[1], "r");
    char version[10];
    int r, g, b;

    fscanf(fi, "%s%d%d%d", version, &height, &width, &MAXCOLOR);

    struct pixel data[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            fscanf(fi, "%d%d%d", &r, &g, &b);
            data[i][j].r = r;
            data[i][j].g = g;
            data[i][j].b = b;
        }
    }

    fclose(fi);

    pthread_mutex_init(&lock, NULL);

    pthread_create(&p1, NULL, (void *)grey_scale, (void *)data);
    // grey_scale(data);
    pthread_create(&p2, NULL, (void *)contrast, (void *)data);
    pthread_join(p1, NULL);

    pthread_join(p2, NULL);
    // contrast(data);

    pthread_mutex_destroy(&lock);

    // Printing to output
    FILE *fo = fopen(argv[2], "w");

    fprintf(fo, "%s\n%d %d\n%d\n", version, height, width, MAXCOLOR);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            fprintf(fo, "%d %d %d ", data[i][j].r, data[i][j].g, data[i][j].b);
        }
        fprintf(fo, "\n");
    }

    fclose(fo);

    end = clock();
    printf("Time taken: %f\n", (double)(end - start) / CLOCKS_PER_SEC);
}
