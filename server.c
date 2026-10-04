#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main(void){
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd==-1){perror("socket"); return 1;}
	
	struct sockaddr_in addr;
	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(8080);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);	
	int yes=1;
	setsockopt(fd,SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
	if(bind(fd,(struct sockaddr *)&addr, sizeof(addr))==-1){
		perror("bind");
		return 1;
	}
	if(listen(fd,10)==-1){
		perror("listen");
		return 1;
	}
	
	printf("fd %d bound to port 8080\n",fd);
	
	struct sockaddr_in client;
	socklen_t len = sizeof(client);
	while(1){

	
	int cfd = accept(fd,(struct sockaddr *)&client, &len);
	if(cfd == -1){ perror("accept"); return 1; }

	char ip[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &client.sin_addr,ip,sizeof(ip));
	printf("accepted fd %d from %s:%d\n",cfd,ip,ntohs(client.sin_port));

	char buf[1024];
	ssize_t n;
	while((n=recv(cfd,buf,sizeof(buf),0))>0){
		printf("got %zd bytes : %.*s",n,(int)n,buf);
		send(cfd,buf,n,0);

	}
	if(n==0) printf("client closed the connection\n");
	else perror("recv");	
	close(cfd);
	}
	close(fd);
	return 0;

}
