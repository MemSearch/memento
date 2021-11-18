#pragma once
#include <iostream>
#include <vector>
#include <ctime>

/*
template<class T1 = short, class T2 = std::string>
class MemInfo {
private:
    T1 id_;
    std::vector<T2> tags_;
    char* data_
public:
    MemInfo(T1 id_, std::vector<T2> tags) : id_(id), tags_(tags) {};
    template<class T3, class T4> friend std::ostream& operator <<(std::ostream& , class MemInfo<T3, T4>&);
    T1 getId() { return id_; }
    std::vector<T2> getTags() { return tags_; }
    void setData() { data_ = ctime(&now); }
    char* detData() { return data_ }
};
*/

template<class T1 = short, class T2 = std::string>
struct MemInfo {
    T1 id;
    std::vector<T2> tags;
    template<class T3, class T4> friend std::ostream& operator <<(std::ostream& , class MemInfo<T3, T4>&);
};

template<class T3, class T4>
std::ostream& operator << (std::ostream& output, MemInfo<T3, T4>& mem) {
    output << "[id: " << mem.id << ", tags: ";
    int memCount = mem.tags.size();
    for (int i = 0; i < memCount; ++i) {
        output << "<" << mem.tags[i];
        if (i != memCount - 1) {
            output << ">, ";
        }
        else {
            output << ">]";
        }
    }
    return output;
} 