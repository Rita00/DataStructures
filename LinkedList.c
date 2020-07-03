#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define DIM 200

//int num_palavras = 0; //Descomentar para obter numero de palavras distintas
//int num_total_palavas = 0; //Descomentar para obter numero total de palavras

typedef struct _num_linha *num_linha;
struct _num_linha {
    int num;
    num_linha next;
};

typedef struct _palavra *palavra;
struct _palavra {
    num_linha lista_linhas;
    char word[DIM];
    palavra next;
};

num_linha insere_numero(int num, num_linha lista_linhas);

palavra insere_palavra(int num, char *word, palavra lista);

palavra pesquisa_palavra(char *word, palavra lista);

void print_lista_palavras(palavra lista);

void print_lista_linhas(num_linha lista);

palavra guarda_linha_palavras(palavra lista);

num_linha insere_numero(int num, num_linha lista_linhas) {
    num_linha aux = lista_linhas;
    num_linha no = calloc(1, sizeof(struct _num_linha));
    no->num = num;
    if (lista_linhas == NULL) {
        return no;
    }
    if (aux->num == num) return lista_linhas;
    while (aux->next != NULL && aux->next->num != num) {
        aux = aux->next;
    }
    if (aux->num == num) {
        return lista_linhas;
    } else {
        aux->next = no;
    }
    return lista_linhas;
}

palavra insere_palavra(int num, char *word, palavra lista) {
    palavra aux;
    palavra no;
    if (lista == NULL) {
        no = calloc(1, sizeof(struct _palavra));
        strcpy(no->word, word);
        no->lista_linhas = insere_numero(num, NULL);
        //num_palavras++; //Descomentar para obter numero de palavras distintas
        //num_total_palavas++; //Descomentar para obter numero total de palavras
        return no;
    } else if (strcasecmp(lista->word, word) >= 0) {
        if (strcasecmp(lista->word, word) == 0) {
            lista->lista_linhas = insere_numero(num, lista->lista_linhas);
            //num_total_palavas++; //Descomentar para obter numero total de palavras
            return lista;
        } else {
            no = calloc(1, sizeof(struct _palavra));
            strcpy(no->word, word);
            no->lista_linhas = insere_numero(num, NULL);
            no->next = lista;
            //num_total_palavas++; //Descomentar para obter numero total de palavras
            //num_palavras++; //Descomentar para obter numero de palavras distintas
            return no;
        }
    } else {
        aux = lista;
        while (aux->next != NULL && strcasecmp(aux->next->word, word) <= 0) {
            aux = aux->next;
        }
        if (strcasecmp(aux->word, word) == 0) {
            aux->lista_linhas = insere_numero(num, aux->lista_linhas);
            //num_total_palavas++; //Descomentar para obter numero total de palavras
        } else {
            no = calloc(1, sizeof(struct _palavra));
            strcpy(no->word, word);
            no->lista_linhas = insere_numero(num, NULL);
            no->next = aux->next;
            aux->next = no;
            //num_total_palavas++; //Descomentar para obter numero total de palavras
            //num_palavras++; //Descomentar para obter numero de palavras distintas
        }
        return lista;
    }
}

palavra pesquisa_palavra(char *word, palavra lista) {
    while (lista != NULL) {
        if (strcasecmp(lista->word, word) == 0) return lista;
        lista = lista->next;
    }
    return NULL;
}

void print_lista_palavras(palavra lista) {
    num_linha aux;
    while (lista != NULL) {
        aux = lista->lista_linhas;
        printf("%s ", lista->word);
        while (aux != NULL) {
            printf("%d ", aux->num);
            aux = aux->next;
        }
        printf("\n");
        lista = lista->next;
    }
}

void print_lista_linhas(num_linha lista) {
    if (lista == NULL) {
        printf("-1\n");
        return;
    }
    while (lista->next != NULL) {
        printf("%d ", lista->num);
        lista = lista->next;
    }
    printf("%d\n", lista->num);
}

