#include <memory>
#include <print>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

/*
享元模式（Flyweight Pattern）用于减少大量细粒度对象的内存占用。核心思想是把对象状态分为两类：

内在状态（Intrinsic State）：可共享、不随环境变化，放在享元对象中。

外在状态（Extrinsic State）：每个对象特有、随环境变化，由客户端保存并在调用时传入。

通过享元工厂缓存内在状态相同的对象，让多个“逻辑对象”共享同一个享元实例。
*/

// =========================
// 享元：树的类型（内在状态）
// =========================
class TreeType {
public:
    TreeType(std::string_view name,
             std::string_view color,
             std::string_view texture)
        : name_(name), color_(color), texture_(texture) {}

    // 外在状态通过参数传入
    void draw(double x, double y, int age) const {
        std::println("绘制树木: {} (颜色: {}, 纹理: {}) "
                     "位置: ({}, {}), 树龄: {}",
                     name_, color_, texture_, x, y, age);
    }

private:
    std::string name_;
    std::string color_;
    std::string texture_;
};

// =========================
// 享元工厂：缓存并共享 TreeType
// =========================
class TreeTypeFactory {
public:
    std::shared_ptr<TreeType> getTreeType(std::string_view name,
                                          std::string_view color,
                                          std::string_view texture) {
        // 用内在状态组合出唯一 key
        std::string key = std::string(name) + "_" +
                          std::string(color) + "_" +
                          std::string(texture);

        auto it = cache_.find(key);
        if (it != cache_.end()) {
            return it->second;   // 命中缓存，共享已有享元
        }

        auto type = std::make_shared<TreeType>(name, color, texture);
        cache_.emplace(std::move(key), type);
        return type;
    }

    [[nodiscard]] std::size_t size() const {
        return cache_.size();
    }

private:
    std::unordered_map<std::string, std::shared_ptr<TreeType>> cache_;
};

// =========================
// 外在状态：每棵树的位置和树龄
// =========================
class Tree {
public:
    Tree(std::shared_ptr<TreeType> type, double x, double y, int age)
        : type_(std::move(type)), x_(x), y_(y), age_(age) {}

    void draw() const {
        type_->draw(x_, y_, age_);
    }

private:
    std::shared_ptr<TreeType> type_;   // 共享享元
    double x_;
    double y_;
    int age_;
};

// =========================
// 森林：管理大量树木
// =========================
class Forest {
public:
    void plantTree(double x, double y, int age,
                   std::string_view name,
                   std::string_view color,
                   std::string_view texture) {
        auto type = factory_.getTreeType(name, color, texture);
        trees_.push_back(std::make_unique<Tree>(std::move(type), x, y, age));
    }

    void draw() const {
        for (const auto& tree : trees_) {
            tree->draw();
        }
    }

    [[nodiscard]] std::size_t treeCount() const {
        return trees_.size();
    }

    [[nodiscard]] std::size_t typeCount() const {
        return factory_.size();
    }

private:
    TreeTypeFactory factory_;
    std::vector<std::unique_ptr<Tree>> trees_;
};

// =========================
// 客户端
// =========================
int main() {
    Forest forest;

    // 种 5 棵树，但只涉及 2 种 TreeType
    forest.plantTree(1, 2, 10, "橡树", "绿色", "粗糙");
    forest.plantTree(3, 4, 20, "橡树", "绿色", "粗糙");
    forest.plantTree(5, 6, 15, "松树", "深绿色", "光滑");
    forest.plantTree(7, 8, 5,  "橡树", "绿色", "粗糙");
    forest.plantTree(9, 10, 25, "松树", "深绿色", "光滑");

    forest.draw();

    std::println("\n树木总数: {}", forest.treeCount());
    std::println("享元类型数: {}", forest.typeCount());

    return 0;
}