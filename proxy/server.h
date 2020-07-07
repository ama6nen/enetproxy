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
    
    void doPacketQueue();
    void doHandleOutgoingForServiceThread();
    void handle_outgoing();
    void handle_outgoing_incoming_safe(bool incoming); // true = handle incoming, false = handle outgoing
    void handle_incoming();
    bool connect();
    void disconnect(bool reset);
   public:
    int m_user = 0;
    int m_token = 0;
    bool m_started_server_service_thread = false;
    std::string m_server = "213.179.209.168";
    
    struct ENetPacketQueueStruct {
        ENetPacket* e_Packet;
        ENetPeer* peerReceiver; // also contains the host.
    };

    std::vector<ENetPacketQueueStruct> ENetPacketQueue;

    int m_port = 17273;
    int m_proxyport = 17191;
    world m_world;
    bool start();
    void quit();
    bool setup_client();
    void redirect_server(variantlist_t& varlist);
    void send(bool client, int32_t type, uint8_t* data, int32_t len);
    void send(bool client, variantlist_t& list, int32_t netid = -1, int32_t delay = 0);
    void send(bool client, std::string packet, int32_t type = 2);
    void enet_peer_send_safe(ENetPeer* peer, enet_uint8 channel, ENetPacket* packet); // basically means send & flush via this shared function for thread safety.
    void poll();
};
extern server* g_server;

