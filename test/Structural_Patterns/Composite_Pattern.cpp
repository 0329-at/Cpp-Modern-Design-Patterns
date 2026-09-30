#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

// =========================
// Component：统一接口
// =========================
class FileSystemNode {
public:
    virtual ~FileSystemNode() = default;

    // 统一操作：显示节点信息
    virtual void show(int indent = 0) const = 0;

    // 统一操作：获取大小
    [[nodiscard]] virtual std::size_t size() const = 0;
};

// =========================
// Leaf：叶子节点（文件）
// =========================
class File final : public FileSystemNode {
public:
    File(std::string_view name, std::size_t size)
        : name_(name), size_(size) {}

    void show(int indent = 0) const override {
        std::cout << std::string(indent, ' ')
                  << "- 文件: " << name_
                  << " (" << size_ << " 字节)\n";
    }

    [[nodiscard]] std::size_t size() const override {
        return size_;
    }

private:
    std::string name_;
    std::size_t size_;
};

// =========================
// Composite：组合节点（目录）
// =========================
class Directory final : public FileSystemNode {
public:
    explicit Directory(std::string_view name) : name_(name) {}

    // 添加子节点（文件或子目录）
    void add(std::unique_ptr<FileSystemNode> node) {
        children_.push_back(std::move(node));
    }

    void show(int indent = 0) const override {
        std::cout << std::string(indent, ' ')
                  << "+ 目录: " << name_ << "\n";
        for (const auto& child : children_) {
            child->show(indent + 2);
        }
    }

    [[nodiscard]] std::size_t size() const override {
        std::size_t total = 0;
        for (const auto& child : children_) {
            total += child->size();   // 递归汇总
        }
        return total;
    }

private:
    std::string name_;
    std::vector<std::unique_ptr<FileSystemNode>> children_;
};

// =========================
// 客户端代码
// =========================
int main() {
    // 构建目录树
    auto root = std::make_unique<Directory>("root");

    auto docs = std::make_unique<Directory>("docs");
    docs->add(std::make_unique<File>("readme.md", 1024));
    docs->add(std::make_unique<File>("manual.pdf", 20480));

    auto images = std::make_unique<Directory>("images");
    images->add(std::make_unique<File>("logo.png", 4096));
    images->add(std::make_unique<File>("banner.jpg", 8192));

    root->add(std::make_unique<File>("main.cpp", 2048));
    root->add(std::move(docs));
    root->add(std::move(images));

    // 统一对待文件和目录
    root->show();
    std::cout << "\n总大小: " << root->size() << " 字节\n";

    return 0;
}