#ifndef DEFS_H
#define DEFS_H 1

const char * const directories[] = {
"/factor",
"/fib",
"/add",
"/sub",
"/mul",
"/div",
"/exp"
};

const char * const documents[] = {
					 "/factor/n is a file containing all the non trivial factors of 'n'. Example: /factor/138 acts as a file containing the numbers 2, 3, 23, one per line and ending with a newline."
					,"/fib/n is a file containing first 'n' fibonacci numbers. Example: /fib/3 acts as a file containing the numbers 1, 1, 2, one per line and ending with a newline."
					,"/add/x/y is a file containing the number x+y. Example: /add/5/3 acts as a file containing the number 8 and ending with a newline."
					,"/sub/x/y is a file containing the number x-y. Example: /sub/5/3 acts as a file containing the number 2 and ending with a newline."
					,"/mul/x/y is a file containing the number x*y. Example: /mul/5/3 acts as a file containing the number 15 and ending with a newline."
					,"/div/x/y is a file containing the number x/y. Example: /div/5/3 acts as a file containing the number 1.6666 and ending with a newline."
					,"/exp/x/y is a file containing the number x^y. Example: /add/2/3 acts as a file containing the number 8 and ending with a newline."
};

const char * divideByZeroException = "Divide by zero error.\n";
const char * numberFormatException = "Please enter +ve integers only.\n";
const char * overflowException = "Operating your input is giving overflow.\n";

#endif // defs.h
