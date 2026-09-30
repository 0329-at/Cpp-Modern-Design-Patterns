#include <iostream>
#include <string>
#include <utility>

// 产品：电脑
class Computer {
public:
    // 声明生成器为友元，让生成器可以访问私有构造函数
    friend class ComputerBuilder;

    void show() const {
        std::cout << "电脑配置:\n"
                  << "  CPU : " << cpu_ << "\n"
                  << "  内存: " << memory_ << "\n"
                  << "  硬盘: " << disk_ << "\n"
                  << "  显卡: " << gpu_ << "\n";
    }

private:
    // 私有构造函数，只能由 ComputerBuilder 创建
    Computer(std::string cpu, std::string memory,
             std::string disk, std::string gpu)
        : cpu_(std::move(cpu)),
          memory_(std::move(memory)),
          disk_(std::move(disk)),
          gpu_(std::move(gpu)) {}

    std::string cpu_;
    std::string memory_;
    std::string disk_;
    std::string gpu_;
};

// 链式生成器
class ComputerBuilder {
public:
    ComputerBuilder& setCPU(std::string_view cpu) {
        cpu_ = cpu;
        return *this;   // 返回自身引用，支持链式调用
    }

    ComputerBuilder& setMemory(std::string_view memory) {
        memory_ = memory;
        return *this;
    }

    ComputerBuilder& setDisk(std::string_view disk) {
        disk_ = disk;
        return *this;
    }

    ComputerBuilder& setGPU(std::string_view gpu) {
        gpu_ = gpu;
        return *this;
    }

    // 最终构建并返回 Computer 对象
    Computer build() {
        return Computer(cpu_, memory_, disk_, gpu_);
    }

private:
    // 默认配置，未设置的项将使用默认值
    std::string cpu_    = "默认 CPU";
    std::string memory_ = "默认内存";
    std::string disk_   = "默认硬盘";
    std::string gpu_    = "默认显卡";
};

int main() {
    // 1. 构建高端游戏电脑
    Computer gamingPC = ComputerBuilder()
                            .setCPU("Intel i9-13900K")
                            .setMemory("32GB DDR5 6000MHz")
                            .setDisk("2TB NVMe SSD")
                            .setGPU("NVIDIA RTX 4090")
                            .build();

    std::cout << "=== 游戏电脑 ===\n";
    gamingPC.show();

    // 2. 构建办公电脑
    Computer officePC = ComputerBuilder()
                            .setCPU("Intel i5-13400")
                            .setMemory("16GB DDR4 3200MHz")
                            .setDisk("512GB NVMe SSD")
                            .setGPU("Intel UHD Graphics 730")
                            .build();

    std::cout << "\n=== 办公电脑 ===\n";
    officePC.show();

    // 3. 构建入门电脑，只设置部分配置，其余使用默认值
    Computer basicPC = ComputerBuilder()
                           .setCPU("Intel Celeron N4020")
                           .setMemory("8GB DDR4")
                           .build();

    std::cout << "\n=== 入门电脑 ===\n";
    basicPC.show();

    return 0;
}