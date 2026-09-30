#include <cstddef>
#include <iterator>
#include <print>
#include <vector>

/*
迭代器模式（Iterator Pattern）用于顺序访问聚合对象中的元素，而不暴露其内部表示。它把遍历逻辑从聚合对象中抽离出来，让客户端可以用统一方式遍历不同聚合。

在现代 C++ 中，迭代器模式已经深度融入语言：范围 for、STL 算法、ranges 都基于迭代器概念。因此推荐直接实现符合 C++ 迭代器要求的类，而不是老式的 hasNext() / next()。
*/

// =========================
// 聚合类：整数集合
// =========================
class IntCollection {
public:
    void add(int value) {
        data_.push_back(value);
    }

    // =========================
    // 具体迭代器
    // =========================
    class Iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type        = int;
        using difference_type   = std::ptrdiff_t;
        using pointer           = const int*;
        using reference         = const int&;

        explicit Iterator(const int* ptr) : ptr_(ptr) {}

        reference operator*() const { return *ptr_; }
        pointer operator->() const { return ptr_; }

        Iterator& operator++() {
            ++ptr_;
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp = *this;
            ++ptr_;
            return tmp;
        }

        bool operator==(const Iterator& other) const {
            return ptr_ == other.ptr_;
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }

    private:
        const int* ptr_;
    };

    // =========================
    // 创建迭代器
    // =========================
    [[nodiscard]] Iterator begin() const {
        return Iterator(data_.data());
    }

    [[nodiscard]] Iterator end() const {
        return Iterator(data_.data() + data_.size());
    }

private:
    std::vector<int> data_;
};

// =========================
// 客户端
// =========================
int main() {
    IntCollection collection;
    collection.add(10);
    collection.add(20);
    collection.add(30);
    collection.add(40);

    std::println("使用范围 for 遍历:");
    for (int value : collection) {
        std::println("{}", value);
    }

    std::println("\n使用显式迭代器遍历:");
    for (auto it = collection.begin(); it != collection.end(); ++it) {
        std::println("{}", *it);
    }

    return 0;
}