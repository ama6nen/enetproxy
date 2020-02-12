#pragma once
#include <string>

namespace gt {
    extern std::string version;
    extern std::string flag;
    extern bool resolving_uid;
    extern bool resolving_uid2;
    extern bool connecting;
    extern bool in_game;
    extern bool ghost;
    extern bool maxname;
    void send_log(std::string text);
    void resolve_uid_to_name(std::string uid);
    void solve_captcha(std::string text);
}