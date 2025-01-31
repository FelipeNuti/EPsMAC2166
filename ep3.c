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
    /*Pede entrada conforme o executavel oficial*/
    printf("Digite a operacao desejada:\n   1. Codificar\n   2. Decodificar\n   3. Mostrar mensagem\n"); 
    printf("   4. ");
    if (!verborragico) printf("Ativar ");
    else  printf("Destivar ");
    printf("modo verborragico (toggle)\n   0. Finalinzar\n\nOPCAO: ");
    scanf("%d[^\n]", &op);
    
    while (op != 0){
        if (op == 1)
        {
            /*leitura dos nomes, atentando para trailing characters que podem gerar bugs na leitura*/
            printf("Arquivo que contem o desenho original: ");
            scanf(" [^\n]");
            char nomearqD[FNMAX];
            
            scanf("%[^\n][^\n]", nomearqD);


            char nomearqT[FNMAX];
            printf("Arquivo que contem a mensagem a ser ocultada: ");
            scanf(" ");
            scanf("%[^\n][^\n]",  nomearqT);


            char nomearqDl[FNMAX];
            printf("Gravar desenho esteganografado em: ");
            scanf(" ");
            scanf("%[^\n][^\n]", nomearqDl);

            int m, n, max, k;
            int D[MAX][MAX];
            char T[MAX2];

            k = 0;

            if (!LeDesenho(nomearqD, D, &m, &n, &max) /*leitura dos arquivos e interrupcao da execucao em caso de erro*/
            && !LeTexto(nomearqT, T, &k))
            {
                int b, d;
                printf("m = %d n = %d\n", m, n);
                BeDe(k , m, n, &b, &d);
                
                printf(" b = %d\n d = %d\n", b, d);

                int Dl[MAX][MAX];
                int maxl;

                Codifica(D, m, n, T, k, Dl, b, d, verborragico);
                printf("k = %d\n", k);
                maxl = Maximo(Dl, m, n);
                maxl = max + maxl - min(max, maxl); /* ou seja, o maximo para a imagem codificada eh o maximo entre o maximo de D e de Dl*/
                EscreveDesenho(nomearqDl, Dl, m, n, maxl);
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

            if (!LeDesenho(nomearqD, D, &m, &n, &max))
            {   printf("Arquivo que contem o desenho original: m = %d n = %d\n", m, n); /*impressoes conforme pede o enunciado*/
                int cm = m, cn = n;
                if (!LeDesenho(nomearqDl, Dl, &m, &n, &max))
                {
                    printf("Arquivo que contem o desenho esteganografado: m = %d n = %d\n", m, n); 
                    if (m != cm || n != cn)
                    {
                        printf("Dimensoes dos desenhos sao incompativeis\n");
                    }
                    else
                    {
                        DeBeDe(D, Dl, m, n, &b, &d);
                    
                        k = DeCodifica(D, Dl, m, n, b, d, T, verborragico);
                        printf("k = %d\n", k);

                        EscreveTexto(nomearqT, T, k);
                    }
                }
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
                printf("MENSAGEM contida no arquivo output_dec.txt (tudo entre ---> e <---)\n--->");
                while (count < k)
                {
                    printf("%c", T[count]);
                    count++;
                }

                printf("<---\n");
            }

            
        }

        else if (op == 4)
        {
            printf("Modo verborragico ativado\n");
            verborragico = !verborragico; /*troca de modo de operação*/ 
        }

        printf("Digite a operacao desejada:\n   1. Codificar\n   2. Decodificar\n   3. Mostrar mensagem\n");
    printf("   4. ");
    if (!verborragico) printf("Ativar ");
    else  printf("Destivar ");
    printf("modo verborragico (toggle)\n   0. Finalinzar\n\nOPCAO: ");
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

    fscanf(fp, "P2\n%d %d\n%d\n", pn, pm, pmax); /*leitura do "cabecalho"*/

    int i, j;

    for (i = 0; i < *pm; i++)
    {
        for (j = 0; j < *pn; j++)
        {
            fscanf(fp, "%d ", &M[i][j]); /*leitura de cada pixel da imagem*/
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
    char atual = 'a';  /*inicializacao de "atual" com um caracter qualquer*/
    while (fscanf(fp, "%c", &atual) == 1) /*condicao de parada: ler um caracter; caso contratio, eh porque o arquivo ja acabou*/
    {
        T[i] = atual;
        i++;
    } 
    fclose(fp);
    printf("\n");
    *pk = i;
    
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
    return (int) (z-x+256) % (1<<b);
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

    Dl[i][j] = acha_z(D[i][j], b, b); /*coloca o valor de b no primeiro pixel*/
    j+=d;
    
    int K, descarte; 
    K = 0; /*para iterar pelos caracteres de T*/
    descarte = 0; /*numero de bits a serem descartados num caracter de T*/

    for (;i < m; i+= d)
    {
        if (i!=d-1) j = d-1; /*reinicializar j de forma adequada, ja que o primeiro pixel de ocultacao da 1a linha utilizada contem b*/
        for (; j < n; j+= d)
        {
            int y;
            y = ProximosBBits(T, b, &K, &descarte);
            if (y < 0 || acha_y(D[i][j], y, b) < 0){
                printf("Erro no caracter %c, no bit %d\n", T[K], descarte);
                return;
            }
            Dl[i][j] = acha_z(D[i][j], y, b);
            if (modo) 
            {
                printf(" (%d,%d) bits %d original %02x codificado %02x\n", i, j, y, D[i][j], Dl[i][j]);
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
        if (i < d && Dl[i][i] != D[i][i])   /*vai testando se os desenhos tem valores diferentes na "diagonal principal", e o menor valor eh o valor de d*/
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
                if (1) printf(" (%d,%d) bits %d original %02x codificado %02x\n", i, j, y, D[i][j], Dl[i][j]);
            }

            if (bits_ate_agr == 8)
            {
                bits_ate_agr = 0;
                T[k] = c;
                if (c == '\0') break;
                c = 0;
                k++;
                /*printf("%d\n", k);*/
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