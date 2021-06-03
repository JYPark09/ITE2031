#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINE_LENGTH 1000
typedef char string_t[MAX_LINE_LENGTH];

int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);

typedef union
{
    unsigned int code;

    struct
    {
        unsigned int destReg : 3;
        unsigned int unused1 : 13;
        unsigned int regB : 3;
        unsigned int regA : 3;
        unsigned int opcode : 3;
        unsigned int unused0 : 7;
    } r;

    struct
    {
        int offset : 16;
        unsigned int regB : 3;
        unsigned int regA : 3;
        unsigned int opcode : 3;
        unsigned int unused : 7;
    } i;

    struct
    {
        unsigned int unused1 : 16;
        unsigned int regB : 3;
        unsigned int regA : 3;
        unsigned int opcode : 3;
        unsigned int unused0 : 7;
    } j;

    struct
    {
        unsigned int unused1 : 22;
        unsigned int opcode : 3;
        unsigned int unused0 : 7;
    } o;
} inst_t;

int main(int argc, char *argv[])
{
    char *inFileString;
    char *outFileString;
    FILE *inFilePtr;
    FILE *outFilePtr;
    string_t label, opcode, arg0, arg1, arg2;

    if (argc != 3)
    {
        printf("error: usage: %s <assembly-code-file> <machine-code-file>\n", argv[0]);
        exit(1);
    }

    inFileString = argv[1];
    outFileString = argv[2];

    if ((inFilePtr = fopen(inFileString, "r")) == NULL)
    {
        printf("error in opening %s\n", inFileString);
        exit(1);
    }

    if ((outFilePtr = fopen(outFileString, "w")) == NULL)
    {
        printf("error in opening %s\n", outFileString);
        exit(1);
    }

    // here is an example for how to use readAndParse to read a line from inFilePtr.
    if (!readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2))
    {
        // reached end of file.
    }

    // this is how to rewind the file ptr so that you start reading from the beginning of the file.
    rewind(inFilePtr);

    // after doing a readAndParse, you may want to do the following to test the opcode
    if (!strcmp(opcode, "add"))
    {
        // do whatever you need to do for opcode "add".
    }

    return 0;
}

/*
 * Read and parse a line of the assembly-language file. Fields are returned
 * in label, opcode, arg0, arg1, arg2 (these strings must have memory already
 * allocated to them).
 *
 * Return values:
 *     0 if reached end of file
 *     1 if all went well
 *
 * exit(1) if line is too long.
 */
int readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0, char *arg1, char *arg2)
{
    string_t line;
    char *ptr = line;

    // delete prior values.
    label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

    // read the line from the assembly-language file.
    if (fgets(line, MAX_LINE_LENGTH, inFilePtr) == NULL)
    {
        // reached end of file.
        return 0;
    }

    // check for line too long (by looking for a \n).
    if (strchr(line, '\n') == NULL)
    {
        // line too long.
        printf("error: line too long\n");
        exit(1);
    }

    // is there a label?
    ptr = line;
    if (sscanf(ptr, "%[^\t\n\r ]", label))
    {
        // successfully read label; advance pointer over the label.
        ptr += strlen(label);
    }

    /*
     * Parse the rest of the line. Would be nice to have real regular
     * expressions, but scanf will suffice.
     */
    sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]", opcode, arg0, arg1, arg2);
    return 1;
}

// return 1 if string is a number.
int isNumber(char *string)
{
    int i;

    return (sscanf(string, "%d", &i) == 1);
}
