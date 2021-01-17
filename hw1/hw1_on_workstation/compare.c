#include <stdio.h>
#include <string.h>

int main() {
	FILE *fp, *fp1, *fp2;
	fp = fopen("result1.txt", "r");
	if (fp == NULL) printf("fail\n");
	fp1 = fopen("testing_answer.txt", "r");
	if (fp1 == NULL) printf("fail\n");
	fp2 = fopen("acc.txt", "w");
	if (fp2 == NULL) printf("fail\n");

	char str[60], str1[60];
	float correct = 0;
	while ( fgets(str, 60, fp) != NULL && fgets(str1, 60, fp1) != NULL ) {
		if (str[7] == str1[7]) {
			correct++;
		}
	}
	fclose(fp);
	fclose(fp1);

	fprintf(fp2, "%f", correct / 2500);
	return 0;
}
