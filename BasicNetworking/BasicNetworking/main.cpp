#pragma comment (lib,"wininet")
#pragma comment (lib,"wsock32.lib")
#pragma comment (lib,"ws2_32.lib")

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>


int main()
{
	WSADATA data;
	if (WSAStartup(MAKEWORD(2, 2), &data) != NO_ERROR)
		exit(0);

	int handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (handle <= 0)
		exit(0);

	union { char c[4]; unsigned int io; } IPADDRESS;
	sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = INADDR_ANY;
	addr.sin_port = htons(1337);

	bind(handle, (sockaddr*)&addr, sizeof(sockaddr_in));
	//fails if < 0
	
	//DWORD param = 1;
	//ioctlsocket(handle, FIONBIO, &param);

	sockaddr_in target_address, bucket_address;
	int in_length;
	const int buffer_length = 40;
	char out_buffer[buffer_length] = "how about i slap your shit";
	char in_buffer[buffer_length];

	target_address.sin_family = AF_INET;
	target_address.sin_addr.S_un.S_un_b.s_b1 = 127;	//10.15.22.88
	target_address.sin_addr.S_un.S_un_b.s_b2 = 0;
	target_address.sin_addr.S_un.S_un_b.s_b3 = 0;
	target_address.sin_addr.S_un.S_un_b.s_b4 = 1;
	target_address.sin_port = htons(1337);

	int out_bytes = sendto(handle, out_buffer, buffer_length, 0, (sockaddr*)&target_address, sizeof(sockaddr_in));

	int in_bytes = recvfrom(handle, in_buffer, buffer_length, 0, (sockaddr*)&bucket_address, &in_length);

	std::cout << in_buffer << std::endl;

	std::cin >> out_buffer;

	closesocket(handle);
	WSACleanup();


	return 0;
}