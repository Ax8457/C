#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <stdint.h>
#define COUNT 249

/* This script is built to parse eventX files on linux-based OS and focuses on type1 of the input_event struct below : 

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
    [0] = "KEY_RESERVED",
    [1] = "KEY_ESC",
    [2] = "&",          // QWERTY '1'
    [3] = "é",          // QWERTY '2'
    [4] = "\"",         // QWERTY '3'
    [5] = "'",          // QWERTY '4'
    [6] = "(",          // QWERTY '5'
    [7] = "-",          // QWERTY '6'
    [8] = "è",          // QWERTY '7'
    [9] = "_",          // QWERTY '8'
    [10] = "ç",         // QWERTY '9'
    [11] = "à",         // QWERTY '0'
    [12] = ")",         // QWERTY '-'
    [13] = "=",         // QWERTY '='
    [14] = "KEY_BACKSPACE",
    [15] = "KEY_TAB",
    [16] = "a",         // QWERTY 'q'
    [17] = "z",         // QWERTY 'w'
    [18] = "e",
    [19] = "r",
    [20] = "t",
    [21] = "y",
    [22] = "u",
    [23] = "i",
    [24] = "o",
    [25] = "p",
    [26] = "^",         // QWERTY '['
    [27] = "$",         // QWERTY ']'
    [28] = "KEY_ENTER",
    [29] = "KEY_LEFTCTRL",
    [30] = "q",         // QWERTY 'a'
    [31] = "s",
    [32] = "d",
    [33] = "f",
    [34] = "g",
    [35] = "h",
    [36] = "j",
    [37] = "k",
    [38] = "l",
    [39] = "m",         // QWERTY ';'
    [40] = "ù",         // QWERTY '\''
    [41] = "*",         // QWERTY '`'
    [42] = "KEY_LEFTSHIFT",
    [43] = "<",         // QWERTY '\\'
    [44] = "w",         // QWERTY 'z'
    [45] = "x",
    [46] = "c",
    [47] = "v",
    [48] = "b",
    [49] = "n",
    [50] = ",",         // QWERTY 'm'
    [51] = ";",         // QWERTY ','
    [52] = ":",         // QWERTY '.'
    [53] = "!",         // QWERTY '/'
    [54] = "KEY_RIGHTSHIFT",
    [55] = "KEY_KPASTERISK",
    [56] = "KEY_LEFTALT",
    [57] = " ",         // KEY_SPACE
    [58] = "KEY_CAPSLOCK",
    [59] = "KEY_F1",
    [60] = "KEY_F2",
    [61] = "KEY_F3",
    [62] = "KEY_F4",
    [63] = "KEY_F5",
    [64] = "KEY_F6",
    [65] = "KEY_F7",
    [66] = "KEY_F8",
    [67] = "KEY_F9",
    [68] = "KEY_F10",
    [69] = "KEY_NUMLOCK",
    [70] = "KEY_SCROLLLOCK",
    [71] = "KEY_KP7",
    [72] = "KEY_KP8",
    [73] = "KEY_KP9",
    [74] = "KEY_KPMINUS",
    [75] = "KEY_KP4",
    [76] = "KEY_KP5",
    [77] = "KEY_KP6",
    [78] = "KEY_KPPLUS",
    [79] = "KEY_KP1",
    [80] = "KEY_KP2",
    [81] = "KEY_KP3",
    [82] = "KEY_KP0",
    [83] = "KEY_KPDOT",
    [85] = "KEY_ZENKAKUHANKAKU",
    [86] = "KEY_102ND",
    [87] = "KEY_F11",
    [88] = "KEY_F12",
    [89] = "KEY_RO",
    [90] = "KEY_KATAKANA",
    [91] = "KEY_HIRAGANA",
    [92] = "KEY_HENKAN",
    [93] = "KEY_KATAKANAHIRAGANA",
    [94] = "KEY_MUHENKAN",
    [95] = "KEY_KPJPCOMMA",
    [96] = "KEY_KPENTER",
    [97] = "KEY_RIGHTCTRL",
    [98] = "KEY_KPSLASH",
    [99] = "KEY_SYSRQ",
    [100] = "KEY_RIGHTALT",
    [101] = "KEY_LINEFEED",
    [102] = "KEY_HOME",
    [103] = "KEY_UP",
    [104] = "KEY_PAGEUP",
    [105] = "KEY_LEFT",
    [106] = "KEY_RIGHT",
    [107] = "KEY_END",
    [108] = "KEY_DOWN",
    [109] = "KEY_PAGEDOWN",
    [110] = "KEY_INSERT",
    [111] = "KEY_DELETE",
    [112] = "KEY_MACRO",
    [113] = "KEY_MUTE",
    [114] = "KEY_VOLUMEDOWN",
    [115] = "KEY_VOLUMEUP",
    [116] = "KEY_POWER",
    [117] = "KEY_KPEQUAL",
    [118] = "KEY_KPPLUSMINUS",
    [119] = "KEY_PAUSE",
    [120] = "KEY_SCALE",
    [121] = "KEY_KPCOMMA",
    [122] = "KEY_HANGEUL",
    [123] = "KEY_HANJA",
    [124] = "KEY_YEN",
    [125] = "KEY_LEFTMETA",
    [126] = "KEY_RIGHTMETA",
    [127] = "KEY_COMPOSE",
    [128] = "KEY_STOP",
    [129] = "KEY_AGAIN",
    [130] = "KEY_PROPS",
    [131] = "KEY_UNDO",
    [132] = "KEY_FRONT",
    [133] = "KEY_COPY",
    [134] = "KEY_OPEN",
    [135] = "KEY_PASTE",
    [136] = "KEY_FIND",
    [137] = "KEY_CUT",
    [138] = "KEY_HELP",
    [139] = "KEY_MENU",
    [140] = "KEY_CALC",
    [141] = "KEY_SETUP",
    [142] = "KEY_SLEEP",
    [143] = "KEY_WAKEUP",
    [144] = "KEY_FILE",
    [145] = "KEY_SENDFILE",
    [146] = "KEY_DELETEFILE",
    [147] = "KEY_XFER",
    [148] = "KEY_PROG1",
    [149] = "KEY_PROG2",
    [150] = "KEY_WWW",
    [151] = "KEY_MSDOS",
    [152] = "KEY_COFFEE",
    [153] = "KEY_SCREENLOCK",
    [154] = "KEY_ROTATE_DISPLAY",
    [155] = "KEY_MAIL",
    [156] = "KEY_BOOKMARKS",
    [157] = "KEY_COMPUTER",
    [158] = "KEY_BACK",
    [159] = "KEY_FORWARD",
    [160] = "KEY_CLOSECD",
    [161] = "KEY_EJECTCD",
    [162] = "KEY_EJECTCLOSECD",
    [163] = "KEY_NEXTSONG",
    [164] = "KEY_PLAYPAUSE",
    [165] = "KEY_PREVIOUSSONG",
    [166] = "KEY_STOPCD",
    [167] = "KEY_RECORD",
    [168] = "KEY_REWIND",
    [169] = "KEY_PHONE",
    [170] = "KEY_ISO",
    [171] = "KEY_CONFIG",
    [172] = "KEY_HOMEPAGE",
    [173] = "KEY_REFRESH",
    [174] = "KEY_EXIT",
    [175] = "KEY_MOVE",
    [176] = "KEY_EDIT",
    [177] = "KEY_SCROLLUP",
    [178] = "KEY_SCROLLDOWN",
    [179] = "KEY_KPLEFTPAREN",
    [180] = "KEY_KPRIGHTPAREN",
    [181] = "KEY_NEW",
    [182] = "KEY_REDO",
    [183] = "KEY_F13",
    [184] = "KEY_F14",
    [185] = "KEY_F15",
    [186] = "KEY_F16",
    [187] = "KEY_F17",
    [188] = "KEY_F18",
    [189] = "KEY_F19",
    [190] = "KEY_F20",
    [191] = "KEY_F21",
    [192] = "KEY_F22",
    [193] = "KEY_F23",
    [194] = "KEY_F24",
    [200] = "KEY_PLAYCD",
    [201] = "KEY_PAUSECD",
    [202] = "KEY_PROG3",
    [203] = "KEY_PROG4",
    [204] = "KEY_DASHBOARD",
    [205] = "KEY_SUSPEND",
    [206] = "KEY_CLOSE",
    [207] = "KEY_PLAY",
    [208] = "KEY_FASTFORWARD",
    [209] = "KEY_BASSBOOST",
    [210] = "KEY_PRINT",
    [211] = "KEY_HP",
    [212] = "KEY_CAMERA",
    [213] = "KEY_SOUND",
    [214] = "KEY_QUESTION",
    [215] = "KEY_EMAIL",
    [216] = "KEY_CHAT",
    [217] = "KEY_SEARCH",
    [218] = "KEY_CONNECT",
    [219] = "KEY_FINANCE",
    [220] = "KEY_SPORT",
    [221] = "KEY_SHOP",
    [222] = "KEY_ALTERASE",
    [223] = "KEY_CANCEL",
    [224] = "KEY_BRIGHTNESSDOWN",
    [225] = "KEY_BRIGHTNESSUP",
    [226] = "KEY_MEDIA",
    [227] = "KEY_SWITCHVIDEOMODE",
    [228] = "KEY_KBDILLUMTOGGLE",
    [229] = "KEY_KBDILLUMDOWN",
    [230] = "KEY_KBDILLUMUP",
    [231] = "KEY_SEND",
    [232] = "KEY_REPLY",
    [233] = "KEY_FORWARDMAIL",
    [234] = "KEY_SAVE",
    [235] = "KEY_DOCUMENTS",
    [236] = "KEY_BATTERY",
    [237] = "KEY_BLUETOOTH",
    [238] = "KEY_WLAN",
    [239] = "KEY_UWB",
    [240] = "KEY_UNKNOWN",
    [241] = "KEY_VIDEO_NEXT",
    [242] = "KEY_VIDEO_PREV",
    [243] = "KEY_BRIGHTNESS_CYCLE",
    [244] = "KEY_BRIGHTNESS_AUTO",
    [245] = "KEY_DISPLAY_OFF",
    [246] = "KEY_WWAN",
    [247] = "KEY_RFKILL",
    [248] = "KEY_MICMUTE"
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
