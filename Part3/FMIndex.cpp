#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <cmath>
#include <stdlib.h>
#include<sys/time.h>

using namespace std;

int read_count = 0;
int read_length = 0;
int **SA_Final_student;
int **L_counts_student;
char *L_student;
int F_counts_student[]={0,0,0,0};

//Read file to get reads
char** inputReads(char *file_path, int *read_count, int *length){
    FILE *read_file = fopen(file_path, "r");
    int ch, lines=0;
    char **reads;
    do
    {
        ch = fgetc(read_file);
        if (ch == '\n')
            lines++;
    } while (ch != EOF);
    rewind(read_file);
    reads=(char**)malloc(lines*sizeof(char*));
    *read_count = lines;
    int i = 0;
    size_t len = 0;
    for(i = 0; i < lines; i++)
    {
        reads[i] = NULL;
        len = 0;
        getline(&reads[i], &len, read_file);
    }
    fclose(read_file);
    int j=0;
    while(reads[0][j]!='\n')
        j++;
    *length = j+1;
    for(i=0;i<lines;i++)
        reads[i][j]='$';
    return reads;
}

//Rotate read by 1 character
void rotateRead(char *read, char *rotatedRead, int length){
    for(int i=0;i<length-1;i++)
        rotatedRead[i]=read[i+1];
    rotatedRead[length-1]=read[0];
}

//Generate Sufixes and their SA's for a read
char** generateSuffixes(char *read, int length, int read_id){
    char **suffixes=(char**)malloc(length*sizeof(char*));
    suffixes[0]=(char*)malloc(length*sizeof(char));
    for(int j=0;j<length;j++)
        suffixes[0][j]=read[j];
    for(int i=1;i<length;i++){
        suffixes[i]=(char*)malloc(length*sizeof(char));
        rotateRead(suffixes[i-1], suffixes[i], length);
    }
    return suffixes;
}

//Comparator for Suffixes
int compSuffixes(char *suffix1, char *suffix2, int length){
    int ret = 0;
    for(int i=0;i<length;i++){
        if(suffix1[i]>suffix2[i])
            return 1;
        else if(suffix1[i]<suffix2[i])
            return -1;
    }
    return ret;
}

//Swap the data when sorting
void swap(char **tmp_suf, int a, int b){
    char *tmp = (char *)malloc(read_length * sizeof(char));
    memcpy(tmp, tmp_suf[a], read_length * sizeof(char));
    memcpy(tmp_suf[a], tmp_suf[b], read_length * sizeof(char));
    memcpy(tmp_suf[b], tmp, read_length * sizeof(char));
    free(tmp);
    int tmp_int = SA_Final_student[a][0];
    SA_Final_student[a][0] = SA_Final_student[b][0];
    SA_Final_student[b][0] = tmp_int;
    tmp_int = SA_Final_student[a][1];
    SA_Final_student[a][1] = SA_Final_student[b][1];
    SA_Final_student[b][1] = tmp_int;
}

//Generate random integer
int RandomInteger(int low, int high){
    srand(0);
    return rand()%(high-low+1) + low;
}

//Randomly choose a Pivot
int Partition(char **tmp_suf, int l, int r){
    int randnum = RandomInteger(l, r);
    swap(tmp_suf, randnum, r);
    int i = l-1;
    char *x = tmp_suf[r];
    for (int j = l; j < r; j++){
        if (compSuffixes(tmp_suf[j], x, read_length) < 0){
            i++;
            swap(tmp_suf, i, j);
        }
    }
    i += 1;
    swap(tmp_suf, i, r);
    return i;
}

//Sorting data using quicksort algorithm
void QuickSort(char **tmp_suf, int l, int r){
    if (l < r){
        int m = Partition(tmp_suf, l, r);
        QuickSort(tmp_suf, l, m-1);
        QuickSort(tmp_suf, m+1, r);
    }
}

//Make the FMIndex
int** makeFMIndex_student(char ***suffixes_student, int read_count, int read_length, int F_count_student[], char *L_student){
    int i, j;

    SA_Final_student=(int**)malloc(read_count*read_length*sizeof(int*));
    for(i=0;i<read_count*read_length;i++)
        SA_Final_student[i]=(int*)malloc(2*sizeof(int));

    //Temporary storage for collecting together all suffixes
    char **temp_suffixes=(char**)malloc(read_count*read_length*sizeof(char*));

    //Initalization of temporary storage
    for(i=0;i<read_count;i++){
        for(j=0;j<read_length;j++){
            temp_suffixes[i*read_length+j]=(char*)malloc(read_length*sizeof(char));
            memcpy(&temp_suffixes[i*read_length+j], &suffixes_student[i][j],read_length*sizeof(char));
            SA_Final_student[i*read_length+j][0]=j;
            SA_Final_student[i*read_length+j][1]=i;
        }
    }

    char *temp=(char*)malloc(read_length*sizeof(char));

    int **L_count=(int**)malloc(read_length*read_count*sizeof(int*));

    for(i=0;i<read_length*read_count;i++){
        L_count[i]=(int*)malloc(4*sizeof(int));
        for(j=0;j<4;j++){
            L_count[i][j]=0;
        }
    }

    //Focus on improving this for evaluation purpose
    //Sorting of suffixes
    QuickSort(temp_suffixes, 0, read_count * read_length - 1);

    free(temp);
    char this_F = '$';
    j=0;

    //Calculation of F_count's
    for(i=0;i<read_count*read_length;i++){
        int count=0;
        while(temp_suffixes[i][0]==this_F){
            count++;i++;
        }
        F_count_student[j++]=j==0?count:count+1;
        this_F = temp_suffixes[i][0];
        if(temp_suffixes[i][0]=='T')
            break;
    }

    //Calculation of L's and L_count's
    for(i=0;i<read_count*read_length;i++){
        char ch = temp_suffixes[i][read_length-1];
        L_student[i]=ch;
        if(i>0){
            for(int k=0;k<4;k++)
                L_count[i][k]=L_count[i-1][k];
        }
        if(ch=='A')
            L_count[i][0]++;
        else if(ch=='C')
            L_count[i][1]++;
        else if(ch=='G')
            L_count[i][2]++;
        else if(ch=='T')
            L_count[i][3]++;
    }

    return L_count;
}
