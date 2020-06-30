#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

// Buffer overflow middle canary
#define KEY_LEN 6
// Buffer overflow endpoint canary
#define OVR_LEN 100

// Read flag.txt
// Exploit length to xor_sub with
//#define XPL_LEN 54
// Egghunter exploit
//unsigned char exploit[] = "\x11\x00\x11\x00\x11\x00\x11\x00"
//"\x31\xc9\x31\xc0\x31\xd2\xb0\x05\x52\x68\x2e\x74\x78\x74\x68\x66\x6c\x61\x67\x89\xe3\xcd\x80\x50\x53\x51\x58\x59\x5b\xb0\x03\x66\xba\xff\x0f\x42\xcd\x80\x50\x52\x58\x5a\x31\xc0\xb0\x04\xb3\x01\xcd\x80\x50\x53\x58\x5b";

// Print flag
// Exploit length to xor_sub with
#define XPL_LEN 76
// Egghunter exploit
unsigned char exploit[] = "\x11\x00\x11\x00\x11\x00\x11\x00"
// Original
//"\xe9\x1e\x00\x00\x00\xb8\x04\x00\x00\x00\xbb\x01\x00\x00\x00\x59\xba\x24\x00\x00\x00\xcd\x80\xb8\x01\x00\x00\x00\xbb\x00\x00\x00\x00\xcd\x80\xe8\xdd\xff\xff\xff\x33\x67\x67\x5f\x48\x75\x6e\x37\x31\x6e\x67\x5f\x31\x35\x6e\x37\x5f\x6d\x79\x5f\x66\x30\x72\x37\x33\x5f\x31\x32\x33\x38\x38\x34\x37\x39\x33\x0a";
// Encoded
"\xe2\x31\x3b\x3b\x3b\x93\x37\x3b\x3b\x3b\x94\x3a\x3b\x3b\x3b\x72\x95\x17\x3b\x3b\x3b\xfe\xbb\x93\x3a\x3b\x3b\x3b\x94\x3b\x3b\x3b\x3b\xfe\xbb\xe3\xee\xd0\xd0\xd0\x0c\x58\x58\x70\x83\x46\x61\x08\x0a\x61\x58\x70\x0a\x06\x61\x08\x70\x5e\x52\x70\x59\x0b\x4d\x08\x0c\x70\x0a\x0d\x0c\x13\x13\x07\x08\x12\x0c\x45";

// Canary pointers
char key[KEY_LEN], overflow[OVR_LEN];
// Randomize canary values
void calc_canary() {
    // Randomize endpoint canary values with time
    srand(time(NULL));
    for (int i=0; i<OVR_LEN; i++) {
        overflow[i] = rand();
    }

    // 'Randomize' middle canary values by even-odd seconds
    long double k = 3.95, P = 0.02;
    time_t t;
    int seed = 3840 + 124 * (time(&t)%2);
    srand(seed);
    // Use seeded random to generate logistic equation
    for (int i=0; i<rand(); i++) {
        P = k * P * (1 - P);
    }
    // Store logistic equation limits as canary values
    for (int i=0; i<KEY_LEN; i++) {
        P = k * P * (1 - P);
        key[i] = (char)(int)(P * 100);
    }
}

void vuln() {
    // Endpoint canary
    char canary[OVR_LEN];

    // Wrong xor value stored
    char xor_sub[2] = "\x39"; // \x35

    // Force reboot shellcode
    unsigned char shellcode[34] = "\x31\xc0\x31\xdb\x31\xc9\x99\xb0\x58\xbb\xad\xde\xe1\xfe\xb9\x69\x19\x12\x28\xba\x67\x45\x23\x01\xcd\x80";

    // Egghunter shellcode
    // "\x31\xc9\xeb\x05\x66\x81\xc9\xff\x0f\x41\x6a\x43\x58\xcd\x80\x3c\xf2\x74\xf1\xb8\x11\x00\x11\x00\x89\xcf\xaf\x75\xec\xaf\x75\xe9\xff\xe7"

    // Middle canary
    char canary2[KEY_LEN];
    // Buffer
    char print_input[58];

    // Store canary values with generated canary
    memcpy(canary, overflow, OVR_LEN);
    memcpy(canary2, key, KEY_LEN);

    puts("Tell me something interesting?");
    // Buffer overflow vulnerability
    scanf("%s", print_input);

    // Check for stack smashing
    if (memcmp(canary, overflow, OVR_LEN)||memcmp(canary2, key, KEY_LEN)) {
        printf("*** Stack Smashing Detected *** : Canary Value Corrupt!\n");
        exit(-1);
    }

    fflush(stdout);

    // XOR exploit array with overflow value
    for (int i=0; i<XPL_LEN; i++) {
        exploit[i+8] -= 6;
        exploit[i+8] ^= xor_sub[0];
    }

    puts("I didn't like that...");
    sleep(1);

    // Run reboot shellcode
    int (*ret)() = (int(*)())shellcode;
    ret();
}

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);
    calc_canary();
    vuln();
    return 0;
}
