#pragma once
#include <ctime>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include "enet/include/enet.h"
#include "http.h"
#include "server.h"

server* g_server = new server();

int main() {
#ifdef _WIN32
    SetConsoleTitleA("proxy by ama");
#endif
    printf("enet proxy by ama\n");
	printf("Replacing hosts to 127.0.0.1\n");
	std::ofstream MyFile("C:\\Windows\\System32\\drivers\\etc\\hosts");

	MyFile << "127.0.0.1 growtopia1.com\n127.0.0.1 growtopia2.com";

	MyFile.close();
	printf("the hosts has been replaced!\n");
	
    std::thread http(http::run, "127.0.0.1", "17191");
    http.detach();
    printf("HTTP server is running.\n");

    enet_initialize();
    if (g_server->start()) {
        printf("Server & client proxy is running.\n");
        while (true) {
            g_server->poll();
            Sleep(1);
        }
    }   
    else
        printf("Failed to start server or proxy.\n");
}
