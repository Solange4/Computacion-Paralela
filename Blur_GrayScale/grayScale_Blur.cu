#include <cuda.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using namespace std;

#define NUM_CHANNELS 3

__constant__ int BLUR_SIZE = 25;

/*
 * Convertir a escala de grises
*/
__global__
void colorToGreyscaleConversion(unsigned char* Pout, const unsigned char* Pin, int width, int height){
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    // Verifica si las variables fila y columna de los subprocesos están dentro de los límites de la imagen
    if(col < width && row < height) { // CUBRE TODA LA IMAGEN
        int grey_offset = row * width + col; 
        int rgb_offset = grey_offset * NUM_CHANNELS;

        unsigned char r = Pin[rgb_offset + 0]; // rojo
        unsigned char g = Pin[rgb_offset + 1]; // verde
        unsigned char b = Pin[rgb_offset + 2]; // azul

        // Realiza la conversión de rgb a gray promediando los tres canales 
        Pout[grey_offset] = (unsigned char)(0.21f * r + 0.71f * g + 0.07f * b);
    }
}

/*
 * Difuminar
*/
__global__
void blurKernel(unsigned char* Pout, const unsigned char* Pin, int width, int height){
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    // Verifica si las variables fila y columna de los subprocesos están dentro de los límites de la imagen
    if(col < width && row < height) {
        int pixVal = 0;
        int pixels = 0;

        // Calcula la media de los píxeles vecinos
        // Los dos bucles for iteran a través del cuadro que define el valor BLUR_SIZE
        for(int blurrow = -BLUR_SIZE; blurrow < BLUR_SIZE + 1; ++blurrow){
            for(int blurcol = -BLUR_SIZE; blurcol < BLUR_SIZE + 1; ++blurcol){
                int currow = row + blurrow;
                int curcol = col + blurcol;
            
                // Si el elemento está dentro de los límites de la imagen, acumula el valor en la variable pixVal e incrementa el contador de píxeles acumulados.
                if(currow > -1 && currow < height && curcol > -1 && curcol < width){
                    pixVal += Pin[currow * width + curcol];
                    ++pixels; // Cuenta el número de valores de píxeles que se han añadido
                }
            }
        }
    Pout[row * width + col] = (unsigned char)(pixVal / pixels);
    }
}

int main(int argc, char* argv[]){
  // Read the image
  int width, height, channels;
  unsigned char* h_Pin = stbi_load(argv[1], &width, &height, &channels, 0); 
  cout << "Imported image " << argv[1] << " (" << width << " x " << height << ") with " << channels << " channels" << endl;

  // Asigna memoria en el host para la imagen de salida
    int size = width*height;
    unsigned char* h_Pout = new unsigned char[size];

    // Asigna memoria en el device para la imagen de entrada
    unsigned char* d_Pin;
    cudaMalloc((void**)&d_Pin, size*3);
    cudaMemcpy((void*)d_Pin, (void*)h_Pin, size*3, cudaMemcpyHostToDevice);

    // Asigna memoria para escala de grises
    unsigned char* d_Pbw;
    cudaMalloc((void**)&d_Pbw, size);

    // Asigna memoria para Blur    
    unsigned char* d_Pout;
    cudaMalloc((void**)&d_Pout, size);

    dim3 dimGrid(ceil(width/16.0f),ceil(height/16.0f),1);
    dim3 dimBlock(16,16,1);
    cout << "Launching a (" << dimGrid.x << " x " << dimGrid.y << " x " << dimGrid.z << ") grid." << endl;
    cout << "Total number of threads: " << dimGrid.x*dimGrid.y*dimGrid.z*16*16 << endl;
    cout << "Number of pixels: " << width*height << endl;

    // ESCALA DE GRISES
    colorToGreyscaleConversion<<<dimGrid,dimBlock>>>(d_Pbw, d_Pin, width, height);
    cudaMemcpy((void*)h_Pout, (void*)d_Pbw, size, cudaMemcpyDeviceToHost);
    stbi_write_bmp("grayScale.png", width, height, 1, (void*)h_Pout);


    // BLUR
    blurKernel<<<dimGrid,dimBlock>>>(d_Pout, d_Pbw, width, height);
    cudaMemcpy((void*)h_Pout, (void*)d_Pout, size, cudaMemcpyDeviceToHost);
    stbi_write_bmp("Blur.png", width, height, 1, (void*)h_Pout);

    cudaFree(d_Pin);
    cudaFree(d_Pbw);
    cudaFree(d_Pout);
 
    cout << "Closing..." << endl;

    return 0;
}