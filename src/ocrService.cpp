#include "ocrService.hpp"
#include <tesseract/publictypes.h>
#include <leptonica/allheaders.h>

 bool ocrService::initTesseract(){
    ocrService::api = new tesseract::TessBaseAPI();
    //if(ocrService::api->SetVariable("tessedit_write_images", "1") !=1){
    //    std::cout << "write images set fail" << std::endl;
   // }   
   
    char *configs[]={"tesseract.conf"};
    int configs_size = 1;
   // ocrService::api->SetVariable("debug_file", "tesseract_debug.log");
    if (api->Init(NULL, "pol", tesseract::OEM_LSTM_ONLY, configs, configs_size, NULL, NULL, false)) {
     
    //if (api->Init(NULL, "pol")) {
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
    char* outText = ocrService::api->GetUTF8Text(); 
    std::string myString(outText);  
    delete [] outText;                  
    ocrService::api->ClearAdaptiveClassifier();
    return myString;
}

std::string ocrService::returnTSVText(){
    char* outTSV = ocrService::api->GetTSVText(0);
    std::string tsvString(outTSV);
    delete [] outTSV; 
    ocrService::api->ClearAdaptiveClassifier();
    return tsvString;
}

std::string ocrService::returnHOCRText(){
   
    char* outHOCR = ocrService::api->GetHOCRText(0);
    std::string hocrString(outHOCR);
    delete [] outHOCR; 
    ocrService::api->ClearAdaptiveClassifier();
    return hocrString;
}

 cv::Mat ocrService::checkColorDepth(cv::Mat img){
    if (img.channels() != 1) {
         cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);
    }
    return img;
 }

 bool ocrService::deInit(){
    
    ocrService::api->End();
    delete ocrService::api;

    return true;
 }

           
cv::Mat ocrService::filterTest(cv::Mat img){
    //if (img.channels() != 1) {
     //    cv::cvtColor(img, img, cv::COLOR_BGR2GRAY);
   // }

    cv::Mat out;
    cv::Mat gray;
    cv::Mat retval;
   // cv::bilateralFilter(img, out, 10, 50, 75);
   
    if (img.channels() != 1) {
         cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    }
    
    
    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(4.0, cv::Size(10,10));
    //cv::Mat enhanced;
    clahe->apply(gray, out);

    cv::Mat blurred;
    cv::GaussianBlur(out, blurred, cv::Size(5, 5), 1.5);
    //cv::threshold(blurred, out, 120, 255, cv::THRESH_BINARY);
    cv::Mat blackThresh;
    cv::threshold(out, blackThresh, 120, 255, cv::THRESH_BINARY_INV);
 
    cv::Mat blackMask;
    int ksize = 5;  // rozmiar okna 3×3
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(ksize, ksize),cv::Point(-1, -1));
    cv::dilate(
    blackThresh,        
    blackMask,     
    kernel,      
    cv::Point(-1,-1), 
    1,          
    cv::BORDER_DEFAULT
);
   
    //cv::bitwise_not(mask,mask);
   // cv::absdiff(blurred, mask, retval);
    //cv::bitwise_and(out, blackMask, retval);
    //retval = ApplyMaskWhiteBg(gray, blackMask);
    retval=blackMask;
    retval=ExtractBlackOnWthite(gray);
    //cv::medianBlur(out, out, 3);
    //cv::threshold(blurred, out, 150, 255, cv::THRESH_BINARY);
  /*
    cv::Mat background;
    int morphSize = 50; // You can adjust this depending on your document's scale
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(morphSize, morphSize));
    cv::morphologyEx(out, background, cv::MORPH_OPEN, kernel);
    cv::Mat normalized;
    cv::absdiff(out, background, out);
*/
/*
    double maxValue = 255;
    int adaptiveMethod = cv::ADAPTIVE_THRESH_GAUSSIAN_C;
    int thresholdType = cv::THRESH_BINARY;
    int blockSize =  out.cols/100; // size of the neighbourhood area
    double C = 10;       // constant subtracted from the mean or weighted mean
     if(blockSize % 2 == 0) blockSize+=1;

    cv::adaptiveThreshold(out, out,
                      maxValue, 
                      adaptiveMethod, 
                      thresholdType,
                      blockSize,
                      C);
*/
   
    cv::imwrite("output.png", retval);

    return retval;
}

