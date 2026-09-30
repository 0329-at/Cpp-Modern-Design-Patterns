#include <memory>
#include <print>
#include <string>
#include <string_view>
#include <utility>

/*
状态模式（State Pattern）允许一个对象在内部状态改变时改变它的行为，

对象看起来好像修改了它的类。它把状态封装成独立的类，并将动作委托给当前状态对象。
*/

class Order;

// =========================
// State：抽象状态
// =========================
class OrderState {
public:
    virtual ~OrderState() = default;

    // 每个操作返回下一个状态；返回 nullptr 表示保持当前状态
    [[nodiscard]] virtual std::unique_ptr<OrderState> pay(Order& order) = 0;
    [[nodiscard]] virtual std::unique_ptr<OrderState> ship(Order& order) = 0;
    [[nodiscard]] virtual std::unique_ptr<OrderState> complete(Order& order) = 0;
    [[nodiscard]] virtual std::unique_ptr<OrderState> cancel(Order& order) = 0;

    [[nodiscard]] virtual std::string_view name() const = 0;
};

// =========================
// Context：订单
// =========================
class Order {
public:
    explicit Order(std::string id);

    void pay();
    void ship();
    void complete();
    void cancel();

    void setState(std::unique_ptr<OrderState> state) {
        state_ = std::move(state);
        std::println("订单 {} 当前状态: {}", id_, state_->name());
    }

    [[nodiscard]] const std::string& id() const { return id_; }
    [[nodiscard]] std::string_view stateName() const { return state_->name(); }

private:
    std::string id_;
    std::unique_ptr<OrderState> state_;
};

// =========================
// 具体状态：新建
// =========================
class PendingState final : public OrderState {
public:
    [[nodiscard]] std::unique_ptr<OrderState> pay(Order& order) override;
    [[nodiscard]] std::unique_ptr<OrderState> ship(Order& order) override;
    [[nodiscard]] std::unique_ptr<OrderState> complete(Order& order) override;
    [[nodiscard]] std::unique_ptr<OrderState> cancel(Order& order) override;

    [[nodiscard]] std::string_view name() const override { return "新建"; }
};

// =========================
// 具体状态：已支付
// =========================
class PaidState final : public OrderState {
public:
    [[nodiscard]] std::unique_ptr<OrderState> pay(Order& order) override;
    [[nodiscard]] std::unique_ptr<OrderState> ship(Order& order) override;
    [[nodiscard]] std::unique_ptr<OrderState> complete(Order& order) override;
    [[nodiscard]] std::unique_ptr<OrderState> cancel(Order& order) override;

    [[nodiscard]] std::string_view name() const override { return "已支付"; }
};

// =========================
// 具体状态：已发货
// =========================
class ShippedState final : public OrderState {
public:
    [[nodiscard]] std::unique_ptr<OrderState> pay(Order& order) override;
    [[nodiscard]] std::unique_ptr<OrderState> ship(Order& order) override;
    [[nodiscard]] std::unique_ptr<OrderState> complete(Order& order) override;
    [[nodiscard]] std::unique_ptr<OrderState> cancel(Order& order) override;

    [[nodiscard]] std::string_view name() const override { return "已发货"; }
};

// =========================
// 具体状态：已完成
// =========================
class CompletedState final : public OrderState {
public:
    [[nodiscard]] std::unique_ptr<OrderState> pay(Order&) override {
        std::println("订单已完成，无法支付");
        return nullptr;
    }
    [[nodiscard]] std::unique_ptr<OrderState> ship(Order&) override {
        std::println("订单已完成，无法发货");
        return nullptr;
    }
    [[nodiscard]] std::unique_ptr<OrderState> complete(Order&) override {
        std::println("订单已经完成");
        return nullptr;
    }
    [[nodiscard]] std::unique_ptr<OrderState> cancel(Order&) override {
        std::println("订单已完成，无法取消");
        return nullptr;
    }

