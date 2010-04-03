#include <stdio.h>
#include <stdlib.h>
#include "arg.h"

int main (int argc, char **argv) {
	
	// A structure to hold all the cli arguments
	argo *opts = malloc(sizeof(opts));
	
	// process all the cli options and return them in the structure
	process_args(argv,argc,opts);


	// output somd debug info
	if (opts->no_arrows) fprintf(stderr,"opts->no_arrows\n");
	if (opts->no_numbers) fprintf(stderr,"opts->no_numbers\n");
	if (opts->no_Start_mark) fprintf(stderr,"opts->no_Start_mark\n");
	if (opts->sequential_images) fprintf(stderr,"opts->sequential_images\n");
	if (opts->width > 0) fprintf(stderr,"opts->width = %i\n",opts->width);
	if (opts->height > 0) fprintf(stderr,"opts->height = %i\n",opts->height);
	if (opts->svg_file != NULL) fprintf(stderr,"opts->svg_file = %s\n",opts->svg_file);
	return 0;

} // main
