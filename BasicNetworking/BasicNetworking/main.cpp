#pragma comment (lib,"wininet")
#pragma comment (lib,"wsock32.lib")
#pragma comment (lib,"ws2_32.lib")

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include "netbroke.h"

int main()
{
	if (!nsfw::initNetworking())
		return 0;

	nsfw::Socket mysocket;
	if (mysocket.open())
		printf_s("socket opened\n");
	else
		printf_s("socket failed to open\n");

	sockaddr_in target_address, bucket_address;
	nsfw::stoa("10.15.22.54:50000", target_address);	//10.15.22.54:50000		127.0.0.1:1337
	const int buffer_length = 256;
	char out_buffer[buffer_length] = "how about i slap your shit";
	char in_buffer[buffer_length];



	//int out_bytes = sendto(handle, out_buffer, buffer_length, 0, (sockaddr*)&target_address, sizeof(sockaddr_in));

	//int in_bytes = recvfrom(handle, in_buffer, buffer_length, 0, (sockaddr*)&bucket_address, &in_length);

	//std::cout << in_buffer << std::endl;
	while (true)
	{
		std::cin.getline(out_buffer, buffer_length);

		//char input_address[22];
		//std::cin.getline(input_address, buffer_length);
		//nsfw::stoa(input_address, target_address);
		//mysocket.send(out_buffer, buffer_length, target_address);
		if (mysocket.send(out_buffer, buffer_length, target_address))
			printf_s("packet sent\n");
		else
			printf_s("packet failed to send\n");
		mysocket.recv(in_buffer, buffer_length, bucket_address);

		std::cout << in_buffer << std::endl;

	}


	nsfw::termNetworking();
	return 0;
}