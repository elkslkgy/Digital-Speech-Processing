#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hmm.h"
#include <math.h>

double theta[5][50][6];

int main(int argc, char *argv[])
{

	HMM hmms[5];
	load_models( argv[1], hmms, 5);

	FILE *fp, *fp1;
	fp = fopen(argv[2], "r");
	if (fp == NULL) printf("fail");
	fp1 = fopen(argv[3], "w");
	if (fp1 == NULL) printf("fail");

	char str[60];
	while ( fgets(str, 60, fp) != NULL ) {
		//counting theta
		for (int m = 0; m < 5; m++) {
			for (int i = 0; i < 6; i++) {
				theta[m][0][i] = hmms[m].initial[i] * hmms[m].observation[str[0]-'A'][i];
			}
		}

		for (int m = 0; m < 5; m++) {
			for (int t = 1; t < 50; t++) {	//times
				for (int j = 0; j < 6; j++) {	//j = ABCDEF
					//find the max
					double max = 0;
					for (int i = 0; i < 6; i++) {
						double new;
						new = theta[m][t - 1][i] * hmms[m].transition[i][j];
						if (new > max) {
							max = new;
						}
					}
					theta[m][t][j] = max * hmms[m].observation[str[t] - 'A'][j];
				}
			}
		}

		//find the max model
		int max_model;
		double max = 0;
		for (int m = 0; m < 5; m++) {
			for (int i = 0; i < 6; i++) {
				if (theta[m][49][i] > max) {
					max = theta[m][49][i];
					max_model = m;
				}
			}
		}

		fprintf(fp1, "model_0%d.txt %e\n", max_model+1, max);
		
	}
	fclose(fp);
	fclose(fp1);

	return 0;
}
