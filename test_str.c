#include <stdio.h>
#include <stdlib.h>

#define MAX 400
#define MAX2 160000
#define FNMAX 200
#define TRUE 1
#define FALSE 0

int main()
{
    int op=1;
    int verborragico;
    verborragico = FALSE;
    /*printf("Modos de operacao:\n1.Codificacao\n2.Decodifica\n3.Ler texto\n4.Modo verborragico (toggle)\n");
    */scanf("%d", &op);
    
    while (op != 0){
        if (op == 1)
        {
            char nomearqD[FNMAX];
            /* printf("Nome do arquivo de imagem: \n");*/
            scanf("%[^\n]", nomearqD);
            /*printf("\n");*/
            printf("%s\n", nomearqD);
        }
        op--;        

    }
    return 0;
}