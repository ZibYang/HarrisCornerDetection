#include "harrisCornerDetector.hpp"
#include "sobel.hpp"

/**
 * @description First caculate the sobel kernal mask, then Ix Iy
 */
cv::Mat course::cornerHarris(cv::Mat frame, int windowSize, int sobelKernalSize, double k){
    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    // produce sobel
    std::cout<<"[ sobel ] is:"<<std::endl;
    int rowX[sobelKernalSize], colY[sobelKernalSize];
    tools::Sobel(sobelKernalSize,rowX,colY);

    std::cout<<"rowX:";
    tools::displaySobel(sobelKernalSize, rowX);
    std::cout<<"colY:";
    tools::displaySobel(sobelKernalSize, colY);
    
    // produce window function
    std::cout<<"[ window function ] is:"<<std::endl;
    cv::Mat gaussWindow = tools::gaussTemplate(windowSize).clone();

    // caculate Ix, Iy
    cv::Mat Ix = course::caculateI_(gray, windowSize, rowX, colY).clone() / windowSize;
    cv::Mat Iy = course::caculateI_(gray, windowSize, colY, rowX).clone() / windowSize;

    //caculate Ix^2, Iy^2, IxIy
    cv::Mat IxIx = powFrom(Ix, Ix).clone();
    cv::Mat IyIy = powFrom(Iy, Iy).clone();
    cv::Mat IxIy = powFrom(Ix, Iy).clone();

    //times w(x,y)
    cv::Mat wIxIx = course::windowFilter(IxIx, gaussWindow, windowSize).clone();
    cv::Mat wIxIy = course::windowFilter(IxIy, gaussWindow, windowSize).clone();
    cv::Mat wIyIy = course::windowFilter(IyIy, gaussWindow, windowSize).clone();

    // caculate eigen value and R
    int max = 0;
    cv::Mat R = caculateR(wIxIx, wIyIy, wIxIy, k, max).clone();

    // locate the Corner position
    cv::Mat result = course::locateTheCorner(frame, R, max).clone();

    return result;
}

/**
 * @description caculate Ix and Iy
 * k size = 3 or 5 or more
 * [-1 0 1]  [-1  -2 0  2 1]
 * [-2 0 2]  [-4  -8 0  8 4]
 * [-1 0 1]  [-6 -12 0 12 6]
 *           [-4  -8 0  8 4]
 *           [-1  -2 0  2 1]
*/
cv::Mat course::caculateI_(cv::Mat grayImg, int windowSize, int* rowX, int* colY){
    cv::Mat I_(grayImg.rows, grayImg.cols, CV_32S, cv::Scalar::all(0)); // 480 640
    for(int y = 0; y<grayImg.rows; y++){
        for(int x =0; x<grayImg.cols; x++){
            I_.at<int32_t>(y, x) = convolve(grayImg, x, y, windowSize, rowX, colY);
        }
    }
    return I_;
}

/**
 * @desicription convolve in image
 * [a b] * [e f] = [a*e b*f]
 * [c d]   [g h]   [c*g d*h]
*/

int course::convolve(cv::Mat grayImg, int x, int y, int windowSize, int* rowX, int* colY){
    int result = 0;
    int bias = windowSize / 2;
    
    for(int j = -bias; j <= bias; j++){
        for(int i = -bias; i<= bias; i++){
            if(x + i < 0 || y + j < 0 || x + i >= grayImg.cols || y+j >= grayImg.rows)
                continue;
            result += (int)grayImg.at<uchar>(y+j, x+i)*rowX[i + bias]*colY[j + bias];
        }
    }
    return result;
}

/**
 *  @description w(x, y) * [Ix^2 Ixy]
 *                         [Ixy Iy^2]   
 */
cv::Mat course::windowFilter(cv::Mat I, cv::Mat gauss, int windowSize){
    cv::Mat M_(I.rows, I.cols, CV_32S, cv::Scalar::all(0)); // 480 640
    int bias = windowSize/2;
    for(int y = 0; y<I.rows; y++){
        for(int x = 0; x<I.cols; x++){
            for(int i = -bias; i <= bias; i++){
                for(int j = -bias; j <= bias; j++){
                    if(x + i < 0 || y + j < 0 || x + i >= I.cols || y+j >= I.rows)
                        continue;
                    M_.at<int32_t>(y, x) += I.at<int32_t>(y+i, x+j)* gauss.at<float>(i+bias, j+bias);
                    
                }
            }
        }
    }
    return M_;
}
/**
 * @description [a b] ^2 = [a^2 b^2]
 *              [c d]      [c^2 d^2]
 */
