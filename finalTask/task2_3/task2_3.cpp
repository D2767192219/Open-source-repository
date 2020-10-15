// Opencv_Tracker.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
  
#include <opencv2/opencv.hpp>
#include <opencv2/core/ocl.hpp>
#include<opencv2/tracking.hpp>
#include<iostream>
  
using namespace cv;
using namespace std;
  
int main()
{
    //跟踪算法类型
    string trackerTypes[7] = { "BOOSTING", "MIL", "KCF", "TLD","MEDIANFLOW", "MOSSE", "CSRT" };
  
    // Create a tracker 创建跟踪器
    string trackerType = trackerTypes[2];
  
    Ptr<Tracker> tracker;
  
    if (trackerType == "BOOSTING")
        tracker = TrackerBoosting::create();
    if (trackerType == "MIL")
        tracker = TrackerMIL::create();
    if (trackerType == "KCF")
        tracker = TrackerKCF::create();
    if (trackerType == "TLD")
        tracker = TrackerTLD::create();
    if (trackerType == "MEDIANFLOW")
        tracker = TrackerMedianFlow::create();
    if (trackerType == "MOSSE")
        tracker = TrackerMOSSE::create();
    if (trackerType == "CSRT")
        tracker = TrackerCSRT::create();
  
    // Read video 读视频
    VideoCapture video("IMG_2223.mp4");
  
    // Exit if video is not opened 如果没有视频文件
    if (!video.isOpened())
    {
        cout << "Could not read video file" << endl;
        return 1;
    }
  
    // Read first frame 读图
    Mat frame;
    Mat frame1;
    Mat frame2;
    bool ok = video.read(frame);
    const string videoln ="IMG_2223.mp4";
    VideoCapture capRefrnc(videoln);
    capRefrnc.set(CAP_PROP_POS_FRAMES,1);
    capRefrnc >> frame1;
    capRefrnc >> frame2;
    imshow("a frame",frame);
    Mat element =getStructuringElement(MORPH_RECT,Size(8,8));
    Mat out;
    dilate(frame1,frame1,element);
    medianBlur(frame1, frame1, 3);
    //resize(frame1,frame1,Size(frame.cols/4,frame.rows/4),0,0);
    Mat grayImage;
    Mat hsvImage;
    cvtColor(frame1, grayImage,CV_RGB2GRAY);
    cvtColor(frame2,hsvImage,COLOR_BGR2HSV);
    
    medianBlur(grayImage,grayImage,5);//中值模糊
    imshow("mohu",grayImage);  
    threshold(grayImage, grayImage,25,255,THRESH_BINARY);
    /*
    /HSV
    imwrite("hsv.jpg",hsvImage);
    //medianBlur(hsvImage,hsvImage,5);//中值模糊
    int iLowH = 100/2;  
    int iHighH = 340/2;  
 
    int iLowS = 0*255/100;   
    int iHighS = 30 *255/100;  
 
    int iLowV = 70 *255/100;  
    int iHighV = 100 *255/100;  
    
    Mat hsvThresholded;
    inRange(hsvImage, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), hsvThresholded);
    //开操作 (去除一些噪点)  如果二值化后图片干扰部分依然很多，增大下面的size
    Mat element = getStructuringElement(MORPH_RECT, Size(4, 4));  
    morphologyEx(hsvThresholded, hsvThresholded, MORPH_OPEN, element);  
  
    //闭操作 (连接一些连通域)  
    morphologyEx(hsvThresholded, hsvThresholded, MORPH_CLOSE, element);  
    imshow("hsv",hsvThresholded);

    */
    imshow("gray",grayImage);
    Mat cannyImg;
    Canny(grayImage,cannyImg,150,100,3);
        vector<vector<cv::Point>> contours;
        vector<cv::Point> maxAreaContour;
        findContours(cannyImg, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
        vector<vector<Point>>contours_ploy(contours.size());
        vector<Rect> boundRect(contours.size());
        vector<RotatedRect> box(contours.size());
    imshow("canny",cannyImg);
    for(int i=0;i<contours.size();i++)
    {
        approxPolyDP(Mat(contours[i]),contours_ploy[i],3,true);
        drawContours(frame1, contours, i, Scalar(120, 130, 255), 5, 8);
        drawContours(frame1,contours_ploy,i,Scalar(230,130,0),5,LINE_AA);
    }
    for(int i=0;i<contours.size();i++)
    {
        box[i]=minAreaRect(Mat(contours[i]));
        boundRect[i]=boundingRect(Mat(contours[i]));
    }

    double maxArea = 0;
        for (size_t i = 0; i < contours.size(); i++) {
                double area = box[i].size.width*box[i].size.height;
                if (area > maxArea) {
                             maxArea = area;
                             maxAreaContour = contours[i];
                }
        } 
        cv::Rect rect = cv::boundingRect(maxAreaContour);
        cv::rectangle(frame1, cv::Point(rect.x, rect.y), cv::Point(rect.x + rect.width, rect.y + rect.height), cv::Scalar(230,130,255), 5, 8);
        imshow("frame11",frame1);
    
    // Define initial boundibg box 初始检测框
    //Rect2d bbox(287, 23, 86, 320);
    Rect2d bbox = boundingRect(maxAreaContour);
    // Uncomment the line below to select a different bounding box 手动在图像上画矩形框
    //bbox = selectROI(frame, false);
  
    // Display bounding box 展示画的2边缘框
   
    rectangle(frame, bbox, Scalar(255, 0, 0), 2, 1);
    //imshow("Tracking", frame);
  
    //跟踪器初始化
    tracker->init(frame, bbox);

  
    while (video.read(frame))
    {
        // Start timer 开始计时
        double timer = (double)getTickCount();
        video>>frame1;

        // Update the tracking result 跟新跟踪器算法
        bool ok = tracker->update(frame, bbox);
  
        // Calculate Frames per second (FPS) 计算FPS
        float fps = getTickFrequency() / ((double)getTickCount() - timer);
  
        if (ok)
        {
            // Tracking success : Draw the tracked object 如果跟踪到目标画框
            rectangle(frame, bbox, Scalar(255, 0, 0), 2, 1);
        }
        else
        {
            // Tracking failure detected. 没有就输出跟踪失败
            putText(frame, "Tracking failure detected", Point(100, 80), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 255), 2);
        }
  
        // Display tracker type on frame 展示检测算法类型
        putText(frame, trackerType + " Tracker", Point(100, 20), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50, 170, 50), 2);
  
        // Display FPS on frame 表示FPS
        putText(frame, "FPS : " + to_string(int(fps)), Point(100, 50), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50, 170, 50), 2);
  
        // Display frame.
        imshow("Tracking", frame);
        
        //int key=waitKey(1000/30);  //waitKey(int delay） 等待函数
        // Exit if ESC pressed.

        int k = waitKey(30);
        if (k == 27)
        {
            break;
        }
    }

    waitKey(0);
    return 0;
}

