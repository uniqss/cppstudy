#include <asio.hpp>
#include <vector>

#define FPS 20
#define FRAME_TIME_MS 1000 / FPS
struct Skill {
    const int skill_id;
    const int skill_cd_frames;
    int cd_remain_frame;
};
int skill_cd_s2frame(int seconds) {
    return seconds * FPS;
}

asio::awaitable<void> player_skill(asio::io_context& ctx, Skill& skill) {
    while(true){
        std::this_thread::sleep_for()
    }
    co_return;
}
int main() {
    asio::io_context ctx;
    std::vector<Skill> skills = {
        {1001, skill_cd_s2frame(3)}, {1002, skill_cd_s2frame(10)}, {1003, skill_cd_s2frame(5)}};

    for (auto& skill : skills) {
        asio::co_spawn(ctx, player_skill(ctx, skill), asio::detached);
    }
    ctx.run();
    return 0;
}
