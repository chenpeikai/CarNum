#include <iostream>
#include <vector>
#include <cstdio>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/video/background_segm.hpp>
using namespace std;
using namespace cv;
#include "video.h"
#include "frontdector.h"
#include "VideoRecover.hpp"
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#define DEFAULT_PORT 8000
#define MAXLINE 4096
//wacth the road
Quadrilateral middle_road;
Quadrilateral left_road;
Quadrilateral right_road;
//transport the data
void *data_transport(void *);
pthread_t tid;
int main(int arg,char *agv[])
{
	VideoProcessor video;
	VideoRecover v;
	BGFGSegmentor segmentor;
	
/*    if(pthread_create(&tid,NULL,data_transport,NULL) != NULL)
    {
        printf("Create thread error!\n");
        exit(1);
    }*/
	/*³õÊ¼»¯ĞéÄâÏßÈ¦*/
    middle_road.create(Point(200,87),Point(261,87),Point(262,136),Point(182,136),"middle");
    left_road.create(Point(137,87),Point(198,87),Point(180,136),Point(110,136),"left");
    right_road.create(Point(267,87),Point(335,87),Point(352,136),Point(268,136),"right");

	video.setInput("new.avi");
	video.setDelay (33);
	video.setFrameProcessor(&segmentor);
 //   video.setOutput("result.avi");
    namedWindow("input");
	namedWindow("output");
    video.displayOutput("output");
    video.displayInput("input");
	video.run();
}

void *data_transport(void * args)
{
    int    socket_fd, connect_fd;
    struct sockaddr_in     servaddr;
    char    buff[4096];
    char c[30];
    int     n;
      //³õÊ¼»¯Socket
    if( (socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1 ){
        printf("create socket error: %s(errno: %d)\n",strerror(errno),errno);
        exit(0);
        }
           //³õÊ¼»¯
     memset(&servaddr, 0, sizeof(servaddr));
     servaddr.sin_family = AF_INET;
     servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//IPµØÖ·ÉèÖÃ³ÉINADDR_ANY,ÈÃÏµÍ³×Ô¶¯»ñÈ¡±¾»úµÄIPµØÖ·¡£
     servaddr.sin_port = htons(DEFAULT_PORT);//ÉèÖÃµÄ¶Ë¿ÚÎªDEFAULT_PORT



    printf("======waiting for client's request======\n");
    //½«±¾µØµØÖ·°ó¶¨µ½Ëù´´½¨µÄÌ×½Ó×ÖÉÏ
    if( bind(socket_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1){
     printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
     exit(0);
     }
    //¿ªÊ¼¼àÌıÊÇ·ñÓĞ¿Í»§¶ËÁ¬½Ó
    if( listen(socket_fd, 10) == -1){
        printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
        exit(0);
        }
    while(1){
       //×èÈûÖ±µ½ÓĞ¿Í»§¶ËÁ¬½Ó£¬²»È»¶àÀË·ÑCPU×ÊÔ´¡£
    if( (connect_fd = accept(socket_fd, (struct sockaddr*)NULL, NULL)) == -1){
        printf("accept socket error: %s(errno: %d)",strerror(errno),errno);
        continue;
      }
       //½ÓÊÜ¿Í»§¶Ë´«¹ıÀ´µÄÊı¾İ
    n = recv(connect_fd, buff, MAXLINE, 0);
    //¿¿¿
    if(!fork()){ /*×Ó½ø³Ì*/
        sprintf(c,"left\t%d\nright\t%d\nmiddle\t%d\n",left_road.getnum(),right_road.getnum(),middle_road.getnum());
        if(send(connect_fd, c, 30,0) == -1)perror("send error");
         close(connect_fd);
         exit(0);
        }
        buff[n] = '\0';
        printf("recv msg from client: %s\n", buff);

        close(connect_fd);
    }
    close(socket_fd);
    pthread_exit(NULL); //ÍË³öÏß³Ì
}
