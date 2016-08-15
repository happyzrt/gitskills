#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
void main(int argc,char ** s){
int socket_server,socket_connect;
struct sockaddr_in addr_serv;
char buff[80];
int n,sum=0;
FILE *fp;
socket_server=socket(AF_INET,SOCK_STREAM,0);
memset(&addr_serv,0,sizeof(struct sockaddr_in));
addr_serv.sin_family=AF_INET;
addr_serv.sin_addr.s_addr=htonl(INADDR_ANY);
addr_serv.sin_port=htons(8000);
bind(socket_server,(struct sockaddr *)&addr_serv,sizeof(addr_serv));
listen(socket_server,10);
while(1){
socket_connect=accept(socket_server,(struct sockaddr *)NULL,NULL);
if(fork()==0){
send(socket_connect,"hello,you are connected.......\ndownload.......\n",80,0);
fp=fopen(s[1],"rb");
while((n=fread(buff,1,80,fp))!=0)send(socket_connect,buff,n,0);
fclose(fp);
close(socket_connect);
sum++;
system("clear");
printf("%d download\n",sum);
}
}
}
