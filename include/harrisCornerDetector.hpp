#include <iostream>
#include <opencv2/opencv.hpp>


#ifndef HARRISCORNERDETECTOR_HPP
#define HARRISCORNERDETECTOR_HPP
namespace course{
    cv::Mat cornerHarris(cv::Mat grayImage, int blockSize, int ksize, double k);
    int yangHuiTriangle(int row, int col);
    int smooth(int x, int windowSize);
    int diff(int x, int windowSize);
    void Sobel(int windowSize, int* rowX, int* colY);
    void caculateIxAndIy();
}
#endif