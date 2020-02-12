#pragma once
#include <string>
#include "packet.h"

//return value: true - dont send original packet, false - send original packet
namespace events {
    
    namespace out {
        bool variantlist(gameupdatepacket_t * packet);
        bool pingreply(gameupdatepacket_t * packet);
        bool generictext(std::string packet);
        bool gamemessage(std::string packet);
        bool state(gameupdatepacket_t * packet);

    }; // namespace out
    namespace in {
        bool variantlist(gameupdatepacket_t* packet);
        bool generictext(std::string packet);
        bool gamemessage(std::string packet);
        bool sendmapdata(gameupdatepacket_t* packet);
        bool state(gameupdatepacket_t* packet);
        bool tracking(std::string packet);
        bool sendTileActivate(gameupdatepacket_t* packet);
    };  // namespace in
        // namespace in
};      // namespace events