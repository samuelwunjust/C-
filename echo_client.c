//回声服务器对应的客户端
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#define BUF_SIZE 1024

int main(int argc,char *argv[]){
    int sock;
    struct sockaddr_in serv_addr;
    int str_len,recv_len,recv_cnt;
    char message[BUF_SIZE];

    if(argc!=3){
        perror("usage error");
    }
    sock=socket(PF_INET,SOCK_STREAM,0);
    if(sock==-1)perror("socket error");

    memset(&serv_addr,0,sizeof(serv_addr));
serv_addr.sin_family=AF_INET;
serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
serv_addr.sin_port=htons(atoi(argv[2]));

if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)

perror("connect error");
else puts("connected...");
while(1){
    fputs("Input message (Qto quit): .",stdout);
    fgets(message,BUF_SIZE,stdin);
    if(!strcmp(message,"q\n")||!strcmp(message,"Q\n"))break;

   str_len=write(sock,message,strlen(message));
   recv_len=0;
   while(recv_len<str_len){//read the byte one by one
recv_cnt=read(sock,&message[recv_cnt],BUF_SIZE-1);
if(recv_cnt==-1)perror("read error");
recv_len+=recv_cnt;
   }
    message[str_len]=0;
    printf("Message from server:%s",message);
}
close(sock);
return 0;
}


