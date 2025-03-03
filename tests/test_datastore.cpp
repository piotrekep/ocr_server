// test_datastore.cpp
#include <criterion/criterion.h>
#include "datastore.hpp"
#include <opencv2/opencv.hpp>

Test(DataStoreTests, StoreAndGetFIFO) {
    DataStore<cv::Mat> ds;
    
    // Create a dummy image: a 5x5 matrix filled with ones.
    cv::Mat image = cv::Mat::ones(5, 5, CV_8UC1);

    int id = ds.store(image);
    int id2 = ds.store(image);
    auto result = ds.get();
    
    cr_assert(result.first == id, "First out id==%d, got %d", id, result.first);
    cr_assert(!result.second.empty(), "Expected a non-empty image");
    
    auto result2 = ds.get();
    cr_assert(result2.first == id2, "second out id==%d, got %d", id2, result2.first);
    cr_assert(!result2.second.empty(), "Expected a non-empty image");
    
    auto result3 = ds.get();
    cr_assert(result3.first == -1, "empty out id==-1, got %d", result3.first);
    cr_assert(result3.second.empty(), "Expected an empty image");
    
}

Test(DataStoreTests, StoreAndGetById) {
    DataStore<cv::Mat> ds;
    cv::Mat image = cv::Mat::ones(10, 10, CV_8UC1);
    int customId = 42;
    ds.store(customId, image);
    
    auto result = ds.get(customId);
    cr_assert_eq(result.first, customId, "Expected id %d, got %d", customId, result.first);
    cr_assert(!result.second.empty(), "Expected a non-empty image");
}

Test(DataStoreTests, CheckEmpty) {
    DataStore<cv::Mat> ds;
    cr_assert(ds.checkEmpty(), "DataStore should be empty initially");
    ds.store(cv::Mat::zeros(3, 3, CV_8UC1));
    cr_assert(!ds.checkEmpty(), "DataStore should not be empty after storing an element");
}
