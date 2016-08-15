#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
void main(int argc,char** s){
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
recv(sock_me,buff,80,0);
printf("%s",buff);
fp=fopen("out","wb");
while((n=recv(sock_me,buff,80,0))!=0){
fwrite(buff,1,n,fp);
if(n<80)break;
}
printf("success\n");
close(sock_me);
fclose(fp);
}
