#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>


int main(int argc, char* argv[])
{
	pid_t pid; 
	int status;

	pid = fork();//������һ���ӽ���
	if (pid == 0)
	{
		return 3;//�����ӽ���
	}
	else//ִ�и�����
	{
		printf("Child PID: %d\n", pid);
		pid = fork();//�����ڶ����ӽ���
		if (pid == 0)
		{
			exit(7);//�����ӽ���
		}
		else
		{
			printf("child PID: %d\n", pid);
			wait(&status);//��ȡ��һ���������ӽ�����Ϣ
			if (WIFEXITED(status))
			{
				printf("child send one: %d \n", WEXITSTATUS(status));
			}

			wait(&status);//��ȡ�ڶ����������ӽ�����Ϣ
			if (WIFEXITED(status))
			{
				printf("child send two: %d\n", WEXITSTATUS(status));
			}
			sleep(30);
		}
	}
	return 0;
}