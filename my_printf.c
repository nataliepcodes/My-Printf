#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdint.h> // to ensure that the entire memory address is printed for ptr value
#define OCTAL 8 //Digits from 0 to 7, only non-negative
#define DECIMAL 10 //Digits from 0 to 9, positive and negative values, including 0
#define HEXDECIMAL 16 //Digits from 0 to 9, and letters A to F (10-15), only non-negative


void my_putchar(char str)
{
    write(1, &str, 1);
}


void my_putstr(char *str)
{
    int i = 0;
    while(str[i] != '\0')
    {
        my_putchar(str[i]);
        i++;
    }
   // my_putchar('\n');
}


int my_strlen(char* str)
{
    int i = 0;
    while (str[i] != '\0')
    {
        i++;
    }
    return i;
}


void my_reverse_str(char *str)
{
    int len = my_strlen(str);
    for (int i = len -1; i >= 0; i--)
    {
        my_putchar(str[i]);
    }
}


int my_printchar(char character)
{
    int len = 1;

    my_putchar(character);

    return len;
}


int my_printstring(char* str)
{
    int len = 0;
    if (str == NULL)
    {
        my_putstr("(null)");
        len = 6; // length of the string "(null)"
    } 
    else
    {
        len = my_strlen(str);
        int i = 0;
        while (i < len)
        {
            my_putchar(str[i]);
            i++;
        }
    }

    return len;
}


/* Converting an integer number into a character string and output the chars using my_putchar function, the function returns the number of characters that were outputted */
int my_printd(int number)
{
    char str[100];
    int i = 0;
    int len = 0;

    if (number == 0)
    {
        str[i++] = '0';
    }

    // Checking if number is negative and output '-' to the first position of the string
    if (number < 0)
    {
        my_putchar('-');
        // Converting the number to a positive value
        number = -number;
        // Increment the length for the '-' character
        len++;
    }

    while (number > 0)
    {
        str[i++] = '0' + (number % DECIMAL);// '0' is 48 in ASCII 
        number = number / DECIMAL;
    }
    str[i] = '\0';

    my_reverse_str(str);

    len += my_strlen(str);
    return len;
}


/* Converting decimal to octal */
int my_printo(unsigned int number)
{
    char str[100];
    int i = 0;

    while (number > 0)
    {
        str[i++] = '0' + (number % OCTAL);// e.g. for 334 , the remainder of 334 % 8 is 6; '0' aka 48 + 6 is ASCII 54 - decimal 6
        number = number / OCTAL; // e.g for 334, the division of 334 / 8 is 41 - goes to while loop
    }
    str[i] = '\0';

    my_reverse_str(str);

    int len = my_strlen(str);
    return len;
}


int my_printu(unsigned int number)
{
    char str[100];

    int i = 0;

    while (number > 0)
    {
        str[i++] = '0' + (number % DECIMAL);
        number = number / DECIMAL;
    }
    str[i] = '\0';

    my_reverse_str(str);

    int len = my_strlen(str);

    return len;

}


/* Converting the decimal number to hexadecimal representation */
int my_printhex(unsigned long number, char ch)
{
    char hex_str[100];
    char hex_digits[] = "0123456789ABCDEF";
    int i = 0;
    int len = 0;

    while (number > 0)
    {
        hex_str[i++] = hex_digits[number % HEXDECIMAL];
        number /= HEXDECIMAL;
    }
    hex_str[i] = '\0';

    len = my_strlen(hex_str);

    //my_reverse_str(hex_str);

    for (int i = len - 1; i >= 0; i--)
    {
        if (hex_str[i] >= 65 && hex_str[i] <= 90 && ch == 'p')
        {
             my_putchar(hex_str[i] + 32);
        }
        else
        {
            my_putchar(hex_str[i]);
        }
    }
    return len;
}


/* Casting the pointer to uintptr_t, printing its hexdec value preceded by 0x, and returning len of the string */
int my_printptr(void *ptr)
{
    // Cast the void pointer to uintptr_t
    intptr_t ptr_value = (intptr_t) ptr;
    my_putstr("0x");

    // Return the length of the string
    int len = 2 + my_printhex(ptr_value, 'p');//(sizeof(intptr_t) * 2) + 2; // should be always 18 characters (2 for "0x" and 16 for the hexadecimal value)
    return len;
}


int my_printf(char * format_str, ...)
{
    int i = 0;
    int len = 0;

    // Define a va_list variable named args
	va_list args;

    // Begin the argument list with the va_start - macros, act as markers to mark where the arg list starts
    va_start(args, format_str);

    while(format_str[i] != '\0')
    {
        if (format_str[i] == '%')
        {
            i++;
            switch (format_str[i])
            {
                case 'd':
                    len += my_printd(va_arg(args, int)); 
                    break;
                case 'o': 
                    len += my_printo(va_arg(args, unsigned int));
                    break;
                case 'u':
                    len += my_printu(va_arg(args, unsigned int));
                    break;
                case 'x':
                    len += my_printhex(va_arg(args, unsigned long), 'x');
                    break;
                case 'p':
                    len += my_printptr(va_arg(args, intptr_t *)); 
                    break;
                case 'c':
                    len += my_printchar(va_arg(args, int)); // a second argument is the type that is expected to be received ; 'char' changes to 'int' in va_arg()
                    break;
                case 's':
                    len += my_printstring(va_arg(args, char*));
                    break;
                default:
                    my_printchar(format_str[i]);
                    len++;
            }
        }
        else
        {
            my_printchar(format_str[i]);
            len++;
        }
        i++;
    }

    va_end(args);
    
    return len;
}


int main()
{
    int count = my_printf("string: %s\nchar: %c\nint: %d\nneg_int: %d\no: %o\nu: %u\nx: %x\npointer: %p\n", "Hello world!", 'X', 5, -5, 334, 100, 4, &count);
    my_printf("%s!\n", NULL); 
    my_printf("14 -- %x!\n", 14); // E

    int variable = 12;
    my_printf(" my_printf-- var 12 p:%p!\n", &variable);  
    my_printf("1337 %d!\n", 1337); 

    int len = my_printf("%d - %d - %d!\n", 2048, 0, -1337); 
    my_printf("%u!\n", 1337); 

    return 0;
}


