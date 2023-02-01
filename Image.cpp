#include "Image.h"
#include<math.h>
#include<iostream>
#include <vector>

#include <opencv2/opencv.hpp>

#include <string>

#include "mainwindow.h"
#include "./ui_mainwindow.h"

using namespace std;
using namespace cv;



Mat averageFilter(Mat image) {
    int kernelSize = 3; // size of the filter kernel
    Mat filteredImage(image.rows, image.cols, image.type()); // create new image for filtered output



    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            int sumGray = 0;  // sum of pixel values

            int count = 0; // count of pixels in kernel

            // iterate through kernel
            for (int ky = -kernelSize / 2; ky <= kernelSize / 2; ky++) {
                for (int kx = -kernelSize / 2; kx <= kernelSize / 2; kx++) {

                    // check if kernel pixel is within image bounds
                    if (y + ky >= 0 && y + ky < image.rows && x + kx >= 0 && x + kx < image.cols) {
                        sumGray += image.at<uchar>(y + ky, x + kx);
                        count++;
                    }
                }
            }

            // Calculate average and set filtered image pixel
            filteredImage.at<uchar>(y, x) = sumGray / count;
        }
    }

    return filteredImage;
}







Mat medianFilter(Mat image) {
    int kernelSize = 9; // size of the filter kernel
    Mat filteredImage(image.rows, image.cols, image.type()); // create new image for filtered output

    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            int sumGray = 0;  // sum of pixel values

            int count = 0; // count of pixels in kernel

            // iterate through kernel
            vector<double> data(kernelSize*kernelSize, 0);
            for (int ky = -kernelSize / 2; ky <= kernelSize / 2; ky++) {
                for (int kx = -kernelSize / 2; kx <= kernelSize / 2; kx++) {

                    // check if kernel pixel is within image bounds
                    if (y + ky >= 0 && y + ky < image.rows && x + kx >= 0 && x + kx < image.cols) {
                        data[count] = image.at<uchar>(y + ky, x + kx);
                        sumGray += image.at<uchar>(y + ky, x + kx);
                        count++;
                    }
                }
            }
            sort(data.begin(), data.end());
            // Calculate average and set filtered image pixel
            filteredImage.at<uchar>(y, x) = data[(int)(pow(kernelSize,2)/2)];
        }
    }

    return filteredImage;
}



Mat convolutionFilter(Mat image, vector<std::vector<double>> filter, int enhancement) {
    int filterSize = filter.size(); // size of the filter
    int filterRadius = filterSize / 2; // radius of the filter
    Mat filteredImage(image.rows, image.cols, CV_8UC1); // create new image for filtered output

    for (int y = filterRadius; y < image.rows - filterRadius; y++) {
        for (int x = filterRadius; x < image.cols - filterRadius; x++) {
            int sum = 0; // sum of pixel values

            // iterate through kernel
            for (int ky = -filterRadius; ky <= filterRadius; ky++) {
                for (int kx = -filterRadius; kx <= filterRadius; kx++) {
                    sum += image.at<uchar>(y + ky, x + kx) * filter[ky + filterRadius][kx + filterRadius];
                }
            }

            if (enhancement == 0){
                filteredImage.at<uchar>(y, x) = min(255, max(0, sum));
            } else if (enhancement == 1){
                filteredImage.at<uchar>(y, x) = min(255, max(0, sum)) + image.at<uchar>(y, x);
            }
        }
    }

    return filteredImage;
}


Mat apply_Edge_Enhancement(Mat image){

    vector<vector<double>> laplacien_filter
        {
            {0, -1, 0},
            {-1, 4, -1},
            {0, -1, 0}
        };

    int filterSize = laplacien_filter.size(); // size of the filter
    int filterRadius = filterSize / 2; // radius of the filter
    Mat filteredImage(image.rows, image.cols, CV_8UC1); // create new image for filtered output
    //std::cout << "\n salut";
    for (int y = filterRadius; y < image.rows - filterRadius; y++) {
        for (int x = filterRadius; x < image.cols - filterRadius; x++) {
            int sum = 0; // sum of pixel values

            // iterate through kernel
            for (int ky = -filterRadius; ky <= filterRadius; ky++) {
                for (int kx = -filterRadius; kx <= filterRadius; kx++) {
                    sum += image.at<uchar>(y + ky, x + kx) * laplacien_filter[ky + filterRadius][kx + filterRadius];
                }
            }

            filteredImage.at<uchar>(y, x) = 0; //min(255, max(0, sum)) + ;
        }
    }

    return filteredImage;
    //return
}






