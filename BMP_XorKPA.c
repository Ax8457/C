#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//struct
typedef struct {
	char* filetype;
	char* headerhexvalue;
	int headerlen;
} BMPstruct;

//functions

void HexCharToBinChar(char hex,char *bin) {
	switch(hex) {
		case '0': strcpy(bin, "0000"); break;
		case '1': strcpy(bin, "0001"); break;
		case '2': strcpy(bin, "0010"); break;
		case '3': strcpy(bin, "0011"); break;
		case '4': strcpy(bin, "0100"); break;
		case '5': strcpy(bin, "0101"); break;
		case '6': strcpy(bin, "0110"); break;
		case '7': strcpy(bin, "0111"); break;
		case '8': strcpy(bin, "1000"); break;
		case '9': strcpy(bin, "1001"); break;
		case 'A': case 'a': strcpy(bin, "1010"); break;
		case 'B': case 'b': strcpy(bin, "1011"); break;
		case 'C': case 'c': strcpy(bin, "1100"); break;
		case 'D': case 'd': strcpy(bin, "1101"); break;
		case 'E': case 'e': strcpy(bin, "1110"); break;
		case 'F': case 'f': strcpy(bin, "1111"); break;
		default: printf("error, caracter is not valid. \n"); strcpy(bin, "?") ; break; 
	}
}


char * StrHex_To_StrBinary(const char * hex_str) {
	size_t len = strlen(hex_str);
	if (len % 2 != 0) {
		printf("Wrong size, the string isn't hex value\n");
		return NULL;
	}
	char * binary_str = malloc(4 * len + 1);
	char * bin = malloc(4) ;
	for (int i = 0; i < len; i++) {
		HexCharToBinChar(hex_str[i], bin); 
		strcat(binary_str, bin);
	}  	
	return  binary_str;
	free(binary_str); free(bin);
}

char * EncryptedFileHeader_Parser(FILE* file, BMPstruct* bmpstruct){
	unsigned char * buffer = malloc(bmpstruct->headerlen);
	if (!buffer) {
        	perror("malloc error\n");
        	fclose(file);
        	return NULL;
    	}   	
    	fread(buffer, 1,bmpstruct->headerlen , file);
    	char * hexString = malloc(bmpstruct->headerlen + 1);
    	for (int i = 0; i < bmpstruct->headerlen ; i++) {
        	sprintf(&hexString[i*2], "%02X", buffer[i]);
    	}
   	return hexString;
}

size_t FileSize (FILE* file){
	 fseek(file, 0, SEEK_END);
	 size_t file_size =  ftell(file);
	 return file_size;
}

char * LittleEndianPermute(char * str){
	 size_t len = strlen(str);
	 size_t num = len / 2;
	 char * result = malloc(len+1);
	 for (size_t i = 0; i < num ; i++) {
	 	result[2 * (num - i - 1)] = str[2 * i];
         	result[2 * (num - i - 1) + 1] = str[2 * i + 1];
         }
         printf("Little Endian permute :%s\n",result);
         return result;
}


char * FileSizePadding(char * hexFileSize, BMPstruct bmp){
	char * res = malloc((bmp.headerlen-2)* 2);
	res = hexFileSize;
	size_t filesize = strlen(hexFileSize);
	if (filesize % 2 != 0){
		char * prov = malloc((bmp.headerlen-2)* 2);
		prov[0] = '0' ;
		strcat(prov, hexFileSize);
		prov = LittleEndianPermute(prov);
		res = prov;
		
	}
	if (filesize != bmp.headerlen-2){
		char * provbis = malloc((bmp.headerlen-2)* 2);
		provbis = res ;
		for (size_t i = 0; i < bmp.headerlen - filesize + 1; i++){
			strcat(provbis, "0");
		}
		res = provbis;
	}
	printf("Padded File Size in Hex %s\n",res);
	return res ;
}

