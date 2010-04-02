#include <stdbool.h>
#include <limits.h>

typedef struct options {
	bool no_arrows;
	bool no_numbers;
	bool no_Start_mark;
	bool sequential_images;
	int width;
	int height;
	int num_of_files;
	char *filenames[FILENAME_MAX];
} argo;

void process_args(char **argv,int argc, argo *opts);