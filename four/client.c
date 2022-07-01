#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unsitd.h>
#include<sys/socket.h>
#include<arpa/inet.h>

int main(argc, char* argv[])
{
	int sock;
	sockaddr_in serv_sock;
	char message[30];
	int str_len;

	if (argc != 3)
	{
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	//创建套接字，此时套接字并不马上分为服务端和客户端。如果紧接着调用 bind,listen 函数，将成为服务器套接字
	//如果调用 connect 函数，将成为客户端套接字
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
	{
		print("socket() error!\n");
	}

	//初始化套接字和IPV4变量地址
	memset(&serv_sock, 0, sizeof(serv_sock));
	serv_sock.sin_family = AF_INET;
	serv_sock.sin_addr.s_addr = inet_addr(argv[1]);
	serv_sock.sin_port = htons(atoi(argv[2]));

	
	//调用connect向服务端发送请求连接
	if (connect(sock,(struct sockaddr*)&serv_sock,sizeof(serv_sock)) == -1)
	{
		error_handling("connect() error!");
	}

	//调用read读取数据存储到message中
	str_len = read(sock, message, sizeof(message)-1);
	if (str_len == -1)
	{
		error_handling("read() error!");
	}
	//输出数据
	printf("message form the server is: %s", message);

	close(sock);
	return 0;


}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stedrr);
	exit(1);
}