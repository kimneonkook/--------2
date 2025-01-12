#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 4444

int main()
{
    int clientSocket, ret;
    struct sockaddr_in serverAddr;
    char buffer[1024];

    clientSocket=socket(AF_INET,SOCK_STREAM,0);
    if(clientSocket<0)
    {
        printf("ERROR IN CONNECTION\n");
        exit(EXIT_FAILURE);
    }
    memset(&serverAddr, '\0,sizeof(serverAddr));
    serverAddr.sin_family =AF_INET;
    serverAddr.sin_port=htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY; //mporw na sundethw se opoiadhpote dieu8unsh thelw


    ret=connect(clientSocket,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
    if(ret < 0){
        printf("[-]Error in connection.\n");
        exit(1);
    }
//rand() opws kai ston server
    int match_id = rand() % 10;
    int num_tickets = rand() % 3 + 1;

     sprintf(buffer, "%d %d", match_id, num_tickets);
     write(clientSocket, buffer, strlen(buffer), 0);
     int response;
     response = read(clientSocket,buffer,1024);
     printf("%s",buffer);


close(clientSocket);
    return 0;
}