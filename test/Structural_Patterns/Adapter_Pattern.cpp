#include <iostream>
#include <memory>
#include <string>
#include <string_view>

/*
适配器模式（Adapter Pattern）用于把一个类的接口转换成客户端期望的另一个接口，让原本不兼容的类可以一起工作。常见有两种实现：

对象适配器：通过组合持有被适配对象（推荐）。

类适配器：通过多重继承同时继承目标接口和被适配类。
*/

// 新接口
class Printer {
public:
    virtual ~Printer() = default;
    virtual void print(std::string_view text) const = 0;
};

// 已有的、接口不兼容的类
class LegacyPrinter {
public:
    void printOld(const std::string& text) const {
        std::cout << "[Legacy] " << text << "\n";
    }
};

// 对象适配器：通过组合持有
class PrinterAdapter : public Printer {
public:
    explicit PrinterAdapter(std::unique_ptr<LegacyPrinter> legacy)
        : legacy_(std::move(legacy)) {}

    void print(std::string_view text) const override {
        // 将 string_view 转换为 string，以适配旧接口
        legacy_->printOld(std::string(text));
    }

private:
    std::unique_ptr<LegacyPrinter> legacy_;
};

// 类适配器：通过多重继承
class PrinterClassAdapter : public Printer, private LegacyPrinter {
public:
    void print(std::string_view text) const override {
        printOld(std::string(text));
    }
};

int main() {
    // 1. 对象适配器
    std::unique_ptr<Printer> p1 = std::make_unique<PrinterAdapter>(
        std::make_unique<LegacyPrinter>());
    p1->print("Hello via object adapter");

    // 2. 类适配器
    std::unique_ptr<Printer> p2 = std::make_unique<PrinterClassAdapter>();
    p2->print("Hello via class adapter");

    return 0;
}