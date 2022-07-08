#include<stdio.h>
#include<unistd.h>
#include<signal.h>

void timeout(int sig)
{
	if (sig == SIGALRM)
	{
		puts("Time out!");
	}
	alarm(2);
}

int main(int argc, char* argv[])
{
	int i;
	struct sigaction act;
	act.sa_handler = timeout;//存储函数指针
	sigemptyset(&act.sa_mask);//将sa_mask函数的所有位初始化为0
	act.sa_flags = 0;//sa_flags同样初始化为0
	sigaction(SIGALRM, &act, 0);//注册SIGALRM信号的处理器

	alarm(2);//2秒后发生SIGALRM信号

	for (int i = 0; i < 3; i++)
	{
		puts("wait...");
		sleep(100);
	}
	return 0;
}
