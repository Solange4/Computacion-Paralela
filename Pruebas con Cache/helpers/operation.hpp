#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <chrono>
using namespace std;

void initialize_matrix(double **&A, int MAX){
    A = new double*[MAX];
    for( int i=0; i<MAX; i++ )
        A[i] = new double[MAX];
}

void fill_matrix(double **A,int MAX){
    srand(time(NULL));
    for (int i=0;i<MAX;i++)
        for (int j=0;j<MAX;j++)
            *(*(A+i)+j)=1+rand()%(11-1);
}

void fill_vector(double *x, int MAX){
    for (int i=0;i<MAX;i++)
        *(x+i)=1+rand()%(11-1);
}

void fill_matrix_with_ceros(double **A, int MAX){
    for (int i=0;i<MAX;i++)
        for(int j=0;j<MAX;j++)
            *(*(A+i)+j)=0;
}

void fill_vector_with_ceros(double *y, int MAX){
    for (int i=0;i<MAX;i++)
        *(y+i)=0;
}

void print_matrix(double **A, int MAX){
    for (int i=0;i<MAX;i++){
        for (int j=0;j<MAX;j++)
            cout<<*(*(A+i)+j)<<" "; 
        cout<<"\n";
    }
}

void print_vector(double *x, int MAX){
    for (int j=0;j<MAX;j++)
        cout<<*(x+j)<<" ";
}