#include <concepts>
#include <flat_map>
#include <memory>
#include <print>
#include <string>
#include <string_view>
#include <utility>

/*
用 clone() 复制已有对象，而不是每次都指定具体类去 new。
*/

// 抽象原型
class Shape {
public:
    virtual ~Shape() = default;

    // 返回独占所有权
    [[nodiscard]] virtual std::unique_ptr<Shape> clone() const = 0;

    virtual void draw() const = 0;
    virtual void setColor(std::string_view color) = 0;
};

// 具体原型：圆形
class Circle final : public Shape {
public:
    Circle(std::string_view color, double radius)
        : color_(color), radius_(radius) {}

    [[nodiscard]] std::unique_ptr<Shape> clone() const override {
        // 利用拷贝构造创建副本
        return std::make_unique<Circle>(*this);
    }

    void draw() const override {
        std::println("Circle: color={}, radius={}", color_, radius_);
    }

    void setColor(std::string_view color) override {
        color_ = color;
    }

private:
    std::string color_;
    double radius_;
};

// 具体原型：矩形
class Rectangle final : public Shape {
public:
    Rectangle(std::string_view color, double width, double height)
        : color_(color), width_(width), height_(height) {}

    [[nodiscard]] std::unique_ptr<Shape> clone() const override {
        return std::make_unique<Rectangle>(*this);
    }

    void draw() const override {
        std::println("Rectangle: color={}, width={}, height={}",
                     color_, width_, height_);
    }

    void setColor(std::string_view color) override {
        color_ = color;
    }

private:
    std::string color_;
    double width_;
    double height_;
};

// 原型注册表：按 key 保存原型，按需克隆
class ShapeRegistry {
public:
    void registerShape(std::string_view key,
                       std::unique_ptr<Shape> prototype) {
        prototypes_.insert_or_assign(std::string{key}, std::move(prototype));
    }

    [[nodiscard]] std::unique_ptr<Shape> create(std::string_view key) const {
        auto it = prototypes_.find(std::string{key});
        return it != prototypes_.end() ? it->second->clone() : nullptr;
    }

private:
    std::flat_map<std::string, std::unique_ptr<Shape>> prototypes_;
};

int main() {
    // 1. 直接使用原型克隆
    auto originalCircle = std::make_unique<Circle>("red", 10.0);
    auto clonedCircle   = originalCircle->clone();
    clonedCircle->setColor("blue");

    std::println("=== 直接克隆 ===");
    originalCircle->draw();
    clonedCircle->draw();

    // 2. 使用原型注册表
    ShapeRegistry registry;
    registry.registerShape("circle", std::make_unique<Circle>("green", 5.0));
    registry.registerShape("rectangle",
                           std::make_unique<Rectangle>("yellow", 8.0, 4.0));

    auto shape1 = registry.create("circle");
    auto shape2 = registry.create("rectangle");

    if (shape1) shape1->setColor("black");
    if (shape2) shape2->setColor("white");

    std::println("\n=== 注册表克隆 ===");
    if (shape1) shape1->draw();
    if (shape2) shape2->draw();

    return 0;
}