#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define BUF_SIZE 1024
#define OPSZ 4

void error_handling(char* message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}

int main(int argc,char* argv[])
{
    struct sockaddr_in clnt_addr;
    int clnt_sock;
    char opmsg[BUF_SIZE];
    socklen_t clnt_sz;
    int result,opnd_cnt;

    if(argc!=3)
    {
        printf("Usage: %s <IP><Port>\n",argv[0]);
        exit(1);
    }

    clnt_sock=socket(PF_INET,SOCK_STREAM,0);
    if(clnt_sock==-1)
    {
        error_handling("socket() error");
    }

    memset(&clnt_addr,0,sizeof(clnt_addr));
    clnt_addr.sin_family=AF_INET;
    clnt_addr.sin_addr.s_addr=inet_addr(argv[1]);
    clnt_addr.sin_port=htons(atoi(argv[2]));

    if(connect(clnt_sock,(struct sockaddr*)&clnt_addr,sizeof(clnt_addr))==-1)
    {
        error_handling("connect() error");
    }
    else
    {
        puts("connected......\n");
    }

    fputs("operand count: ",stdout);
    scanf("%d",&opnd_cnt);
    opmsg[0]=(char)opnd_cnt;
    for(int i=0;i<opnd_cnt;++i)
    {
        printf("Operand %d:",i+1);
        scanf("%d",(int*)&opmsg[i*OPSZ+1]);
    }
    fgetc(stdin);
    fputs("Operator: ",stdout);
    scanf("%c",&opmsg[opnd_cnt*OPSZ+1]);
    write(clnt_sock,opmsg,opnd_cnt*OPSZ+2);//tranfer to server
    read(clnt_sock,&result,OPSZ);//read the result from the server


    printf("Operation result: %d\n",result);//output the result
    close(clnt_sock);
    return 0;
}