    [[nodiscard]] std::string_view name() const override { return "已完成"; }
};

// =========================
// 具体状态：已取消
// =========================
class CancelledState final : public OrderState {
public:
    [[nodiscard]] std::unique_ptr<OrderState> pay(Order&) override {
        std::println("订单已取消，无法支付");
        return nullptr;
    }
    [[nodiscard]] std::unique_ptr<OrderState> ship(Order&) override {
        std::println("订单已取消，无法发货");
        return nullptr;
    }
    [[nodiscard]] std::unique_ptr<OrderState> complete(Order&) override {
        std::println("订单已取消，无法完成");
        return nullptr;
    }
    [[nodiscard]] std::unique_ptr<OrderState> cancel(Order&) override {
        std::println("订单已经取消");
        return nullptr;
    }

    [[nodiscard]] std::string_view name() const override { return "已取消"; }
};

// =========================
// Order 成员函数实现
// =========================
Order::Order(std::string id) : id_(std::move(id)) {
    setState(std::make_unique<PendingState>());
}

void Order::pay() {
    if (auto next = state_->pay(*this)) {
        setState(std::move(next));
    }
}

void Order::ship() {
    if (auto next = state_->ship(*this)) {
        setState(std::move(next));
    }
}

void Order::complete() {
    if (auto next = state_->complete(*this)) {
        setState(std::move(next));
    }
}

void Order::cancel() {
    if (auto next = state_->cancel(*this)) {
        setState(std::move(next));
    }
}

// =========================
// PendingState 实现
// =========================
std::unique_ptr<OrderState> PendingState::pay(Order& order) {
    std::println("订单 {} 支付成功", order.id());
    return std::make_unique<PaidState>();
}

std::unique_ptr<OrderState> PendingState::ship(Order&) {
    std::println("订单尚未支付，无法发货");
    return nullptr;
}

std::unique_ptr<OrderState> PendingState::complete(Order&) {
    std::println("订单尚未支付，无法完成");
    return nullptr;
}

std::unique_ptr<OrderState> PendingState::cancel(Order& order) {
    std::println("订单 {} 已取消", order.id());
    return std::make_unique<CancelledState>();
}

// =========================
// PaidState 实现
// =========================
std::unique_ptr<OrderState> PaidState::pay(Order&) {
    std::println("订单已支付，无需重复支付");
    return nullptr;
}

std::unique_ptr<OrderState> PaidState::ship(Order& order) {
    std::println("订单 {} 已发货", order.id());
    return std::make_unique<ShippedState>();
}

std::unique_ptr<OrderState> PaidState::complete(Order&) {
    std::println("订单尚未发货，无法完成");
    return nullptr;
}

std::unique_ptr<OrderState> PaidState::cancel(Order& order) {
    std::println("订单 {} 已取消", order.id());
    return std::make_unique<CancelledState>();
}

// =========================
// ShippedState 实现
// =========================
std::unique_ptr<OrderState> ShippedState::pay(Order&) {
    std::println("订单已发货，无法支付");
    return nullptr;
}

std::unique_ptr<OrderState> ShippedState::ship(Order&) {
    std::println("订单已发货，无需重复发货");
    return nullptr;
}

std::unique_ptr<OrderState> ShippedState::complete(Order& order) {
    std::println("订单 {} 已完成", order.id());
    return std::make_unique<CompletedState>();
}

std::unique_ptr<OrderState> ShippedState::cancel(Order&) {
    std::println("订单已发货，无法取消");
    return nullptr;
}

// =========================
// 客户端
// =========================
int main() {
    Order order("ORD-1001");

    order.pay();       // 新建 -> 已支付
    order.ship();      // 已支付 -> 已发货
    order.complete();  // 已发货 -> 已完成

    std::println("");

    Order order2("ORD-1002");
    order2.cancel();   // 新建 -> 已取消
    order2.pay();      // 已取消，无法支付

    return 0;
}