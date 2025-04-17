#ifndef OCRSERVOCE_HPP
#define OCRSERVOCE_HPP

#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <opencv2/opencv.hpp>
#include <vector>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include "datastore.hpp"
#include "opencv2/core/mat.hpp"


class ocrService{
public:

  bool initTesseract();
  bool setMode(tesseract::PageSegMode mode);
  bool loadImage(cv::Mat img);
  bool deInit();
  std::string returnText();
  std::string returnTSVText();
  std::string returnHOCRText();
  cv::Mat checkColorDepth(cv::Mat img);
  cv::Mat filterTest(cv::Mat img);
  cv::Mat Test(cv::Mat img);
  cv::Mat ApplyMaskWhiteBg(const cv::Mat& src, const cv::Mat& mask);
  cv::Mat ExtractBlackOnWthite(const cv::Mat& src);

private:
  tesseract::TessBaseAPI *api;
    
};

#endif 
