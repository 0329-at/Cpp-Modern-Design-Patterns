#include <memory>
#include <print>
#include <string>
#include <string_view>

/*
外观模式（Facade Pattern）为子系统中的一组接口提供一个统一的高层接口，让客户端更容易使用。它不阻止客户端直接访问子系统，只是提供一个简化入口，降低客户端与子系统的耦合。
*/

// =========================
// 子系统：投影仪
// =========================
class Projector {
public:
    void on()  { std::println("投影仪打开"); }
    void off() { std::println("投影仪关闭"); }
    void wideScreenMode() { std::println("投影仪设置为宽屏模式"); }
};

// =========================
// 子系统：音响
// =========================
class Amplifier {
public:
    void on()  { std::println("音响打开"); }
    void off() { std::println("音响关闭"); }
    void setVolume(int level) { std::println("音响音量设置为 {}", level); }
};

// =========================
// 子系统：灯光
// =========================
class Lights {
public:
    void dim(int level) { std::println("灯光调暗到 {}%", level); }
    void on()  { std::println("灯光打开"); }
};

// =========================
// 子系统：流媒体播放器
// =========================
class StreamingPlayer {
public:
    void on()  { std::println("流媒体播放器打开"); }
    void off() { std::println("流媒体播放器关闭"); }
    void play(std::string_view movie) { std::println("播放电影: {}", movie); }
    void stop() { std::println("停止播放"); }
};

// =========================
// 外观类：家庭影院
// =========================
class HomeTheaterFacade {
public:
    HomeTheaterFacade() = default;

    void watchMovie(std::string_view movie) {
        std::println("准备观看电影...");
        lights_.dim(10);
        projector_.on();
        projector_.wideScreenMode();
        amplifier_.on();
        amplifier_.setVolume(5);
        player_.on();
        player_.play(movie);
    }

    void endMovie() {
        std::println("结束电影...");
        player_.stop();
        player_.off();
        amplifier_.off();
        projector_.off();
        lights_.on();
    }

private:
    Projector       projector_;
    Amplifier       amplifier_;
    Lights          lights_;
    StreamingPlayer player_;
};

// =========================
// 客户端
// =========================
int main() {
    HomeTheaterFacade homeTheater;
    homeTheater.watchMovie("星际穿越");
    std::println("");
    homeTheater.endMovie();
    return 0;
}