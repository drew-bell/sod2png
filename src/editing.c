#include <stdio.h>
#include "editing.h"
#include "types.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#pragma mark Prototypes
static void rm_part(xmlNode* a_node, char *node_type);

#pragma mark Functions

/**************************************************************/
/*** Delete a node from the document based on the node type ***/
/**************************************************************/
void rm_part(xmlNode* a_node, char *node_type) {

	// Create a xmlNode for use
    xmlNode *cur_node = NULL;

	// Step through teh xml nodes
	for (cur_node = a_node; cur_node; cur_node = cur_node->next) {

		// Check the current node is an element and not a space
        if (cur_node->type == XML_ELEMENT_NODE) {
	
			// Look at the type of node found and check if it is what we are looking for
			if (!strcmp((char*)cur_node->name,node_type)) {

				// remove the node from the document structure
				xmlUnlinkNode(cur_node);
			}
		}

		// Search recurrsively
        rm_part(cur_node->children,node_type);
    }
}

/**************************************************************/
/* CURRENTLY UNUSED */
/**************************************************************/
void print_EL_names(xmlNode* a_node) {
    xmlNode *cur_node = NULL;
	
    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            printf("node type: Element, name: %s\n", cur_node->name);
        }
		
        print_EL_names(cur_node->children);
    }
}

/**************************************************************/
/*** Process the options entered at the command line        ***/
/**************************************************************/
void process_xml_options(xmlDocPtr doc, argo opts) {

	// A pointer to the root element of the file
	xmlNode *root_EL = NULL;

	// Get the root element
	root_EL = xmlDocGetRootElement(doc);
	
	// Check for sequential images output
	if (opts->sequential_images) {

		// create sequential images from the svg document
		create_sequential_images(root_EL, opts);
		
		// later removing all 
		opts->no_arrows = true;
		opts->no_numbers = true;
		opts->no_Start_mark = true;
	}
	
	if (opts->no_arrows) {
		rm_part(root_EL,"polyline");
	}
		
	if (opts->no_numbers) {
		rm_part(root_EL,"text");
	}
	
	if (opts->no_Start_mark) {
		rm_part(root_EL,"circle");
	}

	
}

void setup_basic_doc(xmlDocPtr ND) {
	
	// Node pointers for the new doc
	xmlNodePtr ND_Rnode = NULL;

	// DTD pointer
	xmlDtdPtr dtd = NULL;

	// Create the root node
	ND_Rnode = xmlNewNode(NULL, BAD_CAST "svg");

	// Set Attributes for the node
	xmlNewProp(ND_Rnode, BAD_CAST "version", BAD_CAST "1.1");
	xmlNewProp(ND_Rnode, BAD_CAST "id", BAD_CAST "Layer_1");
	xmlNewProp(ND_Rnode, BAD_CAST "xmlns", BAD_CAST "http://www.w3.org/2000/svg");
	xmlNewProp(ND_Rnode, BAD_CAST "xmlns:xlink", BAD_CAST "http://www.w3.org/1999/xlink");
	xmlNewProp(ND_Rnode, BAD_CAST "x", BAD_CAST "0px");
	xmlNewProp(ND_Rnode, BAD_CAST "y", BAD_CAST "0px");
	xmlNewProp(ND_Rnode, BAD_CAST "width", BAD_CAST "1000px");
	xmlNewProp(ND_Rnode, BAD_CAST "height", BAD_CAST "1000px");
	xmlNewProp(ND_Rnode, BAD_CAST "viewBox", BAD_CAST "0 0 1000 1000");
	xmlNewProp(ND_Rnode, BAD_CAST "enable-background", BAD_CAST "new 0 0 1000 1000");
	xmlNewProp(ND_Rnode, BAD_CAST "xml:space", BAD_CAST "preserve");
	
	// Set the node to the documents root
	xmlDocSetRootElement(ND, ND_Rnode);
	
	// Add the <!DOCTYPE info>
	dtd = xmlCreateIntSubset(ND, BAD_CAST "svg", BAD_CAST "-//W3C//DTD SVG 1.1//EN", BAD_CAST "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd");	
}


