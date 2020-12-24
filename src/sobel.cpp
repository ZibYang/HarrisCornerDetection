#include "sobel.hpp"

int tools::yangHuiTriangle(int row, int col){
    if(row >= 0 && row<=col){
        if(col == row)
            return 1;
        else{
            int factorialOfCol = 1;
            int factorialOfRow = 1;
            int factorialOfColMinusRow = 1;
            for(int i = 2 ;i<=col;i++)
                factorialOfCol *= i;
            for(int i = 2 ;i<=row;i++)
                factorialOfRow *= i;
            for(int i = 2 ;i<=col-row;i++)
                factorialOfColMinusRow *= i;
            return factorialOfCol/(factorialOfRow*factorialOfColMinusRow);
        }
    }else{
        return 0;
    }
}

int tools::smooth(int x, int windowSize){
    return yangHuiTriangle(x, windowSize-1);
}

int tools::diff(int x, int windowSize){
    return yangHuiTriangle(x-1, windowSize-2) - yangHuiTriangle(x, windowSize-2);
}

void tools::Sobel(int windowSize, int* rowX, int* colY){
    for(int i = 0; i<windowSize; i++){
        for(int j = 0; j<windowSize; j++){
            if(i == 0)
                rowX[j] =  smooth(i, windowSize)*diff(j, windowSize);
            if(j == 0)
                colY[i] =  smooth(i, windowSize)*diff(j, windowSize);
            std::cout<<smooth(i, windowSize)*diff(j, windowSize)<<" ";
        }
        std::cout<<std::endl;
    }

}

void tools::displaySobel(int sobelKernalSize, int* list){
    for(int i = 0;i<sobelKernalSize;i++){
       std::cout<<list[i]<<" ";
    }
    std::cout<<std::endl;
}

cv::Mat tools::gaussTemplate(int windowSize){
    double sigma = 0.8;
    double pi = 3.1415926;
    double sum = 0.0;
    double u, v;
    float gaussData[windowSize*windowSize];
    int center = windowSize/2;
    for(int i = 0 ; i < windowSize; i++){
        u = (i-center) * (i-center);
        for(int j = 0 ; j < windowSize; j++){
            v = (j-center) * (j-center);
            float g = exp(-(u+v) / (2*sigma*sigma));
            gaussData[i*windowSize+j] = g / (2 *pi * sigma * sigma);
            sum += gaussData[i*windowSize+j];
        }
    }
    for(int i = 0 ; i < windowSize; i++){
        for(int j = 0 ; j < windowSize; j++){
            gaussData[i*windowSize+j] /= sum;
            std::cout<<gaussData[i*windowSize+j]<<" ";
        }
        std::cout<<std::endl;
    }
    //cv::Mat filter(windowSize, windowSize, CV_32F, gaussData);
    //return filter;

    // test code
    float window[windowSize][windowSize] ={
        { 2, 4, 5,  4,  2 },
        { 4, 9, 12, 9,  4 },
        { 5, 12,15, 12, 5 },
        { 4, 9, 12, 9,  4 },
        { 2, 4, 5,  4,  2 }
    };

    cv::Mat filter(windowSize, windowSize, CV_32F, window);
    filter = filter/159;
    return filter;
}