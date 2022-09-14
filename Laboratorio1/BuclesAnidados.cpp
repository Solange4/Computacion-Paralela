#include <iostream>
#include <time.h>
#include<stdlib.h>
#include <chrono>
using namespace std;

void inicializar(double **&A, int MAX){
    A = new double*[MAX];
    for( int i=0; i<MAX; i++ )
        A[i] = new double[MAX];
}

void llenar(double **A, double *x, int MAX){
    srand(time(NULL));
    for (int i=0;i<MAX;i++)
        for (int j=0;j<MAX;j++)
            *(*(A+i)+j)=1+rand()%(101-1);

    for (int i=0;i<MAX;i++)
        *(x+i)=1+rand()%(101-1);
}

void ceros(double *y, int MAX){
    for (int i=0;i<MAX;i++)
        *(y+i)=0;
}

void imprimir(double **MatrizA, double *x, int MAX){
    for (int i=0;i<MAX;i++){
        for (int j=0;j<MAX;j++)
            cout<<*(*(MatrizA+i)+j)<<" "; 
        cout<<"\n";
    }
    cout<<"------------------------------------"<<endl;
    for (int j=0;j<MAX;j++)
            cout<<*(x+j)<<" ";
}

int main(){
    // int MAX=10; > 0 0
    // int MAX=100; > 0 0
    // int MAX=200; > 0 0
     // int MAX=250; > 0 0
    int MAX=300;
    // > 0  1.9965
    // int MAX=500; > 0.9976  2.992
    // int MAX=1000; > 3.9984  11.0034
    // int MAX=5000; > 92.7342  411.92
    double **A;
    double *x = new double[MAX]; 
    double *y = new double[MAX];

    inicializar(A,MAX);
    llenar(A,x,MAX);
    // imprimir(A,x,MAX);

    ceros(y,MAX);
    auto start1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < MAX; i++)
        for (int j = 0; j < MAX; j++)
            y[i] += A[i][j]*x[j];
    auto end1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float,std::milli> duration1 = end1 - start1;
    cout<<"Costo medido en tiempo bucle 1: "<<duration1.count()<<endl<<endl;

    ceros(y,MAX);
    auto start2 = std::chrono::high_resolution_clock::now();
    for (int j = 0; j < MAX; j++)
        for (int i = 0; i < MAX; i++)
            y[i] += A[i][j]*x[j];
    auto end2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float,std::milli> duration2 = end2 - start2;
    cout<<"Costo medido en tiempo bucle 2: "<<duration2.count()<<endl<<endl;
    
    
    return 0;
}
