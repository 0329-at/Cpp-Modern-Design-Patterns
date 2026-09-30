#include <print>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

/*
备忘录模式（Memento Pattern）用于在不破坏封装的前提下，捕获并保存一个对象的内部状态，以便以后可以恢复到该状态。它常用于撤销、回滚、快照等场景。
*/

// =========================
// Memento：备忘录
// =========================
class Memento {
public:
    // 只有 TextEditor 可以创建和访问内部状态
    friend class TextEditor;

private:
    explicit Memento(std::string state) : state_(std::move(state)) {}

    std::string state_;
};

// =========================
// Originator：发起人（文本编辑器）
// =========================
class TextEditor {
public:
    void type(std::string_view text) {
        content_ += text;
    }

    [[nodiscard]] std::string_view content() const {
        return content_;
    }

    // 保存当前状态到备忘录
    [[nodiscard]] Memento save() const {
        return Memento(content_);
    }

    // 从备忘录恢复状态
    void restore(const Memento& memento) {
        content_ = memento.state_;
    }

private:
    std::string content_;
};

// =========================
// Caretaker：管理者（历史记录）
// =========================
class History {
public:
    void push(const Memento& memento) {
        history_.push_back(memento);
    }

    [[nodiscard]] Memento pop() {
        if (history_.empty()) {
            throw std::out_of_range("History is empty");
        }
        Memento memento = history_.back();
        history_.pop_back();
        return memento;
    }

    [[nodiscard]] bool empty() const {
        return history_.empty();
    }

private:
    std::vector<Memento> history_;
};

// =========================
// 客户端
// =========================
int main() {
    TextEditor editor;
    History history;

    editor.type("Hello");
    history.push(editor.save());
    std::println("当前内容: {}", editor.content());

    editor.type(" World");
    history.push(editor.save());
    std::println("当前内容: {}", editor.content());

    editor.type("!");
    std::println("当前内容: {}", editor.content());

    // 撤销到 Hello World
    if (!history.empty()) {
        editor.restore(history.pop());
        std::println("撤销后: {}", editor.content());
    }

    // 撤销到 Hello
    if (!history.empty()) {
        editor.restore(history.pop());
        std::println("撤销后: {}", editor.content());
    }

    // 没有更多历史
    if (!history.empty()) {
        editor.restore(history.pop());
        std::println("撤销后: {}", editor.content());
    } else {
        std::println("没有更多历史记录");
    }

    return 0;
}