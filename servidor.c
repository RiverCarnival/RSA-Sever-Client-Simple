#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <math.h>
#include <unistd.h>
#define PORT 8080


void servidor(int server_fd, int new_socket, int valread, int opt, struct sockaddr_in address, int addrlen, char *message, char *buffer, int d, int n)
{
   
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Falha ao criar socket");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);


    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Falha ao atribuir endereço ao socket");
        exit(EXIT_FAILURE);
    }


    if(listen(server_fd, 3) < 0)
    {
        perror("Falha ao ouvir por conexões");
        exit(EXIT_FAILURE);
    }


    if((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
    {
        perror("Falha ao aceitar conexão");
        exit(EXIT_FAILURE);
    }
    
    send(new_socket, message, strlen(message), 0);


    while(1)
    {
        valread = read(new_socket, buffer, 1024);
        
        for (int i = 0; i < strlen(buffer); i++) 
        {
            int num = (int) buffer[i];
            int decript = (int) (pow((double) num, (double) d)) % n; 
            buffer[i] = num - decript; 
        }

        printf("Usuario: %s\n", buffer);

        if (strcmp(buffer, "exit") == 0)
        {
            exit(EXIT_SUCCESS);
            return;
        }

        send(new_socket, buffer, strlen(buffer), 0);

        memset(buffer, 0, sizeof(buffer));
    }

    return;
}

int main() {
    int server_fd, new_socket, valread, d = 11, n = 33;
    int opt = 1;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *message = "A conexao funcionou <3\n";

    servidor(server_fd, new_socket, valread, opt, address, addrlen, message, buffer, d, n);

    return 0;
}
