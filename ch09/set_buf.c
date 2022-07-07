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
	int rev_buf = 3 * 1024, snd_buf = 1024;
	socklen_t len;
	int state;

	sock = socket(PF_INET, SOCK_STREAM, 0);
	state = setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rev_buf, sizeof(rev_buf));//设置输入流缓存
	if (state)
		error_handling("setsockopt() error");
	state = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, sizeof(snd_buf));//设置输出流缓存
	if (state)
		error_handling("setsockopt() error");

	len = sizeof(rev_buf);
	state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rev_buf, &len);//获取输入流缓存
	if (state)
		error_handling("setsockopt() error");
	len = sizeof(snd_buf);
	state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, &len);//获取输出流缓存
	if (state)
		error_handling("setsockopt() error");

	printf("Input buffer size: %d\n", rev_buf);
	printf("Output buffer size: %d\n", snd_buf);
	return 0;
}
