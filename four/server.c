#include<stdio.h>
#include<stdlib.h>
#include<string.h>//�ַ���ͷ�ļ�
#include<unistd.h>//�ļ������ͷ�ļ�
#include<arpa/inet.h>//�ֽ���ת����ͷ�ļ�
#include<sys/socket.h>//�����׽���ͷ�ļ�

void error_handling(char *message);

int main(int argc, char* argv[])
{
	int serv_sock;//����������׽����ļ�������
	int clnt_sock;//�����ͻ����׽����ļ�������

	struct sockaddr_in serv_addr;//�����IPV4��ַ�ṹ�����
	struct sockaddr_in clnt_addr;//�ͻ��ˡ�����
	socklen_t clnt_addr_len;

	char message[] = "hello world!";//����˴洢��Ϣ
	
	if (argc != 2)
	{
		printf("Usage :%s <port> \n", argv[0]);
		exit(1);
	}

	//����IPV4�������ֽڵ��׽���
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
	{
		printf("socket() error!\n");
	}


	memset(&serv_addr, 0, sizeof(serv_addr));//��ʼ������˱�����Ϣ
	serv_addr.sin_family = AF_INET;//��ַ��
	serv_addr.sin_addr.s_addr = htonl(INADDY_ANY);//IP��ַ
	serv_addr.sin_port = htons(atoi(argv[1]));//�˿ں�

	//����bind����Ip��ַ�Ͷ˿ں�
	if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
	{
		error_handling("bind() error!");
	}

	//����listen�������׽���ת��Ϊ�ɼ���״̬
	if (listen(serv_sock, 5) == -1)
	{
		error_handling("listen() error!");
	}

	clnt_addr_len = sizeof(clnt_addr);
	//����accept��������ͻ��˵������������û�����������򲻻᷵�أ�ֱ������������
	clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_len);
	if (clnt_sock == -1)
	{
		error_handling("accept() error!");
	}

	//��message��Ϣд���ͻ���
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