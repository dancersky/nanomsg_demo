#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "./nanomsg/nn.h"
#include "./nanomsg/pair.h"
#include "./nanomsg/bus.h"
#include <sys/time.h>
#include <time.h>

char *url ="inproc://send";
int get_timer_now ()
{
    struct timeval now;
    gettimeofday(&now, NULL);
    //return(now.tv_sec * 1000 + now.tv_usec / 1000);
    return(now.tv_sec * 1000000 + now.tv_usec );
}
void *recv_msg(void *arg)
{
   char *msg1 = NULL;
   int sock1 = nn_socket (AF_SP, NN_PAIR);
   if(sock1 < 0) {
       fprintf(stderr, "fail to create socket: %s\n", nn_strerror(errno));
       exit(errno);
   }
   if ( nn_connect(sock1, url) >= 0 )
       printf("connect successful\n");
   char msg2[1024] = {"bboy born and bboy sky"};
   while (1) {
       usleep(10000);
       int result = nn_recv(sock1, &msg1, NN_MSG,NN_DONTWAIT);
       if (result > 0)
       {
           printf ("RECEIVED \"%s\"\n", msg1);
           nn_freemsg (msg1);
       }
       //nn_send(sock1, msg2,strlen(msg2)+1, 0);
       //sleep(3);
   }
}

int main (const int argc, const char **argv)
{


   int sock = nn_socket (AF_SP, NN_PAIR);
   if(sock < 0) {
       fprintf(stderr, "fail to create socket: %s\n", nn_strerror(errno));
       exit(errno);
   }
   if ( nn_bind(sock, url) >= 0 )
       printf("bind successful\n");
   int to = 100;
   if(nn_setsockopt (sock, NN_SOL_SOCKET, NN_SNDTIMEO, &to, sizeof (to)) < 0) {
       fprintf(stderr, "fail to set sorket opts: %sn", nn_strerror(errno));
       exit(errno);
   }
   pthread_t thread;
   // 创建一个线程接收信息
   pthread_create(&thread, NULL, recv_msg, NULL);

   // 主线程发送信息
   char *msg = "hello boy";
   while(1) {
       char *msg1 = NULL;
       if (strcmp(msg, "q")==0)
           break;
       //printf ("SENDING \"%s\"\n", msg);
       size_t sz_n = strlen (msg) + 1; // '\0' too
       nn_send(sock, msg, sz_n, 0);
       //usleep(300);
       /*
       int result = nn_recv(sock, &msg1, NN_MSG,NN_DONTWAIT);
       if (result > 0)
       {
           printf ("master------RECEIVED: \"%s\"\n", msg1);
           nn_freemsg (msg1);
       }*/
       usleep(5000);
   }
   printf("exit\n");
   nn_shutdown(sock, 0);
   return 0;
}
