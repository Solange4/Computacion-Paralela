#include "./helpers/operation.hpp"

#define MAX 5000

void classical_matrix_multiplication(double **A, double **B, double **C){
    fill_matrix_with_ceros(C,MAX);
    auto start = std::chrono::high_resolution_clock::now();
    for (int i=0;i<MAX;i++)
        for (int j=0;j<MAX;j++)
            for (int k=0;k<MAX;k++)
                *(*(C+i)+j) += *(*(A+i)+k) * *(*(B+k)+j);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float,std::milli> duration = end - start;
    std::cout<<"Costo de la multiplicacion clasica medido en tiempo: "<<duration.count()<<endl; 
}

void block_matrix_multiplication(double **A, double **B, double **C, int b){
    fill_matrix_with_ceros(C,MAX);
    auto start = std::chrono::high_resolution_clock::now();
    for (int ii=0;ii<MAX;ii+=b)
        for (int jj=0;jj<MAX;jj+=b)
            for (int kk=0;kk<MAX;kk+=b)
                for (int i = ii; i < ii+b; i++) 
                    for (int j = jj; j < jj+b; j++)
                        for (int k = kk; k < kk+b; k++)
                            *(*(C+i)+j) += *(*(A+i)+k) * *(*(B+k)+j);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float,std::milli> duration = end - start;
    std::cout<<"Costo de la multiplicacion por bloques medido en tiempo: "<<duration.count()<<endl; 
}

int main(){
    double **A, **B, **C;
     // inicializacion de matrices
    initialize_matrix(A,MAX);
    initialize_matrix(B,MAX);
    initialize_matrix(C,MAX);
    // llenado de datos
    fill_matrix(A,MAX);
    fill_matrix(B,MAX);

    //COMPARACION
    classical_matrix_multiplication(A,B,C);
    block_matrix_multiplication(A,B,C,MAX/2);
    block_matrix_multiplication(A,B,C,MAX/5);
    block_matrix_multiplication(A,B,C,MAX/10);
      
    return 0;
}
