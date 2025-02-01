# C
My C scripts 

**- EventXParser :** This script is built to parse eventX files on linux-based OS and focuses on type1 of the _input_event_ struct. This script parses file given in parameter and prints the key corresponding to any event according _AZERTY_ keymap. It also prints event's time. 

**- BMP_XorKPA :** This script is built to decrypt _BMP_ file (by attempting a _known plain text attack_) which had been encrypted by doing a simple _Xor_ with a secret key. This script is based on the header structure (6 first bytes) expected for a _BMP_ file and so assumes that the key is a 6 _ASCII_ caracters key. A _bruteforce_ option (consisting in testing for different key lengths, up to 6 bytes) will be added soon. 

**- WPA2_8021X_PMK :** This script is built to retreive PMK key used to encrypt data over WPA2 enterprise network. The attack involves getting a capture file of the traffic between user and AP and a capture file of the traffic between AP and radius server.
