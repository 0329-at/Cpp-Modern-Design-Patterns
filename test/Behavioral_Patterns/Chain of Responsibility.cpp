#include <memory>
#include <print>
#include <string>
#include <string_view>
#include <utility>

/*
责任链模式（Chain of Responsibility）把请求的发送者和接收者解耦，让多个处理者都有机会处理请求。

请求沿着处理者链传递，直到某个处理者处理它为止。每个处理者持有下一个处理者的引用，自己不能处理时就转发给下一个。
*/

// 日志级别
enum class LogLevel { Debug, Info, Warning, Error };

std::string_view to_string(LogLevel level) {
    switch (level) {
        case LogLevel::Debug:   return "DEBUG";
        case LogLevel::Info:    return "INFO";
        case LogLevel::Warning: return "WARNING";
        case LogLevel::Error:   return "ERROR";
    }
    return "UNKNOWN";
}

// 请求对象
struct LogRequest {
    LogLevel level;
    std::string message;
};

// =========================
// Handler：抽象处理者
// =========================
class Handler {
public:
    virtual ~Handler() = default;

    // 设置下一个处理者，返回下一个处理者的引用，支持链式调用
    Handler& setNext(std::unique_ptr<Handler> next) {
        next_ = std::move(next);
        return *next_;
    }

    virtual void handle(const LogRequest& request) = 0;

protected:
    std::unique_ptr<Handler> next_;   // 下一个处理者
};

// =========================
// 具体处理者：Debug
// =========================
class DebugHandler final : public Handler {
public:
    void handle(const LogRequest& request) override {
        if (request.level == LogLevel::Debug) {
            std::println("[DEBUG] {}", request.message);
        } else if (next_) {
            next_->handle(request);
        } else {
            std::println("未处理的日志: [{}] {}",
                         to_string(request.level), request.message);
        }
    }
};

// =========================
// 具体处理者：Info
// =========================
class InfoHandler final : public Handler {
public:
    void handle(const LogRequest& request) override {
        if (request.level == LogLevel::Info) {
            std::println("[INFO] {}", request.message);
        } else if (next_) {
            next_->handle(request);
        } else {
            std::println("未处理的日志: [{}] {}",
                         to_string(request.level), request.message);
        }
    }
};

// =========================
// 具体处理者：Warning
// =========================
class WarningHandler final : public Handler {
public:
    void handle(const LogRequest& request) override {
        if (request.level == LogLevel::Warning) {
            std::println("[WARNING] {}", request.message);
        } else if (next_) {
            next_->handle(request);
        } else {
            std::println("未处理的日志: [{}] {}",
                         to_string(request.level), request.message);
        }
    }
};

// =========================
// 具体处理者：Error
// =========================
class ErrorHandler final : public Handler {
public:
    void handle(const LogRequest& request) override {
        if (request.level == LogLevel::Error) {
            std::println("[ERROR] {}", request.message);
        } else if (next_) {
            next_->handle(request);
        } else {
            std::println("未处理的日志: [{}] {}",
                         to_string(request.level), request.message);
        }
    }
};

// =========================
// 客户端
// =========================
int main() {
    // 创建各个处理者
    auto debug   = std::make_unique<DebugHandler>();
    auto info    = std::make_unique<InfoHandler>();
    auto warning = std::make_unique<WarningHandler>();
    auto error   = std::make_unique<ErrorHandler>();

    // 构建责任链：Debug -> Info -> Warning -> Error
    debug->setNext(std::move(info))
         .setNext(std::move(warning))
         .setNext(std::move(error));

    // 发送不同级别的日志
    debug->handle({LogLevel::Debug,   "调试信息"});
    debug->handle({LogLevel::Info,    "普通信息"});
    debug->handle({LogLevel::Warning, "警告信息"});
    debug->handle({LogLevel::Error,   "错误信息"});

    return 0;
}