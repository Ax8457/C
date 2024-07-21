# C
My C scripts 

**- EventXParser.c :** This script is built to parse eventX files on linux-based OS and focuses on type1 of the input_event struct below. This script parses file given in parameter and prints the key corresponding to any event according the keymap below. It also prints event's time.
_struct input_event {__
        __struct timeval time;_
        ___u16 type;_
        ___u16 code;_
        __s32 value;
};_
