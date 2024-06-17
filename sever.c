

/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

unsigned int ip2uint(char *ip){
    unsigned int ip3 , ip2 , ip1, ip0;
    sscanf(ip , "%d.%d.%d.%d" , &ip3 , &ip2 , &ip1 , &ip0);
    return (ip3<<24) + (ip2<<16) + (ip1<<8) + ip0;
}


void error(char *msg){
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]){
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;

     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = 1025;
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");
     bzero(buffer,256);
     n = read(newsockfd,buffer,255);
     if (n < 0) error("ERROR reading from socket");
     // ho quindi oraa in buffer il mag mandato da client
unsigned  char primo = buffer[0];
     // so che il primo sara sempre r
    unsigned char len = buffer [1];
    printf("%u e len : %u \n" ,primo , len);
	
	unsigned char *str = calloc(len , sizeof(unsigned char));
	for(int i = 0 ; i < len ; i++){
	str[i]=buffer[i+2];
	}
	printf("Debug : %s\n", str);
	unsigned int ris = ip2uint(str);
	printf("Debug : %u\n", ris);
// ora ho la stringa corretta in unsigned int , la devo passare a client con 6 byte

unsigned char *finale= calloc(6 , sizeof(unsigned char)); // so gia di avere 6 byte
finale[0]='R';
finale[1]=4;
unsigned int res= htonl(ris);
finale[5]=res>>24;
finale[4]= res>> 16;
finale[3]= res >>8;
finale[2]=res;
	for(int k = 0;  k < 6 ; k++){
	printf("%x\n", finale[k]);
		}
	printf("\n");
	 n = write(newsockfd , finale , 6);	

     return 0; 
}
