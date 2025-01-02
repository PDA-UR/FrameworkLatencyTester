#ifndef UDS_H
#define UDS_H

#include <vector>
#include <thread>
#include <functional>

class UDSHandler {
	private:
		const int BUFFER_SIZE 1024
		int fd = -1;
		char* uds_path;
		thread uds_thread; 
		int server_socket, client_socket;

		bool running;
        vector<function<void()>> callbacks;
        void notify_callbacks();

	public:
		UDSHandler(const char* path);
		void handle_uds(void *args);
        void register_callback(function<void()>f);
		void cleanup();
}

#endif
