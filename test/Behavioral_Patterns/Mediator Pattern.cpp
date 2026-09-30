#include <memory>
#include <print>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

/*
中介者模式（Mediator Pattern）用一个中介对象来封装一系列对象之间的交互，使各对象不需要显式地相互引用，从而降低耦合度，并可以独立地改变它们之间的交互。
*/

class Colleague;

// =========================
// Mediator：抽象中介者
// =========================
class Mediator {
public:
    virtual ~Mediator() = default;

    virtual void addColleague(std::shared_ptr<Colleague> colleague) = 0;
    virtual void sendMessage(std::string_view message, Colleague* sender) = 0;
};

// =========================
// Colleague：抽象同事
// =========================
class Colleague {
public:
    explicit Colleague(std::string_view name, Mediator* mediator)
        : name_(name), mediator_(mediator) {}

    virtual ~Colleague() = default;

    [[nodiscard]] std::string_view name() const { return name_; }

    // 通过中介者发送消息
    virtual void send(std::string_view message) {
        mediator_->sendMessage(message, this);
    }

    // 接收消息，由具体同事实现
    virtual void receive(std::string_view message,
                         std::string_view senderName) = 0;

protected:
    std::string name_;
    Mediator* mediator_;   // 非拥有指针，中介者生命周期长于同事
};

// =========================
// ConcreteColleague：具体同事（用户）
// =========================
class User final : public Colleague {
public:
    using Colleague::Colleague;

    void receive(std::string_view message,
                 std::string_view senderName) override {
        std::println("[{} 收到来自 {} 的消息]: {}",
                     name_, senderName, message);
    }
};

// =========================
// ConcreteMediator：具体中介者（聊天室）
// =========================
class ChatRoom final : public Mediator {
public:
    void addColleague(std::shared_ptr<Colleague> colleague) override {
        colleagues_.push_back(std::move(colleague));
    }

    void sendMessage(std::string_view message, Colleague* sender) override {
        for (auto& colleague : colleagues_) {
            // 不把消息发回给发送者自己
            if (colleague.get() != sender) {
                colleague->receive(message, sender->name());
            }
        }
    }

private:
    std::vector<std::shared_ptr<Colleague>> colleagues_;
};

// =========================
// 客户端
// =========================
int main() {
    auto chatRoom = std::make_shared<ChatRoom>();

    auto alice   = std::make_shared<User>("Alice", chatRoom.get());
    auto bob     = std::make_shared<User>("Bob", chatRoom.get());
    auto charlie = std::make_shared<User>("Charlie", chatRoom.get());

    chatRoom->addColleague(alice);
    chatRoom->addColleague(bob);
    chatRoom->addColleague(charlie);

    alice->send("大家好！");
    bob->send("你好 Alice！");

    return 0;
}