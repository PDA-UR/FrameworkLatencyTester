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

using namespace std;

// TODO: handle failure
UDSHandler::UDSHandler(const char* path)
{
    socklen_t addrlen;
    ssize_t size;
    struct sockaddr_un address;
    const int y = 1;
    uds_path = path;

	//int success = server_socket=socket (AF_LOCAL, SOCK_STREAM, 0);
	int success = server_socket=socket (AF_LOCAL, SOCK_DGRAM, 0);

    unlink(path);

    address.sun_family = AF_LOCAL;
    strcpy(address.sun_path, path);

    if (bind ( server_socket,
                (struct sockaddr *) &address,
                sizeof (address)) != 0) {
        //printf( "port is not free!\n");
    cout << "#port is not free" << endl;
    }

    if (chmod(path, 0777) == -1)
    {
	cout << "#failed to chmod" << endl;
    }


	//system("chmod 755 yourExeFile")


    //listen (server_socket, 5);
    //addrlen = sizeof (struct sockaddr_in);
    //while (1) {
    //    client_socket = accept ( server_socket,
    //            (struct sockaddr *) &address,
    //            &addrlen );
    //    if (client_socket > 0)
    //    {
    //        //printf ("client connected\n");
    //        break;
    //    }
    //}

    //cout << "finished init uds on " << path << endl;
	running = 1;
    //pthread_create(&uds_thread, NULL, handle_uds, NULL); 
    uds_thread = thread(&UDSHandler::handle_uds, this);
}

void UDSHandler::handle_uds()
{
	const int message_length = 2;

	//cout << "handle_uds() " << running << " " << server_socket << endl;

    while(running)
    {
	    //cout << "running" << endl;


		//char* message = (char *) malloc(message_length * sizeof(char));
		char message[message_length];
		//int size = recv(client_socket, message, message_length, MSG_WAITALL);
		//int size = recv(server_socket, message, message_length + 1, MSG_WAITALL);
		int size = recv(server_socket, message, message_length, 0);
		//int size = recvfrom(server_socket, message, message_length, 0, nullptr, nullptr);

		//cout << "received message: " << size << " " << message << endl;

		uint64_t current_time = get_micros();

		//if (size == message_length && measure)
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

		// for debugging
		//if(result != 1)
		//{
		//    cout << buffer << endl;
		//}
	}

	//cout << "end handle_uds()" << endl;
}

void UDSHandler::cleanup()
{
	running = 0;
	uds_thread.join();
	//close(client_socket);
	close(server_socket);
        unlink(uds_path);
}
