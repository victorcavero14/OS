#include <stdio.h>
#include <stdlib.h>
#include <err.h>

int main(int argc, char* argv[]) {
	FILE* file=NULL;
	long unsigned int bytes;

	if (argc!=3) {
		fprintf(stderr,"Usage: %s <file_name>\n",argv[0]);
		exit(1);
	}

	/* Open file */
	if ((file = fopen(argv[1], "r")) == NULL)
		err(2,"The input file %s could not be opened",argv[1]);

	if((bytes = atoi(argv[2])) < 1) err(2, "Invalid number %s", argv[2]);

	char data[bytes];

	while(fread(&data,bytes,1,file) == 1)
	{
		fwrite(&data,bytes,1,stdout);
	}

	/* Read file byte by byte */

	//while ((c = getc(file)) != EOF) {
		/* Print byte to stdout */ /*
		ret=putc((unsigned char) c, stdout);

		if (ret==EOF){
			fclose(file);
			err(3,"putc() failed!!");
		}
	}
	*/




	fclose(file);
	return 0;
}
