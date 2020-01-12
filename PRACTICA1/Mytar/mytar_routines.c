#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "mytar.h"

extern char *use;

/** Copy nBytes bytes from the origin file to the destination file.
 *
 * origin: pointer to the FILE descriptor associated with the origin file
 * destination:  pointer to the FILE descriptor associated with the destination file
 * nBytes: number of bytes to copy
 *
 * Returns the number of bytes actually copied or -1 if an error occured.
 */
int
copynFile(FILE * origin, FILE * destination, int nBytes)
{
	// Complete the function

	int c,contador = 0;

		/* Read file byte by byte */
		while ((c = getc(origin)) != EOF && (contador < nBytes)) {
			/* Print byte to stdout */
			putc(c, destination);
			contador++;
		}

	return contador;
}

/** Loads a string from a file.
 *
 * file: pointer to the FILE descriptor 
 * 
 * The loadstr() function must allocate memory from the heap to store 
 * the contents of the string read from the FILE. 
 * Once the string has been properly built in memory, the function returns
 * the starting address of the string (pointer returned by malloc()) 
 * 
 * Returns: !=NULL if success, NULL if error
 */
char*
loadstr(FILE * file)
{
	// Complete the function
	int n, contador = 0;
	char *buffer;

	while((n!= EOF) && (n!=(int)'\0'))
	{
		n = getc(file);
		contador++;
		if(n == EOF) return NULL;
	}

	buffer = (char*)malloc(contador);
	fseek(file, -contador, SEEK_CUR); //vuelve a la pos inicial
	fread(buffer,1,contador, file);

	return buffer;
}

/** Read tarball header and store it in memory.
 *
 * tarFile: pointer to the tarball's FILE descriptor 
 * nFiles: output parameter. Used to return the number 
 * of files stored in the tarball archive (first 4 bytes of the header).
 *
 * On success it returns the starting memory address of an array that stores 
 * the (name,size) pairs read from the tar file. Upon failure, the function returns NULL.
 */
stHeaderEntry*
readHeader(FILE * tarFile, int *nFiles)
{
	// Complete the function
	int i ;
		stHeaderEntry *stH = NULL;
		if(tarFile == NULL){
			perror("Ha habido un problema con el tarFile");
			return NULL;
			}
		else {
			fread(nFiles, sizeof(int), 1, tarFile);

			stH = (stHeaderEntry *) malloc(sizeof(stHeaderEntry) * (*nFiles));

			for (i = 0; i < (*nFiles); i++) {
				stH[i].name = loadstr(tarFile);
				fread(&stH[i].size, sizeof(int), 1, tarFile);
			}

			return stH;
		}

}

/** Creates a tarball archive 
 *
 * nfiles: number of files to be stored in the tarball
 * filenames: array with the path names of the files to be included in the tarball
 * tarname: name of the tarball archive
 * 
 * On success, it returns EXIT_SUCCESS; upon error it returns EXIT_FAILURE. 
 * (macros defined in stdlib.h).
 *
 * HINTS: First reserve room in the file to store the tarball header.
 * Move the file's position indicator to the data section (skip the header)
 * and dump the contents of the source files (one by one) in the tarball archive. 
 * At the same time, build the representation of the tarball header in memory.
 * Finally, rewind the file's position indicator, write the number of files as well as 
 * the (file name,file size) pairs in the tar archive.
 *
 * Important reminder: to calculate the room needed for the header, a simple sizeof 
 * of stHeaderEntry will not work. Bear in mind that, on disk, file names found in (name,size) 
 * pairs occupy strlen(name)+1 bytes.
 *
 */
int
createTar(int nFiles, char *fileNames[], char tarName[])
{
	// Complete the function
	FILE *tFile, *iFile;
	unsigned headerSize;
	stHeaderEntry* header;

	if(nFiles<=0){
		fprintf (stderr, "%s", use);
		return EXIT_FAILURE;
	}

	tFile= fopen(tarName, "wx");
	headerSize = sizeof(int);
	header = (stHeaderEntry*)malloc(sizeof(stHeaderEntry) *nFiles);

	for(int i=0; i < nFiles; ++i){
		int nameSize = strlen(fileNames[i]) + 1;
		header[i].name = (char*)malloc(nameSize);
		strcpy(header[i].name, fileNames[i]);
		headerSize += nameSize + sizeof(header->size);

	}

	fseek(tFile, headerSize, SEEK_SET);
	for(int i=0; i< nFiles; ++i){
		iFile = fopen(fileNames[i], "r");
		header[i].size = copynFile(iFile, tFile, INT_MAX);
		fclose(iFile);
	}

	rewind(tFile);
	fwrite(&nFiles, sizeof(int),1,tFile);

	for(int i=0; i<nFiles; ++i){
		fwrite(header[i].name, 1, strlen(header[i].name) + 1, tFile);
		fwrite(&(header[i].size), sizeof(header[i].size), 1, tFile);
	}

	fprintf(stdout, "El archivo mtar se ha creado satisfactoriamente\n");

	for(int i=0; i<nFiles;++i){
		free(header[i].name);
	}

	free(header);
	fclose(tFile);

	return EXIT_SUCCESS;
}

