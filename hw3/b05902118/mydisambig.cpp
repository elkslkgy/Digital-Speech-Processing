//Being taught by the student who took this course before.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include "Ngram.h"

using namespace std;

typedef struct {	//一個中文字，可能需要用三個char去存
	char character[5] = {};
}Character;

typedef struct {	//我現在這個中文字是什麼，後面可能接什麼中文字
	Character word;
	vector <Character> possible_words;
}Word;

typedef struct {	//maxPosition:最大機率的index是哪裡，我現在這個位置的機率
	int maxPosition;
	double p;
}P;

typedef struct {	//我這個位置之後可能可以有好幾種可能性，每一個可能性的機率跟她在往後往哪走的機率
	vector<P> possibility;
}Possibility;

double getBigramProb(const char *word1, const char *word2, Ngram &lm, Vocab &voc) {
	VocabIndex wid1 = voc.getIndex(word1);
	VocabIndex wid2 = voc.getIndex(word2);
	if (wid1 == Vocab_None)
		wid1 = voc.getIndex(Vocab_Unknown);
	if (wid2 == Vocab_None)
		wid2 = voc.getIndex(Vocab_Unknown);
	VocabIndex content[] = {wid1, Vocab_None};
	return lm.wordProb(wid2, content);
}	//算機率

int main(int argc, char *argv[])
{
	//open Files
	FILE *INPUT, *MAP;
	char lmFileName[20];
	int order;
	for (int i = 0; i < argc; i++) {
		if (strcmp(argv[i], "-text") == 0)
			INPUT = fopen(argv[i + 1], "r");
		else if (strcmp(argv[i], "-map") == 0)
			MAP = fopen(argv[i + 1], "r");
		else if (strcmp(argv[i], "-lm") == 0)
			strcpy(lmFileName, argv[i + 1]);
		else if (strcmp(argv[i], "-order") == 0)
			order = atoi(argv[i + 1]);
	}

	//language model
	Vocab voc;
	Ngram lm(voc, order);
	File lmFile(lmFileName, "r");
	lm.read(lmFile);
	lmFile.close();

	//testData
	char buffer[5000];	//buffer是拿到testData裡面的一行資料
	while (fgets(buffer, 5000, INPUT) != NULL) {
		if (buffer[strlen(buffer) - 1] == '\n')
			buffer[strlen(buffer) - 1] = '\0';
		
		vector<Word> a_line;	//這行字用a_line做紀錄
		//開頭字
		Word start_word;
		Character start_char;
		strcpy(start_char.character, "<s>");
		start_word.word = start_char;
		start_word.possible_words.push_back(start_char);
		a_line.push_back(start_word);

		//put words into a line
		char *char_cut;
		char_cut = strtok(buffer, " ");	//把每個中文字中間用空白鍵隔開

		while (char_cut != NULL) {
			Word word_temp;	//把現在這個文字是什麼存進去word_temp.word
			word_temp.word.character[0] = char_cut[0];
			word_temp.word.character[1] = char_cut[1];
			word_temp.word.character[2] = '\0';

			//從MAP最前面開始找
			fseek(MAP, 0, SEEK_SET);
			char map_temp[5000];
			while (fgets(map_temp, 5000, MAP) != NULL) {	//如果在map裡面找到一樣的字，那就開始往後把會出現的字都放到possible_words裡面
				if (map_temp[0] == char_cut[0] && map_temp[1] == char_cut[1]) {	//在map裡面找到第一個字是一樣的了
					for (int i = 3; i < strlen(map_temp); i++) {	//把map裡面這這國字後面所有的可能性都存到possible_words裡面
						Character temp;
						if (map_temp[i] != ' ') {
							temp.character[0] = map_temp[i];
							i++;
							temp.character[1] = map_temp[i];
							i++;
							temp.character[2] = '\0';
							word_temp.possible_words.push_back(temp);
						}
					}
				}
			}
			a_line.push_back(word_temp);	//這個中文字解決，推進這行的vector
			char_cut = strtok(NULL, " ");	//繼續依照空白建分隔找到下一個中文字
		}
		//結尾字
		Word end_word;
		Character end_char;
		strcpy(end_char.character, "</s>");
		end_word.word = end_char;
		end_word.possible_words.push_back(end_char);
		a_line.push_back(end_word);

		//初始化
		vector<Possibility> a_lineP;	//一行文字的機率

		P start_p;
		start_p.maxPosition = 0;
		start_p.p = 0.0;
		Possibility start_pos;
		start_pos.possibility.push_back(start_p);
		a_lineP.push_back(start_pos);

		for (int i = 1; i < a_line.size(); i++) {	//從一行裡面第一個字開始走，跳過<s>
			Possibility possibility_temp;
			for (int j = 0; j < a_line[i].possible_words.size(); j++) {
				P p_temp;
				double p_max = -60000;
				int max_index;
				double temp;
				for (int k = 0; k < a_line[i - 1].possible_words.size(); k++) {
					temp = a_lineP[i - 1].possibility[k].p + getBigramProb(a_line[i - 1].possible_words[k].character, a_line[i].possible_words[j].character, lm, voc);	//現在這個位置的機率，加上這個位置往前走一種可能性的機率
					if (temp > p_max) {
						p_max = temp;
						max_index = k;
					}
				}
				p_temp.maxPosition = max_index;
				p_temp.p = p_max;
				possibility_temp.possibility.push_back(p_temp);
			}
			a_lineP.push_back(possibility_temp);
		}
		int road_temp = 0;
		vector<int> road;
		road.push_back(road_temp);
		for (int i = a_lineP.size() - 1; i > 0; i--) {
			road_temp = a_lineP[i].possibility[road_temp].maxPosition;
			road.push_back(road_temp);
		}
		for (int i = 0; i < a_line.size(); i++)
			printf("%s ", a_line[i].possible_words[road[a_line.size() - 1 - i]].character);
		printf("\n");
	}
	return 0;
}
