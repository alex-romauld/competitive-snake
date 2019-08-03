/*
  Host and Connect (Peer to Peer)
  Send And Recv GameState Info
*/

#ifndef SERVER_H
#define SERVER_H

#include <winsock2.h>
#include <iostream>
#include "game_components.h"

struct game_net{
	bool is_host;
	WSADATA WSAData;
	SOCKET server, client;
	SOCKADDR_IN serverAddr, clientAddr;

	int init_host(const char* port);
	int connect_to(const char* addr, const char* port);

	void send_dir(compass dir);
	compass recv_dir();
};

#endif // SERVER_H
