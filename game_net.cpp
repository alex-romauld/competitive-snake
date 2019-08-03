#include "game_net.h"

int game_net::init_host(const char* port)
{
	int clientAddrSize = sizeof(clientAddr);

	WSAStartup(MAKEWORD(2,0), &WSAData);
	server = socket(AF_INET, SOCK_STREAM, 0);

	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons((unsigned)atoi(port));

	bind(server, (SOCKADDR *)&serverAddr, sizeof(serverAddr));


	listen(server, 0);

	std::cout << "Waiting for peer...";

	// Accept New Connection
	client = accept(server, (SOCKADDR *)&clientAddr, &clientAddrSize);
	if(client != INVALID_SOCKET){
		std::cout << "Connected!" << std::endl;
	}

	is_host = true;
	return 1;
}


int game_net::connect_to(const char* addr, const char* port)
{
	WSAStartup(MAKEWORD(2,0), &WSAData);
	server = socket(AF_INET, SOCK_STREAM, 0);

	const char* lh = "localhost";
	if(strcmp(addr, lh) == 0)
		addr = "127.0.0.1\0";

	clientAddr.sin_addr.s_addr = inet_addr(addr);
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons((unsigned)atoi(port));

	std::cout << "Connecting To Host...";
	if(connect(server, (SOCKADDR *)&clientAddr, sizeof(clientAddr)) == SOCKET_ERROR){
		std::cout << "Failed!" << std::endl;
		return 0;
	}

	is_host = false;
	std::cout << "Connected!" << std::endl;
	return 1;
}

void game_net::send_dir(compass dir)
{

	int id = 0;
	if(dir == NORTH)
	  id = 1;
	if(dir == SOUTH)
	  id = 2;
	if(dir == EAST)
	  id = 3;
	if(dir == WEST)
	  id = 4;
	char buf[sizeof(int)];
	memcpy(&buf, &id, sizeof(id));


	if(is_host)	send(client, buf, sizeof(buf), 0);
	else		send(server, buf, sizeof(buf), 0);
}

compass game_net::recv_dir()
{
	compass r;

	int id = 0;
	char buf[sizeof(id)];

	if(is_host)	recv(client, buf, sizeof(buf), 0);
	else	 	recv(server, buf, sizeof(buf), 0);

	memcpy(&id, &buf, sizeof(id));

	r = NONE;
	if(id == 1) r = NORTH;
	if(id == 2) r = SOUTH;
	if(id == 3) r = EAST;
	if(id == 4) r = WEST;

	return r;
}
