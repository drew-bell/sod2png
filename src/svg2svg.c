
void render_to_svg (FILE *in, FILE *out, double scale, int width, int height) {

	// File handle
	FILE fout;

	// open file for writing
	fout = fopen(out,"w");

	// Dump the file to document
	xmlDocDump(fout, doc);

}
