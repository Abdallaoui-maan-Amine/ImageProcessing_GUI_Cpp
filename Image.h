#pragma once

#include<iostream>
#include <string>
#include <opencv2/opencv.hpp>



using namespace std;
using namespace cv;


class Image
{
    string path_;



public:

    Image(string path);

    Mat load_Image(int spaceColor);

    Mat applyConvolution(Mat img, vector< vector<double> > filter, int enhancement);

    Mat apply_Average_Filter(Mat img);

    Mat apply_Median_Filter(Mat img);

    Mat apply_Gradient_Filter(Mat Convolved_X_image, Mat Convolved_Y_image);

    Mat apply_Segmentation_by_Color_Selection(Mat img, int r_min, int r_max, int g_min, int g_max, int b_min, int b_max);

    Mat apply_Edge_Enhancement(Mat img);

    Mat apply_Canny_Filter(Mat img);

    Mat apply_hough_Algorithm(Mat img, int t_count);

    Mat show_Histogramm(Mat img);



};





