#ifndef VALUEWHEEL_H
#define VALUEWHEEL_H

#include <Arduino.h>

#include <vector>

template <typename T>
class ValueWheel {
public:
    // Constructor that takes a vector of values of type T
    ValueWheel(const std::vector<T>& values) : values_(values), index_(0) {}

    // Method to increase the index
    void increaseIndex() {
        index_ = (index_ + 1) % values_.size();
    }

    // Method to decrease the index
    void decreaseIndex() {
        if (index_ == 0) {
            index_ = values_.size() - 1;
        } else {
            --index_;
        }
    }

    void setIndex(size_t idx) {
        index_ = idx % values_.size();
    }

    // Method to get the value at the current index
    T getValue() const {
        return values_[index_];
    }

    // Method to get the value at a specified index, with rollover
    T getViaIndex(size_t idx) const {
        return values_[idx % values_.size()];
    }

    // Method to get the current index
    size_t getIndex() const {
        return index_;
    }

private:
    std::vector<T> values_; // The array of values of type T
    size_t index_; // The current index
};

#endif