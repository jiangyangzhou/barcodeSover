#ifndef BARCODE_LOCATE_H_INCLUDED
#define BARCODE_LOCATE_H_INCLUDED


#include <stdio.h>
#include <iostream>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\opencv.hpp>
#include <zbar.h>
#include "SingleLinkList.h"
#include "seqSet.h"
#include "seqList.h"
#include <string>  
#include <list>  
#include <vector>  
#include <map>  
#include <stack>  
#include <array>

using namespace std;
using namespace cv;
using namespace zbar;

struct region{
	pair<int,int> leftup;
	pair<int,int> rightdown = pair<int,int>(0,0);
	Mat *Map;
	int direction=0;

};

void EnableImg(Mat &src, Mat &dst, float perc1 = 0.03, float perc2 = 0.03) 
{
	int bright_num[256] = { 0 };
	dst = Mat::zeros(src.rows, src.cols, CV_8U);
	for (int r = 0; r < src.rows; r++) {
		for (int c = 0; c < src.cols; c++)
			bright_num[src.at<uchar>(r, c)] += 1;
	}
	int B_l_num = 0;
	int B_h_num = 0;
	int B_l = 0;
	int B_h = 0;

	for (int i = 0; i < 256; i++) {
		B_l_num += bright_num[i];
		if (B_l_num >= perc1 * src.rows * src.cols) {
			B_l = i;
			break;
		}
	}

	for (int i = 255; i >= 0; i--) {
		B_h_num += bright_num[i];
		if (B_h_num >= (perc2 * src.rows * src.cols)) {
			B_h = i;
			break;
		}
	}

	int interval = B_h - B_l + 1;
	for (int r = 0; r < src.rows; r++) {
		for (int c = 0; c < src.cols; c++) {
			if (src.at<uchar>(r, c) < B_l)
				dst.at<uchar>(r, c) = 0;
			else if (src.at<uchar>(r, c) > B_h)
				dst.at<uchar>(r, c) = 255;
			else
				dst.at<uchar>(r, c) = (src.at<uchar>(r, c) - B_l + 1) * 255 / interval;
		}
	}
}

