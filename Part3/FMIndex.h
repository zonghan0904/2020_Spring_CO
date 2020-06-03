#ifndef __FMINDEX_H__
#define __FMINDEX_H__

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <cmath>
#include <stdlib.h>
#include<sys/time.h>

extern int read_count;
extern int read_length;

extern int **SA_Final_student;
extern int **L_counts_student;
extern char *L_student;
extern int F_counts_student[];

//Read file to get reads
char** inputReads(char *file_path, int *read_count, int *length);

//Rotate read by 1 character
void rotateRead(char *read, char *rotatedRead, int length);

//Generate Sufixes and their SA's for a read
char** generateSuffixes(char *read, int length, int read_id);

//Comparator for Suffixes
int compSuffixes(char *suffix1, char *suffix2, int length);

void swap(char **tmp_suf, int a, int b);

int RandomInteger(int low, int high);

int Partition(char **tmp_suf, int l, int r);

void QuickSort(char **tmp_suf, int l, int r);

int** makeFMIndex_student(char ***suffixes_student, int read_count, int read_length, int F_count_student[], char *L_student);

#endif
