#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<signal.h>

void read_childproc(int sig)
{
	int status;
	pid_t pid = waitpid(-1, &status, WNOHANG);
	if (WIFEXITED(status))
	{
		printf("Removed proc id: %d \n", pid);
		printf("Child send : %d\n", WEXITSTATUS(status));
	}
}

int main(int argc, char* argv[])
{
	pid_t pid;
	struct sigaction act;
	act.sa_handler = read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGCHLD, &act, 0);

	pid = fork();
	if (pid == 0)
	{
		puts("this is child pro 7");
		sleep(10);
		return 7;
	}
	else
	{
		printf("child proc id: %d\n", pid);
		pid = fork();
		if (pid == 0)
		{
			puts("this is child pro 11");
			sleep(10);
			exit(11);
		}
		else
		{
			printf("child proc id: %d\n", pid);
			for (int i = 0; i < 5; i++)
			{
				puts("wait...");
				sleep(5);
			}
		}
	}
	return 0;
}