void Seed_Filling(cv::Mat& binImg, cv::Mat& labelImg, int * region_num,vector<region> &region_vec)   //种子填充法  
{
	// 8邻接方法 

	int height = binImg.rows;
	int width = binImg.cols;
	Mat img1 = Mat::zeros(height,width, CV_8U);
	Mat element = getStructuringElement(MORPH_RECT,
		Size(3, 3),
		Point(1,1));

	dilate(binImg, img1, element);
	erode(img1, binImg, element);


	if (binImg.empty() ||
		binImg.type() != CV_8UC1)
	{
		return;
	}

	labelImg.release();
	binImg.convertTo(labelImg, CV_32SC1);

	
	uint16_t label = 0;
	int rows = binImg.rows - 1;
	int cols = binImg.cols - 1;
	for (int value = 1; value <= 8; value++) {
		
		for (int i = 1; i < rows - 1; i++)
		{
			int* data = labelImg.ptr<int>(i);
			for (int j = 1; j < cols - 1; j++)
			{
				if (data[j] == value)
				{
					uint16_t label = 100 * value;
					region reg = {pair<int,int>(i,j), pair<int,int>(i,j), &labelImg, value };
					int count = 0;
					std::stack<std::pair<int, int>> neighborPixels;
					std::stack<std::pair<int, int>> copy_neiPixels;
					neighborPixels.push(std::pair<int, int>(i, j));     // 像素位置: <i,j>  
					++label;  // 没有重复的团，开始新的标签  
					while (!neighborPixels.empty())
					{
						std::pair<int, int> curPixel = neighborPixels.top(); //如果与上一行中一个团有重合区域，则将上一行的那个团的标号赋给它  
						int curX = curPixel.first;
						int curY = curPixel.second;
						labelImg.at<int>(curX, curY) = label;

						neighborPixels.pop();
						copy_neiPixels.push(std::pair<int, int>(curX, curY));
						if (reg.rightdown.first < curX)
							reg.rightdown.first = curX;
						if (reg.rightdown.second < curY)
							reg.rightdown.second = curY;
						count++;

						if (labelImg.at<int>(curX, curY - 1) == value)
						{//左边  
							neighborPixels.push(std::pair<int, int>(curX, curY - 1));
						}
						if (labelImg.at<int>(curX, curY + 1) == value)
						{// 右边  
							neighborPixels.push(std::pair<int, int>(curX, curY + 1));
						}
						if (labelImg.at<int>(curX - 1, curY) == value)
						{// 上边  
							neighborPixels.push(std::pair<int, int>(curX - 1, curY));
						}
						if (labelImg.at<int>(curX + 1, curY) == value)
						{// 下边  
							neighborPixels.push(std::pair<int, int>(curX + 1, curY));
						}
						if (labelImg.at<int>(curX - 1, curY - 1) == value)
						{//左上边  
							neighborPixels.push(std::pair<int, int>(curX - 1, curY - 1));
						}
						if (labelImg.at<int>(curX - 1, curY + 1) == value)
						{// 右上边  
							neighborPixels.push(std::pair<int, int>(curX - 1, curY + 1));
						}
						if (labelImg.at<int>(curX + 1, curY - 1) == value)
						{// 左下边  
							neighborPixels.push(std::pair<int, int>(curX + 1, curY - 1));
						}
						if (labelImg.at<int>(curX + 1, curY + 1) == value)
						{// 右下边  
							neighborPixels.push(std::pair<int, int>(curX + 1, curY + 1));
						}

						if (labelImg.at<int>(curX, curY - 2) == value)
						{//左左边  
							neighborPixels.push(std::pair<int, int>(curX, curY - 2));
						}
						if (labelImg.at<int>(curX, curY + 2) == value)
						{// 右右边  
							neighborPixels.push(std::pair<int, int>(curX, curY + 2));
						}
						if (labelImg.at<int>(curX + 2, curY) == value)
						{// 上上边  
							neighborPixels.push(std::pair<int, int>(curX + 2, curY));
						}
						if (labelImg.at<int>(curX - 2, curY) == value)
						{// 下下边  
							neighborPixels.push(std::pair<int, int>(curX - 2, curY));
						}


						//cout << "count is :" << count<<"  and label is:"<<label<<"  value is"<<value<<endl;

					}
					cout << "what happened?";

					if (count < 50) {
						while (!copy_neiPixels.empty()) {
							std::pair<int, int> curPixel = copy_neiPixels.top();
							int curX = curPixel.first;
							int curY = curPixel.second;
							labelImg.at<int>(curX, curY) = 0;
							copy_neiPixels.pop();
						}
						label--;
						cout << "what happened?";
					}
					else {
						region_vec.push_back(reg);
					}
				}
			}
		}
		region_num[value-1] = label;

	}
	return;
}

void get_region(Mat & label_img, pair<int, int> &leftup, pair<int, int> &rightdown, int simbol) {
	int height = label_img.rows;
	int width = label_img.cols;
	leftup = pair<int, int>(1000, 1000);
	rightdown = pair<int, int>(0, 0);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (label_img.at<int>(i, j) == simbol) {
				if (i<leftup.first)
					leftup.first = i;
				if (i>rightdown.first)
					rightdown.first = i;
				if (j < leftup.second)
					leftup.second = j;
				if (j > rightdown.second)
					rightdown.second = j;
			}
		}
	}
}

