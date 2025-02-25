#include "datastore.hpp"

void DataStore::store(const std::string& key, const std::string& data) {
    std::lock_guard<std::mutex> lock(mutex_);
    data_[key] = data;
}

 std::string DataStore::get(const std::string& key) const{
    std::lock_guard<std::mutex> lock(mutex_);
    auto it = data_.find(key);
    return (it != data_.end()) ? it->second : "";
}