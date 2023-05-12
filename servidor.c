#include <stdio.h> // printf
#include <stdlib.h> // exit 
#include <string.h> // strlen
#include <sys/socket.h> //socket
#include <arpa/inet.h> // inet_pton AF_INET
#include <math.h> // pow
#include <unistd.h>// read
#define PORT 8080

// função principal que cria o servidor
void servidor(int server_fd, int new_socket, int valread, int opt, struct sockaddr_in address, int addrlen, char *message, char *buffer, int d, int n)
{
   // aqui é criado o servidor TCP, se o retorno foi diferente de 0, quer dizer que deu tudo errado na criação do soket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        printf("Falha ao criar socket");
        exit(EXIT_FAILURE);
    }

    // Aqui se o servidor for desligado abrupdamente ele libera o acesso ao endereço e a porta, não deixando ocupado caso feche o server
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
    {
        printf("Desligando o server, fechamento inesperado");
        exit(EXIT_FAILURE);
    }

    //Atribuição do IP e porta e indicação de quais portas ele deve ouvir
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    //atribuir endereço e porta, se for menor que 0 significa que deu tudo errado e o server não foi criado
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        printf("Falha ao atribuir endereço ao socket");
        exit(EXIT_FAILURE);
    }

    // quantas conexões ele pode aceitar, se for menor que 0 algo deu errado
    if(listen(server_fd, 3) < 0)
    {
        printf("Falha ao ouvir por conexões");
        exit(EXIT_FAILURE);
    }

    // aceita novas entradas na rede
    if((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
    {
        printf("Falha ao aceitar conexão");
        exit(EXIT_FAILURE);
    }
    

    // envia uma mensagem para o cliente
    send(new_socket, message, strlen(message), 0);


    //loop de funcionamento do servidor para escutar o que o cleinte envia
    while(1)
    {
        // lê o que chegou no soket
        valread = read(new_socket, buffer, 1024);
        
        // descriptografa a mensagem do cliente usando a chave privada
        for (int i = 0; i < strlen(buffer); i++) 
        {
            int num = (int) buffer[i];
            int decript = (int) (pow((double) num, (double) d)) % n; 
            buffer[i] = num - decript; 
        }

        // exibe o que o cliente enviou 
        printf("Usuario: %s\n", buffer);

        // se for um EXIT, o soket fecha junto com o programa
        if (strcmp(buffer, "exit") == 0)
        {
            exit(EXIT_SUCCESS);
            return;
        }

        //Envia a mesma mensagem que chegou para os clientes
        send(new_socket, buffer, strlen(buffer), 0);

        //limpa o buffer
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


    printf("\nProgramando por um mundo melhor!\n"); //sim é só eu enchendo o saco kkkk
    return 0;
}