void create_sequential_images(xmlNode *OD_Rnode, argo opts) {

	// A counter for how many files in the sequence have been output
	int n = 1;

	// make the output file pretty
	xmlIndentTreeOutput = 1;

	// Document pointer to the memory document to be built
	xmlDocPtr ND = NULL;

	// Node pointers for the new doc
	xmlNodePtr ND_Rnode = NULL, node_copy = NULL;

	// Create document and a node
	ND = xmlNewDoc(BAD_CAST "1.0");

	// Setup the basic output document
	setup_basic_doc(ND);

	// Get the root node for the new document
	ND_Rnode = xmlDocGetRootElement(ND);
	
	// Start parsing the original file
	xmlNode *cur_node = NULL;
	cur_node = OD_Rnode->children;
	cur_node = cur_node->next;

	char kanji[FILENAME_MAX];
	
	strncpy(kanji,opts->svg_file,strlen(opts->svg_file)-4);
	
	for (cur_node = cur_node->next; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {

			// Remove all the arrows, text and startmarks in the new doc to this point
			rm_part(ND_Rnode,"polyline");
			rm_part(ND_Rnode,"text");
			rm_part(ND_Rnode,"circle");
			change_fill_colour(ND_Rnode,"path");

			// Copy the current node
			node_copy = xmlCopyNode(cur_node, 1);
			
			// add the copy to the New Document
			xmlAddChild(ND_Rnode, node_copy);
			
			// Make an image
			push_out_image(ND,opts,n,kanji);

			// increase the filename number
			n++;
		}
    }
}

void dump_tmp(xmlDocPtr ND) {
	
	// File handle for the temp output file
	FILE *tf;
	
	// Create a tmp intermediary file
	tf = fopen("/tmp/svg2png_tmpfile","w");
	if (NULL == tf) {
		perror("/tmp/svg2png_tmpfile");
		exit(1);
	}
	
	// output the altered file
	xmlDocDump(tf,ND);

	// close the temp file
	fclose(tf);
}

char* out_file_string(argo opts, int n) {
	
	// If the is no output filename set, set the output to the same as the input
		char kanji[FILENAME_MAX];
		
		// copy the input file name minus the last 4 chars (extention) 
		// **** This will fail if the input file has a 4 char extention!!!***
		strncpy (kanji, opts->svg_file, strlen (opts->svg_file)-4);
			
	if (NULL == opts->out_file) {

		// malloc the memory for the var and assign it
		opts->out_file = (char*)malloc (FILENAME_MAX);
		
		// Append the outputformat extention
		strncat (kanji, "_out", 4);
		strncat (kanji, opts->out_format, 4);
		strncpy (opts->out_file, kanji, FILENAME_MAX);
	}

	if (opts->sequential_images) {
	// Create the output image name
	sprintf(opts->out_file, "%s stroke %d_%s", kanji, n, opts->out_format);

	}
}

void push_out_image(xmlDocPtr ND, argo opts, int n, char *kanji) {

	// Output file name
	char *filename;

	//File name handles
	FILE *output_format, *svg;
	
	// Dump the tmp file
	dump_tmp(ND);

	filename = out_file_string (opts,n);

	// open file for writing
	output_format = fopen(filename,"w");
	if (output_format == NULL) {
		perror(filename);
		exit(1);
	}
	
	// open the temp file for conversion to png
	svg = fopen("/tmp/svg2png_tmpfile","r");
	if (svg == NULL) {
		perror("/tmp/svg2png_tmpfile");
		exit(1);
	}	
	// Pointer to function
	svg_cairo_status_t (*render_functptr)(FILE*,FILE*,double,int,int) = NULL;
	
	// point the render function to the correct one
	render_functptr = get_render_function(filename);
	
	// render the output file
	render_functptr(svg, output_format, 1.0, opts->width, opts->height);

	// close all files
	fclose(svg);
	fclose(output_format);
	
	// delete the temp file
	unlink("/tmp/svg2png_tmpfile");
}

void change_fill_colour(xmlNode *a_node, char *node_type) {
	xmlNode *cur_node = NULL;
	for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
			if (!strcmp((char*)cur_node->name,node_type)) {
				
				// remove any property called "fill"
				xmlUnsetProp(cur_node,(unsigned char*)"fill");
				
				// set a fill colour for the node
				xmlNewProp(cur_node, BAD_CAST "fill", BAD_CAST "#333333");
			}
		}
        change_fill_colour(cur_node->children,node_type);
    }	
}
