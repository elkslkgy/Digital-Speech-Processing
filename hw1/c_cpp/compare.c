#include <stdio.h>
#include <string.h>

int main() {
	FILE *fp, *fp1;
	fp = fopen("result1.txt", "r");
	if (fp == NULL) printf("fail");
	fp1 = fopen("testing_answer.txt", "r");
	if (fp1 == NULL) printf("fail");

	char str[60], str1[60];
	float correct = 0;
	while ( fgets(str, 60, fp) != NULL && fgets(str1, 60, fp) != NULL ) {
		if (strcmp(str, str1) == 0) {
			correct++;
		}
	}
	fclose(fp);
	fclose(fp1);

	printf("%f\n", correct / 2500);
	return 0;
}