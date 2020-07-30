/*compilation command
gcc mathfs.c -o mathfs `pkg-config fuse --cflags --libs`*/


#ifndef MATHFS_H
#define MATHFS_H 1

#include <stdlib.h>
#include <string.h>
#include <float.h>

#include "defs.h"

#define NUM_DIRECTS 7
#define BUFF_SIZE 1024

int toInt(double x)
{
	return (int)x;
}

typedef struct{
	const char * dirName;
	char * (*funcPtr)(char *, double, double);
	int argCount;
} Entry;.


Entry * getEntry(int i){
	Entry * newEntry = (Entry *) malloc(sizeof(Entry));
	newEntry->dirName = directories[i];
	newEntry->argCount = 0;
	return newEntry;
}


int new_dir(struct stat * st){
	st->st_mode = S_IFDIR | 0755;
	st->st_nlink = 2;
	return 0;
}

int new_file(struct stat * st, size_t sz){
	st->st_mode = S_IFREG | 0444;
	st->st_nlink = 1;
	st->st_size = sz;
	return 0;
}

int tokenize(char ** tokens, const char * path){
	char * str;
	int i = 0;
	const char s[2] = "/";

	/*char temp[200];
	strcpy(temp , path); this doesn't work because temp is a local variable*/

	str = strtok(path, s);    // strtok breaks the string into pieces
	while(str != NULL){
		if(i > 3){
			break;
		}
		tokens[i++] = str;
		str = strtok(NULL, s);
	}
	return i;
}

int is_number(char * string){

	char ch;
	int len, flag;

	ch = string[0];

	if(!( ch == '-' || ch == '.' || (ch >= '0' && ch <= '9')))
		return -1;


	len = strlen(string);
	flag = 0;

	for(int i = 1 ; i < len ; ++i){
		ch = string[i];
		if(ch == '.')
        {
			if(flag == 1)
				return -1;

			flag = 1;
		}

		else if(!(ch >= '0' && ch <= '9'))
			return -1;

	}



	return 0;
}

void cleanup(Entry **array){

	for(int i = 0 ; i < NUM_DIRECTS ; ++i){
		free(array[i]);
	}
	return;
}

// converts number to a string and adds '\n' at the end
char * doubleToString(char * dest, double num){
	snprintf(dest, BUFF_SIZE, "%g", num);
	dest[strlen(dest)] = '\n';
	return &dest[0];
}


char * factor(char * res, double x, double y) // y is useless as the factor uses only one number unlike "add" or "sub"
{

	char temp[100];
	res[0] = '\0';

	int num = toInt(x);

	if(x != num || x < 0){
		memcpy(res, numberFormatException, strlen(numberFormatException));
		return &res[0];
	}

	if(x == 0){
		res[0] = '\n';
		return &res[0];
	}else if(x == 1){
		snprintf(res, BUFF_SIZE, "%d\n", 1);
		return &res[0];
	}


	int len;

	for (int i = 1 ; i<= num ; i++) // loop through all the numbers to find the factors
    {
		if ( num % i == 0 ) {

			if(*res == '\0')
				snprintf(res, BUFF_SIZE, "%d\n", i );


            else {
				snprintf(temp, BUFF_SIZE, "%d\n", i ); // add the number and '\n' for every factor
				len = strlen(res);
				strncat(res, temp, BUFF_SIZE-len);
			}
		}

	}
	return &res[0];
}




char * fib(char * res, double x, double y) //  y is useless
{

	int * fibonnaciNumbers;
	char temp[128];
	int len, i;
	if(x != toInt(x) || x < 0){
		memcpy(res, numberFormatException, strlen(numberFormatException));
		return &res[0];
	}

	fibonnaciNumbers = malloc(x * sizeof(double));
	if(x == 0){
		res[0] = '\n';
		return &res[0];
	}else if(x == 1){
		fibonnaciNumbers[0] = 0;
		snprintf(res, BUFF_SIZE, "%d\n", fibonnaciNumbers[0]);
		return &res[0];
	}
	fibonnaciNumbers[0] = 0;
	fibonnaciNumbers[1] = 1.0;
	snprintf(res, BUFF_SIZE, "%d\n", fibonnaciNumbers[0]);
	snprintf(temp, BUFF_SIZE, "%d\n", fibonnaciNumbers[1]);
	len = strlen(res);
	strncat(res, temp, BUFF_SIZE-len);

	for (i = 2 ; i < x ; i++){
		fibonnaciNumbers[i] = fibonnaciNumbers[i-1] + fibonnaciNumbers[i-2];
		snprintf(temp, BUFF_SIZE, "%d\n", fibonnaciNumbers[i]);
		len = strlen(res);
		strncat(res, temp, BUFF_SIZE-len);
	}
	free(fibonnaciNumbers);
	return &res[0];
}




char * add(char * res, double x, double y){
	double sum = x + y;

	if(sum-x != y){
		memcpy(res, overflowException, strlen(overflowException));
		return &res[0];
	}
	return doubleToString(res, sum);
}




char * sub(char * res, double x, double y){
	double dif = x - y;
	if(dif + y != x){
		memcpy(res, overflowException, strlen(overflowException));
		return &res[0];
	}
	return doubleToString(res, dif);
}



char * mul(char * res, double x, double y){
	double prod = x*y;
	if(prod/y != x){
		memcpy(res, overflowException, strlen(overflowException));
		return &res[0];
	}
	return doubleToString(res, prod);
}



char * divide(char * res, double x, double y){
	if(y == 0){
		memcpy(res, divideByZeroException, strlen(divideByZeroException));
		return &res[0];
	}
	return doubleToString(res, x/y);
}


char * expo(char * res, double x, double y){

	double curr , ans;
	if(x == 0){
		if(y < 0){
			memcpy(res, divideByZeroException, strlen(divideByZeroException));
			return &res[0];
		}
		ans = 0;
	}
	else if(y == 0){
		ans = 1;
	}else if(y > 0){
		ans = x;
		for(int i = 1 ; i < y ; ++i){
			curr = ans;
			ans = ans*x;
			if(ans/x != curr){
				memcpy(res, overflowException, strlen(overflowException));
				return &res[0];
			}
		}
	}else{
		ans = 1/x;
		for(int i = -1 ; i > y ; --i){
			ans = ans/x;
		}
	}
	return doubleToString(res, ans);
}



void fill_table(Entry **array){
	int i;
	for(i = 0 ; i < NUM_DIRECTS ; ++i){
		array[i] = getEntry(i);
	}

	// sorry pk!!
	array[0]->funcPtr = &factor;
	array[0]->argCount = 1;

	array[1]->funcPtr = &fib;
	array[1]->argCount = 1;

	array[2]->funcPtr = &add;
	array[2]->argCount = 2;

	array[3]->funcPtr = &sub;
	array[3]->argCount = 2;

	array[4]->funcPtr = &mul;
	array[4]->argCount = 2;

	array[5]->funcPtr = &divide;
	array[5]->argCount = 2;

	array[6]->funcPtr = &expo;
	array[6]->argCount = 2;
}

#endif /* mathfs.h */