Mat Segmentation_by_Color_Selection(Mat image, int r_min, int r_max, int g_min, int g_max, int b_min, int b_max){


    cvtColor(image, image, COLOR_BGR2RGB);
    Mat segmentedImage(image.size(), image.type());

    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            Vec3b color = image.at<Vec3b>(i, j);
            int b = color[2], g = color[1], r = color[0];
            //int b = color[0], g = color[1], r = color[2];
            if (r >= r_min && r <= r_max && g >= g_min && g <= g_max && b >= b_min && b <= b_max) {
                segmentedImage.at<Vec3b>(i, j) = image.at<Vec3b>(i, j);
                //std::cout <<"\n\n --> " image.at<Vec3b>(i, j)[0] << ", " << image.at<Vec3b>(i, j)[1], ", " image.at<Vec3b>(i, j)[2];
            } else {
                segmentedImage.at<Vec3b>(i, j) = Vec3b(0,0,0);
            }
        }
    }
    return segmentedImage;
}


Mat gradient_Convolution_XY(Mat Convolved_X_image, Mat Convolved_Y_image){

    int rows = Convolved_X_image.rows;
    int columns = Convolved_X_image.cols;

    Mat filteredImage(rows, columns, CV_8UC1);

    for(int i=0; i<rows; i++){
        for(int j=0; j<columns; j++ ){
            filteredImage.at<uchar>(i, j) = sqrt( pow(Convolved_X_image.at<uchar>(i, j),2) + pow(Convolved_Y_image.at<uchar>(i, j),2));
        }
    }
    return filteredImage;
}


Mat histogramm(Mat img){
    // Create occurrence vector
    std::vector<int> histogram(256, 0);
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            histogram[img.at<uchar>(i, j)]++;
        }
    }

    // Draw histogram
    int hist_w = 512, hist_h = 300;
    int bin_w = cvRound((double)hist_w / 256);
    Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(200, 200, 200));
    for (int i = 0; i < 256; i++) {
        cv::rectangle(histImage, cv::Point(i*bin_w, hist_h), cv::Point((i + 1)*bin_w, hist_h - 20*round(histogram[i] * hist_h / img.total()) ), cv::Scalar(0, 0, 255), -1);
    }

    return histImage;
}





Mat angles;
Mat gFiltered;
Mat grayscaled;
Mat img;
Mat hough_space;



Mat toGrayScale(Mat img)
{
    grayscaled = Mat(img.rows, img.cols, CV_8UC1); //To one channel
    for (int i = 0; i < img.rows; i++)
        for (int j = 0; j < img.cols; j++)
        {
            int b = img.at<Vec3b>(i, j)[0];
            int g = img.at<Vec3b>(i, j)[1];
            int r = img.at<Vec3b>(i, j)[2];

            double newValue = (r * 0.2126 + g * 0.7152 + b * 0.0722);
            grayscaled.at<uchar>(i, j) = newValue;

        }
    return grayscaled;
}

Mat threshold(Mat imgin, int low, int high)
{
    if (low > 255)
        low = 255;
    if (high > 255)
        high = 255;

    Mat EdgeMat = Mat(imgin.rows, imgin.cols, imgin.type());

    for (int i = 0; i < imgin.rows; i++)
    {
        for (int j = 0; j < imgin.cols; j++)
        {
            EdgeMat.at<uchar>(i, j) = imgin.at<uchar>(i, j);
            if (EdgeMat.at<uchar>(i, j) > high)
                EdgeMat.at<uchar>(i, j) = 255;
            else if (EdgeMat.at<uchar>(i, j) < low)
                EdgeMat.at<uchar>(i, j) = 0;
            else
            {
                bool anyHigh = false;
                bool anyBetween = false;
                for (int x = i - 1; x < i + 2; x++)
                {
                    for (int y = j - 1; y < j + 2; y++)
                    {
                        if (x <= 0 || y <= 0 || EdgeMat.rows || y > EdgeMat.cols) //Out of bounds
                            continue;
                        else
                        {
                            if (EdgeMat.at<uchar>(x, y) > high)
                            {
                                EdgeMat.at<uchar>(i, j) = 255;
                                anyHigh = true;
                                break;
                            }
                            else if (EdgeMat.at<uchar>(x, y) <= high && EdgeMat.at<uchar>(x, y) >= low)
                                anyBetween = true;
                        }
                    }
                    if (anyHigh)
                        break;
                }
                if (!anyHigh && anyBetween)
                    for (int x = i - 2; x < i + 3; x++)
                    {
                        for (int y = j - 1; y < j + 3; y++)
                        {
                            if (x < 0 || y < 0 || x > EdgeMat.rows || y > EdgeMat.cols) //Out of bounds
                                continue;
                            else
                            {
                                if (EdgeMat.at<uchar>(x, y) > high)
                                {
                                    EdgeMat.at<uchar>(i, j) = 255;
                                    anyHigh = true;
                                    break;
                                }
                            }
                        }
                        if (anyHigh)
                            break;
                    }
                if (!anyHigh)
                    EdgeMat.at<uchar>(i, j) = 0;
            }
        }
    }
    return EdgeMat;
}

