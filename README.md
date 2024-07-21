# C
My C scripts 

**- EventXParser.c :** This script is built to parse eventX files on linux-based OS and focuses on type1 of the input_event struct below. This script parses file given in parameter and prints the key corresponding to any event according the keymap below. It also prints event's time.
_struct input_event {
        struct timeval time;
        __u16 type;
        __u16 code;
        __s32 value;
};_
