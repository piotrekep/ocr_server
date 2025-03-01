/*#include "datastore.hpp"

int DataStore::store(const cv::Mat& img) {
    std::pair<int, cv::Mat> data;
    data.first=id;
    data.second=img;
    std::lock_guard<std::mutex> lock(mutex_);
    data_.push_back(data);
    id++;
    return id;
}

 std::pair<int, cv::Mat> DataStore::get(void){
    std::lock_guard<std::mutex> lock(mutex_);
        if(data_.empty()){

        return std::make_pair(-1, cv::Mat());
    }
    return data_.back();
}

 bool DataStore::checkEmpty(void){
    if(data_.empty()) return true;
    else return false;
 }
 */