cv::Mat course::powFrom(cv::Mat a, cv::Mat b){
    cv::Mat result(a.rows, a.cols, CV_32S, cv::Scalar::all(0));
    for(int y = 0; y < a.rows; y++)
        for(int x = 0; x < a.cols; x++){
            result.at<int32_t>(y, x) = a.at<int32_t>(y, x)*b.at<int32_t>(y, x);
    }
    return result;
}

cv::Mat course::caculateR(cv::Mat IxIx, cv::Mat IyIy, cv::Mat IxIy, int k, int& max){
    std::cout<<"--------------------------------------------------"<<"\n";
    std::cout<<"Ready to caculate R:"<<IxIx.rows<<" ,"<<IxIx.cols<<std::endl;
    int rows = IxIx.rows;
    int cols = IxIx.cols;

    cv::Mat lambda1(rows, cols, CV_32S, cv::Scalar::all(0));
    cv::Mat lambda2(rows, cols, CV_32S, cv::Scalar::all(0));
    cv::Mat R(rows, cols, CV_32S, cv::Scalar::all(0));

    for(int y = 0; y <rows; y++){
        for(int x = 0;x<cols; x++){
            double mData[] = {(double)IxIx.at<int32_t>(y, x), (double)IxIy.at<int32_t>(y, x), (double)IxIy.at<int32_t>(y, x), (double)IyIy.at<int32_t>(y, x)};
            cv::Mat M(2, 2, CV_64FC1, mData);
            cv::Mat eigenValue;
            cv::eigen(M, eigenValue);
            lambda1.at<int32_t>(y, x) = (int)eigenValue.at<double>(0, 0);
            lambda2.at<int32_t>(y, x) = (int)eigenValue.at<double>(1, 0);

            
            // int traceM = lambda1.at<int32_t>(y, x) + lambda2.at<int32_t>(y, x);
            // int detM = lambda1.at<int32_t>(y, x) * lambda2.at<int32_t>(y, x);
            int traceM = (double)IxIx.at<int32_t>(y, x) + (double)IyIy.at<int32_t>(y, x);
            int detM = (double)IxIx.at<int32_t>(y, x)*(double)IyIy.at<int32_t>(y, x) - (double)IxIy.at<int32_t>(y, x)*(double)IxIy.at<int32_t>(y, x);

            R.at<int32_t>(y, x) = detM - k*traceM*traceM;

            if(R.at<int32_t>(y, x) > max){
                max = R.at<int32_t>(y, x);
            }
            // test code
            // std::cout<<R.at<int32_t>(y, x)<<" ";
        }
        //test code
        //std::cout<<std::endl;
    }
    std::cout<<"saving:"<<std::endl;
    cv::imwrite("lambda1.jpg",lambda1);
    cv::imwrite("lambda2.jpg",lambda2);
    cv::imwrite("R.jpg",R);
    return R;
}

cv::Mat course::locateTheCorner(cv::Mat frame, cv::Mat R, int max){
    std::cout<<"--------------------------------------------------"<<"\n";
    std::cout<<"Max:"<<max<<"  ";
    std::cout<<R.rows<<" ,"<<R.cols<<std::endl;
    int threshold = 0.1 * max;
    int radius = 8;

    for(int y = 0; y<R.rows; y++){
        for(int x = 0; x<R.cols; x++){
            bool flag = true;
            for(int i = -radius; i <= radius; i++)
                for(int j = -radius; j <= radius; j++){
                    // test code
                    // std::cout<<"i:"<<i<<" j:"<<j<<" y+i"<<y+i<<" x+j"<<x+j<<std::endl;
                    if( x+j <0 || y+i<0 || x+j > R.cols || y+i >R.rows)
                        continue;
                    if(R.at<int32_t>(y, x) < R.at<int32_t>(y+i, x+j) || (R.at<int32_t>(y, x) == R.at<int32_t>(y+i, x+j) && (i<0||j<0)))
                        flag = false;
                }
            if(flag && R.at<int32_t>(y, x) > threshold){
                // std::cout<<"x"<<x<<",y"<<y<<" :"<<R.at<int32_t>(y, x)<<std::endl;
                // char as;
                // std::cin >> as;
                if(x>radius && y > radius && x<R.cols - radius && y<R.rows - radius){
                    cv::circle(frame, cv::Point(x, y), 5, cv::Scalar(0.0, 165.0, 255.0), 2);
                }
            }
        }
    }
    return frame;
}