palavra guarda_linha_palavras(palavra lista) {
    char buffer[DIM];
    char buffer2[DIM];
    int linha = 0;
    int i;
    float miliseconds;
    //clock_t t; //Descomentar para correr testes de Carregamentos
    //FILE* fp = fopen("medicoes.txt", "a"); //Descomentar para correr testes de Carregamentos
    //t = clock(); //Descomentar para correr testes de Carregamentos
    while (1) {
        scanf("%[^ ().,;\n]", buffer);
        if (strcasecmp(buffer, "FIM") == 0) {
            scanf("%[.]", buffer2);
            if (buffer2[0] == '.') {
                //t = clock() - t; //Descomentar para correr testes de Carregamentos
                //miliseconds = 1000 * (float) t / CLOCKS_PER_SEC; //Descomentar para correr testes de Carregamentos
                //fprintf(fp, "%f\n", miliseconds); //Descomentar para correr testes de Carregamentos
                //fclose(fp); //Descomentar para correr testes de Carregamentos
                printf("GUARDADO.\n");
                //printf("Total palavras diferentes: %d\nTotal palavras: %d\n", num_palavras, num_total_palavas); //Descomentar para obter numero de palavras
                return lista;
            } else {
                lista = insere_palavra(linha, buffer, lista);
                scanf("%[ ().,;\n]", buffer2);
                for (i = 0; buffer2[i] != '\0'; i++)
                    if (buffer2[i] == '\n') {
                        linha++;
                    }
            }
        } else {
            lista = insere_palavra(linha, buffer, lista);
            scanf("%[ ().,;\n]", buffer2);
            for (i = 0; buffer2[i] != '\0'; i++)
                if (buffer2[i] == '\n') {
                    linha++;
                }
        }
    }
}

void associa_palavra(int num, num_linha lista) {
    while (lista != NULL) {
        if (lista->num == num) {
            printf("ENCONTRADA.\n");
            return;
        }
        lista = lista->next;
    }
    printf("NAO ENCONTRADA.\n");
}

void destroi_lista_linhas(num_linha lista){
    num_linha aux;
    while(lista != NULL){
        aux = lista;
        lista = lista->next;
        free(aux);
    }
}

void destroi_lista_palavras(palavra lista){
    palavra aux;
    while(lista != NULL){
        aux = lista;
        lista = lista->next;
        destroi_lista_linhas(aux->lista_linhas);
        free(aux);
    }
}

int main() {
    char buffer[DIM], word[DIM];
    int num;
    char car;
    palavra res_pesq;
    palavra BDpalavras = NULL;
    //clock_t t; //Descomentar para testes de LINHAS's ou ASSOC's
    scanf("%s%c", buffer, &car);
    while (strcasecmp(buffer, "TCHAU") != 0) {
        if (strcasecmp(buffer, "TEXTO") == 0) {
            destroi_lista_palavras(BDpalavras);
            BDpalavras = NULL;
            BDpalavras = guarda_linha_palavras(BDpalavras);
            //t = clock(); //Descomentar para testes de LINHAS's ou ASSOC's
        } else if (strcasecmp(buffer, "LINHAS") == 0) {
            scanf("%s", word);
            if ((res_pesq = pesquisa_palavra(word, BDpalavras)) != NULL)
                print_lista_linhas(res_pesq->lista_linhas);
            else {
                print_lista_linhas(NULL);
            }
        } else if (strcasecmp(buffer, "ASSOC") == 0) {
            scanf("%s", word);
            scanf("%d", &num);
            if ((res_pesq = pesquisa_palavra(word, BDpalavras)) != NULL)
                associa_palavra(num, res_pesq->lista_linhas);
            else {
                printf("NAO ENCONTRADA.\n");
            }
        }
        scanf("%s", buffer);
    }
    //t = clock() - t; //Descomentar para testes de LINHAS's ou ASSOC's
    //printf("%f\n", 1000 * (float) t / CLOCKS_PER_SEC); //Descomentar para testes de LINHAS's ou ASSOC's
    destroi_lista_palavras(BDpalavras);
    return 0;
}