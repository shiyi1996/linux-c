/*************************************************************************
	> File Name: cli.c
	> Author: 师毅
	> Mail:  shiyi19960604@gmail.com
	> Created Time: 2015年08月06日 星期四 01时10分43秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<pthread.h>

#define IPADDR "192.168.20.145"
#define PORT_NUM 8001

char user_name[20];

typedef struct User
{
        int id;
        char name[20];
        char pass[20];
}user_t;

typedef struct News
{
    int type;
    char buf[200];
	char from_name[20];
	char to_name[20]; 
	user_t user;
}new_t;

void* def(void* conn)
{
	new_t new;
	int conn_fd = *(int*)conn;
	int ret;

	while(1)
	{	
		ret = recv(conn_fd, &new, sizeof(new_t), 0);
		if( ret < 0)
		{
			perror("recv");
			exit(1);
		}
		else if(ret == 0)
		{
			close(conn_fd);
			printf("服务器关闭\n");
			exit(1);
		}
		
		switch(new.type)
		{
			case 3:
				if(strcmp(new.from_name, user_name) == 0)
				{
					printf("！！！ %s 不存在，消息发送失败\n",new.to_name);
				}
				else
				{
					printf("%s : %s\n",new.from_name, new.buf);
				}
				break;
			case 4:
				break;
			case 5:
				break;
			case 6:
				break;
			case 7:
				break;
			default:
				break;
		}
	}
	return NULL;
}

//创建套接字并连接服务端
int main()
{
	int conn_fd;
	new_t new;
    struct sockaddr_in serv_addr;
    pthread_t thread;
    
    memset(&serv_addr,0,sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT_NUM);
    inet_aton(IPADDR,&serv_addr.sin_addr);

    conn_fd = socket(AF_INET,SOCK_STREAM,0);
    if(conn_fd < 0)
    {
		perror("socket");
        return 0;
    }

    if(connect(conn_fd,(struct sockaddr*)&serv_addr,sizeof(struct sockaddr_in)) < 0)
    {
        perror("connect");
        return 0;
    }

	while(1)
	{
		printf("<1>注册 <2>登陆 <0>退出\n");
		scanf("%d",&new.type);
		switch(new.type)
		{
			case 1:
				printf("输入用户名：");
				scanf("%s",new.user.name);
				printf("输入密码：");
				scanf("%s",new.user.pass);
				send(conn_fd, &new, sizeof(new_t), 0);
				recv(conn_fd, &new, sizeof(new_t), 0);
				printf("%s\n",new.buf);
				break;
			case 2:
				printf("输入用户名：");
				scanf("%s",new.user.name);
				strcpy(user_name,new.user.name);
				printf("输入密码：");
				scanf("%s",new.user.pass);
				send(conn_fd, &new, sizeof(new_t), 0);				
				recv(conn_fd, &new, sizeof(new_t), 0);
				printf("%s\n",new.buf);
				break;
			case 0:
				return 0;
			default:
				break;
		}

		if(strcmp(new.buf, "登陆成功") == 0)
		{
			break;
		}
	}	

	pthread_create(&thread, NULL, def, (void*)&conn_fd);

	while(1)
	{
		printf("<3>私聊 <4>群发 <5>查询在线用户 <0>退出\n");
		scanf("%d",&new.type);
		switch(new.type)
		{
			case 0:
				return 0;
			case 3:
				strcpy(new.from_name,user_name);
				printf("输入好友名：");
				scanf("%s",new.to_name);
				printf("输入消息内容：");
				scanf("%s",new.buf);
				send(conn_fd, &new, sizeof(new_t), 0);
			case 4:
				
				break;
			case 5:
				break;
			default:
				break;
		}
	}
	
	return 0;	
}

