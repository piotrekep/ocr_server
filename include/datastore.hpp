#ifndef DATASTORE_HPP
#define DATASTORE_HPP

#include <string>
#include <list>
#include <mutex>
#include <utility>
#include <opencv2/opencv.hpp>
#include <algorithm> 

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
        return data.first;
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
            
            return std::make_pair(-1, T());
        }
        
        auto element = data_.front();
        data_.pop_front();   
        return element;
    }

    std::pair<int, T> get(int id){
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = std::find_if(data_.begin(),data_.end(),[id](const std::pair<int, T>& element) {
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
    inline static int id = 0;
    std::list<std::pair<int, T>> data_;
    mutable std::mutex mutex_;
};

#endif 