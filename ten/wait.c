#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>


int main(int argc, char* argv[])
{
	pid_t pid; 
	int status;

	pid = fork();//创建第一个子进程
	if (pid == 0)
	{
		return 3;//结束子进程
	}
	else//执行父进程
	{
		printf("Child PID: %d\n", pid);
		pid = fork();//创建第二个子进程
		if (pid == 0)
		{
			exit(7);//结束子进程
		}
		else
		{
			printf("child PID: %d\n", pid);
			wait(&status);//获取第一个结束的子进程信息
			if (WIFEXITED(status))
			{
				printf("child send one: %d \n", WEXITSTATUS(status));
			}

			wait(&status);//获取第二个结束的子进程信息
			if (WIFEXITED(status))
			{
				printf("child send two: %d\n", WEXITSTATUS(status));
			}
			sleep(30);
		}
	}
	return 0;
}