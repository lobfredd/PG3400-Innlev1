// https://www.youtube.com/watch?v=aQiWF4E8flQ
#include "../headers/number.h"
#include "../headers/sortUtilities.h"

void q_sort(int count, Number *numbers){
	if(count < 2) return;

	Number pivot = numbers[count-1];
	int wall = -1;
	for(int i = 0; i < count-1; i++){
		if(numbers[i].value <= pivot.value){
			swap(&numbers[++wall], &numbers[i]);
		} 
	}
	swap(&numbers[++wall], &numbers[count-1]); //Place pivot in final place

	int rightCount = count - wall;
	int leftCount = count - rightCount;
	q_sort(leftCount, numbers); //Sort left side of pivot
	q_sort(rightCount, &numbers[leftCount]); //Sort right side of pivot
} 

void quicksort(NumberList *numbers){
	q_sort(numbers->count, numbers->array);
}

