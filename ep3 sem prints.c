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

int Maximo(int D[MAX][MAX], int m, int n );

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
    scanf("%d[^\n]", &op);
    
    while (op != 0){
        if (op == 1)
        {
            printf("Nome do arquivo de imagem: ");
            scanf(" [^\n]");
            char nomearqD[FNMAX];
            
            scanf("%[^\n][^\n]", nomearqD);


            char nomearqT[FNMAX];
            printf("Nome do arquivo de texto: ");
            scanf(" ");
            scanf("%[^\n][^\n]",  nomearqT);


            char nomearqDl[FNMAX];
            printf("Nome do arquivo que armazenara a imagem resultante: ");
            scanf(" ");
            scanf("%[^\n][^\n]", nomearqDl);


            printf("Guardei os nomes\n");

            int m, n, max, k;
            int D[MAX][MAX];
            char T[MAX2];

            k = 0;

            if (!LeDesenho(nomearqD, D, &m, &n, &max) 
            && !LeTexto(nomearqT, T, &k))
            {
                printf("Li desenho\nm = %d\n n = %d\n", m, n);
                printf("Li texto\n");
                int b, d;

                BeDe(k , m, n, &b, &d);
                printf("Achei b e d: b = %d\nd = %d\n", b, d);

                int Dl[MAX][MAX];
                int maxl;

                Codifica(D, m, n, T, k, Dl, b, d, verborragico);
                maxl = Maximo(Dl, m, n);
                printf("Achei o maximo de Dl\n");
                maxl = max + maxl - min(max, maxl); /* ou seja, o maximo para a imagem codificada eh o maximo entre o maximo de D e de Dl*/
                printf("Codifiquei a mensagem\n");
                EscreveDesenho(nomearqDl, Dl, m, n, maxl);
                printf("Escrevi o arquivo\n");
            }
        }

        else if (op == 2)
        {
            printf("Nome do arquivo que contem o desenho original: ");
            scanf(" [^\n]");
            char nomearqD[FNMAX];
            
            scanf("%[^\n][^\n]", nomearqD);


            char nomearqDl[FNMAX];
            printf("Nome do arquivo que contem o desenho esteganografado: ");
            scanf(" ");
            scanf("%[^\n][^\n]",  nomearqDl);


            char nomearqT[FNMAX];
            printf("Nome do arquivo que armazenara a mensagem decodificada: ");
            scanf(" ");
            scanf("%[^\n][^\n]", nomearqT);

            int m=0, n=0, max=0, k=0, b=0, d=0;
            int D[MAX][MAX];
            int Dl[MAX][MAX];
            char T[MAX2];

            if (!LeDesenho(nomearqD, D, &m, &n, &max) && 
            !LeDesenho(nomearqDl, Dl, &m, &n, &max))
            {

                DeBeDe(D, Dl, m, n, &b, &d);
                printf("m = %d\nn = %d\nb = %d\nd = %d\n", m, n, b, d);
                

                k = DeCodifica(D, Dl, m, n, b, d, T, verborragico);
                printf("k = %d\n", k);

                EscreveTexto(nomearqT, T, k);
            }

        }

        else if (op == 3)
        {
            char nomearqT[FNMAX];
            char T[MAX2];
            int k, count = 0;
            printf("Nome do arquivo com a mensagem: ");
            scanf(" ");
            scanf("%[^\n][^\n]", nomearqT);

            if (!LeTexto(nomearqT, T, &k))
            {

                while (count < k)
                {
                    printf("%c", T[count]);
                    count++;
                }

                printf("\n");
            }

            
        }

        else if (op == 4)
        {
            printf("Modo verborragico ativado\n");
            verborragico = !verborragico; /*troca de modo de operação*/ 
        }

        printf("Modos de operacao:\n1.Codificacao\n2.Decodifica\n3.Ler texto\n4.Modo verborragico (toggle)\n");
        scanf("%d", &op);
    }

    return 0;
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

    fscanf(fp, "P2\n%d %d\n%d\n", pn, pm, pmax);

    int i, j;

    for (i = 0; i < *pm; i++)
    {
        for (j = 0; j < *pn; j++)
        {
            fscanf(fp, "%d ", &M[i][j]);
        }
    }
    fclose(fp);

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

    int i = 0;
    char atual = 'a';
    while (fscanf(fp, "%c", &atual) == 1)
    {
        T[i] = atual;
        i++;
    } 
    fclose(fp);
    printf("\n");
    *pk = i;
    printf("k  = %d\n", *pk);
    
    return 0;
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
        printf("Meu chute de b eh %d\n", 1<<exp);
        printf("%d >= %d\n", (m * n - 1) * (1<<exp), 8*k);
        if (((m * n - 1) * (1<<exp)) >= 8 * k)
        {
            *pb = (1<<exp);
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
    return (int) ((x/(1<<b))*(1<<b) + ((x+y)%(1<<b)))%256; /*x>>b<<b eh o maior multiplo de 2^b menor ou igual a x.
    1<<b vale 2^b*/ 
}

