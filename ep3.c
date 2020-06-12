#include <stdio.h>
#include <stdlib.h>

#define MAX 400
#define MAX2 160000
#define FNMAX 200
#define TRUE 1
#define FALSE 0

int LeDesenho( char nomearq[FNMAX], int M[MAX][MAX],
            int *pm, int *pn, int *pmax );

int LeTexto( char nomearq[FNMAX], char T[MAX2], int *pk );

int BeDe( int k, int m, int n, int *pb, int *pd );

void Codifica( int D[MAX][MAX], int m, int n, char T[MAX2], int k,
                int Dl[MAX][MAX], int b, int d, int modo );

int Maximo( int D[MAX][MAX], int m, int n );

int EscreveDesenho( char nomearq[FNMAX], int M[MAX][MAX],
int m, int n, int max );

void DeBeDe( int D[MAX][MAX], int Dl[MAX][MAX],
            int m, int n, int *pb, int *pd );

int DeCodifica( int D[MAX][MAX], int Dl[MAX][MAX], int m, int n,
                int b, int d, char T[MAX2], int modo );

int EscreveTexto( char nomearq[FNMAX], char T[MAX2], int k );

int min(int a, int b); /*retorna o minimo entre a e b*/

int cond_d (int d, int m, int n, int b, int k); /*verifica se um valor de d eh valido*/

int acha_z(int x, int y, int b); /*determina o valor z do pixel codificado a partir do valor inicial x e do numero
codificado y*/

int acha_y(int x, int z, int b); /*determina o valor codificado y a partir dos valores z da imagem codificada
e x da imagem original*/

int main()
{
    int op;
    int verborragico;
    verborragico = FALSE;
    printf("Modos de operacao:\n1.Codificacao\n2.Decodifica\n3.Ler texto\n4.Modo verborragico (toggle)\n");
    scanf("%d", &op);
    
    while (op != 0)
}

int LeDesenho( char nomearq[FNMAX], int M[MAX][MAX],
            int *pm, int *pn, int *pmax )
{
    FILE *fp;
    if (!(fp = fopen(nomearq, "r")))
    {
        printf("Erro ao abrir o arquivo %s\n", nomearq);
        return 1;
    }

    fscanf(fp, "P2\n%d %d\n%d", pm, pn, pmax);

    int i, j;

    for (i = 0; i < *pm; i++)
    {
        for (j = 0; j < *pn; j++)
        {
            fscanf(fp, "%d", &M[i][j]);
        }
    }

    return 0;

}

int LeTexto( char nomearq[FNMAX], char T[MAX2], int *pk )
{
    FILE *fp;
    if (!(fp = fopen(nomearq, "r")))
    {
        printf("Erro ao abrir o arquivo %s\n", nomearq);
        return 1;
    }

    int i = -1;
    char atual = '0';

    do
    {
        fscanf(fp, "%c", &atual);
        i++;
        T[i] = atual;

    } while (atual != '\0');
    *pk = i;
}

int min(int a, int b)
{
    if (a < b) return a;
    return b;
}

int cond_d (int d, int m, int n, int b, int k)
{
    return ((m/d)*(n/d) * b >= 8*k + b);
}

int BeDe( int k, int m, int n, int *pb, int *pd )
{
    /*Como escolheremos b divisor de 8, os unicos valores possiveis sao 1, 2, 4, e 8
    Condicao para um b valido: (mn-1)*b >= 8*k
    Basta testar todos os valores possiveis de b, em ordem crescente, e para no 1o que der certo.
    O prof. Yoshiharu autorizou em aula (gravada) o uso de bitshifs*/

    int exp = 0;

    while (exp < 4)
    {
        if ((m * n - 1) * 1<<exp >= 8 * k)
        {
            *pb = 1<<exp;
            break;
        }
        exp++;
    }

    if (exp >= 4)
    {
        return 1;
    }

    /*A condicao para um d valido eh (m/d)*(n/d) * b >= 8*k + b
    Busca binaria para otimizar 1 <= d <= min(m, n)*/

    int alto, baixo, atual;

    baixo = 1;
    alto = min(m, n);
    atual = (alto+baixo)/2;

    if (!cond_d(baixo, m, n, *pb, k)) 
    {
        return 1;
    }

    while (alto - baixo > 1)
    {
        if (cond_d(atual, m, n, *pb, k))
        {
            baixo = atual;
            atual = (alto+baixo)/2;
        }

        else
        {
            alto = atual;
            atual = (alto+baixo)/2;
        }
        
    }
    *pd = baixo;
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
    return x>>b<<b + (x+y)%(1<<b); /*x>>b<<b eh o maior multiplo de 2^b menor ou igual a x.
    1<<b vale 2^b*/ 
}

