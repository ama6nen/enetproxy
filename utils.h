#pragma once
#include <stdint.h>
#include <string>
#include <vector>
#include "enet/include/enet.h"
#include "packet.h"
#include "proton/variant.hpp"

#define RESET "\033[0m"
#define BLACK "\033[30m"              /* Black */
#define RED "\033[31m"                /* Red */
#define GREEN "\033[32m"              /* Green */
#define YELLOW "\033[33m"             /* Yellow */
#define BLUE "\033[34m"               /* Blue */
#define MAGENTA "\033[35m"            /* Magenta */
#define CYAN "\033[36m"               /* Cyan */
#define WHITE "\033[37m"              /* White */
#define BOLDBLACK "\033[1m\033[30m"   /* Bold Black */
#define BOLDRED "\033[1m\033[31m"     /* Bold Red */
#define BOLDGREEN "\033[1m\033[32m"   /* Bold Green */
#define BOLDYELLOW "\033[1m\033[33m"  /* Bold Yellow */
#define BOLDBLUE "\033[1m\033[34m"    /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m" /* Bold Magenta */
#define BOLDCYAN "\033[1m\033[36m"    /* Bold Cyan */
#define BOLDWHITE "\033[1m\033[37m"   /* Bold White */

#define PRINTS(msg, ...) printf("[SERVER]: " msg, __VA_ARGS__);
#define PRINTC(msg, ...) printf("[CLIENT]: " msg, __VA_ARGS__);
#define MALLOC(type, ...) (type*)(malloc(sizeof(type) __VA_ARGS__))
#define get_packet_type(packet) (packet->dataLength > 3 ? *packet->data : 0)
#define DO_ONCE            \
    ([]() {                \
        static char o = 0; \
        return !o && ++o;  \
    }())
#ifdef _WIN32
#define INLINE __forceinline
#else //for gcc/clang
#define INLINE __attribute__((always_inline))
#endif

namespace utils {
    char* get_text(ENetPacket* packet);
    gameupdatepacket_t* get_struct(ENetPacket* packet);
    int random(int min, int max);
    std::string generate_rid();
    uint32_t hash(uint8_t* str, uint32_t len);
    std::string generate_mac(const std::string& prefix = "02");
    std::string hex_str(unsigned char data);
    std::string random(uint32_t length);
    __forceinline uint8_t* get_extended(gameupdatepacket_t* packet) {
        return reinterpret_cast<uint8_t*>(&packet->m_data_size);
    }
    bool replace(std::string& str, const std::string& from, const std::string& to);
    bool is_number(const std::string& s);
} // namespace utils