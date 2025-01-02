#include "uds.h"

// TODO: not sure if all of them are needed
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

// TODO: handle failure
UDSHandler::UDSHandler(const char* path)
{
    socklen_t addrlen;
    ssize_t size;
    struct sockaddr_un address;
    const int y = 1;

	int success = server_socket=socket (AF_LOCAL, SOCK_STREAM, 0);

    unlink(path);

    address.sun_family = AF_LOCAL;
    strcpy(address.sun_path, path);

    if (bind ( server_socket,
                (struct sockaddr *) &address,
                sizeof (address)) != 0) {
        //printf( "port is not free!\n");
    }
    listen (server_socket, 5);
    addrlen = sizeof (struct sockaddr_in);
    while (1) {
        client_socket = accept ( server_socket,
                (struct sockaddr *) &address,
                &addrlen );
        if (client_socket > 0)
        {
            //printf ("client connected\n");
            break;
        }
    }

	running = 1;
    //pthread_create(&uds_thread, NULL, handle_uds, NULL); 
    uds_thread = thread(&UDSHandler::handle_uds, this);
}

void UDSHandler::handle_uds(void *args)
{
    while(runnning)
    {

		char* uds_signal = (char *) malloc(2 * sizeof(char));
		int size = recv(client_socket, uds_signal, 2, MSG_WAITALL);

		if (size == 2) notify_callbacks();

		// hand over to message parser
		//int result = parseMessage(buffer);

		// for debugging
		//if(result != 1)
		//{
		//    cout << buffer << endl;
		//}
	}

}

void UDSHandler::register_callback(function<void()>f)
{
    callbacks.push_back(f);
}

void UDSHandler::notify_callbacks()
{
	int i = 0;
    for (auto& f : callbacks)
    {
        f();
    }
}

void UDSHandler::cleanup()
{
	running = 0;
	uds_thread.join();
	close(client_socket);
	close(server_socket);
}