int acha_y(int x, int z, int b)
{
    return (z-x+256) % (1<<b);
}

void Codifica( int D[MAX][MAX], int m, int n, char T[MAX2], int k,
                int Dl[MAX][MAX], int b, int d, int modo)
{
    int i, j; /*indices para iterar pelas matrizes*/;

    for (i = 0; i < m; i++) /*copia o conteudo de D para Dl*/
    {
        for (j = 0; j < n; j++)
        {
            Dl[i][j] = D[i][j];
        }
    }

    i = d-1;
    j = d-1; /*como estipulado no enunciado*/

    Dl[i][j] = b; /*coloca o valor de b no primeiro pixel; no enunciado ha ambiguidade sobre se eh
    preciso codificar b E d nesse pixel; no inicio diz-se uma coisa, mas mais a frente diz-se outra.
    Como d pode ser muito grande, isso poderia desorganizar a codificacao*/
    
    int K, descarte; 
    K = 0; /*para iterar pelos caracteres de T*/
    descarte = 0; /*numero de bits a serem descartados num caracter de T*/

    for (i = d - 1; i < m; i+= d)
    {
        for (j = 2*d - 1; j < n; j+= d)
        {
            int y;
            y = ProximosBBits(T, b, &K, &descarte);
            Dl[i][j] = acha_z(D[i][j], y, b);
            if (modo) 
            {
                printf("Posicao [%d, %d]; Inteiro codificado: %d; Tom de cinza original: %02x; Tom de cinza codificado: %02x\n", i, j, y, D[i][j], Dl[i][j]);
            }
        }
    }

}

int Maximo( int D[MAX][MAX], int m, int n )
{
    int i, j, max; /*indices para iterar pelas matrizes*/;

    max = 0;

    for (i = 0; i < m; i++) /*achar o maximo*/
    {
        for (j = 0; j < n; j++)
        {
            if (D[i][j]>max) max = D[i][j];
        }
    }
}

int EscreveDesenho( char nomearq[FNMAX], int M[MAX][MAX],
                    int m, int n, int max )
{
    FILE *fp;
    if (!(fp = fopen(nomearq, "w")))
    {
        printf("Erro ao abrir o arquivo %s\n", nomearq);
        return 1;
    }

    fprintf(fp, "P2\n%d %d\n%d\n", m, n, max);

    int i, j; /*iteradores*/

    for (i = 0; i < m; i++)
    {
        for (j = 0; j < n; j++)
        {
            fprintf(fp, "%d", M[i][j]);
            if (j != n-1)
            {
                fprintf(fp, " ");
            }
        }
        fprintf(fp, "\n");
    }

    return 0;
}

void DeBeDe( int D[MAX][MAX], int Dl[MAX][MAX],
            int m, int n, int *pb, int *pd )
{
    int d, i; 
    d = (m+n)/2; /*d eh certamente menor que (m+n)/2*/

    for (i = 0; i < m && i < n; i++)
    {
        if (i < d && Dl[i][i] != D[i][i])
        {
            d = i+1;
        }
    }

    *pd = d;
    *pb = Dl[d-1][d-1];
}

int DeCodifica( int D[MAX][MAX], int Dl[MAX][MAX], int m, int n,
                int b, int d, char T[MAX2], int modo )
{
    int i, j; /*indices para iterar pelas matrizes*/;

    i = d-1;
    j = d-1; /*como estipulado no enunciado*/

    int k, bits_ate_agr; 
    k = 0; /*para iterar pelos caracteres de T*/
    bits_ate_agr = 0; /*numero de bits ja colocados num caracter de T*/
    char c = 0; /*caractere do texto comeca com valor zero em binario*/

    for (i = d - 1; i < m; i+= d)
    {
        for (j = 2*d - 1; j < n; j+= d)
        {
            int y;
            y = acha_y(D[i][j], Dl[i][j], b);
            c = c << b | y; /*desloca os algarismos (em binario) de c de b unidades para a direita e coloca os b algarismos lidos no espaco gerado*/
            bits_ate_agr += b;
            if (modo) 
            {
                printf("Posicao [%d, %d]; Inteiro codificado: %d; Tom de cinza original: %02x; Tom de cinza codificado: %02x\n", i, j, y, D[i][j], Dl[i][j]);
            }

            if (bits_ate_agr == 8)
            {
                bits_ate_agr = 0;
                T[k] = c;
                c = 0;
                k++;
            }
        }
    }
    return k;
}

int EscreveTexto( char nomearq[FNMAX], char T[MAX2], int k )
{
    FILE *fp;
    if (!(fp = fopen(nomearq, "w")))
    {
        printf("Erro ao abrir o arquivo %s\n", nomearq);
        return 1;
    }
    fprintf(fp, "%s", T);
}