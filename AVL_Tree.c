/*Solucao baseada em https://www.geeksforgeeks.org/avl-tree-set-1-insertion/*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define DIM 200

//int num_palavras = 0; //Descomentar para obter numero de palavras distintas
//int num_total_palavas = 0; //Descomentar para obter numero total de palavras
//int num_rotacoes = 0;  //Descomentar para obter numero de rotacoes

typedef struct _num_linha *num_linha;
struct _num_linha {
    int num;
    num_linha left;
    num_linha right;
    int height;
};

typedef struct _palavra *palavra;
struct _palavra {
    num_linha lista_linhas;
    char word[DIM];
    palavra left;
    palavra right;
    int height;
};

num_linha insere_numero_avl(int num, num_linha lista_linhas);

num_linha roda_direita_numLinha(num_linha no);

num_linha roda_esq_numLinha(num_linha no);

palavra insere_palavra(int num, char *word, palavra tree);

palavra pesquisa_palavra(char *word, palavra tree);

void print_lista_palavras(palavra tree);

char print_lista_linhas(num_linha lista, char first);

palavra guarda_linha_palavras(palavra tree);

palavra roda_esq(palavra no);

palavra roda_direita(palavra no);

/*AVL PARA GUARDAR OS NUMERO DAS LINHAS EM QUE UMA PALAVRA OCORRE*/
num_linha insere_numero_avl(int num, num_linha lista_linhas) {
    int dif_h, maximo;
    num_linha no;
    if (lista_linhas == NULL) {
        no = calloc(1, sizeof(struct _palavra));
        no->num = num;
        return no;
    }

    if (num < lista_linhas->num) {
        lista_linhas->left = insere_numero_avl(num, lista_linhas->left);
    } else if (num > lista_linhas->num) {
        lista_linhas->right = insere_numero_avl(num, lista_linhas->right);
    }

    maximo = 0;
    if (lista_linhas->left != NULL) {
        maximo = lista_linhas->left->height;
    }
    if (lista_linhas->right != NULL && lista_linhas->right->height > maximo) {
        maximo = lista_linhas->right->height;
    }

    lista_linhas->height = 1 + maximo;

    if (lista_linhas->left == NULL && lista_linhas->right == NULL) {
        dif_h = lista_linhas->height;
    } else if (lista_linhas->right == NULL) {
        dif_h = lista_linhas->left->height;
    } else if (lista_linhas->left == NULL) {
        dif_h = -lista_linhas->right->height;
    } else {
        dif_h = lista_linhas->left->height - lista_linhas->right->height;
    }

    if (dif_h > 1 && lista_linhas->left != NULL && lista_linhas->left->num > num) {
        return roda_direita_numLinha(lista_linhas);
    }
    if (dif_h < -1 && lista_linhas->right != NULL && lista_linhas->right->num < num) {
        return roda_esq_numLinha(lista_linhas);
    }
    if (dif_h > 1 && lista_linhas->left != NULL && lista_linhas->left->num < num) {
        lista_linhas->left = roda_esq_numLinha(lista_linhas->left);
        return roda_direita_numLinha(lista_linhas);
    }
    if (dif_h < -1 && lista_linhas->right != NULL && lista_linhas->right->num > num) {
        lista_linhas->right = roda_direita_numLinha(lista_linhas->right);
        return roda_esq_numLinha(lista_linhas);
    }
    return lista_linhas;
}

num_linha roda_direita_numLinha(num_linha no) {
    int maximo;
    num_linha left_node = no->left;
    num_linha left_right_node = left_node->right;

    left_node->right = no;
    no->left = left_right_node;

    maximo = 0;
    if (no->left != NULL) {
        maximo = no->left->height;
    }
    if (no->right != NULL && no->right->height > maximo) {
        maximo = no->right->height;
    }
    no->height = 1 + maximo;

    maximo = 0;
    if (left_node->left != NULL) {
        maximo = left_node->left->height;
    }
    if (left_node->right != NULL && left_node->right->height > maximo) {
        maximo = left_node->right->height;
    }
    left_node->height = 1 + maximo;
    return left_node;
}

