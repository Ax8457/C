#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <stdint.h>
#define COUNT 183

/* This script is built to parse eventX files on linux-based OS and focuses on type1 (Key pressed) of the input_event struct below : 

struct input_event {
        struct timeval time;
        __u16 type;
        __u16 code;
        __s32 value;
};

This script parses file given in parameter and prints the key corresponding to any event according the keymap below. It also prints event's time.
/!\ The keymap used below is adapted to the AZERTY Keyboard /!\

*/

const char* keymap[COUNT] = {
    [0] = "RESERVED",
    [1] = "ESC",
    [2] = "&",
    [3] = "é",
    [4] = "BACKSLASH",
    [5] = "'",
    [6] = "(",
    [7] = "-",
    [8] = "è",
    [9] = "_",
    [10] = "ç",
    [11] = "à",
    [12] = ")",
    [13] = "=",
    [14] = "BACKSPACE",
    [15] = "TAB",
    [16] = "a",
    [17] = "z",
    [18] = "e",
    [19] = "r",
    [20] = "t",
    [21] = "y",
    [22] = "u",
    [23] = "i",
    [24] = "o",
    [25] = "p",
    [26] = "^",
    [27] = "$",
    [28] = "ENTER",
    [29] = "LEFTCTRL",
    [30] = "q",
    [31] = "s",
    [32] = "d",
    [33] = "f",
    [34] = "g",
    [35] = "h",
    [36] = "j",
    [37] = "k",
    [38] = "l",
    [39] = "m",
    [40] = "ù",
    [41] = "*",
    [42] = "LEFTSHIFT",
    [43] = "<",
    [44] = "w",
    [45] = "x",
    [46] = "c",
    [47] = "v",
    [48] = "b",
    [49] = "n",
    [50] = ",",
    [51] = ";",
    [52] = ":",
    [53] = "!",
    [54] = "RIGHTSHIFT",
    [55] = "ASTERISK",
    [56] = "LEFTALT",
    [57] = "SPACE",
    [58] = "CAPSLOCK",
    [59] = "F1",
    [60] = "F2",
    [61] = "F3",
    [62] = "F4",
    [63] = "F5",
    [64] = "F6",
    [65] = "F7",
    [66] = "F8",
    [67] = "F9",
    [68] = "F10",
    [69] = "NUMLOCK",
    [70] = "SCROLLLOCK",
    [71] = "7",
    [72] = "8",
    [73] = "9",
    [74] = "-",
    [75] = "4",
    [76] = "5",
    [77] = "6",
    [78] = "+",
    [79] = "1",
    [80] = "2",
    [81] = "3",
    [82] = "0",
    [83] = ".",
    [85] = "ZENKAKUHANKAKU",
    [86] = "102ND",
    [87] = "F11",
    [88] = "F12",
    [89] = "RO",
    [90] = "KATAKANA",
    [91] = "HIRAGANA",
    [92] = "HENKAN",
    [93] = "KATAKANAHIRAGANA",
    [94] = "MUHENKAN",
    [95] = "JPCOMMA",
    [96] = "ENTER",
    [97] = "RIGHTCTRL",
    [98] = "/",
    [99] = "SYSRQ",
    [100] = "RIGHTALT",
    [101] = "LINEFEED",
    [102] = "HOME",
    [103] = "UP",
    [104] = "PAGEUP",
    [105] = "LEFT",
    [106] = "RIGHT",
    [107] = "END",
    [108] = "DOWN",
    [109] = "PAGEDOWN",
    [110] = "INSERT",
    [111] = "DELETE",
    [112] = "MACRO",
    [113] = "MUTE",
    [114] = "VOLUMEDOWN",
    [115] = "VOLUMEUP",
    [116] = "POWER",
    [117] = "EQUAL",
    [118] = "PLUSMINUS",
    [119] = "PAUSE",
    [120] = "SCALE",
    [121] = "COMMA",
    [122] = "HANGEUL",
    [123] = "HANJA",
    [124] = "YEN",
    [125] = "LEFTMETA",
    [126] = "RIGHTMETA",
    [127] = "COMPOSE",
    [128] = "STOP",
    [129] = "AGAIN",
    [130] = "PROPS",
    [131] = "UNDO",
    [132] = "FRONT",
    [133] = "COPY",
    [134] = "OPEN",
    [135] = "PASTE",
    [136] = "FIND",
    [137] = "CUT",
    [138] = "HELP",
    [139] = "MENU",
    [140] = "CALC",
    [141] = "SETUP",
    [142] = "SLEEP",
    [143] = "WAKEUP",
    [144] = "FILE",
    [145] = "SENDFILE",
    [146] = "DELETEFILE",
    [147] = "XFER",
    [148] = "PROG1",
    [149] = "PROG2",
    [150] = "WWW",
    [151] = "MSDOS",
    [152] = "COFFEE",
    [153] = "DIRECTION",
    [154] = "CYCLEWINDOWS",
    [155] = "MAIL",
    [156] = "BOOKMARKS",
    [157] = "COMPUTER",
    [158] = "BACK",
    [159] = "FORWARD",
    [160] = "CLOSECD",
    [161] = "EJECTCD",
    [162] = "EJECTCLOSECD",
    [163] = "NEXTSONG",
    [174] = "EXIT",
    [175] = "MOVE",
    [176] = "EDIT",
    [177] = "SCROLLUP",
    [178] = "SCROLLDOWN",
    [179] = "(",
    [180] = ")",
    [181] = "NEW",
    [182] = "REDO",
};
 
