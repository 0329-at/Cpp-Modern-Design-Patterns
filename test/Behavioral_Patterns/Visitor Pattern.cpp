#include <numbers>   // C++20: std::numbers::pi
#include <print>     // C++23: std::println
#include <variant>
#include <vector>

/*
std::variant + std::visit 实现“访问者模式”

例如：AST 节点处理、JSON 值遍历、图形系统、配置项处理、消息分发。
*/

// =========================
// 具体元素：普通结构体
// =========================
struct Circle {
    double radius;
};

struct Rectangle {
    double width;
    double height;
};

// 用 variant 表示封闭的形状集合
using Shape = std::variant<Circle, Rectangle>;


template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};

// C++17 CTAD 推导指引
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

// =========================
// 访问者 1：计算面积
// =========================
struct AreaVisitor {
    void operator()(const Circle& c) const {
        double area = std::numbers::pi * c.radius * c.radius;
        std::println("圆形面积: {:.2f}", area);
    }

    void operator()(const Rectangle& r) const {
        double area = r.width * r.height;
        std::println("矩形面积: {:.2f}", area);
    }
};

// =========================
// 访问者 2：绘制形状
// =========================
struct DrawVisitor {
    void operator()(const Circle& c) const {
        std::println("绘制圆形，半径: {:.2f}", c.radius);
    }

    void operator()(const Rectangle& r) const {
        std::println("绘制矩形，宽: {:.2f}，高: {:.2f}",
                     r.width, r.height);
    }
};

// =========================
// 也可以直接用 lambda 组合，无需定义类
// =========================
double computeArea(const Shape& shape) {
    return std::visit(overloaded{
        [](const Circle& c) {
            return std::numbers::pi * c.radius * c.radius;
        },
        [](const Rectangle& r) {
            return r.width * r.height;
        }
    }, shape);
}

// =========================
// 客户端
// =========================
int main() {
    std::vector<Shape> shapes = {
        Circle{5.0},
        Rectangle{4.0, 6.0}
    };

    std::println("=== 计算面积（类访问者） ===");
    for (const auto& shape : shapes) {
        std::visit(AreaVisitor{}, shape);
    }

    std::println("\n=== 绘制形状（类访问者） ===");
    for (const auto& shape : shapes) {
        std::visit(DrawVisitor{}, shape);
    }

    std::println("\n=== 使用 lambda 组合计算面积 ===");
    for (const auto& shape : shapes) {
        std::println("面积: {:.2f}", computeArea(shape));
    }

    return 0;
}