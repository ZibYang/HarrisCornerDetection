#ifndef HARRISCORNERDETECTOR_HPP
#define HARRISCORNERDETECTOR_HPP
#include "common.h"

namespace course{
   

    int convolve(cv::Mat grayImg, int x, int y, int windowSize, int* rowX = nullptr, int* colY = nullptr);
    cv::Mat cornerHarris(cv::Mat grayImage, int blockSize, int ksize, double k);
    cv::Mat caculateI_(cv::Mat grayImg, int windowSize, int* rowX = nullptr, int* colY = nullptr);
    cv::Mat caculateR(cv::Mat IxIx, cv::Mat IyIy, cv::Mat IxIy, int k, int& max);
    cv::Mat windowFilter(cv::Mat I, cv::Mat gauss, int windowSize);
    cv::Mat powFrom(cv::Mat a, cv::Mat b);
    cv::Mat locateTheCorner(cv::Mat frame, cv::Mat R, int max);
    
}
#endif