#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int height, width, MAXCOLOR;

int fd[2];
clock_t start, end;

struct pixel
{
	int r, g, b;
};

void grey_scale(FILE *fp)
{	
	struct pixel image[height][width];
	int r, g, b;
	
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{	
			fscanf(fp, "%d%d%d", &r, &g, &b);
			image[i][j].r = r;
			image[i][j].g = g;
			image[i][j].b = b;
		}
	}

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			int grey = (image[i][j].r * 0.299) + (image[i][j].g * 0.587) + (image[i][j].b * 0.114);
			image[i][j].r = grey;
			image[i][j].g = grey;
			image[i][j].b = grey;
			write(fd[1], &image[i][j], sizeof(struct pixel));
		}
	}
}


void contrast(struct pixel image[height][width])
{

	float F = 0.5;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			struct pixel p;
			read(fd[0], &p, sizeof(struct pixel));

			image[i][j].r = (int)(p.r + 50) * F;
			image[i][j].g = (int)(p.g + 50) * F;
			image[i][j].b = (int)(p.b + 50) * F;

			if (image[i][j].r > 255)
				image[i][j].r = 255;
			if (image[i][j].g > 255)
				image[i][j].g = 255;
			if (image[i][j].b > 255)
				image[i][j].b = 255;
		}
	}


}

int main(int argc, char **argv)
{
	start = clock();
	// Reading from input
	FILE *fi = fopen(argv[1], "r");
	char version[10];
	pid_t pid;

	fscanf(fi, "%s%d%d%d", version, &height, &width, &MAXCOLOR);
	

	pipe(fd);
	pid = fork();

	if (pid == 0)
	{
		close(fd[0]);
		grey_scale(fi);

		close(fd[1]);
		exit(0);
	}
	else
	{	
		// struct pixel image[height][width];

		close(fd[1]);

		struct pixel image[height][width];

		// get the contrast image
		contrast(image);
		
		wait(NULL);

		// Writing to output
		FILE *fo = fopen(argv[2], "w");
		fprintf(fo, "%s\n%d %d\n%d\n", version, height, width, MAXCOLOR);
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				fprintf(fo, "%d %d %d\n", image[i][j].r, image[i][j].g, image[i][j].b);
			}
		}
		fclose(fo);

		end = clock();
		printf("Time taken: %f\n", (double)(end - start) / CLOCKS_PER_SEC);

		return 0;
	}
}
