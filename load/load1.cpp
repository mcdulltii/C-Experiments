#include <iostream>
#include <fstream>
#include <dlfcn.h>
#include <stdlib.h>

struct S {
    int a,b;
};

using namespace std;

void create(string payload) {
    ofstream f ( "tmp.cpp" );
    f << payload;
    f.close();

    system ( "/usr/bin/gcc -shared tmp.cpp -o libtmp.so;rm tmp.cpp" );
}

void * load() {
    void * fLib = dlopen ( "./libtmp.so", RTLD_LAZY );
    if ( !fLib ) {
        cerr << "Cannot open library: " << dlerror() << '\n';
    }
    return fLib;
}

void remove() {
    system ( "rm libtmp.so" );
}

int main ( int argc, char **argv ) {
    string payload = "#include<stdlib.h>\n\
                      struct S {int a,b;};\n\
                      extern \"C\" void F(S &s) { s.a += s.a; s.b *= s.b; }\n";
    create(payload);

    void * fLib = load();
    if ( fLib ) {
        int ( *fn ) ( S & ) = (int (*)( S & )) dlsym ( fLib, "F" );

        if ( fn ) {
            for(int i=0;i<11;i++) {
                S s;
                s.a = i;
                s.b = i;
                fn(s);
                cout << s.a << " " << s.b << endl;
            }
        }
        dlclose ( fLib );
    }
    
    remove();
    return 0;
}
