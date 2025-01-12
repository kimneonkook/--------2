#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX 10
#define PORT 4444

void create_matches();
void Client_Data(int client_socket);

typedef struct {
    int match_id;
    char team1[MAX];
    char team2[MAX];
    int available_tickets;
    float price;
}football_match;

football_match matches[MAX];
//int pipes[MAX][2];

int main() {
	
	printf("welcome to football tickets server.\n");
	
	create_matches();
	
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

//create socket file descriptor
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY; //gia na sundethw se opoia ip thelw

//anathesh tou socket sthn dieu8unsh
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error binding");
        close(server_socket);
        exit(EXIT_FAILURE);
    }
//akroash gia sundeseis twn clients
    if (listen(server_socket, 10) == -1) {
        perror("Error listening");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port 4444...\n");

    while (1) {
        client_socket=accept(server_socket, (struct sockaddr)&client_addr, &client_len);
        if (client_socket == -1) {
            perror("Error accepting connection");
            continue;
        }

        pid_t child_pid = fork();
        if (child_pid == 0) {
            // This is the child process
            close(server_socket);

           Client_Data(client_socket);

            close(client_socket);
            exit(0);
        } else if (child_pid < 0) {
            perror("Error forking");
            close(client_socket);
            continue;
        }

        // This is the parent process
        close(client_socket);
        //perimenw to child process
        while (waitpid(-1, NULL, WNOHANG) > 0);

        for (int i = 0; i < MAX; i++) 
        {
        int updated_tickets;
        read(pipes[i][0], &updated_tickets, sizeof(int));
        matches[i].available_tickets = updated_tickets;
    }

    return 0;
}

void create_matches()
{
 for(int i=0;i<MAX;i++){
	 	matches[i].match_id=i;
	 	matches[i].team1="team1";
	 	matches[i].team2="team2";
	 	matches[i].available_tickets=rand() % 5+1;
	 	matches[i].price=(float)(rand() % 100 + 1);
	 
  printf("Football match:%d , %s vs %s ,with id:%d  available tickes:%d and price: %2f",i,matches[i].team1,matches[i].team2,matches[i].match_id,matches[i].available_tickets,matches[i].price);
}
}

void Client_Data(int client_socket){
	char buffer[100];
	int match_id,num_tickets;
	
	while(1){
		//diavazw ton client
		read(client_socket,buffer,sizeof(100));
		sscanf(buffer,"%d %d",&match_id,&num_tickets);
		
		if(match_id<0 || match_id>=MAX){
			write(client_socket,"Invalid match ID",strlen("Invalid match ID"));
			continue;
		}
		if (num_tickets <= 0 || num_tickets > matches[match_id].available_tickets) {
            write(client_socket, "Invalid number of tickets or not enough available", strlen("Invalid number of tickets or not enough available"));
            continue;
        }
        
        float total_cost = num_tickets * matches[match_id].price;
        matches[match_id].available_tickets -= num_tickets;

        char response[100];
        sprintf(response, "Successfully purchased %d tickets for %.2f", num_tickets, total_cost);
        write(client_socket, response, strlen(response));
	}	
}