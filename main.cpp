/*
 * main.cpp
 *
 *  Created on: Jan 09, 2017
 *     Author: karruzz
 */

#include <atomic>
#include <chrono>
#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <utility>

#include <signal.h>
#include <sys/prctl.h>

#include "sync_queue.h"
#include "axis.h"
#include "processer.h"
#include "writer.h"
#include "socket.h"


void set_thread_name(const char* name)
{
	prctl(PR_SET_NAME, name, 0, 0, 0);
}

void listen_messages(Socket& sock, LockQueue& queue, std::string ip, int port)
{
	set_thread_name("listen");
	Utils::LogInfo() << "listen messages thread started\n";

	try {
		sock.start_listen(ip.c_str(), port, queue);
	} catch (const std::exception& e) {
		Utils::LogError() << "listen thread: " << e.what();
	}
}

static std::atomic_bool stop_process(false);
void process_messages(LockQueue& queue)
{
	set_thread_name("process");
	Utils::LogInfo() << "process messages thread started\n";

	Writer writer;
	writer.open("out.dat");
	Processer processer;
	while(!stop_process.load()) {
		if (queue.empty()){
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			continue;
		}

		Packet packet = queue.pop();
		BinsFrame frame;
		if (!frame.from_bytes(packet.bytes.data(), packet.bytes.size()))
			continue;

		if (processer.process(frame))
			writer.write(processer.get_orientation());
	}
	writer.close();
}

static const int crash_signals[] = {SIGILL, SIGABRT, SIGBUS, SIGFPE, SIGSEGV};

static
void sig_wait()
{
	sigset_t sset;
	sigfillset(&sset);
	for (int i = 0; i < sizeof(crash_signals)/sizeof(crash_signals[0]); ++i)
		sigdelset(&sset, crash_signals[i]);
	pthread_sigmask(SIG_SETMASK, &sset, NULL);

	for (;;) {
		int sig = 0;
		if (sigwait(&sset, &sig) != 0) {
			Utils::LogError() << "ERROR: sigwait error\n";
			continue;
		}
		switch (sig) {
			case SIGINT:
			case SIGTERM:
				Utils::LogInfo() << "SIGTERM signal received, terminating\n";
				return;
			case SIGHUP:
				Utils::LogInfo() << "SIGHUP signal received\n";
				return;
			default:
				Utils::LogWarning() << strsignal(sig) << " signal, ignoring\n";
		}
	}
}

int main(int argc, char *argv[])
{
	std::string ip = argc > 1 ? argv[1] : "192.168.0.100";
	int port  = argc > 2 ? std::stoi(argv[2]) : 12346;

	LockQueue queue;
	Socket sock;
	std::thread process_thr(process_messages,  std::ref(queue));
	std::thread listen_thr(listen_messages, std::ref(sock), std::ref(queue), ip, port);

	sig_wait();
	sock.stop_listen();
	stop_process.store(true);

	listen_thr.join();
	process_thr.join();

	Utils::LogInfo() << "server stopped\n";

	return 0;
}