num_linha roda_esq_numLinha(num_linha no) {
    int maximo;
    num_linha right_node = no->right;
    num_linha right_left_node = right_node->left;

    right_node->left = no;
    no->right = right_left_node;

    maximo = 0;
    if (no->left != NULL) {
        maximo = no->left->height;
    }
    if (no->right != NULL && no->right->height > maximo) {
        maximo = no->right->height;
    }
    no->height = 1 + maximo;

    maximo = 0;
    if (right_node->left != NULL) {
        maximo = right_node->left->height;
    }
    if (right_node->right != NULL && right_node->right->height > maximo) {
        maximo = right_node->right->height;
    }
    right_node->height = 1 + maximo;
    return right_node;
}

/*AVL PARA GUARDAR CADA PALAVRA (DIFERENTE)*/
palavra insere_palavra(int num, char *word, palavra tree) {
    int dif_h, maximo;
    palavra no;
    if (tree == NULL) {
        no = calloc(1, sizeof(struct _palavra));
        strcpy(no->word, word);
        no->lista_linhas = insere_numero_avl(num, NULL);
        no->height = 1;
        //num_palavras++;  //Descomentar para obter numero de palavras distintas
        //num_total_palavas++; //Descomentar para obter numero total de palavras
        return no;
    }

    if (strcasecmp(tree->word, word) > 0) {
        tree->left = insere_palavra(num, word, tree->left);
    } else if (strcasecmp(tree->word, word) < 0) {
        tree->right = insere_palavra(num, word, tree->right);
    } else {
        tree->lista_linhas = insere_numero_avl(num, tree->lista_linhas);
        //num_total_palavas++; //Descomentar para obter numero total de palavras
        return tree;
    }

    maximo = 0;
    if (tree->left != NULL) {
        maximo = tree->left->height;
    }
    if (tree->right != NULL && tree->right->height > maximo) {
        maximo = tree->right->height;
    }

    tree->height = 1 + maximo;

    if (tree->left == NULL && tree->right == NULL) {
        dif_h = tree->height;
    } else if (tree->right == NULL) {
        dif_h = tree->left->height;
    } else if (tree->left == NULL) {
        dif_h = -tree->right->height;
    } else {
        dif_h = tree->left->height - tree->right->height;
    }

    if (dif_h > 1 && tree->left != NULL && strcasecmp(tree->left->word, word) > 0) {
        return roda_direita(tree);
    }
    if (dif_h < -1 && tree->right != NULL && strcasecmp(tree->right->word, word) < 0) {
        return roda_esq(tree);
    }
    if (dif_h > 1 && tree->left != NULL && strcasecmp(tree->left->word, word) < 0) {
        tree->left = roda_esq(tree->left);
        return roda_direita(tree);
    }
    if (dif_h < -1 && tree->right != NULL && strcasecmp(tree->right->word, word) > 0) {
        tree->right = roda_direita(tree->right);
        return roda_esq(tree);
    }
    return tree;
}

palavra roda_direita(palavra no) {
    //num_rotacoes++; //Descomentar para obter numero de rotacoes
    int maximo;
    palavra left_node = no->left;
    palavra left_right_node = left_node->right;

    left_node->right = no;
    no->left = left_right_node;

    maximo = 0;
    if (no->left != NULL) {
        maximo = no->left->height;
    }
    if (no->right != NULL && no->right->height > maximo) {
        maximo = no->right->height;
    }
    no->height = 1 + maximo;

    maximo = 0;
    if (left_node->left != NULL) {
        maximo = left_node->left->height;
    }
    if (left_node->right != NULL && left_node->right->height > maximo) {
        maximo = left_node->right->height;
    }
    left_node->height = 1 + maximo;
    return left_node;
}

palavra roda_esq(palavra no) {
    int maximo;
    //num_rotacoes++; //Descomentar para obter numero de rotacoes
    palavra right_node = no->right;
    palavra right_left_node = right_node->left;

    right_node->left = no;
    no->right = right_left_node;

    maximo = 0;
    if (no->left != NULL) {
        maximo = no->left->height;
    }
    if (no->right != NULL && no->right->height > maximo) {
        maximo = no->right->height;
    }
    no->height = 1 + maximo;

    maximo = 0;
    if (right_node->left != NULL) {
        maximo = right_node->left->height;
    }
    if (right_node->right != NULL && right_node->right->height > maximo) {
        maximo = right_node->right->height;
    }
    right_node->height = 1 + maximo;
    return right_node;
}

