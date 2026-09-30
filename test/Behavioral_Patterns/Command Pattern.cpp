#include <memory>
#include <print>
#include <string>
#include <string_view>
#include <utility>

/*
命令模式（Command Pattern）把请求封装成对象，让客户端可以用不同的请求参数化其他对象，并支持排队、记录日志、撤销等操作。
*/

// =========================
// Receiver：接收者
// =========================
class Light {
public:
    void on()  { std::println("灯打开了"); }
    void off() { std::println("灯关闭了"); }
};

class Stereo {
public:
    void on()  { std::println("音响打开了"); }
    void off() { std::println("音响关闭了"); }
    void setVolume(int volume) {
        std::println("音响音量设置为 {}", volume);
    }
};

// =========================
// Command：抽象命令
// =========================
class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
    virtual void undo() = 0;
};

// =========================
// ConcreteCommand：具体命令
// =========================

// 开灯命令
class LightOnCommand final : public Command {
public:
    explicit LightOnCommand(std::shared_ptr<Light> light)
        : light_(std::move(light)) {}

    void execute() override { light_->on(); }
    void undo() override    { light_->off(); }

private:
    std::shared_ptr<Light> light_;
};

// 关灯命令
class LightOffCommand final : public Command {
public:
    explicit LightOffCommand(std::shared_ptr<Light> light)
        : light_(std::move(light)) {}

    void execute() override { light_->off(); }
    void undo() override    { light_->on(); }

private:
    std::shared_ptr<Light> light_;
};

// 开音响命令
class StereoOnCommand final : public Command {
public:
    explicit StereoOnCommand(std::shared_ptr<Stereo> stereo)
        : stereo_(std::move(stereo)) {}

    void execute() override {
        stereo_->on();
        stereo_->setVolume(10);
    }
    void undo() override {
        stereo_->off();
    }

private:
    std::shared_ptr<Stereo> stereo_;
};

// 关音响命令
class StereoOffCommand final : public Command {
public:
    explicit StereoOffCommand(std::shared_ptr<Stereo> stereo)
        : stereo_(std::move(stereo)) {}

    void execute() override {
        stereo_->off();
    }
    void undo() override {
        stereo_->on();
        stereo_->setVolume(10);
    }

private:
    std::shared_ptr<Stereo> stereo_;
};

// =========================
// Invoker：调用者
// =========================
class RemoteControl {
public:
    void setCommand(std::unique_ptr<Command> command) {
        command_ = std::move(command);
        lastCommand_ = nullptr;   // 新命令，不能撤销之前的操作
    }

    void pressButton() {
        if (command_) {
            command_->execute();
            lastCommand_ = command_.get();   // 记录最近执行的命令，用于撤销
        }
    }

    void pressUndo() {
        if (lastCommand_) {
            lastCommand_->undo();
            lastCommand_ = nullptr;
        } else {
            std::println("没有可撤销的操作");
        }
    }

private:
    std::unique_ptr<Command> command_;
    Command* lastCommand_ = nullptr;   // 非拥有指针，指向当前命令
};

// =========================
// 客户端
// =========================
int main() {
    // 创建接收者
    auto livingRoomLight = std::make_shared<Light>();
    auto stereo = std::make_shared<Stereo>();

    RemoteControl remote;

    // 1. 开灯
    std::println("--- 开灯 ---");
    remote.setCommand(std::make_unique<LightOnCommand>(livingRoomLight));
    remote.pressButton();

    // 撤销开灯
    std::println("--- 撤销 ---");
    remote.pressUndo();

    // 2. 开音响
    std::println("\n--- 开音响 ---");
    remote.setCommand(std::make_unique<StereoOnCommand>(stereo));
    remote.pressButton();

    // 撤销开音响
    std::println("--- 撤销 ---");
    remote.pressUndo();

    // 3. 关灯（此时灯是关的，执行关灯再撤销会开灯）
    std::println("\n--- 关灯 ---");
    remote.setCommand(std::make_unique<LightOffCommand>(livingRoomLight));
    remote.pressButton();

    std::println("--- 撤销 ---");
    remote.pressUndo();

    // 4. 没有可撤销的操作
    std::println("\n--- 再次撤销 ---");
    remote.pressUndo();

    return 0;
}