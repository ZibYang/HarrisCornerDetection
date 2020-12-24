#ifndef SOBEL_HPP
#define SOBEL_HPP

#include "common.h"

namespace tools
{
    int yangHuiTriangle(int row, int col);
    int smooth(int x, int windowSize);
    int diff(int x, int windowSize);
    void Sobel(int windowSize, int* rowX, int* colY);
    cv::Mat gaussTemplate(int windowSize);
    void displaySobel(int sobelKernalSize, int* list);

} // namespace tools
#endif