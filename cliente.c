#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <math.h>
#define PORT 8080


void cliente(int sock, int valread, struct sockaddr_in serv_addr, char *buffer, char *nickname, int cript, int e, int n)
{
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
        printf("\n Falha ao criar socket \n");
        return;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nEndereço inválido/ Não suportado \n");
        return;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    {
        printf("\nFalha na conexão \n");
        return;
    }
    
    //send(sock, nickname, strlen(nickname), 0);

    while(1) 
    {
        printf("Insira a menssagem: ");
        scanf("%s", buffer);
        
     if(strcmp(buffer, "exit") == 0)
    {
        send(sock, buffer, strlen(buffer), 0);
        exit(EXIT_SUCCESS);
        return;
    }


        for (int i = 0; i < strlen(buffer); i++) 
        {
            int num = (int)buffer[i]; 
            int cript = (int)(pow((double) num, (double) e)) % n; 
            buffer[i] = num + cript; 
        }

        send(sock, buffer, strlen(buffer), 0);
        printf("mensagem enviada\n");

   
        
        memset(buffer, 0, sizeof(buffer));
    }


    return;
}

int main() 
{
    int sock = 0, valread, cript, e = 23, n = 33;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    char *nickname = "River";

    cliente(sock, valread, serv_addr, buffer, nickname, cript, e, n);


    return 0;
}
