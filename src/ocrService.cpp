#include "ocrService.hpp"
#include <tesseract/publictypes.h>

 bool ocrService::initTesseract(){
    ocrService::api = new tesseract::TessBaseAPI();
    
    if (api->Init(NULL, "pol")) {
        fprintf(stderr, "Could not initialize tesseract.\n");
        return false;
    }
   
    return true;
 }

 bool ocrService::setMode(tesseract::PageSegMode mode){
    /*  PSM_OSD_ONLY = 0,      ///< Orientation and script detection only.
  PSM_AUTO_OSD = 1,      ///< Automatic page segmentation with orientation and
                         ///< script detection. (OSD)
  PSM_AUTO_ONLY = 2,     ///< Automatic page segmentation, but no OSD, or OCR.
  PSM_AUTO = 3,          ///< Fully automatic page segmentation, but no OSD.
  PSM_SINGLE_COLUMN = 4, ///< Assume a single column of text of variable sizes.
  PSM_SINGLE_BLOCK_VERT_TEXT = 5, ///< Assume a single uniform block of
                                  ///< vertically aligned text.
  PSM_SINGLE_BLOCK = 6, ///< Assume a single uniform block of text. (Default.)
  PSM_SINGLE_LINE = 7,  ///< Treat the image as a single text line.
  PSM_SINGLE_WORD = 8,  ///< Treat the image as a single word.
  PSM_CIRCLE_WORD = 9,  ///< Treat the image as a single word in a circle.
  PSM_SINGLE_CHAR = 10, ///< Treat the image as a single character.
  PSM_SPARSE_TEXT = 11, ///< Find as much text as possible in no particular order.
  PSM_SPARSE_TEXT_OSD = 12, ///< Sparse text with orientation and script det.
  PSM_RAW_LINE = 13, ///< Treat the image as a single text line, bypassing
                     ///< hacks that are Tesseract-specific.*/
//tesseract::PSM_AUTO_ONLY  
    api->SetPageSegMode(mode);


 return true;
 }


 bool ocrService::loadImage(cv::Mat img){
    if (img.channels() != 1) {
         cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);
    }
    ocrService::api->SetImage(img.data, img.cols, img.rows, img.channels(), img.step);

    return true;
 }

std::string ocrService::returnText(){
    char* outText = ocrService::api->GetUTF8Text(); // dynamically allocated
    std::string myString(outText);  // copy its contents into a std::string
    delete [] outText;                  // free the Tesseract allocation
    return myString;
}


 bool ocrService::deInit(){
    
    ocrService::api->End();
    delete ocrService::api;

    return true;
 }