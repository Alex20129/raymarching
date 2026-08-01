#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <zmq.hpp>
#include <string>
#include "scene.hpp"

Scene *gScene;

int main(int argc, char *argv[])
{
	static const int kNumberOfThreads = 2;
	zmq::context_t context (kNumberOfThreads);
	zmq::socket_t socket (context, zmq::socket_type::rep);
	socket.bind ("tcp://*:5555");

	while (true)
	{
		zmq::message_t request;

		//  Wait for next request from client
		zmq::recv_result_t result = socket.recv (request, zmq::recv_flags::none);
		if(!result)
		{
			continue;
		}
		std::cout << "Received "<< request.to_string() << std::endl;

		//  Pretend to do some 'work'
		sleep(1);

		//  Send reply back to client
		std::string kReplyString("ok.");
		zmq::message_t reply (kReplyString.length());
		memcpy (reply.data (), kReplyString.data(), kReplyString.length());
		socket.send (reply, zmq::send_flags::none);
	}
	return 0;
}
