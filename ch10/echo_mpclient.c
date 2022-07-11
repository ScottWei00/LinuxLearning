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

	//�����׽���
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error");

	//��ʼ���׽���
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(argv[1]);
	addr.sin_port = htons(atoi(argv[2]));

	//�����˷�����������
	if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1)
	{
		error_handling("bind() error");
	}

	//�ͷ�������ӳɹ����ӽ��̽���д�������̽��ж�ȡ
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
			shutdown(sock, SHUT_WR);//��������˴���EOF����Ϊfork�����������ļ��׽���������������ͨ�����ε���close����
			return;
		}
		write(sock, message, strlen(message));
	}
}
