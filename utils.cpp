#pragma once
#include "utils.h"
#include <algorithm>
#include <chrono>
#include <random>
#include "proton/variant.hpp"

char* utils::get_text(ENetPacket* packet) {
    gametankpacket_t* tank = reinterpret_cast<gametankpacket_t*>(packet->data);
    memset(packet->data + packet->dataLength - 1, 0, 1);
    return static_cast<char*>(&tank->m_data);
}
gameupdatepacket_t* utils::get_struct(ENetPacket* packet) {
    if (packet->dataLength < sizeof(gameupdatepacket_t) - 4)
        return nullptr;
    gametankpacket_t* tank = reinterpret_cast<gametankpacket_t*>(packet->data);
    gameupdatepacket_t* gamepacket = reinterpret_cast<gameupdatepacket_t*>(packet->data + 4);
    if (gamepacket->m_packet_flags & 8) {
        if (packet->dataLength < gamepacket->m_data_size + 60) {
            printf("got invalid packet. (too small)\n");
            return nullptr;
        }
        return reinterpret_cast<gameupdatepacket_t*>(&tank->m_data);
    } else
        gamepacket->m_data_size = 0;
    return gamepacket;
}
std::mt19937 rng;

int utils::random(int min, int max) {
    if (DO_ONCE)
        rng.seed((unsigned int)std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(rng);
}

std::string utils::generate_rid() {
    std::string rid_str;

    for (int i = 0; i < 16; i++)
        rid_str += utils::hex_str(utils::random(0, 255));

    std::transform(rid_str.begin(), rid_str.end(), rid_str.begin(), std::toupper);

    return rid_str;
}
uint32_t utils::hash(uint8_t* str, uint32_t len) {
    if (!str)
        return 0;
    uint8_t* n = (uint8_t*)str;
    uint32_t acc = 0x55555555;
    if (!len)
        while (*n)
            acc = (acc >> 27) + (acc << 5) + *n++;
    else
        for (uint32_t i = 0; i < len; i++)
            acc = (acc >> 27) + (acc << 5) + *n++;
    return acc;
}
std::string utils::generate_mac(const std::string& prefix) {
    std::string x = prefix + ":";
    for (int i = 0; i < 5; i++) {
        x += utils::hex_str(utils::random(0, 255));
        if (i != 4)
            x += ":";
    }
    return x;
}
const char hexmap_s[17] = "0123456789abcdef";
std::string utils::hex_str(unsigned char data) {
    std::string s(2, ' ');
    s[0] = hexmap_s[(data & 0xF0) >> 4];
    s[1] = hexmap_s[data & 0x0F];
    return s;
}

std::string utils::random(uint32_t length) {
    static auto randchar = []() -> char {
        const char charset[] =
            "0123456789"
            "qwertyuiopasdfghjklzxcvbnm"
            "QWERTYUIOPASDFGHJKLZXCVBNM";
        const uint32_t max_index = (sizeof(charset) - 1);
        return charset[utils::random(INT16_MAX, INT32_MAX) % max_index];
    };

    std::string str(length, 0);
    std::generate_n(str.begin(), length, randchar);
    return str;
}

bool utils::replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if (start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}
bool utils::is_number(const std::string& s) {
    return !s.empty() && std::find_if(s.begin() + (*s.data() == '-' ? 1 : 0), s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}