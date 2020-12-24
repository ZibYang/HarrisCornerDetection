#include "harrisCornerDetector.hpp"

void harrisCornerDetector_opencv(cv::Mat frame, int windowSize, int sobelKernalSize, double k);

int main(int argc, char** argv){
    int blockSize = std::atoi(argv[1]);
    int kSize = std::atoi(argv[2]);
    double k = std::atof(argv[3]);

    // param display
    std::cout<<"[ Block Size ] is :"<<blockSize<<std::endl;
    std::cout<<"[ K Size ] is :"<<kSize<<std::endl;
    std::cout<<"[ k ] is :"<<k<<std::endl;

    // test code 
    // cv::Mat frame = cv::imread(argv[4]);

    // cv::imshow("",course::cornerHarris(frame, blockSize, kSize, k));
    // cv::waitKey(0);

    try{
        cv::VideoCapture capture(0);
        cv::Mat frame;
        int keyInput;
        while(capture.read(frame)){
            keyInput = cv::waitKey(1);
            if(keyInput == 32) // press \Space to pause
            {
                harrisCornerDetector_opencv(frame, blockSize, kSize, k);
                cv::imshow("Harris Corner Detector", course::cornerHarris(frame, blockSize, kSize, k));
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

void harrisCornerDetector_opencv(cv::Mat frame, int windowSize, int sobelKernalSize, double k){
    cv::Mat frameImg = frame.clone();
    int thresh = 30; //当前阈值
    cv::Mat dstImage;//目标图
	cv::Mat normImage;//归一化后的图

    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    cv::cornerHarris(gray, dstImage, windowSize, sobelKernalSize, k, cv::BORDER_DEFAULT);
    // 归一化与转换
	cv::normalize(dstImage, normImage, 0, 255, cv::NORM_MINMAX, CV_32FC1, cv::Mat());

	// 将检测到的，且符合阈值条件的角点绘制出来
	for (int j = 0; j < normImage.rows; j++)
	{
		for (int i = 0; i < normImage.cols; i++)
		{
			if ((int)normImage.at<float>(j, i) > thresh + 80)
			{
				cv::circle(frameImg, cv::Point(i, j), 5, cv::Scalar(10, 10, 255), 2, 8, 0);
			}
		}
	}
    cv::imshow("Harris Corner Detector from openCV", frameImg);

}