#ifndef DATASTORE_HPP
#define DATASTORE_HPP

#include <string>
#include <unordered_map>
#include <mutex>
#include <opencv2/opencv.hpp>


template <typename T>
class DataStore {
public:
    
    int store(const T& input) {
    std::pair<int,  T> data;
    data.first=id;
    data.second=input;
    std::lock_guard<std::mutex> lock(mutex_);
    data_.push_back(data);
    id++;
    return id;
    }

    void store(int id,const T& input) {
    std::pair<int, T> data;
    data.first=id;
    data.second=input;
    std::lock_guard<std::mutex> lock(mutex_);
    data_.push_back(data);
    }

    std::pair<int, T> get(){
    std::lock_guard<std::mutex> lock(mutex_);
    if(data_.empty()){

        return std::make_pair(-1, cv::Mat());
    }
    auto element = data_.back();  // Retrieve the last element
    data_.pop_back();   
    return element;
}

   
    bool checkEmpty(void){
    if(data_.empty()) return true;
    else return false;
 }

private:
    inline static int id = 0;
    std::vector<std::pair<int, T>> data_;
    mutable std::mutex mutex_;
};

/*
class DataStore {
public:
    
    int store(const cv::Mat& img);

    std::pair<int, cv::Mat>get(void);
    
    bool checkEmpty(void);

private:
    inline static int id=0;
    std::vector<std::pair<int, cv::Mat>> data_;
    mutable std::mutex mutex_; 
};

*/
#endif 