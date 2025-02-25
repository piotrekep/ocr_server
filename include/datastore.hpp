#ifndef DATASTORE_HPP
#define DATASTORE_HPP

#include <string>
#include <unordered_map>
#include <mutex>

class DataStore {
public:
    
    void store(const std::string& key, const std::string& data);

    std::string get(const std::string& key) const;

private:
    std::unordered_map<std::string, std::string> data_;
    mutable std::mutex mutex_; 
};

#endif 