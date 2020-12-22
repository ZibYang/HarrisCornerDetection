#include "harrisCornerDetector.hpp"

cv::Mat course::cornerHarris(cv::Mat gray, int a, int b, double c){
    return gray;
}

int course::yangHuiTriangle(int row, int col){
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

int course::smooth(int x, int windowSize){
    return yangHuiTriangle(x, windowSize-1);
}

int course::diff(int x, int windowSize){
    return yangHuiTriangle(x-1, windowSize-2) - yangHuiTriangle(x, windowSize-2);
}

void course::Sobel(int windowSize, int* rowX, int* colY){
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
void course::caculateIxAndIy(){

}