//一个很普通的多线程（互斥量）。

#include<bits/stdc++.h>
using namespace std;
#define TH_SIZE 100
void *thread_inc(void *arg);
void *thread_des(void *arg);
long long sum=0;
pthread_mutex_t mutexx;
int main(int argc,char *argv[]){
pthread_t t_pid[TH_SIZE];
pthread_mutex_init(&mutexx,NULL);
for(int i=0;i<TH_SIZE;i++){
    if(i%2!=0)pthread_create(&t_pid[i],NULL,thread_inc,NULL);
else pthread_create(&t_pid[i],NULL,thread_des,NULL);

}
for(int i=0;i<TH_SIZE;i++)
pthread_join(t_pid[i],NULL);

cout<<"res is:"<<sum<<endl;

return 0;
}

void *thread_inc(void *arg){
    pthread_mutex_lock(&mutexx);
for(int i=0;i<50000000;i++){
    sum++;
}
pthread_mutex_unlock(&mutexx);
return NULL;
}

void *thread_des(void *arg){
   
for(int i=0;i<50000000;i++){ pthread_mutex_lock(&mutexx);
    sum--;
    pthread_mutex_unlock(&mutexx);
}

return NULL;
}
