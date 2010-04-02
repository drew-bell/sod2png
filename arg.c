#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <limits.h>
#include "arg.h"

void null_options(argo *opts) {
	// Null out all the options
	opts->no_arrows = false;
	opts->no_numbers = false;
	opts->no_Start_mark = false;
	opts->sequential_images = false;
	opts->width = 0;
	opts->height = 0;
	opts->num_of_files = 0;
}

// check to see if the file already exists
int file_exists(char *file) {
	if (0 != access(file,R_OK)) {
		return 0;
	}
	return 1;
}

// determine if the file is a jpeg based on the extention
int is_svg(char *file) {
	char *ext;
	if ((ext = strrchr(file, '.')))	{
		if ((strcasecmp(ext,".svg") == 0)) {	
			return 1;// if it is an image, return true
		}
	}
	return 0;	// if not a jpg, return false
}

void process_args(char **argv,int argc, argo *opts) {
	
	// makesure the arguments are at default levels
	null_options(opts);
	
	int c;
	
	// the long options for the program
	static struct option long_options[] = {
		/* These options set a flag.   */
		{"no-arrows", no_argument, NULL, 'a'},
		{"no-numbers", no_argument, NULL, 'n'},
		{"no-startmark", no_argument, NULL, 'm'},
		/* These options don't set a flag.
		 We distinguish them by their indices.  */
		{"width", required_argument, NULL, 'w'},
		{"height", required_argument, NULL, 'h'},
		{"sequence", no_argument, NULL, 's'},
		{NULL, 0, NULL, 0}
	};
	
	// parse the options and set the values in the options structure
	while ((c = getopt_long(argc, argv, "namsw:h:", long_options, NULL)) != -1)
		switch (c) {
			case 'w':
				opts->width = atoi(optarg);
				break;
			case 'h':
				opts->height = atoi(optarg);
				break;
			case 's':
				opts->sequential_images = true;
				break;
			case 'n':
				opts->no_numbers = true;
				break;
			case 'm':
				opts->no_Start_mark = true;
				break;
			case 'a':
				opts->no_arrows = true;
				break;
			default:
				break;
		}

	int m = optind;
	int file_count = 0;
	char file[FILENAME_MAX];
	/* Print any remaining command line arguments (not options). */
	if (optind < argc) {
		while (optind < argc) {
			strcpy(file,argv[optind++]);
			if (file_exists(file)) 
				if (is_svg(file)) {
					file_count++;
				}
		}
	}
	
	printf("%i\n",file_count);	file_count = 0;
	optind = m;

	char filesnames[file_count+1][FILENAME_MAX];
	if (optind < argc) {
		while (optind < argc) {
			strcpy(file,argv[optind++]);
			if (file_exists(file)) 
				if (is_svg(file)) {
					strcpy(filesnames[file_count++],file);
					printf("File :%s\n",file);
				}
		}
	}

}