cv::Mat ocrService::Test(cv::Mat img)
{
    // 1. Konwersja do skali szarości (jeśli potrzebna)
    cv::Mat src;
    if (img.channels() == 3 || img.channels() == 4) {
        cv::cvtColor(img, src, cv::COLOR_BGR2GRAY);
    } else {
        src = img.clone();
    }

    // 2. Usunięcie drobnego szumu półtonowego
    cv::Mat denoised;
    cv::medianBlur(src, denoised, 3);

    // 3. Aproksymacja tła przez operację zamknięcia morfologicznego
    int ksize = 7;
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(ksize, ksize));
    cv::Mat background;
    cv::morphologyEx(denoised, background, cv::MORPH_CLOSE, kernel);

    // 4. Wydzielenie różnicy (tekst jest ciemniejszy niż tło)
    cv::Mat diff;
    cv::absdiff(background, denoised, diff);

    // 5. Normalizacja kontrastu różnicy
    cv::normalize(diff, diff, 0, 255, cv::NORM_MINMAX);

    // 6. Progowanie z Otsu do uzyskania maski liter
    cv::Mat mask;
    cv::threshold(diff, mask, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

    // 7. Czyszczenie maski (usunięcie drobnych artefaktów)
    cv::Mat cleanMask;
    cv::morphologyEx(mask, cleanMask, cv::MORPH_OPEN,
                     cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));

    // 8. Inwersja maski (tekst czarny na białym tle)
    cv::Mat finalMask;
    cv::bitwise_not(cleanMask, finalMask);

    // Zwracamy gotową maskę binarną (czarny tekst na białym tle)
    return finalMask;
}

cv::Mat ocrService::ApplyMaskWhiteBg(const cv::Mat& src, const cv::Mat& mask)
{
    // 1. Utwórz wynikowy obraz wypełniony bielą
    cv::Mat result(src.size(), src.type(), cv::Scalar::all(255));

    // 2. Skopiuj piksele z src tam, gdzie mask==255
    src.copyTo(result, mask);

    return result;
}

  cv::Mat ocrService::ExtractBlackOnWthite(const cv::Mat& src){
        // 1. Utwórz wynikowy obraz wypełniony bielą
    cv::Mat result(src.size(), src.type(), cv::Scalar::all(255));

    cv::Mat denoised;
    //cv::medianBlur(src, denoised, 5);
    cv::GaussianBlur(src, denoised, cv::Size(5, 5), 5);


    int ksizeMorph = 32;
    cv::Mat kernelMorph = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(ksizeMorph, ksizeMorph));
    cv::Mat background;
    cv::morphologyEx(denoised, background, cv::MORPH_CLOSE, kernelMorph);
    
    cv::Mat backgroundMask;
    cv::medianBlur(background, background, 11);
    cv::threshold(background, backgroundMask, 240, 255, cv::THRESH_BINARY_INV);

    cv::Mat blackThresh;
    cv::threshold(denoised, blackThresh, 200, 255, cv::THRESH_BINARY_INV);

    cv::Mat finalMask;
    cv::bitwise_not(backgroundMask, finalMask);

    cv::Mat sumWhite;
    cv::bitwise_and(blackThresh, finalMask, sumWhite);
    
    cv::Mat blackMask;
    int ksize = 5;  // rozmiar okna 3×3
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(ksize, ksize),cv::Point(-1, -1));
    cv::dilate(sumWhite,blackMask,kernel,cv::Point(-1,-1),1,cv::BORDER_DEFAULT);
    //result = blackThresh;//removeLargeWhiteAreas(blackMask, 0.01);
    result = ApplyMaskWhiteBg(src, blackMask);
    //result = blackThresh;

    return result;
  }


