#include "./helpers/operation.hpp"

#define MAX 10000

int main(){
    double **A;
    double *x = new double[MAX]; 
    double *y = new double[MAX];
    // inicializacion de matriz
    initialize_matrix(A,MAX);
    // llenado de datos
    fill_matrix(A,MAX);
    fill_vector(x,MAX);

    // COMPARACION
    // Bucle 1
    fill_vector_with_ceros(y,MAX);
    auto start1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < MAX; i++)
        for (int j = 0; j < MAX; j++)
            y[i] += A[i][j]*x[j];
    auto end1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float,std::milli> duration1 = end1 - start1;
    cout<<"Costo medido en tiempo bucle 1: "<<duration1.count()<<endl<<endl;

    // Bucle 2
    fill_vector_with_ceros(y,MAX);
    auto start2 = std::chrono::high_resolution_clock::now();
    for (int j = 0; j < MAX; j++)
        for (int i = 0; i < MAX; i++)
            y[i] += A[i][j]*x[j];
    auto end2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float,std::milli> duration2 = end2 - start2;
    cout<<"Costo medido en tiempo bucle 2: "<<duration2.count()<<endl<<endl;
    
    return 0;
}
