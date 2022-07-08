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
	act.sa_handler = timeout;//�洢����ָ��
	sigemptyset(&act.sa_mask);//��sa_mask����������λ��ʼ��Ϊ0
	act.sa_flags = 0;//sa_flagsͬ����ʼ��Ϊ0
	sigaction(SIGALRM, &act, 0);//ע��SIGALRM�źŵĴ�����

	alarm(2);//2�����SIGALRM�ź�

	for (int i = 0; i < 3; i++)
	{
		puts("wait...");
		sleep(100);
	}
	return 0;
}
