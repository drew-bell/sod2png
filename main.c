#include <stdio.h>
#include <stdlib.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include "arg.h"
#include "editing.h"

int main (int argc, char **argv) {
	
	// A structure to hold all the cli arguments
	argo *opts = malloc(sizeof(opts));

	null_options(opts);
	
	xmlNode *root_element = NULL;

	// process all the cli options and return them in the structure
	process_args(argv,argc,opts);

	// Do a little testing on the xml libs to check they are OK
	LIBXML_TEST_VERSION
	
	/*** Example files for testing ***/
	opts->svg_file = "1.svg";
	opts->png_file = "output.png";
	
	// Load the file to memory
	xmlDocPtr doc;
	doc = xmlReadFile(opts->svg_file, NULL, 0);
	
	/*Get the root element node */
    root_element = xmlDocGetRootElement(doc);
	
	// Pass the first node of the doc in memory for removal of unwanted parts
	process_xml_options(root_element, opts);

	// Pass the altered image to the cairo
	xmlSaveFile("tmp.svg",doc);
	// output the filtered file to png
	
	// output some debug info
	if (opts->sequential_images) fprintf(stderr,"opts->sequential_images\n");
	if (opts->width > 0) fprintf(stderr,"opts->width = %i\n",opts->width);
	if (opts->height > 0) fprintf(stderr,"opts->height = %i\n",opts->height);
	if (opts->svg_file != NULL) fprintf(stderr,"opts->svg_file = %s\n",opts->svg_file);
	if (opts->png_file != NULL) fprintf(stderr,"opts->png_file = %s\n",opts->png_file);
	
	/* Clean up */
	xmlCleanupParser();
	xmlMemoryDump();
	xmlFreeDoc(doc);
	
	return (0);

} // main
