#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/epoll.h>

#define BUF_SIZE 100
#define EPOLL_SIZE 50
void error_handling(char *buf)
{
	fputs(buf, stderr);
	fputc('\n', stderr);
	exit(1);
}
int main(int argc,char *argv[]){
  	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t adr_sz;
	int str_len, i;
	char buf[BUF_SIZE];

	struct epoll_event *ep_events;//结构体缓冲
	struct epoll_event event;//要检测的事件
	int epfd, event_cnt;//空间，发生变化的文件描述符数量

	if(argc!=2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));
	
	if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error");
	if(listen(serv_sock, 5)==-1)
		error_handling("listen() error");

//创建空间，和缓冲
	epfd=epoll_create(EPOLL_SIZE);
	ep_events=malloc(sizeof(struct epoll_event)*EPOLL_SIZE);

//设置变化的信息
	event.events=EPOLLIN;
	event.data.fd=serv_sock;	
	epoll_ctl(epfd, EPOLL_CTL_ADD, serv_sock, &event);


while(1)
	{
		event_cnt=epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);//调用epoll——wait，检测发生目标变化文件描述符的个数
		if(event_cnt==-1)
		{
			puts("epoll_wait() error");
			break;
		}

		for(i=0; i<event_cnt; i++)//循环
		{
			if(ep_events[i].data.fd==serv_sock)//是服务端变化
			{
				adr_sz=sizeof(clnt_adr);
				clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);//连接
				event.events=EPOLLIN;//注册客户端文件描述符
				event.data.fd=clnt_sock;
				epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sock, &event);//注册
				printf("connected client: %d \n", clnt_sock);
			}
			else//不是客户端，就传输数据
			{
					str_len=read(ep_events[i].data.fd, buf, BUF_SIZE);//读
					if(str_len==0)    // 读完了
					{//删除
						epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
						close(ep_events[i].data.fd);
						printf("closed client: %d \n", ep_events[i].data.fd);
					}
					else
					{
						write(ep_events[i].data.fd, buf, str_len);    //写回去
					}
	
			}
		}
	}
close(serv_sock);
close(epfd);

return 0;

}
