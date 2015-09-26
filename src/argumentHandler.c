#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "headers/numberList.h"
#include "headers/sorts.h"
#include "headers/staticStrings.h"

struct option longOptions[] = {{"bench", optional_argument, 0,  999}};

void flush(FILE *inputStream){
	char c;
	do{
		fscanf(inputStream, "%c", &c);
	}while(c != '\n' && c != EOF);
}

void trim(char *string){
    string[strcspn(string, "\n")] = '\0';
}

// http://stackoverflow.com/questions/4553012/checking-if-a-file-is-a-directory-or-just-a-file
bool isFile(const char *path){
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

bool filePathInArgs(char *filePath, int argc, char *argv[]){
	for(int i = 1; i < argc; i++){
		if(argv[i][0] != '-' && access(argv[i], F_OK) != -1 ) {
    		memcpy(filePath, argv[i], strlen(argv[i])+1); //including null terminator
    		return true;
    	}
	}
	return false;
} 

bool targetInArgs(int *num, int argc, char *argv[]){
	for(int i = 1; i < argc; i++){
		if(sscanf(argv[i], "%d", num) == 1) {
	    	return true;
	    }
	}
	return false;
}

void askForFilePath(char *filePath, FILE *inputStream){
	do{
		printf(REQUEST_VALID_FILE_PATH);
		fgets(filePath, 260, inputStream);
		if(filePath[0] != '\n' && filePath[strlen(filePath)-1] != '\n') flush(inputStream);
		trim(filePath);
	} while(!isFile(filePath) || access(filePath, F_OK) == -1);
}

//http://stackoverflow.com/questions/4072190/check-if-input-is-integer-type-in-c
void askForTarget(int *num, int argc, char *argv[], FILE *inputStream){
	char term;
	char line[30];
	do{
		printf(REQUEST_INTEGER);
		fgets(line, 30, inputStream);
		if(line[0] != '\n' && line[strlen(line)-1] != '\n') flush(inputStream);
	}while((sscanf(line, "%d%c", num, &term) != 2 || term != '\n'));
}

Sort askForNewSortOption(FILE *inputStream){
	printf(REQUEST_SORT_ALGORITHM);
	char c[3];
	fgets(c, 3, inputStream);
	if(c[0] != '\n' && c[strlen(c)-1] != '\n') flush(inputStream);
    return (memchr(SHORT_ARGUMENTS, c[1], strlen(SHORT_ARGUMENTS)) != NULL) ? c[1] : -1;
}

void getFilePath(char *filePath, int argc, char *argv[]){
	if(filePathInArgs(filePath, argc, argv));
	else askForFilePath(filePath, stdin);
}

Sort getSortOption(int argc, char *argv[]){
	Sort option = getopt(argc, argv, SHORT_ARGUMENTS);
	while(option == UNDEFINED){
		option = askForNewSortOption(stdin);
	}
	return option;
}

int getTarget(int argc, char *argv[]){
	int num;
	if(targetInArgs(&num, argc, argv));
	else askForTarget(&num, argc, argv, stdin);
	return num;
}

int benchmarkFlag(int argc, char *argv[], int *argument){
	int c = getopt_long(argc, argv, SHORT_ARGUMENTS, longOptions, 0);
	optind=1; //resetting getopt
	if(c == 999 && optarg != NULL)
		sscanf(optarg, "%d", argument);
	return c == 999 ? 1 : 0;
}