#include <iostream>
#include <memory>
#include <string>

/* 
简单工厂
定义: 
一个工厂类根据传入的参数，决定创建哪一种具体产品。
*/

template<class... Ts> struct Fruit : Ts... {
    using Ts::operator()...;
};

struct Apple { 
    void print() { 
        std::println("apple print"); 
    } 
};

struct Pineapplce {
    void print() { 
        std::println("pineapple print"); 
    } 
};

static constexpr auto FruitFactory = Fruit {
    []<typename T>(const T& ) { return std::make_unique<T>(); }
};

auto test() {
    auto apple = FruitFactory(Apple{});
    auto pineapplce = FruitFactory(Pineapplce{});
    
    apple->print();
    pineapplce->print();
}