#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>

void error_handling(char* message);

int main(int argc, char* argv[])
{
	int sock;
	struct sockaddr_in serv_sock;
	char message[30];
	int str_len;

	if (argc != 3)
	{
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	//�����׽��֣���ʱ�׽��ֲ������Ϸ�Ϊ����˺Ϳͻ��ˡ���������ŵ��� bind,listen ����������Ϊ�������׽���
	//������� connect ����������Ϊ�ͻ����׽���
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
	{
		printf("socket() error!\n");
	}

	//��ʼ���׽��ֺ�IPV4������ַ
	memset(&serv_sock, 0, sizeof(serv_sock));
	serv_sock.sin_family = AF_INET;
	serv_sock.sin_addr.s_addr = inet_addr(argv[1]);
	serv_sock.sin_port = htons(atoi(argv[2]));

	
	//����connect�����˷�����������
	if (connect(sock,(struct sockaddr*)&serv_sock,sizeof(serv_sock)) == -1)
	{
		error_handling("connect() error!");
	}

	//����read��ȡ���ݴ洢��message��
	str_len = read(sock, message, sizeof(message)-1);
	if (str_len == -1)
	{
		error_handling("read() error!");
	}
	//�������
	printf("message form the server is: %s\n", message);

	close(sock);
	return 0;


}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
