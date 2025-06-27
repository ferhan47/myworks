#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    cv::Mat img = cv::imread("Test.jpg");


    // BGR'den HSV'ye dönüştürüyoruz çünkü BGR de ışığın tonu rengimizi etkileyebiliyor ama HSV'de H rengi belirlediği için daha rahat oluyor
    cv::Mat imgHSV;
    cv::cvtColor(img, imgHSV, cv::COLOR_BGR2HSV);

    // Kırmızı için alt ve üst HSV aralıkları belirliyoruz (kırmızı 0 ve 180'e yakın yerlerde 2 bölgeye ayrılıyor)
    cv::Mat mask1, mask2, mask;

    // Açık kırmızı
    cv::inRange(imgHSV, cv::Scalar(0, 100, 100), cv::Scalar(10, 255, 255), mask1);

    // Koyu kırmızı
    cv::inRange(imgHSV, cv::Scalar(160, 100, 100), cv::Scalar(179, 255, 255), mask2);

    // İki maskeyi birleştiriyoruz normalinde  '||' or'dur ama burda bir boolean değil bitwise işlemi yapılıyor ondan dolayı '|' kullanılıyor
    mask = mask1 | mask2;

    // Maskeyi uyguluyoruz. Maske istediğimiz renkleri otomatik 255'e (beyaza) kalanları 0(siyaha) çeker bu örnekte sadece kırmızı yerleri beyaz yapacak
    cv::Mat result;
    // Burda bitwise_and fonksiyonu iki fotoğraf alıp ortak pixelleri ortaya çıkarır(img,img olduğu için fotonun kendisi burda)
    // ardından mask ile and işlemi yapar ve bunu resulta assign eder bu sayede karşımıza maskte 1 olmuş(yani beyaz ve istediğimiz yerler olan) kısımlar çıkar(kendi renginde).
    cv::bitwise_and(img, img, result, mask);

    cv::imshow("Orijinal", img);
    cv::imshow("Kırmızı Maske", mask);
    cv::imshow("Tespit Edilen Renk", result);
    //milisaniye içindeki(burda 20 saniye durucak ve kod devam edecek)
    cv::waitKey(20000);
    //Kontür kullanıcaz çerçeve çizmek için kontürler yakın renkteki pixellerin birleşmesinden oluşan yapılardır. Sınır tanımlamaya yardımcı olur.
    std::vector<std::vector<cv::Point>> contours;
    //Hiyerarşi iç içe kontürleri listeler daha sonrasında en dış kontürü kullanıcaz bu yapı sayesinde
    std::vector<cv::Vec4i> hierarchy;
    //Maskla belirlediğmiz bölgelerin dış kontürlerini hiyerşiden bulup veriyor bize(RETR_EXTERNAL en dıştakileri veriyo hiyerşi sayesınde
    //CHAIN_APPROX_SIMPLE sınırların kapladıkları alanları biraz sınırlandırıyor.)
    cv::findContours(mask, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    //Direkt bunu kullandığımız zaman birkaç çerçeve elde ediyoruz çünkü bazı kontürler direkt olarak birbirlerinin altında olmuyor ve gözüküyorlar.
    //bunndan dolayı en büyük kontürü seçerek istenilen çerçeveyi elde ediyoruz(genelde doğru sonuçç verir tahminimmce).
    int largest_contour_index = -1;
    double max_area = 0;

    for (size_t i = 0; i < contours.size(); i++) {
        double area = cv::contourArea(contours[i]);
        if (area > max_area) {
            max_area = area;
            largest_contour_index = i;
        }
    }
    if (largest_contour_index >= 0) {
        cv::Rect bounding_rect = cv::boundingRect(contours[largest_contour_index]);
        cv::rectangle(img, bounding_rect, cv::Scalar(0, 255, 0), 2);  // Yeşil çerçeve kalınlığı 2
    }

    //ve çerçeveyi elde ediyoruz
    cv::imshow("Detected Color with Rectangle", img);
    //0 sonsuza kadar açık kalması anlamına geliyor
    cv::waitKey(0);

    return 0;
}