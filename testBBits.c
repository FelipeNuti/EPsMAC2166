#include <stdio.h>
#include <stdlib.h>

#define MAX 400
#define MAX2 160000
#define FNMAX 200
#define TRUE 1
#define FALSE 0

int main()
{
    char T[] = {'g'};
    int pik = 0, pib = 0, b = 2;
    int mode = 0;
    while (!pik && mode)
    {
        printf("Inicio:\npik = %d\npib = %d\n", pik, pib);
        printf("ProximosBBits = %d\n", ProximosBBits(T, b, &pik, &pib));
        int a = 0;
        printf("Fim:\npik = %d\npib = %d\n\n", pik, pib);
    }

    if (!mode)
    {
        printf("%d\n", acha_z(120, 1, 1));
    }
    return 0;
}

int ProximosBBits(char T[MAX2], int b, int *pik, int *pib)
{
    char c;
    c = T[*pik]; /*copia o *pik-esimo caracter de T para variavel local c*/

    int dois_a_b = 1<<b; /*usa bitshift para rapidamente computar o valor de 2^b*/
    int proximos;

    proximos = (c >> *pib) % dois_a_b; /*descarta os *pib bits menos significativos e obtem os ultimos
    b bits do resultado*/

    *pib += b; /*incrementa *pib de b unidades*/

    *pik += *pib/8; /*se *pib for maior que 8, ja estamos no proximo caracter*/ 
    *pib = *pib % 8; /*agora *pib deve estar entre 0 e 7*/

    return proximos;

}

int acha_z(int x, int y, int b)
{
    return (x/(1<<b))*(1<<b) + ((x+y)%(1<<b)); /*x>>b<<b eh o maior multiplo de 2^b menor ou igual a x.
    1<<b vale 2^b*/ 
}