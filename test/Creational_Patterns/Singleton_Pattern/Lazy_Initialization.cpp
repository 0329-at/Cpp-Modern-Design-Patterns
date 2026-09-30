#include <iostream>

class Singleton {
public:
    // 第一次调用时才创建实例，C++11 起保证局部静态变量初始化线程安全
    static Singleton& getInstance() {
        static Singleton instance;
        return instance;
    }

    void doSomething() const {
        std::cout << "懒汉单例: doSomething()" << std::endl;
    }

    // 禁止拷贝和赋值
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

private:
    Singleton() {
        std::cout << "懒汉单例: 构造函数被调用" << std::endl;
    }
    ~Singleton() {
        std::cout << "懒汉单例: 析构函数被调用" << std::endl;
    }
};

int main() {
    std::cout << "main 开始" << std::endl;

    Singleton& s1 = Singleton::getInstance();
    Singleton& s2 = Singleton::getInstance();

    s1.doSomething();

    std::cout << "s1 和 s2 是同一实例: " << (&s1 == &s2) << std::endl;

    std::cout << "main 结束" << std::endl;
    return 0;
}