//./test modellist.txt testing_data1.txt result1.txt
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hmm.h"
#include <math.h>

double theta[5][50][6];

// hmm->stat_num  = 6
// hmm->observ_num = 6
// hmm->initial[MAX_STATE] = pi
// hmm->transition[MAX_STATE][MAX_STATE] = aij
// hmm->observation[MAX_OBSERV][MAX_STATE] = b_o
// typedef struct{
//    char *model_name;
//    int state_num;					//number of state
//    int observ_num;					//number of observation
//    double initial[MAX_STATE];			//initial prob.
//    double transition[MAX_STATE][MAX_STATE];	//transition prob.
//    double observation[MAX_OBSERV][MAX_STATE];	//observation prob.

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
				// printf("%f\n", theta[m][0][i]);
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
						// printf("new %f\n", new);
						if (new > max) {
							max = new;
						}
					}
					// printf("Max%d %d %d : %f\n",m, t, j, max);
					// printf("\n");
					theta[m][t][j] = max * hmms[m].observation[str[t] - 'A'][j];
					// printf("theta %f", theta[m][t][j]);
				}
				// printf("\n");
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
					// printf("%d", max_model);
				}
			}
		}
		//printf("model_0%d.txt %f\n", max_model, max);

		fprintf(fp1, "model_0%d.txt\n", max_model+1);
		
		//printf("%s", str);
	}
	fclose(fp);
	fclose(fp1);

	// dump_models( hmms, 5);
	// HMM hmm;
	// loadHMM( &hmm, argv[2] );

	// for (int i = 0; i < 6; i++) {
	// 	gama[0][i] = gama[1][i] = 0;
	// }

	// FILE *fp;
	// fp = fopen(argv[3], "r");
	// if (fp == NULL) {
	// 	printf("fail");
	// }
	// char str[60];
	// while ( fgets(str, 60, fp) != NULL ) {
		
	// 	printf("%s", str);
	// }
	// fclose(fp);




	// dumpHMM( stderr, &hmm );

	// fp = fopen(argv[4], "w");
	// dumpHMM(fp, &hmm);
	// fclose(fp);

	return 0;
}