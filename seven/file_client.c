#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define BUF_SIZE 30

int main(int argc, char* argv[])
{
	struct sockaddr_in serv_addr;
	int serv_sock;
	char buf[BUF_SIZE];
	FILE* fp;
	int read_cnt;

	if (argc != 3)
	{
		printf("Usage:%s <IP><Port>\n", argv[0]);
		exit(1);
	}

	fp=fopen("receive.c", "wb");
	serv_sock= socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));

	connect(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	//从服务器读取数据然后写入文件里
	while ((read_cnt = read(serv_sock, buf, BUF_SIZE)) != 0)
	{
		fwrite((void*)buf, 1, read_cnt, fp);
	}
	puts("receive file data");
	write(serv_sock, "Thank you", 10);//字符串数据传输给服务器
	fclose(fp);
	close(serv_sock);
	return 0;
}
