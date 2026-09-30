#include <memory>
#include <print>
#include <string>
#include <string_view>
#include <utility>

/*
策略模式（Strategy Pattern）定义一系列算法，把每个算法封装起来，并使它们可以互相替换。策略让算法的变化独立于使用它的客户端。
*/

// =========================
// Strategy：抽象策略
// =========================
class PaymentStrategy {
public:
    virtual ~PaymentStrategy() = default;

    virtual void pay(double amount) const = 0;
    [[nodiscard]] virtual std::string_view name() const = 0;
};

// =========================
// ConcreteStrategy：信用卡支付
// =========================
class CreditCardPayment final : public PaymentStrategy {
public:
    CreditCardPayment(std::string_view cardNumber, std::string_view holder)
        : cardNumber_(cardNumber), holder_(holder) {}

    void pay(double amount) const override {
        std::println("使用信用卡支付 ￥{:.2f}，持卡人: {}，卡号: ****{}",
                     amount, holder_,
                     cardNumber_.substr(cardNumber_.size() - 4));
    }

    [[nodiscard]] std::string_view name() const override {
        return "信用卡";
    }

private:
    std::string cardNumber_;
    std::string holder_;
};

// =========================
// ConcreteStrategy：PayPal 支付
// =========================
class PayPalPayment final : public PaymentStrategy {
public:
    explicit PayPalPayment(std::string_view email) : email_(email) {}

    void pay(double amount) const override {
        std::println("使用 PayPal 支付 ￥{:.2f}，账户: {}", amount, email_);
    }

    [[nodiscard]] std::string_view name() const override {
        return "PayPal";
    }

private:
    std::string email_;
};

// =========================
// ConcreteStrategy：加密货币支付
// =========================
class CryptoPayment final : public PaymentStrategy {
public:
    explicit CryptoPayment(std::string_view wallet) : wallet_(wallet) {}

    void pay(double amount) const override {
        std::println("使用加密货币支付 ￥{:.2f}，钱包: {}", amount, wallet_);
    }

    [[nodiscard]] std::string_view name() const override {
        return "加密货币";
    }

private:
    std::string wallet_;
};

// =========================
// Context：购物车
// =========================
class ShoppingCart {
public:
    void setPaymentStrategy(std::unique_ptr<PaymentStrategy> strategy) {
        strategy_ = std::move(strategy);
    }

    void checkout(double amount) const {
        if (!strategy_) {
            std::println("请先选择支付方式");
            return;
        }
        std::println("当前支付方式: {}", strategy_->name());
        strategy_->pay(amount);
    }

private:
    std::unique_ptr<PaymentStrategy> strategy_;
};

// =========================
// 客户端
// =========================
int main() {
    ShoppingCart cart;

    // 1. 信用卡支付
    cart.setPaymentStrategy(
        std::make_unique<CreditCardPayment>("1234-5678-9012-3456", "张三"));
    cart.checkout(299.0);

    // 2. 切换为 PayPal
    std::println("");
    cart.setPaymentStrategy(
        std::make_unique<PayPalPayment>("zhangsan@example.com"));
    cart.checkout(199.5);

    // 3. 切换为加密货币
    std::println("");
    cart.setPaymentStrategy(
        std::make_unique<CryptoPayment>("0xABCDEF1234567890"));
    cart.checkout(500.0);

    return 0;
}