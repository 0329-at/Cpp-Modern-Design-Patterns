#include <iostream>
#include <memory>
#include <utility>

/*
桥接模式（Bridge Pattern）用于将抽象部分与实现部分分离，使它们可以独立变化。

核心是：抽象类持有实现类的指针（组合），而不是通过继承把所有组合都写死。
*/

// =========================
// 实现部分接口：渲染器
// =========================
class Renderer {
public:
    virtual ~Renderer() = default;

    virtual void renderCircle(double x, double y, double radius) const = 0;
    virtual void renderRectangle(double x, double y,
                                 double width, double height) const = 0;
};

// 具体实现 A：OpenGL 渲染器
class OpenGLRenderer final : public Renderer {
public:
    void renderCircle(double x, double y, double radius) const override {
        std::cout << "[OpenGL] Circle at (" << x << ", " << y
                  << "), radius = " << radius << "\n";
    }

    void renderRectangle(double x, double y,
                         double width, double height) const override {
        std::cout << "[OpenGL] Rectangle at (" << x << ", " << y
                  << "), width = " << width
                  << ", height = " << height << "\n";
    }
};

// 具体实现 B：DirectX 渲染器
class DirectXRenderer final : public Renderer {
public:
    void renderCircle(double x, double y, double radius) const override {
        std::cout << "[DirectX] Circle at (" << x << ", " << y
                  << "), radius = " << radius << "\n";
    }

    void renderRectangle(double x, double y,
                         double width, double height) const override {
        std::cout << "[DirectX] Rectangle at (" << x << ", " << y
                  << "), width = " << width
                  << ", height = " << height << "\n";
    }
};

// =========================
// 抽象部分：形状
// =========================
class Shape {
public:
    explicit Shape(std::unique_ptr<Renderer> renderer)
        : renderer_(std::move(renderer)) {}

    virtual ~Shape() = default;

    virtual void draw() const = 0;

protected:
    std::unique_ptr<Renderer> renderer_;   // 桥接：持有实现部分
};

// 修正抽象：圆形
class Circle final : public Shape {
public:
    Circle(std::unique_ptr<Renderer> renderer,
           double x, double y, double radius)
        : Shape(std::move(renderer)),
          x_(x), y_(y), radius_(radius) {}

    void draw() const override {
        renderer_->renderCircle(x_, y_, radius_);
    }

private:
    double x_, y_, radius_;
};

// 修正抽象：矩形
class Rectangle final : public Shape {
public:
    Rectangle(std::unique_ptr<Renderer> renderer,
              double x, double y, double width, double height)
        : Shape(std::move(renderer)),
          x_(x), y_(y), width_(width), height_(height) {}

    void draw() const override {
        renderer_->renderRectangle(x_, y_, width_, height_);
    }

private:
    double x_, y_, width_, height_;
};

// =========================
// 客户端代码
// =========================
int main() {
    // 用 OpenGL 渲染圆形
    std::unique_ptr<Shape> circleOpenGL =
        std::make_unique<Circle>(std::make_unique<OpenGLRenderer>(),
                                 10, 20, 5);

    // 用 DirectX 渲染矩形
    std::unique_ptr<Shape> rectDirectX =
        std::make_unique<Rectangle>(std::make_unique<DirectXRenderer>(),
                                    30, 40, 8, 6);

    // 同一个形状也可以搭配另一种渲染器
    std::unique_ptr<Shape> circleDirectX =
        std::make_unique<Circle>(std::make_unique<DirectXRenderer>(),
                                 10, 20, 5);

    circleOpenGL->draw();
    rectDirectX->draw();
    circleDirectX->draw();

    return 0;
}