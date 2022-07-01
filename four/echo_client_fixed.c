#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define BUF_SIZE 1024

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[])
{
	struct sockaddr_in addr;
	int sock;
	char message[BUF_SIZE];
	int str_len, recv_len, recv_clnt;

	if (argc != 3)
	{
		printf("Usage:%s <IP><port>\n", argv[0]);
		exit(0);
	}
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
	{
		error_handling("socket() error!");
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(argv[1]);
	addr.sin_port = htons(atoi(argv[2]));

	if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1)
	{
		error_handling("connect() error");
	}
	else
	{
		puts("connecting......\n");
	}

	while (1)
	{
		fputs("input message(Q to quit): ", stdout);
		fgets(message, BUF_SIZE, stdin);
		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;
		str_len = write(sock, message, strlen(message));
		recv_len = 0;
		while (recv_len < str_len)
		{
			recv_clnt = read(sock, &message[recv_len], BUF_SIZE - 1);
			if (recv_clnt == -1)
			{
				error_handling("read() error");
			}
			recv_len += recv_clnt;
		}
		message[recv_len] = 0;
		printf("Message from server: %s\n", message);
	}
	close(sock);
	return 0;



}
