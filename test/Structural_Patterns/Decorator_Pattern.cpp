#include <memory>
#include <print>
#include <string>
#include <string_view>
#include <utility>

/*
装饰模式（Decorator Pattern）用于动态地给一个对象添加额外的职责，比通过继承生成子类更灵活。它通过组合包装对象，在保持接口不变的前提下，层层叠加功能。
*/

// =========================
// Component：抽象组件
// =========================
class Beverage {
public:
    virtual ~Beverage() = default;

    [[nodiscard]] virtual std::string description() const = 0;
    [[nodiscard]] virtual double cost() const = 0;
};

// =========================
// ConcreteComponent：具体组件
// =========================
class Espresso final : public Beverage {
public:
    [[nodiscard]] std::string description() const override {
        return "浓缩咖啡";
    }
    [[nodiscard]] double cost() const override {
        return 2.0;
    }
};

class HouseBlend final : public Beverage {
public:
    [[nodiscard]] std::string description() const override {
        return "综合咖啡";
    }
    [[nodiscard]] double cost() const override {
        return 1.5;
    }
};

// =========================
// Decorator：抽象装饰者
// =========================
class CondimentDecorator : public Beverage {
public:
    explicit CondimentDecorator(std::unique_ptr<Beverage> beverage)
        : beverage_(std::move(beverage)) {}

protected:
    std::unique_ptr<Beverage> beverage_;   // 持有被装饰对象
};

// =========================
// ConcreteDecorator：具体装饰者
// =========================
class Mocha final : public CondimentDecorator {
public:
    using CondimentDecorator::CondimentDecorator;   // 继承构造函数

    [[nodiscard]] std::string description() const override {
        return beverage_->description() + " + 摩卡";
    }
    [[nodiscard]] double cost() const override {
        return beverage_->cost() + 0.5;
    }
};

class Whip final : public CondimentDecorator {
public:
    using CondimentDecorator::CondimentDecorator;

    [[nodiscard]] std::string description() const override {
        return beverage_->description() + " + 奶泡";
    }
    [[nodiscard]] double cost() const override {
        return beverage_->cost() + 0.3;
    }
};

// =========================
// 客户端
// =========================
int main() {
    // 一杯浓缩咖啡
    std::unique_ptr<Beverage> beverage = std::make_unique<Espresso>();
    std::println("{}: ￥{:.2f}", beverage->description(), beverage->cost());

    // 动态加摩卡
    beverage = std::make_unique<Mocha>(std::move(beverage));
    std::println("{}: ￥{:.2f}", beverage->description(), beverage->cost());

    // 再加奶泡
    beverage = std::make_unique<Whip>(std::move(beverage));
    std::println("{}: ￥{:.2f}", beverage->description(), beverage->cost());

    // 直接构建双重摩卡 + 综合咖啡
    auto doubleMocha = std::make_unique<Mocha>(
        std::make_unique<Mocha>(
            std::make_unique<HouseBlend>()));
    std::println("{}: ￥{:.2f}", doubleMocha->description(), doubleMocha->cost());

    return 0;
}