#include <stdio.h> // printf
#include <stdlib.h> // exit 
#include <string.h> // strlen
#include <sys/socket.h> //socket
#include <arpa/inet.h> // inet_pton AF_INET
#include <math.h> // pow
#include <unistd.h>// read
#define PORT 8080


void cliente(int sock, int valread, struct sockaddr_in serv_addr, char *buffer, int e, int n)
{
    
    // cria um soket TCP, devido ao sock_stream
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
        printf("\nFalha ao criar socket\n");
        return;
    }
    // configura o endereço ipv4 e a porta que será ouvida
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // converte de texto o ip para o formato binario C é lindo
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nEndereco invalido\n");
        return;
    }

    //tenta conectar ao servidor, passa o soket o ponteiro da estrutura e o tamanho dela, se for menor 0 deu ruim
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    {
        printf("\nFalha na conexao \n");
        return;
    }
    

    // Loop de envio de mensagens e criptografia
    while(1) 
    {
        printf("Insira a menssagem: ");
        scanf("%s", buffer);
    
        // se a mensagem for exit, fecha o cliente, porém antes manda pro servidor para ele fechar também
     if(strcmp(buffer, "exit") == 0)
    {
        send(sock, buffer, strlen(buffer), 0);
        exit(EXIT_SUCCESS);
        return;
    }

        // o laço da criptografia com a chave publica
        for (int i = 0; i < strlen(buffer); i++) 
        {
            int num = (int)buffer[i]; 
            int cript = (int)(pow((double) num, (double) e)) % n; 
            buffer[i] = num + cript; 
        }


        //envia a mensagem
        send(sock, buffer, strlen(buffer), 0);
        printf("mensagem enviada\n");

   
        //apaga o buffer, que é a variavel da mensagem
        memset(buffer, 0, sizeof(buffer));
    }

    return;
}

int main() 
{
    int sock = 0, valread, e = 23, n = 33;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    cliente(sock, valread, serv_addr, buffer, e, n);

    printf("\nProgramando por um mundo melhor!\n"); //sim é só eu enchendo o saco kkkk
    return 0;
}