int acha_y(int x, int z, int b)
{
    return (int) (z-x+256+256) % (1<<b);
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
    printf("Copiei\n");

    i = d-1;
    j = d-1; /*como estipulado no enunciado*/

    Dl[i][j] = acha_z(D[i][j], b, b); /*coloca o valor de b no primeiro pixel*/
    printf("Codifiquei b\n");
    j+=d;
    
    int K, descarte; 
    K = 0; /*para iterar pelos caracteres de T*/
    descarte = 0; /*numero de bits a serem descartados num caracter de T*/

    for (;i < m; i+= d)
    {
        if (i!=d-1) j = d-1;
        for (; j < n; j+= d)
        {
            int y;
            y = ProximosBBits(T, b, &K, &descarte);
            /*printf("Achei o comeco dos proximos b bits b\n");*/
            if (y < 0 || acha_y(D[i][j], y, b) < 0){
                printf("O erro está no %c, no bit %d\n", T[K], descarte);
                return;
            }
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
            if (D[i][j]> max) max = D[i][j];
        }
    }
    printf("O maximo eh %d\n", max);
    return max;
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

    fprintf(fp, "P2\n%d %d\n%d\n", n, m, max);

    int i, j; /*iteradores*/

    for (i = 0; i < m; i++)
    {
        for (j = 0; j < n; j++)
        {
            fprintf(fp, "%d", M[i][j]);
            /*if (j != n-1)
            {
                fprintf(fp, " ");
            }*/
            fprintf(fp, "\n");
        }
        
    }

    fclose(fp);
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

    int b = 1;

    for (; b <=8; b *= 2)
    {
        if (acha_y(D[d-1][d-1], Dl[d-1][d-1], b) == b) /* resolve a equacao b = (z-x mod 2^b)*/
        {
            *pb = b;
            break;
        }
    }

}

int DeCodifica( int D[MAX][MAX], int Dl[MAX][MAX], int m, int n,
                int b, int d, char T[MAX2], int modo )
{
    int i, j; /*indices para iterar pelas matrizes*/;

    i = d-1;
    j = 2*d-1; /*como estipulado no enunciado*/

    int k, bits_ate_agr; 
    k = 0; /*para iterar pelos caracteres de T*/
    bits_ate_agr = 0; /*numero de bits ja colocados num caracter de T*/
    char c = 0; /*caractere do texto comeca com valor zero em binario*/

    for (;i < m; i+= d)
    {
        if (i!=d-1) j = d-1;
        for (; j < n; j+= d)
        {
            int y;
            y = acha_y(D[i][j], Dl[i][j], b);
            c = (c  | (y << bits_ate_agr)); /*desloca os algarismos (em binario) de c de b unidades para a direita e coloca os b algarismos lidos no espaco gerado*/
            bits_ate_agr += b;
            if (modo) 
            {
                if (0 ) printf("Posicao [%d, %d]; Inteiro codificado: %d; Tom de cinza original: %02x; Tom de cinza codificado: %02x\n", i, j, y, D[i][j], Dl[i][j]);
                 if (0 ) printf("c ateh agr = %c\n\n", c);
            }

            if (bits_ate_agr >= 8)
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

    int l = 0;

    for (; l < k; l++)
    {
        if (T[l] > 0) fprintf(fp, "%c", T[l]);
    }
    fclose(fp);
    return 0;
}