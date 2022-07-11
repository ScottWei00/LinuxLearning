#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<arpa/inet.h>
#include<signal.h>

#define BUF_SIZE 30

//错误的输出处理
void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

//子进程的释放
void read_childproc(int sig)
{
	int status;
	pid_t pid = waitpid(-1, &status, WNOHANG);
	if (WIFEXITED(status))
	{
		printf("Remove proc id: %d\n", pid);
		printf("Child send: %d\n", WEXITSTATUS(status));
	}
}

int main(int argc,char* argv[])
{
	struct sockaddr_in serv_addr, clnt_addr;
	int serv_sock, clnt_sock;
	socklen_t len;
	char message[BUF_SIZE];

	pid_t pid;
	struct sigaction act;
	int str_len, status;

	if (argc != 2)
	{
		printf("Usage :%s <port>\n", argv[0]);
		exit(1);
	}

	//避免僵尸进程
	act.sa_handler = read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	status=sigaction(SIGCHLD, &act, 0);

	//创建套接字
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	if (serv_sock == -1)
	{
		error_handling("socket() error");
	}

	//初始化套接字
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	//绑定Ip地址和端口号
	if (bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
	{
		error_handling("bind() error");
	}

	//监听客户端请求
	if (listen(serv_sock, 5) == -1)
	{
		error_handling("listen() error");
	}

	//处理连接请求
	while (1)
	{
		len = sizeof(clnt_addr);
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &len);//获取一个客户端连接
		if (clnt_sock==-1)
			continue;
		else
			puts("new client connected...");

		//创建并将客户端套接字文件描述符传递给子进程处理
		pid = fork();
		if (pid == -1)
		{
			close(clnt_sock);
			continue;
		}
		else if(pid==0)//子进程
		{
			close(serv_sock);
			while ((str_len = read(clnt_sock, message, BUF_SIZE)) != 0)
				write(clnt_sock, message, str_len);
			close(clnt_sock);
			puts("client disconnected...");
			return 0;
		}
		else
		{
			close(clnt_sock);
		}
	} 
	close(serv_sock);
	return 0;
}
