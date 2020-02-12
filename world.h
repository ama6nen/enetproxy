#pragma once
#include <string>
#include <vector>
#include "player.h"

class world {
   public:
    std::string name{};
    std::vector<player> players{};
    player local{};
    bool connected{};
};