Mat sobel()
{

    //Sobel X Filter
    double x1[] = { -1.0, 0, 1.0 };
    double x2[] = { -2.0, 0, 2.0 };
    double x3[] = { -1.0, 0, 1.0 };

    vector<vector<double>> xFilter(3);
    xFilter[0].assign(x1, x1 + 3);
    xFilter[1].assign(x2, x2 + 3);
    xFilter[2].assign(x3, x3 + 3);

    //Sobel Y Filter
    double y1[] = { 1.0, 2.0, 1.0 };
    double y2[] = { 0, 0, 0 };
    double y3[] = { -1.0, -2.0, -1.0 };

    vector<vector<double>> yFilter(3);
    yFilter[0].assign(y1, y1 + 3);
    yFilter[1].assign(y2, y2 + 3);
    yFilter[2].assign(y3, y3 + 3);

    //Limit Size
    int size = (int)xFilter.size() / 2;

    Mat filteredImg = Mat(gFiltered.rows - 2 * size, gFiltered.cols - 2 * size, CV_8UC1);

    angles = Mat(gFiltered.rows - 2 * size, gFiltered.cols - 2 * size, CV_32FC1); //AngleMap

    for (int i = size; i < gFiltered.rows - size; i++)
    {
        for (int j = size; j < gFiltered.cols - size; j++)
        {
            double sumx = 0;
            double sumy = 0;

            for (int x = 0; x < xFilter.size(); x++)
                for (int y = 0; y < xFilter.size(); y++)
                {
                    sumx += xFilter[x][y] * (double)(gFiltered.at<uchar>(i + x - size, j + y - size)); //Sobel_X Filter Value
                    sumy += yFilter[x][y] * (double)(gFiltered.at<uchar>(i + x - size, j + y - size)); //Sobel_Y Filter Value
                }
            double sumxsq = sumx * sumx;
            double sumysq = sumy * sumy;

            double sq2 = sqrt(sumxsq + sumysq);

            if (sq2 > 255) //Unsigned Char Fix
                sq2 = 255;
            filteredImg.at<uchar>(i - size, j - size) = sq2;

            if (sumx == 0) //Arctan Fix
                angles.at<float>(i - size, j - size) = 90;
            else
                angles.at<float>(i - size, j - size) = atan(sumy / sumx);
        }
    }

    return filteredImg;
}

vector<vector<double>> createFilter(int row, int column, double sigmaIn)
{
    vector<vector<double>> filter;

    for (int i = 0; i < row; i++)
    {
        vector<double> col;
        for (int j = 0; j < column; j++)
        {
            col.push_back(-1);
        }
        filter.push_back(col);
    }

    float coordSum = 0;
    float constant = 2.0 * sigmaIn * sigmaIn;

    // Sum is for normalization
    float sum = 0.0;

    for (int x = -row / 2; x <= row / 2; x++)
    {
        for (int y = -column / 2; y <= column / 2; y++)
        {
            coordSum = (x * x + y * y);
            filter[x + row / 2][y + column / 2] = (exp(-(coordSum) / constant)) / (M_PI * constant);
            sum += filter[x + row / 2][y + column / 2];
        }
    }

    // Normalize the Filter
    for (int i = 0; i < row; i++)
        for (int j = 0; j < column; j++)
            filter[i][j] /= sum;

    return filter;

}

