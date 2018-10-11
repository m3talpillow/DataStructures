// Sean Szumlanski
// COP 3502, Spring 2017

// =============================
// Kindred Spirits: testcase16.c
// =============================
// Boundary check to make sure your difficultyRating() and hoursSpent()
// functions are implemented correctly.


#include <stdio.h>
#include "KindredSpirits.h"

int main(void)
{
	int success = 1;

	if (difficultyRating() < 1.0 || difficultyRating() > 5.0)
		success = 0;
	if (hoursSpent() <= 0.0)
		success = 0;

	printf("%s\n", success ? "Success!" : "fail whale :(");

	return 0;
}
