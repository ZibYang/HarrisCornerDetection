#include "harrisCornerDetector.hpp"

int main(int argc, char** argv){
    int blockSize = std::atoi(argv[1]);
    int kSize = std::atoi(argv[2]);
    double k = std::atof(argv[3]);

    // param display
    std::cout<<"[ Block Size ] is :"<<blockSize<<std::endl;
    std::cout<<"[ K Size ] is :"<<kSize<<std::endl;
    std::cout<<"[ k ] is :"<<k<<std::endl;
    std::cout<<"[ sobel ] is:"<<std::endl;
    int rowX[kSize], colY[kSize];
    course::Sobel(kSize,rowX,colY);

    std::cout<<"rowX:";
    for(int i = 0;i<kSize;i++){
       std::cout<<rowX[i]<<" ";
    }
    std::cout<<"colx:";
    for(int i = 0;i<kSize;i++){
        std::cout<<colY[i]<<" ";
    }
    
    try{
        cv::VideoCapture capture(0);
        cv::Mat frame;
        int keyInput;
        while(capture.read(frame)){
            keyInput = cv::waitKey(1);
            if(keyInput == 32) // press \Space to pause
            {
                cv::Mat gray;
                cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
                cv::imshow("Harris Corner Detector", course::cornerHarris(gray, blockSize, kSize, k));
                keyInput = cv::waitKey(0);
            }else{
                cv::imshow("Harris Corner Detector", frame);
            }
            if(keyInput == 27){ // press \Esc to exit
                break;
            }
        }
    }catch(std::exception e){
        std::cout<<e.what()<<std::endl;
    }
    return 0;
}