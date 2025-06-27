#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // Görseli okuyor burda ımreadcolor renk formatı grayscale içinde çalışyor(fotoğraf 3 kanallı).
    // Fotoğrafı bilerek grayscale seçtimki daha net anlamak için gürültüleri.
    cv::Mat img = cv::imread("reporter-camera-noise.png", cv::IMREAD_COLOR);

    // Median 3x3 filtre uygulama yapılıyor burda filtrenin boyutunu çok büyütmemek lazım sonuç istenilen olmayabilir.
    // 3x3 filtreyi koyuyor ve sortlayıp medyanı yerine yazıyor(stride'i genelde 1 paddingide kenarlar için otomatik yapıyor
    // geneldede replicate padding kullanıyor.
    cv::Mat filtered;
    cv::medianBlur(img, filtered, 3);

    // Sonucu kaydediyor. Bu CPU'da çalışır ve paralelizasyon yapmadığı için görece yavaş kalır.
    cv::imwrite("reporter-camera-noise_filtered.png", filtered);

    return 0;
}
