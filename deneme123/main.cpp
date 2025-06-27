#include <opencv2/opencv.hpp>
#include <cuda_runtime.h>
#include <iostream>

using namespace cv;
using namespace std;

// GPUda çaışcak sonrasında CPU ya aktaracak
__global__ void median_filter_kernel(unsigned char* input, unsigned char* output, int width, int height) {
    //burda threadın yeri blok sayısı hangi pikselde çalışacağı tespit ediliyor
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    int idx = y * width + x;
    //burda window array olarak toplpuyor bilgileri gezerek
    if (x >= 1 && x < width - 1 && y >= 1 && y < height - 1) {
        unsigned char window[9];
        int k = 0;
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                int i = (y + dy) * width + (x + dx);
                window[k++] = input[i];
            }
        }

        // Bubble sort algoritması var burda basit bir sort algoritması medyan için
        for (int i = 0; i < 9; i++) {
            for (int j = i + 1; j < 9; j++) {
                if (window[j] < window[i]) {
                    unsigned char tmp = window[i];
                    window[i] = window[j];
                    window[j] = tmp;
                }
            }
        }

        output[idx] = window[4];
    } else {
        output[idx] = input[idx];//kenarlarsa elleme diyorum
    }
}

int main(int argc, char** argv) {
    if (argc < 3) {
        cout << "Kullanım: ./cuda_median_filter input.png output.png" << endl;
        return -1;
    }

    // Görseli okuyor grayscale kullandım iyice hızlansın diye
    Mat img = imread(argv[1], IMREAD_GRAYSCALE);

    int width = img.cols;
    int height = img.rows;
    size_t size = width * height;

    //  Bellek ayırıyoruz
    unsigned char *d_input, *d_output;
    cudaMalloc(&d_input, size);
    cudaMalloc(&d_output, size);

    //  zero copy gibi bir yaklaşım kullanılmıyorsa veriyi bu şekil GPUya kopyalıyoruz işlem yapmak için
    cudaMemcpy(d_input, img.data, size, cudaMemcpyHostToDevice);

    // CUDA başlatılıyor her blokkta 16*16 thread olacağından(1 thread 1 pixeli halledice)
    // formül bu şekilde yapınca tam resim kadar oluyor
    dim3 threads(16, 16);
    dim3 blocks((width + 15) / 16, (height + 15) / 16);
    //Global yazdığımız CUDA fonksiyonunu çağırır
    median_filter_kernel<<<blocks, threads>>>(d_input, d_output, width, height);
    //GPUda işlemler bitmeden aşşağıya geçmemeyi sağlar
    cudaDeviceSynchronize();

    // Çıktıyı al GPUdan CPUya alıyoruz
    //CV_8UC1	Görüntü tipi 8 bit unsigned 0–255 tek kanal (grayscale) manasına geliyor
    Mat output(height, width, CV_8UC1);
    cudaMemcpy(output.data, d_output, size, cudaMemcpyDeviceToHost);

    // Sonucu kaydetme
    imwrite(argv[2], output);

    // Temizlik memory leak olmasın diye temizliyoruz
    cudaFree(d_input);
    cudaFree(d_output);

    return 0;
}
