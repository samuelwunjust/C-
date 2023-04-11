//一个简单回声服务器的服务端


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#define BUF_SIZE 1024
int main(int argc,char *argv[]){
    int serv_sock,clnt_sock;
    char messsage[BUF_SIZE];
int str_len,i;

   struct sockaddr_in serv_addr,clnt_addr;
socklen_t clnt_adr_sz;

if(argc!=2){
    printf("usage erroe :%s",argv[0]);
    exit(1);
}

serv_sock=socket(PF_INET,SOCK_STREAM,0);

if(serv_sock==-1)perror("sock error");
memset(&serv_addr,0,sizeof(serv_addr));
serv_addr.sin_family=AF_INET;
serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
serv_addr.sin_port=htons(atoi(argv[1]));

if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
perror("bind error");

if(listen(serv_sock,5)==-1)perror("listen error");

clnt_adr_sz=sizeof(clnt_addr);

for(int i=0;i<5;i++){
    clnt_sock=accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_adr_sz);
    if(clnt_sock==-1)perror("accept error");
    else printf("thi %dth connected",i+1);
    while((str_len=read(clnt_sock,messsage,BUF_SIZE))!=0)
    write(clnt_sock,messsage,str_len);

    close(clnt_sock);


}
close(serv_sock);

return 0;
}




