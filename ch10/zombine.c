#include<stdio.h>
#include<unistd.h>

int main(int argc, char* argv[])
{
	pid_t pid;
	pid = fork();

	if (pid == 0)
		printf("Hi, I'm a child process\n");
	else
	{
		printf("Child process ID: %d\n", pid);
		sleep(30);//µÈ´ý30s
	}

	if (pid == 0)
		printf("End child process\n");
	else
		printf("End parent process\n");
	return 0;
}