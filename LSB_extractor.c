/* 
	Tool developed during CTF  
*/
#include <stdio.h>
#include <stdlib.h>

unsigned char * extract_FileBytes(const char *filename, int start_pos, size_t *size) {
    FILE *file = fopen(filename, "rb"); 
    if (!file) {
        perror("[x] Error openning file");
        return NULL;
    }

    // cursor 
    if (fseek(file, start_pos, SEEK_SET) != 0) {
        perror("[x] Cursor error");
        fclose(file);
        return NULL;
    }
    
    //estimate file_size and number of bytes to parse 
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, start_pos, SEEK_SET);
    *size = file_size - start_pos;
    if (*size <= 0) {
        printf("[x] Cursor position out of band\n");
        fclose(file);
        return NULL;
    }

    // allocate memory
    unsigned char * buffer = (unsigned char *)malloc(*size);
    if (!buffer) {
        perror("[x] Error allocating memory");
        fclose(file);
        return NULL;
    }
    
    //store bytes and return a tab
    fread(buffer, 1, *size, file);
    fclose(file);
    return buffer;
}

// Extract LSB
void extract_LSB(unsigned char *bytes, int size) {
    int N = size / 8; //8 bytes packets for 8 LSB = 1 ASCII char
    char Flag[1024] = {0}; //to sotre flag
    int Flag_index = 0;

    for (int i = 0; i < 96; i++) { // can put N to extract every single LSB but here 96 was enough to retreive flag
        unsigned char ascii_char = 0;
        //extract 8 LSBs in each packet of 8 bytes 
        for (int j = 0; j < 8; j++) {
            ascii_char |= ((bytes[i*8 + j] & 1) << (7 - j));
        }

        //print
        printf("[=>] Bytes : ");
        for (int g = 0; g < 8; g++) { // print 8 bytes used to extract 8 LSBs 
            printf("0x%02X ", bytes[i*8 + g]);
        }
        printf("\n");
        printf("[+] Extracted ASCII : %c\n", ascii_char);
        
        // Craft flag
        if (ascii_char >= 32 && ascii_char <= 126) { 
            Flag[Flag_index++] = ascii_char;
        }
    }

    // Extracted message
    printf("\n[+] Message retreived : %s\n", Flag);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <file > <cursor position>\n", argv[0]);
        return 1;
    }
    const char *filename = argv[1];
    int start_pos = atoi(argv[2]);
    size_t size = 0;
    // Bytes
    unsigned char * bytes = extract_FileBytes(filename, start_pos, &size);
    if (!bytes) {
        return 1;
    }
    extract_LSB(bytes, size);
    free(bytes);
    return 0;
}