//functions 
void printKeyName(uint16_t k) {
    if (k >= 0 && k < COUNT) {
        printf( "%s \n",keymap[k]);   
    } else {
        printf("Unknown key");
    }
}

void print_timeval(struct timeval tv) {
    struct tm *tm_info;
    tm_info = localtime(&tv.tv_sec);
    char buffer[30];
    strftime(buffer, 30, "%Y-%m-%d %H:%M:%S | ", tm_info);
    printf("%s", buffer);
}

//main
int main(int argc, char *argv[]) {
    if (argc < 2){
    	printf("Usage: %s <event file to parse> \n", argv[0]);
    	printf("%s -h for help \n", argv[0]);
    	return -1;
    	}
    if (argc > 3){
    	printf("Usage: %s <event file to parse> \n", argv[0]);
    	printf("%s -h for help \n", argv[0]);
    	return -1;
    	}
    	
    if (strcmp(argv[1],"-h")==0){ printf("Options:\n -all : to print code & time for all types of key (0,1 & 4) while parsing the file. \n -h : to get help.\n"); return -1; }	
    if (argc == 3 && strcmp(argv[2],"-h")==0){ printf("Options:\n -all : to print code & time for all types of key (0,1 & 4) while parsing the file. \n -h : to get help.\n"); return -1; }
    if (argc == 2 && strncmp(argv[1], "-", 1) == 0 && strcmp(argv[1],"-h") != 0){ printf("Invalid option, -h to get help\n"); return -1; }
    if (argc == 3 && strncmp(argv[2], "-", 1) == 0 && strcmp(argv[2],"-all") != 0){ printf("Invalid option, -h to get help\n"); return -1; }
    if (argc == 3 && strcmp(argv[2],"-all") != 0){ printf("Invalid parameter, -h to get help\n"); return -1; }
    
    
    int file_descriptor = open(argv[1] , O_RDONLY);
    if (file_descriptor == -1) {
        perror("file openning error");
        return -1;
    }
    
    struct input_event event;
    
    while (1) {
        ssize_t n = read(file_descriptor, (void *) &event, sizeof(struct input_event));
        if (n == (ssize_t)-1) {
            perror("Error while reading");
            close(file_descriptor);
            return -1;
        } else if (n != sizeof(struct input_event)) {
            fprintf(stderr, "Error: Wrong size \n");
            close(file_descriptor);
            return -1;
        }

        uint16_t keycode = event.code;
        uint16_t keytype = event.type;
        
       //-all option enabled
       if ( argc == 3 && strcmp(argv[2],"-all")==0 ){
	        if (file_descriptor == -1) {
			perror("file openning error");
			return -1;
    		} else{
    			printf("Type: %u | ", keytype);
    			print_timeval(event.time);
        		printf(" => ");
        		printKeyName(keycode);
    		}
       } else if (keytype == 1) {
        	print_timeval(event.time);
        	printf(" => ");
        	printKeyName(keycode);
       }
    }
    close(file_descriptor);
    return 0;
}