Mat useFilter(Mat img_in, vector<vector<double>> filterIn)
{
    int size = (int)filterIn.size() / 2;

    cout << "\n\n " << img_in.rows - 2 * size;
    cout << "\n\n " << img_in.cols - 2 * size << "\n\n";

    Mat filteredImg = Mat(img_in.rows - 2 * size, img_in.cols - 2 * size, CV_8UC1);
    for (int i = size; i < img_in.rows - size; i++)
    {
        for (int j = size; j < img_in.cols - size; j++)
        {
            double sum = 0;

            for (int x = 0; x < filterIn.size(); x++)
                for (int y = 0; y < filterIn.size(); y++)
                {
                    sum += filterIn[x][y] * (double)(img_in.at<uchar>(i + x - size, j + y - size));
                }

            filteredImg.at<uchar>(i - size, j - size) = sum;
        }

    }
    return filteredImg;
}

Mat nonMaxSupp( Mat sFiltered, Mat angles)
{
    Mat nonMaxSupped = Mat(sFiltered.rows - 2, sFiltered.cols - 2, CV_8UC1);
    for (int i = 1; i < sFiltered.rows - 1; i++) {
        for (int j = 1; j < sFiltered.cols - 1; j++) {
            float Tangent = angles.at<float>(i, j);

            nonMaxSupped.at<uchar>(i - 1, j - 1) = sFiltered.at<uchar>(i, j);
            //Horizontal Edge
            if (((-22.5 < Tangent) && (Tangent <= 22.5)) || ((157.5 < Tangent) && (Tangent <= -157.5)))
            {
                if ((sFiltered.at<uchar>(i, j) < sFiltered.at<uchar>(i, j + 1)) || (sFiltered.at<uchar>(i, j) < sFiltered.at<uchar>(i, j - 1)))
                    nonMaxSupped.at<uchar>(i - 1, j - 1) = 0;
            }
            //Vertical Edge
            if (((-112.5 < Tangent) && (Tangent <= -67.5)) || ((67.5 < Tangent) && (Tangent <= 112.5)))
            {
                if ((sFiltered.at<uchar>(i, j) < sFiltered.at<uchar>(i + 1, j)) || (sFiltered.at<uchar>(i, j) < sFiltered.at<uchar>(i - 1, j)))
                    nonMaxSupped.at<uchar>(i - 1, j - 1) = 0;
            }

            //-45 Degree Edge
            if (((-67.5 < Tangent) && (Tangent <= -22.5)) || ((112.5 < Tangent) && (Tangent <= 157.5)))
            {
                if ((sFiltered.at<uchar>(i, j) < sFiltered.at<uchar>(i - 1, j + 1)) || (sFiltered.at<uchar>(i, j) < sFiltered.at<uchar>(i + 1, j - 1)))
                    nonMaxSupped.at<uchar>(i - 1, j - 1) = 0;
            }

            //45 Degree Edge
            if (((-157.5 < Tangent) && (Tangent <= -112.5)) || ((22.5 < Tangent) && (Tangent <= 67.5)))
            {
                if ((sFiltered.at<uchar>(i, j) < sFiltered.at<uchar>(i + 1, j + 1)) || (sFiltered.at<uchar>(i, j) < sFiltered.at<uchar>(i - 1, j - 1)))
                    nonMaxSupped.at<uchar>(i - 1, j - 1) = 0;
            }
        }
    }
    return nonMaxSupped;
}

Mat  plot_line(Mat RGB, Mat GRAY, Mat accumulator, vector<double> rhos, vector<double> thetas, int t_count) {
    // threshold

    //imshow("Detection des lignes", RGB);
    int edge_width = GRAY.size().height;
    for (int y = 0; y < accumulator.size().height; y++) {
        for (int x = 0; x < accumulator.size().width; x++) {
            if (accumulator.at<int>(y, x) > t_count) {
                double rho = rhos[y];
                double theta = thetas[x];
                double a = cos(theta * 3.14 / 180);
                double b = sin(theta * 3.14 / 180);
                int x1 = (int)(edge_width / 2 + 500);
                int x2 = (int)(edge_width / 2 - 500);
                int y1 = (int)((rho - x1 * a) / b);
                int y2 = (int)((rho - x2 * a) / b);
                line(RGB, Point(y1, x1), Point(y2 + 8, x2 + 2), Scalar(rand()%255, rand()%255, rand()%255), 2);
            }
        }
    }

    return RGB;

    // imshow("Detection des lignes", RGB);
    // waitKey(0);
}

vector<double> linspace(double start, double end, int num_points) {
    std::vector<double> result;
    double step = (end - start) / (num_points - 1);
    for (int i = 0; i < num_points; i++) {
        result.push_back(start + i * step);
    }
    return result;
}


