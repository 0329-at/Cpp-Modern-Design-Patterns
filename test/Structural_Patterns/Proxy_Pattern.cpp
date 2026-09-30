#include <memory>
#include <print>
#include <string>
#include <string_view>
#include <utility>

/*
代理模式（Proxy Pattern）为其他对象提供一个代理，由代理控制对原对象的访问。
客户端不直接操作真实对象，而是通过代理间接访问，从而可以在访问前后加入额外逻辑，如延迟加载、权限检查、日志、引用计数等。
*/

// =========================
// Subject：抽象接口
// =========================
class Image {
public:
    virtual ~Image() = default;
    virtual void display() const = 0;
};

// =========================
// RealSubject：真实对象
// =========================
class RealImage final : public Image {
public:
    explicit RealImage(std::string_view filename)
        : filename_(filename) {
        loadFromDisk();
    }

    void display() const override {
        std::println("显示图片: {}", filename_);
    }

private:
    void loadFromDisk() const {
        std::println("从磁盘加载图片: {} (耗时操作)", filename_);
    }

    std::string filename_;
};

// =========================
// Proxy：虚拟代理
// =========================
class ImageProxy final : public Image {
public:
    explicit ImageProxy(std::string_view filename)
        : filename_(filename) {}

    void display() const override {
        // 延迟加载：第一次访问时才创建真实对象
        if (!realImage_) {
            realImage_ = std::make_unique<RealImage>(filename_);
        }
        realImage_->display();
    }

private:
    std::string filename_;
    mutable std::unique_ptr<RealImage> realImage_;  // mutable 允许在 const 方法中修改
};

// =========================
// 客户端
// =========================
int main() {
    std::println("创建图片代理...");
    std::unique_ptr<Image> image = std::make_unique<ImageProxy>("photo.jpg");

    std::println("\n第一次显示：");
    image->display();   // 此时才真正加载

    std::println("\n第二次显示：");
    image->display();   // 不再加载，直接显示

    return 0;
}