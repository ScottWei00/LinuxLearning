#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define BUF_SIZE 30
int main(int argc, char* argv[])
{
	struct sockaddr_in serv_addr, clnt_addr;
	int serv_sock, clnt_sock;
	socklen_t clnt_sz;
	char buf[BUF_SIZE];
	FILE* fp;
	int read_cnt;

	if (argc != 2)
	{
		printf("Usage:%s <IP><Port>\n", argv[0]);
		exit(1);
	}

	fp = fopen("file_server.c", "rb");//���ļ�
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);//�����׽���
	//��ʼ��
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));
	//���׽���
	bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	listen(serv_sock, 5);//��������

	clnt_sz = sizeof(clnt_addr);
	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_sz);//���տͻ�������

	while (1)
	{
		read_cnt = fread((void*)buf, 1, BUF_SIZE, fp);//��ȡ�ļ�������
		//��ȡ���ݲ�����BUF_SIZE��д���ͻ���
		if (read_cnt < BUF_SIZE)
		{
			write(clnt_sock, buf, read_cnt);
			break;
		}
		write(clnt_sock, buf, BUF_SIZE);
	}

	shutdown(clnt_sock, SHUT_WR);//�ر������
	read(clnt_sock, buf, BUF_SIZE);
	printf("message from client: %s \n", buf);

	fclose(fp);
	close(clnt_sock);
	close(serv_sock);
	return 0;

}
