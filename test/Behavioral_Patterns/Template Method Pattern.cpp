#include <print>

// =========================
// AbstractClass：抽象基类
// =========================
class Beverage {
public:
    virtual ~Beverage() = default;

    // 模板方法：定义算法骨架，非虚，子类不应修改
    void prepareRecipe() const {
        boilWater();                 // 通用步骤
        brew();                      // 子类实现
        pourInCup();                 // 通用步骤
        if (customerWantsCondiments()) {
            addCondiments();         // 子类实现
        }
    }

protected:
    // 通用步骤：基类提供默认实现
    virtual void boilWater() const {
        std::println("煮沸水");
    }

    virtual void pourInCup() const {
        std::println("倒入杯中");
    }

    // 基本步骤：由子类实现
    virtual void brew() const = 0;
    virtual void addCondiments() const = 0;

    // 钩子：默认返回 true，子类可覆盖
    [[nodiscard]] virtual bool customerWantsCondiments() const {
        return true;
    }
};

// =========================
// ConcreteClass：咖啡
// =========================
class Coffee final : public Beverage {
protected:
    void brew() const override {
        std::println("用沸水冲泡咖啡粉");
    }

    void addCondiments() const override {
        std::println("加糖和牛奶");
    }
};

// =========================
// ConcreteClass：茶
// =========================
class Tea final : public Beverage {
protected:
    void brew() const override {
        std::println("用沸水浸泡茶叶");
    }

    void addCondiments() const override {
        std::println("加柠檬");
    }

    // 覆盖钩子：茶不加调料
    [[nodiscard]] bool customerWantsCondiments() const override {
        return false;
    }
};

// =========================
// 客户端
// =========================
int main() {
    std::println("=== 制作咖啡 ===");
    Coffee coffee;
    coffee.prepareRecipe();

    std::println("\n=== 制作茶 ===");
    Tea tea;
    tea.prepareRecipe();

    return 0;
}