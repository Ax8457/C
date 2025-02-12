/* 
	BMP tool developed during CTF  
	Docu : https://www.fil.univ-lille.fr/~sedoglav/C/steganographie.pdf
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BMPHEADERSIZE 54 //BMP headerlen
#define FIELDSNUMBER 15 // Number of field in BMP header

//struct to store data related to each field of BMP Header
typedef struct {
    	char name[20];       
    	unsigned char *data; 
    	size_t size;         
} HeaderEntry;
	
//Hashmap to store headers and bytes linked 
typedef struct {
    	HeaderEntry *headers; //1 struct Headerentry for each field of header
    	size_t N; // number of fields     
} HeaderDictionary;

//extract and print headers
char* extract_BMPheaders(FILE* bmpFile, size_t headerLen){
	
	// list of name and size corelated to fields to complete structs 
	const char *headerFields[] = { //15 fields for 54 bytes
        "Identifier", "FileSize", "Reserved", "DataOffset", "HeaderSize",
        "Width", "Height", "Planes", "BitsPerPixels", "Compression",
        "BitmapDataSize", "HResolution", "VResolution", "Colors", "ImportantColors"
    	};
    	const int headerSizes[] = {2, 4, 4, 4, 4, 4, 4, 2, 2, 4, 4, 4, 4, 4, 4};
    	
    	//read and store headers bytes 
	unsigned char * buffer = malloc(headerLen);
	if (buffer == NULL) {
        	perror("[x] Error allocating memory.\n");
        	return NULL;
    	}
    	size_t bytesRead = fread(buffer, 1, headerLen, bmpFile);
    	if (bytesRead != headerLen) {
        	perror("[x] Error reading bytes.\n");
        	free(buffer);
        	return NULL;
    	}
    	
    	//create a dict
    	HeaderDictionary dict;
    	dict.N = FIELDSNUMBER;
    	dict.headers = malloc(dict.N * sizeof(HeaderEntry));
    	if (dict.headers == NULL) {
        	perror("[x] Error allocating memory for dictionary.\n");
        	free(buffer);
        	return NULL;
    	}
	
	size_t offset = 0; //adjust dynamicaly offset to properly parse header to 54 bytes 
    	for (size_t i = 0; i < dict.N; i++) {
        	strcpy(dict.headers[i].name, headerFields[i]); //field name
        	dict.headers[i].size = headerSizes[i]; // field size 
        	dict.headers[i].data = malloc(headerSizes[i]); // allocate memory
        	
        	//handle errors 
        	if (dict.headers[i].data == NULL) {
            		perror("[x] Error allocating memory for fields data.\n");
            		for (size_t j = 0; j < i; j++) {
                		free(dict.headers[j].data);
            		}
            		free(dict.headers);
            		free(buffer);
            		return NULL;
        	}
        	
        	//populate dict with correct bytes 
		memcpy(dict.headers[i].data, buffer + offset, headerSizes[i]); // Bytes extracted are copied in the wright entry
    		offset += headerSizes[i]; //dynamicaly adjust offset	
	}
	//print results 
	printf("=============================\n"); // /!\ TO READ BYTES => little endian format 
    	printf("==> BMP Header Dictionary <==\n");
    	printf("=============================\n");
    	for (size_t i = 0; i < dict.N; i++) {
        	printf("%s: ", dict.headers[i].name);
        		for (size_t j = 0; j < dict.headers[i].size; j++) {
            			printf("%02X ", dict.headers[i].data[j]);
        		}
        	printf("\n");
    	}
    	printf("=============================\n");
	return "";
}

int main(int argc, char* argv[]){
	if (argc != 2) {
        	fprintf(stderr,"/!\ Usage: %s <Bitmap File>\n", argv[0]);
        	return -1;
    	}
    	
    	//open file 
	FILE *file = fopen(argv[1], "rb");
    	if (file == NULL) {
        	perror("[x] Error while opening file.\n");
        	return -1;
    	}
    	
    	char* str = extract_BMPheaders(file, BMPHEADERSIZE);
    	
}

