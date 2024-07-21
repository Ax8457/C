#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void StrHex_To_StrBinary(const char * hex_str) {

	size_t len = strlen(hex_str);
	if (len % 2 != 0) {
		printf("Wrong size, the string isn't hex value\n");
		return ;
	}

	char * binary_str = malloc(4 * len + 1);
	char * bin = malloc(4) ;
	for (int i = 0; i < len; i++) {
		HexCharToBinChar(hex_str[i], bin); 
		strcat(binary_str, bin);
	}  
	printf("Hex value converted : %s\n", binary_str); 
	free(binary_str); free(bin);
	return;
}

//main
int main(int argc, char* argv[]){

	if (argc < 2 || argc > 2 ){
    	printf("Usage: %s <Hex string to convert> \n", argv[0]);
    	return -1;
    	}
    	    	
	const char *str_bytes = argv[1];
	printf("Hex Value: %s\n", str_bytes);
	StrHex_To_StrBinary(str_bytes);
	return 0;
}





