palavra pesquisa_palavra(char *word, palavra tree) {
    if (tree == NULL) return tree;
    if (strcasecmp(tree->word, word) > 0) {
        return pesquisa_palavra(word, tree->left);
    } else if (strcasecmp(tree->word, word) < 0) {
        return pesquisa_palavra(word, tree->right);
    } else return tree;
}

void print_lista_palavras(palavra tree) {
    if (tree == NULL) return;
    print_lista_palavras(tree->left);
    printf("%s ", tree->word);
    print_lista_linhas(tree->lista_linhas, 1);
    printf("\n");
    print_lista_palavras(tree->right);
}

char print_lista_linhas(num_linha lista, char first) {
    if (lista == NULL) {
        return first;
    }
    first = print_lista_linhas(lista->left, first);
    if (first) {
        first = 0;
        printf("%d", lista->num);
    } else printf(" %d", lista->num);
    first = print_lista_linhas(lista->right, first);
    return first;
}

palavra guarda_linha_palavras(palavra tree) {
    char buffer[DIM];
    char buffer2[DIM];
    int linha = 0;
    int i;
    //float miliseconds; //Descomentar para correr testes de Carregamentos
    //clock_t t; //Descomentar para correr testes de Carregamentos
    //t = clock(); //Descomentar para correr testes de Carregamentos
    //FILE* fp = fopen("medicoes.txt", "a");
    while (1) {
        scanf("%[^ ().,;\n]", buffer);
        if (strcasecmp(buffer, "FIM") == 0) {
            scanf("%[.]", buffer2);
            if (buffer2[0] == '.') {
                //t = clock() - t; //Descomentar para correr testes de Carregamentos
                //miliseconds = 1000 * (float) t / CLOCKS_PER_SEC; //Descomentar para correr testes de Carregamentos
                //printf("%d\n", num_rotacoes); //Descomentar para obter numero de rotacoes
                //fprintf(fp, "%f\n", miliseconds); //Descomentar para correr testes de Carregamentos
                printf("GUARDADO.\n");
                //printf("Total palavras diferentes: %d\nTotal palavras: %d\n", num_palavras, num_total_palavas); //Descomentar para obter numero de palavras
                return tree;
            } else {
                tree = insere_palavra(linha, buffer, tree);
                scanf("%[ ().,;\n]", buffer2);
                for (i = 0; buffer2[i] != '\0'; i++)
                    if (buffer2[i] == '\n') {
                        linha++;
                    }
            }
        } else {
            tree = insere_palavra(linha, buffer, tree);
            scanf("%[ ().,;\n]", buffer2);
            for (i = 0; buffer2[i] != '\0'; i++)
                if (buffer2[i] == '\n') {
                    linha++;
                }
        }
    }
}

void associa_palavra(int num, num_linha lista) {
    if (lista == NULL) {
        printf("NAO ENCONTRADA\n");
        return;
    }
    if (lista->num == num) {
        printf("ENCONTRADA.\n");
        return;
    } else if (lista->num < num) {
        associa_palavra(num, lista->right);
    } else if (lista->num > num) {
        associa_palavra(num, lista->left);
    }
}

void destroi_lista_linhas(num_linha lista) {
    if (lista == NULL) return;
    destroi_lista_linhas(lista->left);
    destroi_lista_linhas(lista->right);
    free(lista);
}

void destroi_tree(palavra tree) {
    if (tree == NULL) return;
    destroi_tree(tree->left);
    destroi_tree(tree->right);
    destroi_lista_linhas(tree->lista_linhas);
    free(tree);
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
            destroi_tree(BDpalavras);
            BDpalavras = NULL;
            BDpalavras = guarda_linha_palavras(BDpalavras);
            //t = clock(); //Descomentar para testes de LINHAS's ou ASSOC's
        } else if (strcasecmp(buffer, "LINHAS") == 0) {
            scanf("%s", word);
            res_pesq = pesquisa_palavra(word, BDpalavras);
                if (!res_pesq || print_lista_linhas(res_pesq->lista_linhas, 1)) {
                    printf("-1\n");
                }else
                    printf("\n");
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
    destroi_tree(BDpalavras);
    return 0;
}
