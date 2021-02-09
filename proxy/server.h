#pragma once
#include <string>
#include "proton/variant.hpp"
#include "enet/include/enet.h"
#include "world.h"

class server {
   private:
    ENetHost* m_proxy_server;
    ENetHost* m_real_server;
    ENetPeer* m_server_peer;
    ENetPeer* m_gt_peer;
  
    
    void handle_outgoing();
    void handle_incoming();
    bool connect();
    void disconnect(bool reset);
   public:
    struct TankPacket {
       int32_t PacketType;
       int32_t NetID;
       int32_t SecondaryNetID;
       int32_t ExtDataMask;
       float Padding;
       int32_t MainValue;
       float X, Y;
       float XSpeed, YSpeed;
       int32_t SecondaryPadding;
       int32_t PunchX, PunchY;
       int32_t ExtDataSize;
    };
    BYTE* PackForSendingRaw(TankPacket* dataStruct) {
        BYTE* data = new BYTE[56];
        for (int i = 0; i < 56; i++) {
            data[i] = 0;
        }
        memcpy(data, &dataStruct->PacketType, 4);
        memcpy(data + 4, &dataStruct->NetID, 4);
        memcpy(data + 8, &dataStruct->SecondaryNetID, 4);
        memcpy(data + 12, &dataStruct->ExtDataMask, 4);
        memcpy(data + 16, &dataStruct->Padding, 4);
        memcpy(data + 20, &dataStruct->MainValue, 4);
        memcpy(data + 24, &dataStruct->X, 4);
        memcpy(data + 28, &dataStruct->Y, 4);
        memcpy(data + 32, &dataStruct->XSpeed, 4);
        memcpy(data + 36, &dataStruct->YSpeed, 4);
        memcpy(data + 40, &dataStruct->SecondaryPadding, 4);
        memcpy(data + 44, &dataStruct->PunchX, 4);
        memcpy(data + 48, &dataStruct->PunchY, 4);
        memcpy(data + 52, &dataStruct->ExtDataSize, 4);
        return data;
    }
    int m_user = 0;
    int m_token = 0;
    std::string m_server = "213.179.209.168";
    int m_port = 17198;
    int m_proxyport = 17191;
    world m_world;
    bool start();
    void quit();
    bool setup_client();
    void redirect_server(variantlist_t& varlist);
    void send(bool client, int32_t type, uint8_t* data, int32_t len);
    void send(bool client, variantlist_t& list, int32_t netid = -1, int32_t delay = 0);
    void send(bool client, std::string packet, int32_t type = 2);
    void send(bool client, uint8_t* data, int32_t len, void* a4, int32_t a1);
    void poll();
};
extern server* g_server;

