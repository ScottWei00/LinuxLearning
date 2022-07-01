#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define BUF_SIZE 1024
void error_handling(char* message);

int main(int argc, char* argv[])
{
	int sock;
	struct sockaddr_in serv_sock;
	char message[BUF_SIZE];
	int str_len;

	if (argc != 3)
	{
		printf("Usage:%s <IP><port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
	{
		printf("sock() error!\n");
	}

	memset(&serv_sock, 0, sizeof(serv_sock));
	serv_sock.sin_family = AF_INET;
	serv_sock.sin_addr.s_addr = inet_addr(argv[1]);
	serv_sock.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr*)&serv_sock, sizeof(serv_sock)) == -1)
	{
		error_handling("connect() error");
	}
	else
	{
		printf("connecting......");
	}

	while (1)
	{
		fputs("Input message(q to quit):", stdout);
		fgets(message, BUF_SIZE, stdin);
		if (!strcmp(message, "q\n")||!strcmp(message, "Q\n"))
		{
			break;
		}
		write(sock, message, strlen(message));//写入服务端
		str_len = read(sock, message, BUF_SIZE - 1);//从服务端读取
		message[str_len] = 0;
		printf("Message from server: %s\n", message);
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
