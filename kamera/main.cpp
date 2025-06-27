#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // Kamera açma
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Kamera açılamadı!" << std::endl;
        return -1;
    }

    // Kırmızı için HSV aralıkları:
    int lowH1 = 0, highH1 = 10;
    int lowH2 = 160, highH2 = 179;
    int lowS = 100, highS = 255;
    int lowV = 100, highV = 255;

    cv::Mat frame, hsv, mask1, mask2, mask;
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;

    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        // HSV'ye dönüştür
        cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

        // Maskeleri oluştur
        cv::inRange(hsv, cv::Scalar(lowH1, lowS, lowV), cv::Scalar(highH1, highS, highV), mask1);
        cv::inRange(hsv, cv::Scalar(lowH2, lowS, lowV), cv::Scalar(highH2, highS, highV), mask2);
        mask = mask1 | mask2;

        // Gürültü azaltma
        cv::erode(mask, mask, cv::Mat(), cv::Point(-1,-1), 1);
        cv::dilate(mask, mask, cv::Mat(), cv::Point(-1,-1), 1);

        // Konturları bul
        contours.clear();
        hierarchy.clear();
        cv::findContours(mask, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        if (!contours.empty()) {
            // En büyük konturu seç
            int idxMax = 0;
            double maxArea = 0;
            for (size_t i = 0; i < contours.size(); i++) {
                double a = cv::contourArea(contours[i]);
                if (a > maxArea) {
                    maxArea = a;
                    idxMax = static_cast<int>(i);
                }
            }
            // Bounding rectangle
            cv::Rect rect = cv::boundingRect(contours[idxMax]);
            // Çok küçükse ihmal et
            if (maxArea > 500) {
                cv::rectangle(frame, rect, cv::Scalar(0, 255, 0), 2);
            }
        }

        // Ekranda gösterme
        cv::imshow("Kamera", frame);
        cv::imshow("Mask", mask);

        char key = static_cast<char>(cv::waitKey(30));
        if (key == 27) break;
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}
