#include<bits/stdc++.h>
#include<semaphore.h>
using namespace std;


void* read(void*arg);
void* write(void *arg);
static sem_t sem_one;
static sem_t sem_two;
 static int  num;
 int sum;
int main(int argc ,char *argv[]){
pthread_t t_pid1,t_pid2;
sem_init(&sem_one,0,1);
sem_init(&sem_two,0,0);

pthread_create(&t_pid1,NULL,read,NULL);
pthread_create(&t_pid2,NULL,write,NULL);

pthread_join(t_pid1,NULL);
pthread_join(t_pid2,NULL);

sem_destroy(&sem_one);
sem_destroy(&sem_two);

return 0;
}

void *read(void*arg){
for(int i=0;i<5;i++){
    cout<<"input the message"<<endl;
    sem_wait(&sem_one);
    cin>>num;
    sem_post(&sem_two);
}
return NULL;

}

void *write(void *arg){
    for(int i=0;i<5;i++){
        sem_wait(&sem_two);
        sum+=num;
        sem_post(&sem_one);
    }
    cout<<"res is:"<<sum<<endl;
    return NULL;
}
