//
// Created by Kapilan M on 26/05/2018.
// Purpose: to read the binary file input from path that is passed as a parameter
#include <stdio.h>
#include <stdlib.h>


#define NO_OF_REGS 16
#define MEMSIZE 65536

//open the file
FILE *openFile(char *path)
{
    FILE *file = fopen(path, "rb");
    return file;
}

//to check if file exists or is null
int fileExists(char *path)
{
    FILE *file = openFile(path);
    if (!file)
    {
        return 1;
    }
    fclose(file);
    return 0;
}


//to get the size of the file
long unsigned int getSize(FILE *file)
{
    fseek(file, 0, SEEK_END);
    long unsigned int size = ftell(file);
    fseek(file, 0, SEEK_SET);
    return size;
}

//this function is INCOMPLETE
void loadFile(char *path) {

    FILE *bin = 0;
    bin = openFile(path);
    long unsigned int size = 0;

    if (fileExists(path) == 1) {
        fprintf(stderr, "Error opening file.\n");
        exit(EXIT_FAILURE);
    }

    size = getSize(bin) + 1;
    //prevent memory overflow
    if (size > MEMSIZE) {
        fprintf(stderr, "Emulator memory overflow\n");
        exit(EXIT_FAILURE);
    }
}