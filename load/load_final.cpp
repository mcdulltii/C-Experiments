#include <iostream>
#include <fstream>
#include <array>
#include <string>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <bits/stdc++.h>
// Load start and end binary addresses
extern unsigned char* _start;
extern unsigned char* __etext;

// Load first hide string function using 0x49 XOR
template<typename Char>
static const Char se = 0x49;
template<typename Char,
         typename std::basic_string<Char>::size_type Length>
struct obfuscated_string
{
    using String = std::basic_string<Char>;
    const std::array<const Char, Length> storage;
    operator String() const
    {
        String s{storage.data(), Length};
        for (auto& c: s)
            c ^= se<Char>;
        return s;
    }
};
template<typename ctype, ctype...st>
constexpr obfuscated_string<ctype, sizeof... (st)> operator ""_hidden()
{
    return { { (st ^ se<ctype>)... } };
}

// Load second hide string function using 0x26 XOR
template<typename Char>
static const Char sf = 0x26;
template<typename Char,
         typename std::basic_string<Char>::size_type Length>
struct obfuscated_string2
{
    using String = std::basic_string<Char>;
    const std::array<const Char, Length> storage;
    operator String() const
    {
        String s{storage.data(), Length};
        for (auto& c: s)
            c ^= sf<Char>;
        return s;
    }
};
template<typename ctype, ctype...st>
constexpr obfuscated_string2<ctype, sizeof... (st)> operator ""_hidden2()
{
    return { { (st ^ sf<ctype>)... } };
}

void __attribute__((constructor)) flag (void);

// Detect for breakpoints (minus one to avoid false positives)
int dbp() {
    int co = 0;
    char* st = (char*)&_start;
    char* en = (char*)&__etext;
    while (st != en) {
        if (((*(volatile unsigned *)st) & 0xFF) == 0xCC) {
            ++co;
        }
        ++st;
    }
    return co-1;
}

using namespace std;

// Pipe for loaded function
struct S {
    string a, b;
    int i;
};

// Execute CLI commands
int ex(string cm) {
    FILE* pp = popen(cm.c_str(), "r");
    if (!pp) return 1;
    pclose(pp);
    return 0;
}

// Create tmp.cpp and tmplib.so files for loaded functions
void cr(string pl) {
    static const auto me = "/tmp/tmp.cpp"_hidden;
    string tF = me;
    ofstream f ( tF );
    f << pl;
    f.close();
    
    static const auto mes = "/usr/bin/gcc -shared /tmp/tmp.cpp -o /tmp/libtmp.so; rm /tmp/tmp.cpp"_hidden;
    string cm = mes;
    if (ex ( cm )) puts("Incorrect");
}

// Load function from libtmp.so
void * lo() {
    static const auto me = "/tmp/libtmp.so"_hidden;
    string cm = me;
    void * fL = dlopen ( cm.c_str(), RTLD_LAZY );
    if ( !fL ) {
        puts("Incorrect");
    }
    return fL;
}

// Remove libtmp.so file
void re() {
    static const auto me = "rm /tmp/libtmp.so"_hidden;
    string cm = me;
    if (ex ( cm )) puts("Incorrect");
}

// Prints prompt
void flag(void) {
    printf("Guess the flag?\n");
}

// Create function payload for string compare function
int main ( int argc, char **argv ) {
    int co = dbp();
    if (co != 0) {
        puts("Incorrect");
        return 0;
    }

    static const auto me = "\
        #include<stdlib.h>\n\
        #include<string>\n\
        struct S {std::string a,b;int i;};\n\
        extern \"C\" void F(S &s) {\n\
            if ((s.a.length()==0||s.b.length()==0)||s.a.compare(s.b)!=0||s.a.length()!=s.b.length()){\n\
                s.i=1;\n\
                return;\n\
            }\n\
            s.i=0;}\n\
        "_hidden;
    string pl = me;
    cr(pl);

    void * fL = lo();
    if ( fL ) {
        int ( *fn ) ( S & ) = (int (*)( S & )) dlsym ( fL, "F" );

        if ( fn ) {
            S s;
            static const auto ar = "Wh3r35_my_f2nc710n_at_8495729728405"_hidden2;
            s.a = ar;
            getline(cin,s.b);
            fn(s);
            if (s.i) {
                puts("Incorrect");
            } else {
                puts("Correct");
            }
        }
        dlclose ( fL );
    }
    
    re();
    return 0;
}
