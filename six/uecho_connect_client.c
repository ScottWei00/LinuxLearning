#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUF_SIZE 30

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[])
{
	struct sockaddr_in clnt_addr, serv_addr;
	char message[BUF_SIZE];
	socklen_t addr_sz;
	int sock;
	int str_len;


	if (argc != 3)
	{
		printf("Usage:%s <IP><Port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (sock == -1)
	{
		error_handling("socket() error!");
	}

	memset(&clnt_addr, 0, sizeof(clnt_addr));
	clnt_addr.sin_family = AF_INET;
	clnt_addr.sin_addr.s_addr = inet_addr(argv[1]);
	clnt_addr.sin_port = htons(atoi(argv[2]));
	
	connect(sock,(struct sockaddr*)&clnt_addr,sizeof(clnt_addr));

	while (1)
	{
		fputs("Insert messgae(q to quit):", stdout);
		fgets(message, sizeof(message), stdin);
		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
		{
			break;
		}

		//sendto(sock, message, strlen(message), 0, (struct sockaddr*)&clnt_addr, sizeof(clnt_addr));
		write(sock,message,strlen(message));
		//addr_sz = sizeof(serv_addr);
		//str_len = recvfrom(sock, message, BUF_SIZE, 0, (struct sockaddr*)&serv_addr, &addr_sz);
		str_len=read(sock,message,sizeof(message)-1);
		message[str_len] = 0;
		printf("Message from server: %s", message);
	}
	close(sock);
	return 0;
}