Mat canny_Filter(Mat img) {

    grayscaled = toGrayScale(img);




    vector<vector<double>> filter = createFilter(3, 3, 1);

    //Print filter
    for (int i = 0; i < filter.size(); i++)
    {
        for (int j = 0; j < filter[i].size(); j++)
        {
            cout << filter[i][j] << " ";
        }
    }
    //grayscaled = Mat(grayscaled); //Grayscale the image
    gFiltered = Mat(useFilter(grayscaled, filter)); //Gaussian Filter
    Mat sFiltered = Mat(sobel()); //Sobel Filter

    Mat non = Mat(nonMaxSupp(sFiltered, angles)); //Non-Maxima Suppression
    Mat filtred_Image_by_Canny = Mat(threshold(non, 20, 40)); //Double Threshold and Finalize


    return filtred_Image_by_Canny;
}



Mat hough_Algorithm(Mat img, int t_count) {


    Mat thres = canny_Filter(img);

    double rhoResolution = 0.5;
    double thetaResolution = 0.5;

    Size size = thres.size();
    int M = size.width;
    int N = size.height;

    vector<double> theta = linspace(-90, 0, ceil(90 / thetaResolution) + 1);

    vector<double> theta1 = theta;
    std::reverse(theta1.begin() + 2, theta1.end());

    for (int i = 0; i < theta.size(); i++) {
        theta1[i] = -1 * theta1[i];
    }

    theta.insert(theta.begin(), theta1.begin(), theta1.end());

    //calculer la ditance de la diagonale
    double  D = sqrt(pow((M - 1), 2) + pow((N - 1), 2));
    double  q = ceil(D / rhoResolution);
    double  nrho = 2 * q + 1;

    vector<double> rho = linspace(-q * rhoResolution, q * rhoResolution, nrho);

    //initialisation de la matrice d'accumulation
    Mat h = Mat::zeros(rho.size(), theta.size(), CV_32S);
    vector<double> t(theta.size(), 1);
    vector<double> cost(theta.size(), 1);
    vector<double> sint(theta.size(), 1);
    vector<double> r(theta.size(), 1);

    for (int i = 0; i < theta.size(); i++) {
        t[i] = (theta[i] * 3.14) / 180;
        cost[i] = cos(t[i]);
        sint[i] = sin(t[i]);
    }

    for (int y = 0; y < M; y++) {
        for (int x = 0; x < N; x++) {
            if (thres.at<uchar>(x, y)) {
                for (int i = 0; i < t.size(); i++) {
                    r[i] = (x - 1) * cost[i] + (y - 1) * sint[i];
                    r[i] = round(r[i] / rhoResolution) + q + 1;
                }


                for (int k = 0; k < t.size(); k++) {
                    h.at<int>(r[k], k) += 1;
                    //h.at<int>(r[k], k) = 1;
                    hough_space = Mat(h);
                }
            }
        }
    }

    Mat RGB = plot_line(img, grayscaled, h, rho, theta, t_count);
    return RGB;

}









// ######################################################################################






Image::Image(string path){
    path_ = path;
}



Mat Image::load_Image(int spaceColor){
    return imread(path_, spaceColor);
}


Mat Image::show_Histogramm(Mat img){
    return histogramm(img);
}




Mat Image::apply_Average_Filter(Mat img){
    return averageFilter(img);
}

Mat Image::apply_Median_Filter(Mat img){
    return medianFilter(img);
}


Mat Image::applyConvolution(Mat img, vector< vector<double> > filter, int enhancement){
    return convolutionFilter(img, filter, enhancement);
}
/*
Mat Image::apply_Edge_Enhancement(Mat image){
    //Mat filteredImage(img.rows, img.cols, CV_8UC1);
    //return filteredImage;
    //return apply_Edge_Enhancement(image);
    vector<vector<double>> laplacien_filter
        {
            {0, -1, 0},
            {-1, 4, -1},
            {0, -1, 0}
        };
    return convolutionFilter(image, laplacien_filter);

}*/

Mat Image::apply_Gradient_Filter(Mat Convolved_X_image, Mat Convolved_Y_image){
    return gradient_Convolution_XY(Convolved_X_image, Convolved_Y_image);
}


Mat Image::apply_Segmentation_by_Color_Selection(Mat img, int r_min, int r_max, int g_min, int g_max, int b_min, int b_max){
    return Segmentation_by_Color_Selection(img, r_min, r_max, g_min, g_max, b_min, b_max);
}




Mat Image::apply_Canny_Filter(Mat img){
    return canny_Filter(img);
}

Mat Image::apply_hough_Algorithm(Mat img, int t_count){
    return hough_Algorithm(img, t_count);
}