void barcode_locate(string filename)
{
	Mat img;
	img = imread(filename);      //加载图像文件

								//	waitKey(50000);
	imshow("lena", img);//显示图片
	Mat imgGray, imgGaussian;
	cvtColor(img, imgGray, COLOR_BGR2GRAY);   //转换为灰度图
	GaussianBlur(imgGray, imgGaussian, Size(3, 3), 0.3, 0.3);  //高斯模糊

	Mat imgEnable; //直方图增强

	//equalizeHist(imgGaussian, imgEnable);
	EnableImg(imgGaussian, imgEnable);
//	imshow("enableImg", imgEnable);

	Mat dst_sobel[4];                 //sobel算子得到的边缘图
	Mat s_kernel[4];                  //sobel核
	s_kernel[0] = (Mat_<double>(3, 3) << -3, 0, 3, -10, 0, 10, -3, 0, 3);
	s_kernel[1] = (Mat_<double>(3, 3) << 0, -3, -10, 3, 0, -3, 10, 3, 0);
	s_kernel[2] = (Mat_<double>(3, 3) << -3, -10, -3, 0, 0, 0, 3, 10, 3);
	s_kernel[3] = (Mat_<double>(3, 3) << -10, -3, 0, -3, 0, 3, 0, 3, 10);

	for (int i = 0; i < 4; i++) {
		s_kernel[i] = s_kernel[i] / 16;
		filter2D(imgEnable, dst_sobel[i], CV_16S, s_kernel[i]);
		Mat dst;
		convertScaleAbs(dst_sobel[i], dst);
		dst_sobel[i] = dst;
		// string a = "sobel";
		//imshow(a+char(i+68), dst_sobel[i]);
	}       //获取sobel边缘图

	int height = img.rows;
	int width = img.cols;
	int region_size = 19;
	int stride = 9;
	int thre1 = 30;       //梯度大于thre1被认定
	int thre2 = 10;       //第二大梯度方向与第一大梯度差小于thre2被认定
	double thre3 = 0.4;     //无梯度比例不超过区域面积*thre3

	//cout << "height is:" << height << endl;
	//cout << "width is:" << width << endl;


	Mat direMap = Mat::zeros(height, width, CV_8U);      //保存每个点的方向

	for (int r = 0; r < height; r++) {
		for (int c = 0; c < width; c++) {
			int max_a = dst_sobel[0].at<uchar>(r, c);
			int max_b = max_a;
			int a = 0;
			int b = 0;
			for (int i = 1; i < 4; i++) {
				if (dst_sobel[i].at<uchar>(r, c) > max_a) {
					max_b = max_a;
					max_a = dst_sobel[i].at<uchar>(r, c);
					b = a;
					a = i;
				}
				else if (dst_sobel[i].at<uchar>(r, c) > max_b) {
					max_b = dst_sobel[i].at<uchar>(r, c);
					b = i;
				}
			}
			if (max_a < thre1)
				direMap.at<uchar>(r, c) = 0;
			else if (max_a > thre1 && (max_a - max_b) < thre2 && (a - b == 1 || a - b == -1 || 8 + a - b == 1 || 8 + b - a == 1))
				direMap.at<uchar>(r, c) = a + b + 1;
			else
				direMap.at<uchar>(r, c) = 2 * a + 1;
		}
	}
	//direMap记录每一个点的方向，记录两个最大方向，分为八个方向，由1到8.
	//cout << direMap;
	cout << "what happened?292";

	Mat region_dir = Mat::zeros(height / stride, width / stride, CV_8U);
	int region_area = region_size*region_size;

	for (int r = 0; r < height - region_size; r = r + stride) {
		for (int c = 0; c < width - region_size; c = c + stride) {
			int dirNum[9] = { 0,0,0,0,0,0,0,0,0 };
			for (int i = r; i < r + region_size; i++)
				for (int j = c; j < c + region_size; j++)
					dirNum[direMap.at<uchar>(i, j)] += 1;
			int max_a = dirNum[0];
			int a = 0;
			for (int i = 0; i < 9; i++) {
				if (dirNum[i] > max_a) {
					max_a = dirNum[i];
					a = i;
				}
			}

			if (max_a > 0.4 * region_area)
				region_dir.at<uchar>(r / stride, c / stride) = a;
			else
				region_dir.at<uchar>(r / stride, c / stride) = 0;
		}
	}
//	cout << region_dir << endl;   //得到一个区域的方向

	Mat link_area0 = Mat::zeros(height, width, CV_8U);


	cout << "what happened?327";

	Mat labelImg = Mat::zeros(height / stride, width / stride, CV_8U);
	//get_near_graph(region_dir, link_area1, height, width, stride, region_size);
	int region_num[8] = {0};
	vector<region> region_vec;

	Seed_Filling(region_dir, labelImg, region_num,region_vec);
	cout << labelImg.rows << " " << labelImg.cols << endl;

	for (int j = 0; j < 8; j++) cout << region_num[j]<< " ";
	cv::imshow("labelImg", labelImg);
	for (int i = 1; i <= 8; i++) {
		for (int n = 100*i+1; n <= region_num[i-1]; n++){
			pair<int, int> leftup;
			pair<int, int> rightdown;
			get_region(labelImg, leftup, rightdown, n);
			cout << leftup.first << " " << leftup.second << endl;
			cout << rightdown.first << " " << rightdown.second << endl;

			ImageScanner scanner;
			scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);
			cout << imgGray.rows << " " << rightdown.first*stride - leftup.first*stride << endl;
			cout << imgGray.cols << " " << rightdown.second*stride - leftup.second*stride << endl;
			Rect rect(leftup.second*stride, leftup.first*stride, rightdown.second*stride - leftup.second*stride + region_size, rightdown.first*stride - leftup.first*stride + region_size);
			Mat roi_img = imgGray(rect);
			int width = roi_img.cols;
			int height = roi_img.rows;
			Mat r_img;
			int mean = 0;
			for (int r = 0; r < height; r++)
				for (int c = 0; c < width; c++)
					mean += roi_img.at<uchar>(r, c);
			mean /= width*height;
			threshold(roi_img, r_img, mean, 255, CV_THRESH_BINARY);
			//cout << roi_img;
			if (!roi_img.data)
			{
				cout << "请确认图片" << endl;
				system("pause");
			}
			cout << "what happened?";


			Image imageZbar(width, height, "Y800", (uchar *)roi_img.data, width * height);
			scanner.scan(imageZbar); //扫描条码                            
			Image::SymbolIterator symbol = imageZbar.symbol_begin();
			if (imageZbar.symbol_begin() == imageZbar.symbol_end())
			{
				cout << "查询条码失败，请检查图片！" << endl;
			}
			for (; symbol != imageZbar.symbol_end(); ++symbol)
			{
				cout << "类型：" << endl << symbol->get_type_name() << endl << endl;
				cout << "条码：" << endl << symbol->get_data() << endl << endl;
			}
			string a = "roi_img";
			cout << "rows" << roi_img.rows << "cols" << roi_img.cols << endl;
			imshow(a + char(n + 48), roi_img);
			//cout << roi_img;
			imageZbar.set_data(NULL, 0);
		}
	}
	//icvprCcaBySeedFill(region_dir, labelImg);
	//cout << labelImg << endl;
	cout << "here ok?384"<<endl;
/*	for (int i = 0; i < labelImg.rows; i++) {
		for (int j = 0; j < labelImg.cols; j++)
			labelImg.at<uchar>(i,j) = (labelImg.at<uchar>(i,j)-100) * 20;
	}*/
	cout << imgGray.rows - region_size << " " << imgGray.cols - region_size << endl;
	cout<< (imgGray.rows - region_size)/stride << " " << (imgGray.cols - region_size)/stride << endl;
	cout << labelImg.rows << " " << labelImg.cols<<endl;
	cout << "type is:"<<labelImg.type()<<endl;
	cvtColor(img, imgGray, COLOR_BGR2GRAY);   //转换为灰度图
	imshow("gray", imgGray);
	for (int i = 0; i < imgGray.rows-region_size; i++) {
		for (int j = 0; j < imgGray.cols - region_size; j++) {
			//cout << imgGray.at<uchar>(i, j)<<" ";
			imgGray.at<uchar>(i, j) = (region_dir.at<uchar>(i / stride, j / stride))*50;
		}
	}

	imshow("imgGray",imgGray);

	imshow("link_area0", link_area0);
	waitKey(10000000);

}


#endif 
