#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
void main(int argc,char** s)
{
    int sock_me,n;
    FILE *fp;
    struct sockaddr_in addr_serv;
    char buff[80];
    sock_me=socket(AF_INET,SOCK_STREAM,0);
    memset(&addr_serv,0,sizeof(addr_serv));
    addr_serv.sin_family=AF_INET;
    addr_serv.sin_port=htons(8000);
    inet_pton(AF_INET,s[1],&addr_serv.sin_addr);
    connect(sock_me,(struct sockaddr *)&addr_serv,sizeof(addr_serv));
    {   //tcp连接服务器成功后，获取内核赋予socket的本地ip
        char local_ip[20];
        socklen_t serv_len = sizeof(addr_serv);
        getsockname(sock_me,(struct sockaddr*)&addr_serv,&serv_len);
        inet_ntop(AF_INET,&addr_serv.sin_addr,local_ip,sizeof(local_ip));
        printf("local_ip:%s\tlocal_port:%d\n",local_ip,ntohs(addr_serv.sin_port));

        getpeername(sock_me,(struct sockaddr*)&addr_serv,&serv_len);
        inet_ntop(AF_INET,&addr_serv.sin_addr,local_ip,sizeof(local_ip));
        printf("peer_ip:%s\tpeer_port:%d\n",local_ip,ntohs(addr_serv.sin_port));
    }
    recv(sock_me,buff,80,0);
    printf("%s",buff);
    fp=fopen("out","wb");
    while((n=recv(sock_me,buff,80,0))!=0)
    {
        fwrite(buff,1,n,fp);
        //printf("%d\n",n);
        if(n<80)break;
    }
    send(sock_me,buff,80,0);
    if(n==0)printf("7\n");
    printf("success\n");
    close(sock_me);
    fclose(fp);
}
