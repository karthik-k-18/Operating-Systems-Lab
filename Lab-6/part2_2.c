#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/shm.h>

int height, width, MAXCOLOR;
pthread_t p1, p2;
sem_t *lock;
char *SEM_NAME = "shared_semaphore";
clock_t start, end;

struct pixel
{
    int r, g, b;
};

void grey_scale()
{
    // read shared memory
    int shmid = shmget(0x1234, sizeof(struct pixel) * height * width, 0666);
    struct pixel *image = (struct pixel *)shmat(shmid, NULL, 0);

    sem_t *lock = sem_open(SEM_NAME, O_CREAT, 0644);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int grey = (image[i * width + j].r * 0.299) + (image[i * width + j].g * 0.587) + (image[i * width + j].b * 0.114);
            image[i * width + j].r = grey;
            image[i * width + j].g = grey;
            image[i * width + j].b = grey;
        }
    }

    sem_post(lock);
    sem_close(lock);
}

void contrast()
{
    // read shared memory
    int shmid = shmget(0x1234, sizeof(struct pixel) * height * width, 0666);
    struct pixel *image = (struct pixel *)shmat(shmid, NULL, 0);

    sem_t *lock = sem_open(SEM_NAME, O_CREAT, 0644);
    sem_wait(lock);

    float F = 0.5;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i * width + j].r = (int)(image[i * width + j].r + 50) * F;
            image[i * width + j].g = (int)(image[i * width + j].g + 50) * F;
            image[i * width + j].b = (int)(image[i * width + j].b + 50) * F;

            if (image[i * width + j].r > 255)
                image[i * width + j].r = 255;
            if (image[i * width + j].g > 255)
                image[i * width + j].g = 255;
            if (image[i * width + j].b > 255)
                image[i * width + j].b = 255;
        }
    }

    sem_post(lock);
    sem_close(lock);
}

int main(int argc, char **argv)
{
    start = clock();
    // Reading from input
    FILE *fi = fopen(argv[1], "r");
    char version[10];
    int r, g, b;

    fscanf(fi, "%s%d%d%d", version, &height, &width, &MAXCOLOR);

    // struct pixel data[height][width];
    int shmid = shmget(0x1234, sizeof(struct pixel) * height * width, IPC_CREAT | 0666);
    struct pixel *data = (struct pixel *)shmat(shmid, NULL, 0);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            fscanf(fi, "%d%d%d", &r, &g, &b);

            data[i * width + j].r = r;
            data[i * width + j].g = g;
            data[i * width + j].b = b;
        }
    }

    fclose(fi);

    lock = sem_open(SEM_NAME, O_CREAT, 0644, 0);
    // create a shared variable to store a data using shmget()

    // sem_init(&lock, 5500, 1);

    pid_t pid = fork();
    if (pid == 0)
    {
        grey_scale();
    }
    else
    {
        contrast();

        // Printing to output
        FILE *fo = fopen(argv[2], "w");

        fprintf(fo, "%s\n%d %d\n%d\n", version, height, width, MAXCOLOR);

        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                fprintf(fo, "%d %d %d ", data[i * width + j].r, data[i * width + j].g, data[i * width + j].b);
            }
            fprintf(fo, "\n");
        }

        fclose(fo);

        sem_close(lock);
        sem_unlink(SEM_NAME);
        end = clock();
        printf("Time taken: %f\n", (double)(end - start) / CLOCKS_PER_SEC);
    }
}
