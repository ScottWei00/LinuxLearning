#include<stdio.h>
#include<stdlib.h> 
#include<unistd.h>
#include<arpa/inet.h>
#include<netdb.h>

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[])
{
	struct hostent* host; 
	
	if (argc != 2)
	{
		printf("Usage :%s <IP>\n", argv[0]);
		exit(1);
	}
	//���ú�����������ת��ΪIp������hostent�ṹ���ַ
	host = gethostbyname(argv[1]);
	//����ٷ�����
	if (!host)
	{
		error_handling("gethostbyname() error");
	}

	printf("official name: %s\n", host->h_name);
	//ȷ����IPV4����IPV6
	for (int i = 0; host->h_aliases[i]; ++i)
	{
		printf("Aliases: %d %s \n", i + 1, host->h_aliases[i]);
	}
	printf("Address type: %s \n", (host->h_addrtype == AF_INET) ? "AF_INET":"AF_INET6");
	//Ip��ַ��Ϣ
	for (int i = 0; host->h_addr_list[i]; ++i)
	{
		printf("IP address: %d %s \n", i + 1, inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
	}
	return 0;

}
