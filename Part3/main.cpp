#include <iostream>
#include "FMIndex.h"

using namespace std;

int main(int argc, char *argv[]){

    char * read_data_file= "P1000.txt";   // input DATA

    char **reads = inputReads(read_data_file, &read_count, &read_length);//Input reads from file

    char ***suffixes_student=(char***)malloc(read_count*sizeof(char**));
    L_student=(char*)malloc(read_count*read_length*sizeof(char*));

    //-----------Time capture start--------------------
    struct timeval  TimeValue_Start;
    struct timeval  TimeValue_Final;
    struct timezone TimeZone_Start;
    struct timezone TimeZone_Final;
    long time_start, time_end;
    double time_overhead_default, time_overhead_student;

    gettimeofday(&TimeValue_Start, &TimeZone_Start);
    time_start = TimeValue_Start.tv_sec * 1000000 + TimeValue_Start.tv_usec;

    for(int i=0;i<read_count;i++){
        suffixes_student[i]=generateSuffixes(reads[i], read_length, i);
    }

    L_counts_student = makeFMIndex_student(suffixes_student, read_count, read_length, F_counts_student, L_student);

    gettimeofday(&TimeValue_Final, &TimeZone_Final);
    time_end = TimeValue_Final.tv_sec * 1000000 + TimeValue_Final.tv_usec;
    time_overhead_student = (time_end - time_start)/1000000.0;

    //----------------For debug purpose only-----------------
    //for(int i=0;i<read_count*read_length;i++)
    //    cout<<L[i]<<"\t"<<SA_Final[i][0]<<","<<SA_Final[i][1]<<"\t"<<L_counts[i][0]<<","<<L_counts[i][1]<<","<<L_counts[i][2]<<","<<L_counts[i][3]<<endl;
    //--------------------------------------------------

    cout <<  "student: " << time_overhead_student << endl;
    return 0;
}
