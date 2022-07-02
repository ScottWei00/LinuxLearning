#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define BUF_SIZE 1024
#define OPSZ 4

void error_handling(char* message);
int calculate(int opnum,int oparr[],char val);

int main(int argc, char* argv[])
{
    struct sockaddr_in serv_addr,clnt_addr;
    int serv_sock,clnt_sock;
    char message[BUF_SIZE];
    socklen_t clnt_addr_sz;
    int recv_len,recv_cnt;
    int result,opnd_cnt;
    char opInfo[BUF_SIZE];

    if(argc!=2)
    {
        printf("Usage: %s <IP><port>\n",argv[0]);
        exit(1);
    }

    serv_sock=socket(PF_INET,SOCK_STREAM,0);
    if(serv_sock==-1)
    {
        error_handling("socket() error");
    }

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_addr.sin_port=htons(atoi(argv[1]));

    if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
    {
        error_handling("bind() error");
    }

    if(listen(serv_sock,5)==-1)
    {
        error_handling("listen() error");
    }

    clnt_addr_sz=sizeof(clnt_addr);
    for(int i=0;i<5;++i)
    {
        opnd_cnt=0;
        clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_sz);
        if(clnt_sock==-1)
        {
            error_handling("accpet() error");
        }
        else
	{
	    printf("connected client %d \n", i + 1);
	}
        read(clnt_sock,&opnd_cnt,1);
        recv_len=0;
        while((opnd_cnt*OPSZ+1)>recv_len)
        {
            recv_cnt=read(clnt_sock,&opInfo[recv_len],BUF_SIZE);
            recv_len+=recv_cnt;
        }
        result=calculate(opnd_cnt,(int*)opInfo,opInfo[recv_len-1]);
        printf("Operation result: %d\n",result);
        write(clnt_sock,(char*)&result,sizeof(result));
        close(clnt_sock);
    }
    close(serv_sock);
    return 0;
}

void error_handling(char* message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}

int calculate(int opnum,int oparr[],char symbol)
{
    int result=oparr[0];
    switch (symbol)
    {
    case '+':
        for(int i=1;i<opnum;i++)
            result+=oparr[i];
        break;
    case '-':
        for(int i=1;i<opnum;i++)
            result-=oparr[i];
        break;
    case '*':
    	for(int i=1;i<opnum;i++)
            result*=oparr[i];
        break; 
    default:
        break;
    } 
    return result;
}
