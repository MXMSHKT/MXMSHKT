#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

char* fileToString(FILE* input, int* stringLength);
char** stringToPtr(char* string, int stringLength, int* arrayLength);
char** ptrArrayCopy(char** ptrArray, int arrayLength);
int stringCmp    (const void* a, const void* b);
int stringBackCmp(const void* a, const void* b);
int printArrayOfStrings(int arrayLength, char** array, FILE* output);
int countSymbolsInFile(FILE* input);

/*!
 	\function
	Main function.
	sort, backsort and write out poem from file;
*/
int main()
    {
        FILE* input = fopen("text.txt", "r");
        assert(input != NULL);
        char* originalPoem = NULL;
        char** originalLinkedPoem = NULL;
        char** sortedLinkedPoem = NULL;
        char** backSortedLinkedPoem = NULL;
        int strLength = 0;
        int arrayLength = 0;
        int i = 0;
        originalPoem = fileToString(input, &strLength);

        originalLinkedPoem = stringToPtr(originalPoem, strLength, &arrayLength);
        sortedLinkedPoem =  ptrArrayCopy(originalLinkedPoem, arrayLength);
        backSortedLinkedPoem = ptrArrayCopy(originalLinkedPoem, arrayLength);

        qsort(sortedLinkedPoem, arrayLength, sizeof(char*), stringCmp);
        qsort(backSortedLinkedPoem, arrayLength, sizeof(char*), stringBackCmp);

        fclose(input);
        FILE* output = fopen ("sorted.txt", "w");

        printArrayOfStrings(arrayLength, sortedLinkedPoem,     output);
        printArrayOfStrings(arrayLength, backSortedLinkedPoem, output);
        printArrayOfStrings(arrayLength, originalLinkedPoem,   output);

        fclose(output);
        free(backSortedLinkedPoem);
        free(sortedLinkedPoem);
        free(originalLinkedPoem);
    }

/*!
	\function
	Function that reads text from file and puts it to the string
 	\param file
	Pointer to the file with poem
	\param stringLength
	Pointer to the variable where length of the final string will be stored;
	\return returns string made from file
*/
char* fileToString(FILE* input, int* stringLength)
    {
        assert(input != NULL);
        assert(stringLength != NULL);

        *stringLength = countSymbolsInFile(input);
        char* string = (char*)calloc(*stringLength, sizeof(char));
        fread(string, *stringLength, 1, input);

        return string;
    }
/*!
	\function This function translates string with "\n" symbols to pointer array 
	\param string String that should be translated
	\param stringLength length of the string that should be translated
	\param arrayLength pointer to the variable where array length should be stored;
*/

char** stringToPtr(char* string, int stringLength, int* arrayLength)
    {
        assert(string!=NULL);
        assert(stringLength >= 0);
        assert(arrayLength!=NULL);

        int i = 0;
        int iterator = 0;
        char** ptrArray = {};

        for(i = 0; i < stringLength; i++)
            if(string[i] == '\n' || string[i] == '\0')
                iterator++;
        *arrayLength = iterator;
        ptrArray = (char**)calloc(*arrayLength, sizeof(char*));

        iterator = 0;
        ptrArray[0] = string;
        for(i = 0; i < stringLength; i++)
            {
                if(string[i] == '\n')
                    {
                        ptrArray[iterator++] = &string[i+1];
                        string[i] = '\0';
                    }
            }
        return ptrArray;
    }
/*!
	\function Copyes pointer array
	\param ptrArray pointer array that should be copied;
	\param arrayLength length of the array
*/
char** ptrArrayCopy(char** ptrArray, int arrayLength)
    {
        assert(ptrArray != NULL);
        assert(arrayLength >= 0);

        char** returned = (char**)calloc(arrayLength, sizeof(char*));
        int i = 0;
        for(i = 0; i < arrayLength; i++)
            {
                returned[i] = ptrArray[i];
            }
        return returned;
    }
/*!
	\function Comparator for qsort
*/
int stringCmp(const void* a, const void* b)
    {
        assert(a != NULL);
        assert(b != NULL);

        const char* pa = *(const char**)a;
        const char* pb = *(const char**)b;
        return strcasecmp(pa, pb);
    }

int stringBackCmp(const void* a, const void* b)
    {
        assert(a != NULL);
        assert(b != NULL);

        const char* pa = *(const char**)a;
        const char* pb = *(const char**)b;
        int aLen = strlen(pa);
        int bLen = strlen(pb);

        while((pa[aLen] == pb[bLen] || ispunct(pa[aLen]) || ispunct(pb[bLen])) && aLen >= 0 && bLen >= 0)
            {
                if(ispunct(pa[aLen]) || pa[aLen] == ' ')
                    aLen--;
                else if(ispunct(pb[bLen]) || pb[bLen] == ' ')
                    bLen--;
                else
                    {
                        aLen--;
                        bLen--;
                    }
            }
        return(aLen - bLen);
    }

int printArrayOfStrings(int arrayLength, char** array, FILE* output)
    {
        assert(array != NULL);
        assert(arrayLength >= 0);
        int i = 0;
        for(i = 0; i < arrayLength; i++)
            if(array[i][0] != '\0')
                fprintf(output, "%s\n", array[i]);
    }

int countSymbolsInFile(FILE* input)
    {
        assert(input != NULL);

        fseek(input, 0L, SEEK_END);
        int length = ftell(input);
        fseek(input, 0L, SEEK_SET);
        return length;
    }
