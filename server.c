#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<errno.h>
int main(int argc,char ** s)
{
    int socket_server,socket_connect;
    struct sockaddr_in addr_serv;
    char buff[80];
    int n,sum=0;
    FILE *fp;
    extern int errno;

    //create server socket
    socket_server=socket(AF_INET,SOCK_STREAM,0);
    if(socket_server < 0)
    {
        printf("create socket error:%s\n",strerror(errno));
        return -1;
    }
    memset(&addr_serv,0,sizeof(struct sockaddr_in));

    //set the server address
    addr_serv.sin_family=AF_INET;
    addr_serv.sin_addr.s_addr=htonl(INADDR_ANY);
    addr_serv.sin_port=htons(8000);

    //server address bind to the server socket 
    bind(socket_server,(struct sockaddr *)&addr_serv,sizeof(addr_serv));

    //begin listening
    listen(socket_server,10);
    while(1)
    {
        sum=sum+1;
        n = sizeof(addr_serv);
        socket_connect=accept(socket_server,(struct sockaddr *)&addr_serv,&n);
        if(socket_connect <= 0)
        {
            return -1;
        }
        if(fork()==0)
        {
            int ret_size;
	        ret_size = send(socket_connect,"hello,you are connected.......\ndownloading.......\n",80,MSG_NOSIGNAL);
            if(ret_size < 0)
            {
                if(errno == EAGAIN || errno == EWOULDBLOCK)
                {
                    return 0;
                }
            }
	        fp = fopen(s[1],"rb");
            if(NULL == fp)
            {
                return -1;
            }
	        while((n=fread(buff,1,80,fp))!=0)
            {
	            ret_size = send(socket_connect,buff,n,MSG_NOSIGNAL);
                if(ret_size < 0)
                {
                    if(errno == EAGAIN || errno == EWOULDBLOCK)
                    {
                        return 0;
                    }
                    if(errno == EPIPE)
                    {
                        printf("send error:%s\n",strerror(errno));
                    }
                }
	        }
	        recv(socket_connect,buff,80,0);
	        fclose(fp);
	        close(socket_connect);
	        printf("%d downloaded\n",sum);
	        return 0;
        }
    }
    return 0;
}
