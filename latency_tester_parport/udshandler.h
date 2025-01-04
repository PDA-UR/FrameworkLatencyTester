#ifndef UDS_H
#define UDS_H

#include <thread>

#define UDS_PATH "/tmp/latency_tester_socket"

class UDSHandler {
	private:
		int fd = -1;
		char* uds_path;
		int server_socket, client_socket;

		bool running;
		thread uds_thread; 

	public:
        uint64_t compositor_start_time[100000];
        uint64_t compositor_start_count;
        uint64_t compositor_end_time[100000];
        uint64_t compositor_end_count;

		bool measure;

		UDSHandler(const char* path);
		void handle_uds();
		void cleanup();
};

#endif
