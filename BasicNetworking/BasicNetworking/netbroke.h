#pragma once
#pragma comment (lib,"wsock32.lib")
#pragma comment (lib,"ws2_32.lib")

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <cstdio>


namespace nsfw
{
	struct IPADDRESS
	{
		int ip[4];
		int port;
	};
	bool initNetworking()
	{
		WSADATA data;
		if (WSAStartup(MAKEWORD(2, 2), &data) != NO_ERROR)
			return false;
		return true;
	}
	bool termNetworking()
	{
		WSACleanup();
		return true;
	}

	bool stoa(const char *src, sockaddr_in &taddr)
	{
		IPADDRESS address;
		int pass = sscanf_s(src, "%d.%d.%d.%d:%d", &address.ip[0], &address.ip[1], &address.ip[2], &address.ip[3], &address.port);
		if (pass != 5)
			return false;
		for (int i = 0; i < 4; i++)
		{
			if (address.ip[i] > 255 || address.ip[i] < 0)
				return false;
		}
		if (address.port > 65535 || address.port < 0)
			return false;
		
		taddr.sin_family = AF_INET;
		taddr.sin_addr.S_un.S_un_b.s_b1 = address.ip[0];
		taddr.sin_addr.S_un.S_un_b.s_b2 = address.ip[1];
		taddr.sin_addr.S_un.S_un_b.s_b3 = address.ip[2];
		taddr.sin_addr.S_un.S_un_b.s_b4 = address.ip[3];
		taddr.sin_port = htons(address.port);
		return true;
	}

	class Socket
	{
		int handle = 0;
	public:
		~Socket()
		{
			close();
		}
		bool isOpen() const
		{
			return handle > 0;
		}
		bool open(unsigned short port = 0)
		{
			handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

			if (handle <= 0)
				return false;

			//union { char c[4]; unsigned int io; } IPADDRESS;
			sockaddr_in addr;

			addr.sin_family = AF_INET;
			addr.sin_addr.S_un.S_addr = INADDR_ANY;
			addr.sin_port = htons(port);

			if (bind(handle, (sockaddr*)&addr, sizeof(sockaddr_in)) < 0)
				return false;

			return true;
			//fails if < 0

			//DWORD param = 1;
			//ioctlsocket(handle, FIONBIO, &param);
		}
		bool close()
		{
			if (!isOpen())
				return false;
			closesocket(handle);
			handle = 0;
			return true;
		}
		bool send(const char *data, int dlen, const sockaddr_in &addr)
		{
			//char out[256] = 
			return sendto(handle, data, dlen, 0, (sockaddr*)&addr, sizeof(sockaddr_in)) == dlen;
		}
		bool recv(char *data, int dlen, sockaddr_in &addr)
		{
			int in_length = sizeof(sockaddr_in);
			return recvfrom(handle, data, dlen, 0, (sockaddr*)&addr, &in_length) == dlen;
		}

	};
}