char * StrXor(char * str1, char * str2){

	size_t size1 = strlen(str1);
	size_t size2 = strlen(str2);
	if (size1 != size2){
		printf("error, sizes mismatches\n");
		return NULL ;
	}
	
	char * res = malloc (size1);
	
	for(size_t i = 0; i < size1; i++){
		if (str1[i] == str2[i]){
			strcat(res, "0");
		} else {
			strcat(res, "1");
		}
	}
	return res;
	
}

char  binary_to_char(char * binary_str) {
    int value = 0;
    for (int i = 0; i < 8; ++i) {
        if (binary_str[i] == '1') {
            value += (1 << (7 - i)); 
        }
    }
    return (char)value;
}


char * binary_to_string( char * binary_str) {
    int len = strlen(binary_str);
    if (len % 8 != 0) {
        printf("Wrong size, the string must be a multiple of 8.\n");
        return NULL;
    }

    int num_chars = len / 8;
    char *result = (char *)malloc(num_chars + 1); 
    for (int i = 0; i < num_chars; ++i) {
        char binary_segment[9]; 
        strncpy(binary_segment, binary_str + (i * 8), 8);
        binary_segment[8] = '\0'; 
        result[i] = binary_to_char(binary_segment);
    }
    result[num_chars] = '\0';

    return result;
}

char * xor_BMP_Key(unsigned char * image_data, size_t size, char * key) {
    int key_len = strlen(key);
    char * res = malloc(size);
    for (int i = 0; i < size; ++i) {
        res[i] = image_data[i] ^ key[i % key_len];
    }
    
    return res;
}

//main
int main(int argc, char* argv[]){
	if (argc != 2) {
        	fprintf(stderr, "Usage: %s <ecrypted file name>\n", argv[0]);
        	return -1;
    	}
    	
    	//struct
    	BMPstruct bmp = {"BMP", NULL, 6};
	bmp.headerhexvalue = malloc(bmp.headerlen*2);
   	strcpy(bmp.headerhexvalue,"424d");
   	
    	// open file
    	FILE *file = fopen(argv[1], "rb");
    	if (file == NULL) {
        	perror("Error while opening file.\n");
        	return -1;
    	}

	//extract first 6 bytes of the encryopted file 
	char * hexHeaderEnc = EncryptedFileHeader_Parser(file,&bmp);
	printf ("Header Encrypted Hex value : %s\n",hexHeaderEnc);
	size_t file_size = FileSize(file);
	printf ("File size : %ld\n",file_size);
	char * hexFileSize = malloc(sizeof(file_size));
	sprintf(hexFileSize, "%0lX", file_size);
	printf("File size in hexa: %s\n", hexFileSize);
	
	//add bytes to header
	hexFileSize = FileSizePadding(hexFileSize, bmp);
	strcat(bmp.headerhexvalue, hexFileSize);
	printf("Padded Header hex value expected %s\n",bmp.headerhexvalue);
	
	//binary conversion 
	char * binary_header1 = StrHex_To_StrBinary(bmp.headerhexvalue);
	char * binary_header2 = StrHex_To_StrBinary(hexHeaderEnc);
	printf("Binary Bmp Header : %s\n",binary_header1);
	printf("Binary Encrypted Header : %s\n",binary_header2); 
	
	char * res = StrXor(binary_header1, binary_header2);
	printf("Possible Key to Decrypt BMP file : %s\n", res);
	
	char * ascii_key = binary_to_string(res);
	printf("Key converted to ASCII: %s\n", ascii_key);
    	
    	//decyrpt BMP image
    	printf("Attemping to decrypt file with the retreived key ['%s']\n",ascii_key);
    	unsigned char * image_data = malloc(file_size);
    	rewind(file);//replace the pointer at the beginning of the file (cf function FileSize)
    	fread(image_data, 1, file_size, file);
    	fclose(file);
    	image_data = xor_BMP_Key(image_data, file_size, ascii_key);
    	FILE *file_dec = fopen("decrypt_bmp.bmp", "wb");
    	fwrite(image_data, 1, file_size, file_dec);
    	fclose(file_dec);
    	if (file_dec){
    		printf("BMP File successfully decrypted. A file had been saved at decrypt_bmp.bmp\n");
    	}else {printf("Error");}
    	return 0;
}
