// randomizer
// prints random lines from text files
// usage: randomizer [filename] [number of lines to print]
//
// author: Mikko Kärenlampi (mikko.karenlampi@pm.me)
// updated: 9.4.2024
//
// i coded this just to see if i still remember c (i do)

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

int lines = 0;
long *indx = NULL;

int calculate_lines(FILE *f)
{
	assert(f != NULL);

	fseek(f, 0, SEEK_SET);
	int c, lines = 0;
	while ((c = getc(f)) != EOF)
		if (c == '\n') lines++;
	return lines;
}

void build_line_index(FILE *f)
{
	assert(f != NULL);
	assert(lines > 0);

	fseek(f, 0, SEEK_SET);
	indx = (long *)malloc(lines * sizeof(long));
	int c, i = 0;
	indx[0] = 0;
	while ((c = getc(f)) != EOF)
		if (c == '\n') indx[++i] = ftell(f);
}

void print_nth_line(FILE *f, int n)
{
	assert(f != NULL);
	assert(indx != NULL);
	assert(n >= 0 && n < lines);

	fseek(f, indx[n] * sizeof(char), SEEK_SET);
	int c;
	while ((c = getc(f)) != EOF)
	{
		putchar(c);
		if (c == '\n') break;
	}
}

int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		printf("usage: randomizer [filename] [number of lines to print]\n");
		exit(1);
	}

	int n = atoi(argv[2]);
	char *name = argv[1];
	FILE *f = fopen(name, "r");

	if (f == NULL)
	{
		printf("couldn't open file '%s'\n", name);
		exit(2);
	}

	lines = calculate_lines(f);

	if (n <= 0 || n > lines)
	{
		printf("number of lines in file: %d\n", lines);
		exit(3);
	}

	build_line_index(f);

	int *list = (int *)malloc(lines * sizeof(int));
	for (int i = 0; i < lines; i++) list[i] = i;
	srand(time(NULL));
	for (int i = 0; i < n; i++)
	{
		int l = rand() % (lines - i);
		print_nth_line(f, list[l]);
		list[l] = list[lines - i - 1];
	}
	free(list);

	fclose(f);

	return 0;
}
