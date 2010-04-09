#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include "arg.h"
#include "editing.h"
#include <limits.h>

int main (int argc, char **argv) {

	FILE *png;

	// A structure to hold all the cli arguments
	argo *opts = malloc(sizeof(opts));

	// Null out all options before using
	null_options(opts);
	
	// create an element to hold the root node
	xmlNode *root_element = NULL;

	// process all the cli options and return them in the structure
	process_args(argv,argc,opts);

	// Do a little testing on the xml libs to check they are OK
	LIBXML_TEST_VERSION

	// Allocate the xmldoc handle
	xmlDocPtr doc;

	// Read the file into memory
	doc = xmlParseFile(opts->svg_file);

	// Get the root element node
	root_element = xmlDocGetRootElement(doc);

	// Pass the first node of the doc in memory for removal of unwanted parts
	process_xml_options(root_element, opts);

	if (NULL == opts->png_file) {
		xmlDocDump(stdout,doc); 
	} else {
	// create an output file handle
	png = fopen(opts->png_file,"w");

	// output the altered file
	xmlDocDump(png,doc);
	}
	/* Clean up */
	xmlCleanupParser();
	xmlMemoryDump();
	xmlFreeDoc(doc);
	
	return (0);

} // main
