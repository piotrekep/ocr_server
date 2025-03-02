#ifndef DATASTORE_HPP
#define DATASTORE_HPP

#include <string>
#include <list>
#include <mutex>
#include <utility>
#include <opencv2/opencv.hpp>

template <typename T>
class DataStore {
public:
    
    int store(const T& input) {
        std::pair<int, T> data;
        data.first = id;
        data.second = input;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            data_.push_back(data);
        }
        id++;
        return id;
    }

    
    void store(int id, const T& input) {
        std::pair<int, T> data;
        data.first = id;
        data.second = input;
        std::lock_guard<std::mutex> lock(mutex_);
        data_.push_back(data);
    }

    
    std::pair<int, T> get(){
        std::lock_guard<std::mutex> lock(mutex_);
        if(data_.empty()){
            
            return std::make_pair(-1, cv::Mat());
        }
        
        auto element = data_.front();
        data_.pop_front();   
        return element;
    }

    
    bool checkEmpty(void){
        std::lock_guard<std::mutex> lock(mutex_);
        return data_.empty();
    }

private:
    inline static int id = 0;
    std::list<std::pair<int, T>> data_;
    mutable std::mutex mutex_;
};

#endif 