#include <iostream>
#include <fstream>
#include <string>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
extern unsigned char* _start;
extern unsigned char* __etext;

int DetectBP() {
    int count = 0;
    char* start = (char*)&_start;
    char* end = (char*)&__etext;
    while (start != end) {
        if (((*(volatile unsigned *)start) & 0xFF) == 0xCC) {
            ++count;
        }
        ++start;
    }
    return count-1;
}

using namespace std;

struct S {
    string a;
};

int exec(string command) {
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) return 1;
    pclose(pipe);
    return 0;
}

void create(string payload) {
    ofstream f ( "/tmp/tmp.cpp" );
    f << payload;
    f.close();
    
    if (exec ( "/usr/bin/gcc -shared /tmp/tmp.cpp -o /tmp/libtmp.so; rm /tmp/tmp.cpp" )) puts("Incorrect");
}

void * load() {
    void * fLib = dlopen ( "/tmp/libtmp.so", RTLD_LAZY );
    if ( !fLib ) {
        puts("Incorrect");
    }
    return fLib;
}

void remove() {
    if (exec ( "rm /tmp/libtmp.so" )) puts("Incorrect");
}

int main ( int argc, char **argv ) {
    if (DetectBP() > 0) {
        puts("Incorrect");
        return 0;
    }

    string payload = "\
        #include<stdlib.h>\n\
        #include<string>\n\
        using namespace std;\n\
        struct S {string a;};\n\
        extern \"C\" void F(S &s) { s.a = \"Correct\"; }\n\
                      ";
    create(payload);

    void * fLib = load();
    if ( fLib ) {
        int ( *fn ) ( S & ) = (int (*)( S & )) dlsym ( fLib, "F" );

        if ( fn ) {
            S s;
            s.a = "Incorrect";
            fn(s);
            cout << s.a << endl;
        }
        dlclose ( fLib );
    }
    
    remove();
    return 0;
}
