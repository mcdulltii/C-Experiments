#include <stdio.h>
#include <stdarg.h>
#include <cstring>

bool is_printable( char c )
{
    return !( c & (1 << 7) );
}

void c(char *buf, ...)
{
	va_list args;
	va_start (args, buf);     /* Initialize the argument args. */

	char arg = va_arg(args, int);
	
	while( arg ) {
		sprintf(buf, "%s%c", buf, arg);
		arg = va_arg(args, int);
	}

	va_end (args);                  /* Clean up. */
}


int main(int argc, char const *argv[])
{
	char str[512] = {0};
	c(str, 'E','x','a','m','p', 'l', 'e', '\0');

	printf("The word is: %s\n", str);
	printf("It's size is: %zu\n", strlen(str));

	return 0;
}
