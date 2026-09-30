#include <iostream>
#include <memory>

/*
工厂方法
定义: 
定义一个创建对象的接口，但让子类决定实例化哪个类。工厂方法把对象的创建延迟到子类。
*/

class Product {
public:
    virtual ~Product() = default;
    virtual void use() const = 0;
};

class ConcreteProductA : public Product {
public:
    void use() const override {
        std::cout << "使用产品 A\n";
    }
};

class ConcreteProductB : public Product {
public:
    void use() const override {
        std::cout << "使用产品 B\n";
    }
};

class Factory {
public:
    virtual ~Factory() = default;
    virtual std::unique_ptr<Product> createProduct() const = 0;
};

class FactoryA : public Factory {
public:
    std::unique_ptr<Product> createProduct() const override {
        return std::make_unique<ConcreteProductA>();
    }
};

class FactoryB : public Factory {
public:
    std::unique_ptr<Product> createProduct() const override {
        return std::make_unique<ConcreteProductB>();
    }
};

int main() {
    std::unique_ptr<Factory> factory = std::make_unique<FactoryA>();
    auto product = factory->createProduct();
    product->use();
    return 0;
}