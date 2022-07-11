#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define BUF_SIZE 1024

void error_handling(char* message);
void read_routine(int sock, char* buf);
void write_routine(int sock, char* buf);

int main(int argc, char* argv[])
{
	struct sockaddr_in addr;
	int sock; 
	char message[BUF_SIZE]; 


	if (argc != 3)
	{
		printf("Usage: %s <IP><Port>\n", argv[2]);
		exit(0);
	}

	//创建套接字
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error");

	//初始化套接字
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(argv[1]);
	addr.sin_port = htons(atoi(argv[2]));

	//向服务端发起连接请求
	if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1)
	{
		error_handling("bind() error");
	}

	//和服务端连接成功后，子进程进行写，父进程进行读取
	pid_t pid = fork();
	if (pid == 0)
		write_routine(sock, message);
	else
	{
		read_routine(sock, message);
	}
	close(sock);
	return 0;
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void read_routine(int sock, char* message)
{
	while (1)
	{
		int str_len = read(sock, message, BUF_SIZE);
		if (str_len == 0)
			return;
		message[str_len] = 0;
		printf("Message from server: %s", message);
	}
}

void write_routine(int sock, char* message)
{
	while (1)
	{
		fgets(message, BUF_SIZE, stdin);
		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
		{
			shutdown(sock, SHUT_WR);//向服务器端传递EOF，因为fork函数复制了文件套接字描述符，所以通过１次调用close不够
			return;
		}
		write(sock, message, strlen(message));
	}
}
