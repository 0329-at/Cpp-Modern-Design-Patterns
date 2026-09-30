#include <iostream>

class Singleton {
public:
    static Singleton& getInstance() {
        return instance;   // 直接返回早已创建好的唯一实例
    }

    void doSomething() const {
        std::cout << "饿汉单例: doSomething()" << std::endl;
    }

    // 禁止拷贝和赋值
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

private:
    Singleton() {
        std::cout << "饿汉单例: 构造函数被调用" << std::endl;
    }
    ~Singleton() {
        std::cout << "饿汉单例: 析构函数被调用" << std::endl;
    }

    static Singleton instance;   // 静态成员声明
};

// 类外定义并初始化，程序启动时（进入 main 之前）即构造
Singleton Singleton::instance = nullptr;

int main() {
    std::cout << "main 开始" << std::endl;

    Singleton& s1 = Singleton::getInstance();
    Singleton& s2 = Singleton::getInstance();

    s1.doSomething();

    std::cout << "s1 和 s2 是同一实例: " << (&s1 == &s2) << std::endl;

    std::cout << "main 结束" << std::endl;
    return 0;
}