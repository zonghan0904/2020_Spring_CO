#include <iostream>
#include <string>
#include "FMIndex.h"

using namespace std;

int main(int argc, char *argv[]){

    char * read_data_file= "P10000.txt";   // input DATA

    char **reads = inputReads(read_data_file, &read_count, &read_length);//Input reads from file

    char ***suffixes_student=(char***)malloc(read_count*sizeof(char**));
    L_student=(char*)malloc(read_count*read_length*sizeof(char*));

    string input;
    cout << "please type input: ";
    cin >> input;
    int length = input.length();

    int top = 0;
    int bottom = read_count * read_length;
    bool find = true;
    int match_num = 0;
    int check_data[read_count * read_length] = {0};

    int I_table[4] = {0, 0, 0, 0};
    int c_top = 0, c_bottom = 0;

    //-----------Time capture start--------------------
    struct timeval  TimeValue_Start;
    struct timeval  TimeValue_Final;
    struct timezone TimeZone_Start;
    struct timezone TimeZone_Final;
    long time_start, time_end;
    double time_overhead_student;

    gettimeofday(&TimeValue_Start, &TimeZone_Start);
    time_start = TimeValue_Start.tv_sec * 1000000 + TimeValue_Start.tv_usec;

    for(int i=0;i<read_count;i++){
        suffixes_student[i]=generateSuffixes(reads[i], read_length, i);
    }

    L_counts_student = makeFMIndex_student(suffixes_student, read_count, read_length, F_counts_student, L_student);

    for (int i = 0; i < 4; i++){
	if (i == 0) I_table[i] = F_counts_student[i] - 1;
	else I_table[i] = I_table[i-1] + F_counts_student[i];
    }

    for (int i = length - 1; i >= 0; i--){
	char c = input[i];
	int index = 0;
	switch (c){
	    case 'A':
		index = 0;
		break;
	    case 'C':
		index = 1;
		break;
	    case 'G':
		index = 2;
		break;
	    case 'T':
		index = 3;
		break;
	    default:
		break;
	}

	if (top - 1 < 0) c_top = 0;
	else c_top = L_counts_student[top - 1][index];
	if (bottom - 1 < 0) c_bottom = 0;
	else c_bottom = L_counts_student[bottom - 1][index];

	top = c_top + I_table[index];
	bottom = c_bottom + I_table[index];

	if (bottom <= top){
	    find = false;
	    match_num = 0;
	    break;
	}
    }

    if (find == true){
	for (int i = top; i < bottom; i++){
	    if (check_data[SA_Final_student[i][1]] == 0){
	        match_num += 1;
	        check_data[SA_Final_student[i][1]] = 1;
	    }
	}
    }

    gettimeofday(&TimeValue_Final, &TimeZone_Final);
    time_end = TimeValue_Final.tv_sec * 1000000 + TimeValue_Final.tv_usec;
    time_overhead_student = (time_end - time_start)/1000000.0;

    cout << "Number of reads match this substring: " << match_num << endl;

    // cout <<  "Execution time: " << time_overhead_student << endl;
    return 0;
}
