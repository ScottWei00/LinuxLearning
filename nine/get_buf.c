#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[])
{
	int sock;
	int recv_buf, send_buf, state;
	socklen_t len;

	sock = socket(PF_INET, SOCK_STREAM, 0);

	len = sizeof(recv_buf);
	state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&recv_buf, &len);//input stream
	if (state)
		error_handling("getsockopt() error");

	len = sizeof(send_buf);
	state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&send_buf, &len);//output stream
	if (state)
		error_handling("getsockopt() error");

	printf("Input buffer size: %d\n", recv_buf);
	printf("Output buffer size: %d\n", send_buf);
	return 0;
}
