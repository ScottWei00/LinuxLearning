#include<stdio.h>
#include<stdlib.h>
#include<string.h>//字符串头文件
#include<unistd.h>//文件处理的头文件
#include<arpa/inet.h>//字节序转化的头文件
#include<sys/socket.h>//创建套接字头文件

void error_handling(char *message);

int main(int argc, char* argv[])
{
	int serv_sock;//创建服务端套接字文件描述符
	int clnt_sock;//创建客户端套接字文件描述符

	struct sockaddr_in serv_addr;//服务端IPV4地址结构体变量
	struct sockaddr_in clnt_addr;//客户端。。。
	socklen_t clnt_addr_len;

	char message[] = "hello world!";//服务端存储信息
	
	if (argc != 2)
	{
		printf("Usage :%s <port> \n", argv[0]);
		exit(1);
	}

	//创建IPV4的面向字节的套接字
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
	{
		printf("socket() error!\n");
	}


	memset(&serv_addr, 0, sizeof(serv_addr));//初始化服务端变量信息
	serv_addr.sin_family = AF_INET;//地址族
	serv_addr.sin_addr.s_addr = htonl(INADDY_ANY);//IP地址
	serv_addr.sin_port = htons(atoi(argv[1]));//端口号

	//调用bind分配Ip地址和端口号
	if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
	{
		error_handling("bind() error!");
	}

	//调用listen函数将套接字转换为可监听状态
	if (listen(serv_sock, 5) == -1)
	{
		error_handling("listen() error!");
	}

	clnt_addr_len = sizeof(clnt_addr);
	//调用accept函数受理客户端的连接请求，如果没有连接请求则不会返回，直到有连接请求
	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_len);
	if (clnt_sock == -1)
	{
		error_handling("accept() error!");
	}

	//将message信息写给客户端
	write(clnt_sock, message, sizeof(message));
	close(clnt_sock);
	close(serv_sock);
	return 0;
}


void error_handling(void* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}