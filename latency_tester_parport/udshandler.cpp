#include "main.h"
#include "udshandler.h"

// TODO: not sure if all of them are needed
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

#include <cstdlib>
//#include <thread>
#include <stdio.h>
#include <iostream>
#include <cstdio>

using namespace std;

// TODO: handle failure
UDSHandler::UDSHandler(const char* path)
{
	socklen_t addrlen;
	ssize_t size;
	struct sockaddr_un address;
	const int y = 1;
	uds_path = path;

	int success = server_socket=socket (AF_LOCAL, SOCK_DGRAM, 0);
	fcntl(server_socket, F_SETFL, O_NONBLOCK); // set socket non-blocking to avoid lock during cleanup

	//unlink(path);
	remove(uds_path);

	address.sun_family = AF_LOCAL;
	strcpy(address.sun_path, path);

	if (bind ( server_socket,
				(struct sockaddr *) &address,
				sizeof (address)) != 0) {
		cout << "#port is not free" << endl;
	}

	if (chmod(path, 0777) == -1)
	{
		cout << "#failed to chmod" << endl;
	}

	running = 1;
	uds_thread = thread(&UDSHandler::handle_uds, this);
}

void UDSHandler::handle_uds()
{
	const int message_length = 2;

	while(running)
	{
		char message[message_length];
		int size = recv(server_socket, message, message_length, 0);

		uint64_t current_time = get_micros();

		if (size > 0 && measure)
		{
			if (message[0] == 's') // start rendering in compositor
			{
				compositor_start_time[compositor_start_count] = current_time;
				compositor_start_count++;
			}
			else if (message[0] == 'e') // end rendering in compositor
			{
				compositor_end_time[compositor_end_count] = current_time;
				compositor_end_count++;
			}
		}

	}
}

void UDSHandler::cleanup()
{
	running = 0;
	close(server_socket);
	uds_thread.join();
	remove(uds_path);
	//unlink(uds_path);
}
