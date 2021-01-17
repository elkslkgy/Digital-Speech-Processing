//./train 100 model_init.txt seq_model_01.txt model_01.txt
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hmm.h"
#include <math.h>

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
/*
	HMM hmms[5];
	load_models( "modellist.txt", hmms, 5);
	dump_models( hmms, 5);
*/
	int times = atol(argv[1]);

	HMM hmm;
	loadHMM( &hmm, argv[2] );

	for (int time = 0; time < times; time++) {
		double alpha[50][6], beta[50][6], epsilon[6][6], gama_s[6][6];
		double gama[50][6];

		memset(alpha, 0, sizeof(alpha));
		memset(beta, 0, sizeof(beta));
		memset(epsilon, 0, sizeof(epsilon));
		memset(gama_s, 0, sizeof(gama_s));
		memset(gama, 0, sizeof(gama));

		FILE *fp;
		fp = fopen(argv[3], "r");
		if (fp == NULL) {
			printf("fail");
		}

		char str[60];
		while ( fgets(str, 60, fp) != NULL ) {
			//counting alpha (ok)
			for (int i = 0; i < 6; i++) {
				alpha[0][i] = hmm.initial[i] * hmm.observation[str[0]-'A'][i];
			}
			for (int t = 1; t < 50; t++) {	//times
				for (int j = 0; j < 6; j++) {	//j = ABCDEF
					double var = 0;
					for (int i = 0; i < 6; i++) {
						var += (alpha[t - 1][i] * hmm.transition[i][j]);
					}
					alpha[t][j] = var * hmm.observation[str[t] - 'A'][j];
				}
			}

			for (int i = 0; i < 6; i++) {
				beta[49][i] = 1;
			}
			for (int t = 48; t >= 0; t--) {
				for (int i = 0; i < 6; i++) {
					beta[t][i] = 0;
					for (int j = 0; j < 6; j++) {
						beta[t][i] += (hmm.transition[i][j] * hmm.observation[str[t + 1] - 'A'][j]\
							 * beta[t + 1][j]);
					}
				}
			}

			//counting gama
			for (int t = 0; t < 50; t++) {
				double down = 0;
				for (int i = 0; i < 6; i++) {
					down += (alpha[t][i] * beta[t][i]);
				}
				for (int i = 0; i < 6; i++) {
					gama_s[str[t] - 'A'][i] += alpha[t][i] * beta[t][i] / down;
					gama[t][i] += alpha[t][i] * beta[t][i] / down;
				}
			}

			//counting epsilon
			for (int t = 0; t < 49; t++) {
				double down = 0;
				for (int i = 0; i < 6; i++) {
					for (int j = 0; j < 6; j++) {
						down += (alpha[t][i] * hmm.transition[i][j] * hmm.observation[str[t + 1] - 'A'][j]\
						 	* beta[t + 1][j]);
					}
				}
				for (int i = 0; i < 6; i++) {
					for (int j = 0; j < 6; j++) {
						epsilon[i][j] += (alpha[t][i] * hmm.transition[i][j] * hmm.observation[str[t + 1] - 'A'][j]\
							* beta[t + 1][j] / down);
					}
				}
			}
			//printf("%s", str);
		}
		fclose(fp);

		//initial update
		for (int i = 0; i < 6; i++) {
			hmm.initial[i] = gama[0][i] / 10000;
		}

		//transition update
		for (int i = 0; i < 6; i++) {
			double down = 0;
			for (int t = 0; t < 50; t++) {
				down += gama[t][i];
			}
			for (int j = 0; j < 6; j++) {
				hmm.transition[i][j] = epsilon[i][j] / down;
			}
		}

		//observation update
		for (int i = 0; i < 6; i++) {
			double down = 0;
			for (int t = 0; t < 50; t++) {
				down += gama[t][i];
			}
			for (int k = 'A'; k <= 'F'; k++) {
				hmm.observation[k - 'A'][i] = gama_s[k - 'A'][i] / down;
			}
		}

		dumpHMM( stderr, &hmm );

		fp = fopen(argv[4], "w");
		dumpHMM(fp, &hmm);
		fclose(fp);
	}
	return 0;
}