# C
My C scripts 

**- EventXParser.c :** This script is built to parse eventX files on linux-based OS and focuses on type1 of the _input_event_ struct. This script parses file given in parameter and prints the key corresponding to any event according _AZERTY_ keymap. It also prints event's time.  
**- BMP_XorKPA.c :** This script is built to decrypt _BMP File_ which had been encrypted by doing a simple _Xor_ with a secret key. This script is based on the header structure (6 bytes) expected for a _BMP_ file and assumes that the key is a 6 _ASCII_ caracters key. A _bruteforce_ option (consisting in testing for different key lengths, up to 6) will be added soon.   
