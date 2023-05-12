#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>


void RSA()
{
    int p = 2, q = 19;
    int n = 0;
    int totiente = 0;
    int e = 23, d = 11;
    int cript, decript;
    int message = 97;
    n = p * q;
    totiente = (p - 1) * (q - 1);

    if((e*d)%totiente == 1)
    {   
        printf("\nOs valores são válidos\n");
    }

    printf("Valor int da mensagem: %d\n", message);

    cript = (int)(pow((double)message, (double)e)) % n;

    message = message + cript;

    printf("Criptografia: %d\n", cript);
    printf("Valor int da mensagem: %d\n", message);

    decript = (int)(pow((double)cript, (double)d)) % n;

    message = message - decript;

    printf("Descriptografia: %d\n", decript);

    printf("Valor int da mensagem: %d\n", message);

    return;
}


int main() 
{
    RSA();

    return 0;
}
