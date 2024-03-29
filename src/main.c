#include <stdio.h>
#include "headers/numberList.h"
#include "headers/sorts.h"
#include "headers/searches.h"
#include "headers/argumentHandler.h"
#include "headers/staticStrings.h"
#include "headers/staticNumbers.h"
#include "headers/benchmark.h"
#include "headers/streamReader.h"

void printResult(int result, int target, NumberList *numbers) {
	if (result == -1) {
		printf(NUMBER_NOT_FOUND, target);
	} else {
		printf(NUMBER_FOUND, target, result, target, numbers->array[result].originalIndex);
	}
}

void excecuteMainTask (int argc, char *argv[]) {
	NumberList numbers;
	initArray(&numbers, ARRAY_INIT_LENGTH);
	char filePath[MAX_PATH_LENGTH];
	getFilePath(filePath, argc, argv);
	readIntsFromFile(filePath, &numbers);

	Sort option = getSortOption(argc, argv);
	printf(SORTING_STARTED);
	sort(option, &numbers);
	printf(SORTING_FINISHED);

	int target = getTarget(argc, argv);
	int result = binarySearch(&numbers, target);
	printResult(result, target, &numbers);

	deInitArray(&numbers);
}

int main(int argc, char *argv[]){
	int benchSize = 1000;
	if(benchmarkFlag(argc, argv, &benchSize)){
		runBenchmark(benchSize);
	} else if (helperFlag(argc, argv)) {
		printf(HELPER_TEXT);
	} else {
		 excecuteMainTask(argc, argv);
	}
  	return 0;
}