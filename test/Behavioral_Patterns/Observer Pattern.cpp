#include <algorithm>
#include <functional>
#include <memory>
#include <print>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

/*
观察者模式（Observer Pattern）定义对象间的一对多依赖关系：当一个对象（被观察者/主题）状态改变时，所有依赖它的对象（观察者）都会自动收到通知并更新。
*/

// =========================
// Observer：抽象观察者
// =========================
class Observer {
public:
    virtual ~Observer() = default;

    virtual void update(std::string_view stockName,
                        double price) = 0;
};

// =========================
// Subject：抽象主题
// =========================
class Subject {
public:
    virtual ~Subject() = default;

    virtual void attach(std::shared_ptr<Observer> observer) = 0;
    virtual void detach(const std::shared_ptr<Observer>& observer) = 0;
    virtual void notify() = 0;
};

// =========================
// ConcreteSubject：具体主题（股票）
// =========================
class Stock final : public Subject {
public:
    explicit Stock(std::string_view name) : name_(name) {}

    void attach(std::shared_ptr<Observer> observer) override {
        observers_.push_back(std::move(observer));
    }

    void detach(const std::shared_ptr<Observer>& observer) override {
        std::erase_if(observers_,
                      [&](const auto& o) { return o == observer; });
    }

    void setPrice(double price) {
        price_ = price;
        std::println("--- {} 价格更新为 {:.2f} ---", name_, price_);
        notify();
    }

    void notify() override {
        for (const auto& observer : observers_) {
            observer->update(name_, price_);
        }
    }

    [[nodiscard]] std::string_view name() const { return name_; }
    [[nodiscard]] double price() const { return price_; }

private:
    std::string name_;
    double price_ = 0.0;
    std::vector<std::shared_ptr<Observer>> observers_;
};

// =========================
// ConcreteObserver：具体观察者（投资者）
// =========================
class Investor final : public Observer {
public:
    explicit Investor(std::string_view name) : name_(name) {}

    void update(std::string_view stockName, double price) override {
        std::println("[{}] 收到通知: {} 当前价格 {:.2f}",
                     name_, stockName, price);
    }

private:
    std::string name_;
};

// =========================
// 另一种观察者：使用 std::function 的轻量级观察者
// =========================
class LambdaObserver final : public Observer {
public:
    using Callback = std::function<void(std::string_view, double)>;

    explicit LambdaObserver(Callback cb) : callback_(std::move(cb)) {}

    void update(std::string_view stockName, double price) override {
        callback_(stockName, price);
    }

private:
    Callback callback_;
};

// =========================
// 客户端
// =========================
int main() {
    auto stock = std::make_shared<Stock>("AAPL");

    auto alice   = std::make_shared<Investor>("Alice");
    auto bob     = std::make_shared<Investor>("Bob");
    auto charlie = std::make_shared<LambdaObserver>(
        [](std::string_view name, double price) {
            std::println("[Charlie-回调] {} 价格变为 {:.2f}", name, price);
        });

    stock->attach(alice);
    stock->attach(bob);
    stock->attach(charlie);

    stock->setPrice(150.0);

    // Bob 取消订阅
    std::println("\n--- Bob 取消订阅 ---");
    stock->detach(bob);

    stock->setPrice(155.5);

    return 0;
}