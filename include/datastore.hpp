#ifndef DATASTORE_HPP
#define DATASTORE_HPP

#include <string>
#include <list>
#include <mutex>
#include <utility>
#include <opencv2/opencv.hpp>
#include <algorithm> 
#include <chrono>

template <typename T>
class DataStore {
public:
    
    int64_t store(const T& input) {
        std::pair<int64_t, T> data;
        auto now = std::chrono::system_clock::now();
        auto epoch_seconds = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
        data.first = epoch_seconds;
        data.first = data.first*1000 + id;
        data.second = input;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            data_.push_back(data);
        }
        id++;
        return data.first;
    }

    
    void store(int64_t id, const T& input) {
        std::pair<int64_t, T> data;
        data.first = id;
        data.second = input;
        std::lock_guard<std::mutex> lock(mutex_);
        data_.push_back(data);
    }

    
    std::pair<int64_t, T> get(){
        std::lock_guard<std::mutex> lock(mutex_);
        if(data_.empty()){
            
            return std::make_pair(-1, T());
        }
        
        auto element = data_.front();
        data_.pop_front();   
        return element;
    }

    std::pair<int64_t, T> get(int64_t id){
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = std::find_if(data_.begin(),data_.end(),[id](const std::pair<int64_t, T>& element) {
                return element.first == id;
            }
            );
            if (it != data_.end()) {
                auto ret = *it;
                data_.erase(it);
               return ret;  
            }
    return std::make_pair(-1, T());  
}

    
    bool checkEmpty(void){
        std::lock_guard<std::mutex> lock(mutex_);
        return data_.empty();
    }

private:
    inline static int64_t id = 0;
    std::list<std::pair<int64_t, T>> data_;
    mutable std::mutex mutex_;
};

#endif 