/** Extract files stored in a tarball archive
 *
 * tarName: tarball's pathname
 *
 * On success, it returns EXIT_SUCCESS; upon error it returns EXIT_FAILURE. 
 * (macros defined in stdlib.h).
 *
 * HINTS: First load the tarball's header into memory.
 * After reading the header, the file position indicator will be located at the 
 * tarball's data section. By using information from the 
 * header --number of files and (file name, file size) pairs--, extract files 
 * stored in the data section of the tarball.
 *
 */
int
extractTar(char tarName[])
{
	// Complete the function
	FILE *tFile = NULL;
	FILE *dFile = NULL;
	int i;
	unsigned char* c;
	int nFiles;

	stHeaderEntry *stH = NULL;
	//stH = (stHeaderEntry *) malloc(sizeof(stHeaderEntry) * (nFiles));
	tFile = fopen(tarName, "r");

	if(tFile == NULL){
		perror("Error en el archivo tar. Inténtalo de nuevo!");
		return EXIT_FAILURE;
	}

	stH = readHeader(tFile, &nFiles);


	for (i = 0; i < nFiles; i++) {

		dFile = fopen(stH[i].name, "w");
		if(dFile == NULL){
			perror("Ha habido un error con el fichero destino.");
			return EXIT_FAILURE;
		}
		else {
			c = (unsigned char*) malloc(stH[i].size);
			copynFile(tFile,dFile, stH[i].size);
			fclose(dFile);
		}
	}

	for (i = 0; i < nFiles; i++){
		free(stH[i].name);
	}

	free(stH);

	fclose(tFile);

	printf("\nEnhorabuena !! El fichero se acaba de extraer correctamente\n\n");

	return EXIT_SUCCESS;
}

//extension 1: listar ficheros del mtar

int listTar(char tarName[])
{
	int ret = EXIT_SUCCESS, i = 0;
	FILE* fo = fopen(tarName, "r");
	int* nFiles = malloc(sizeof(int));
	stHeaderEntry* stHE;

	if(fo == NULL)
	{
		perror("Error al abrir el archivo");
		ret = EXIT_FAILURE;
	}
	else
	{
		stHE = readHeader(fo,nFiles);

		if(stHE == NULL) ret = EXIT_FAILURE;
		else
		{
			printf("Existen en el .mtar %d archivos.\n\n", *nFiles);

			for(; i < *nFiles; i++)
			{
				printf("Nombre del fichero %d : %s.\n", i,stHE[i].name);
				printf("Tam del fichero %d : %d.\n\n", i,stHE[i].size);
			}

			free(stHE);
		}

		fclose(fo);
	}

	return ret;
}

//extension 2: add fichero de un mtar

int addFile(char tarName[], char fileName[])
{
	int ret = EXIT_SUCCESS, i;
	FILE* fo = fopen(tarName, "r");
	int* nFiles = malloc(sizeof(int));
	stHeaderEntry* stHE;
	stHeaderEntry* stHEn;


	if(fo == NULL)
	{
		perror("Error al abrir el archivo");
		ret = EXIT_FAILURE;
	}
	else
	{
		stHE = readHeader(fo,nFiles);

		if(stHE == NULL) ret = EXIT_FAILURE;
		else
		{
			stHEn = (stHeaderEntry *) malloc(sizeof(stHeaderEntry) * (*nFiles + 1));

			for(i = 0; i < *nFiles; i++)
			{
				stHEn[i].name = stHE[i].name;
				stHEn[i].size = stHE[i].size;
			}
			stHEn[i].name = fileName;
			stHEn[i].size = strlen(fileName); //mal habria que leer el ficheroo para saber su tammm

			*nFiles = *nFiles + 1;

			//Tenemos el Header nuevo faltan por copiar los datos









			free(stHE);
			free(stHEn);
		}

		fclose(fo);
		}

	return ret;
}

//extension 3: eliminar fichero de un mtar

int removeFile(char tarName[], char fileName[])
{
	int ret = EXIT_SUCCESS;



	return ret;
}
