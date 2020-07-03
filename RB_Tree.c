#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define DIM 200
#define RED 1
#define BLACK 0

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
    palavra pai;
    char color;
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

palavra rotacao_adeq_recoloracao(palavra new_node);

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

/*ARVORE (RED-BLACK) PARA GUADAR PALAVRAS (DIFERENTES)*/
palavra inserir_arvore(palavra novo_no, int num, palavra tree) {
    if (tree == NULL) {
        novo_no->lista_linhas = insere_numero_avl(num, novo_no->lista_linhas);
        //num_total_palavas++; //Descomentar para obter numero total de palavras
        //num_palavras++; //Descomentar para obter numero de palavras distintas
        return novo_no;
    }

    if (strcasecmp(tree->word, novo_no->word) > 0) {
        tree->left = inserir_arvore(novo_no, num, tree->left);
        tree->left->pai = tree;
    } else if (strcasecmp(tree->word, novo_no->word) < 0) {
        tree->right = inserir_arvore(novo_no, num, tree->right);
        tree->right->pai = tree;
    } else {
        tree->lista_linhas = insere_numero_avl(num, tree->lista_linhas);
        //num_total_palavas++; //Descomentar para obter numero total de palavras
        return tree;
    }
    return tree;
}

palavra insere_palavra(int num, char *word, palavra tree) {
    palavra new_node;
    new_node = calloc(1, sizeof(struct _palavra));
    strcpy(new_node->word, word);
    tree = inserir_arvore(new_node, num, tree);
    if(new_node->lista_linhas == NULL) {
        free(new_node);
        return tree;
    }
    new_node->color = RED;
    palavra no_tio;
    while (new_node->pai != NULL && new_node->color != BLACK && new_node->pai->color == RED) {
        if (new_node->pai == new_node->pai->pai->left) {
            no_tio = new_node->pai->pai->right;
            if (no_tio != NULL && no_tio->color == RED) {
                new_node->pai->color = BLACK;
                no_tio->color = BLACK;
                new_node->pai->pai->color = RED;
                new_node = new_node->pai->pai;
            } else if (no_tio == NULL || no_tio->color == BLACK) {
                rotacao_adeq_recoloracao(new_node);
            }
        } else {
            no_tio = new_node->pai->pai->left;
            if (no_tio != NULL && no_tio->color == RED) {
                new_node->pai->color = BLACK;
                no_tio->color = BLACK;
                new_node->pai->pai->color = RED;
                new_node = new_node->pai->pai;
            } else if (no_tio == NULL || no_tio->color == BLACK) {
                rotacao_adeq_recoloracao(new_node);
            }
        }
    }
    while(tree->pai) tree = tree->pai;
    tree->color = BLACK;
    return tree;
}

palavra rotacao_adeq_recoloracao(palavra new_node) {
    palavra aux = new_node;
    new_node = new_node->pai;
    if (new_node->pai->left && new_node->right && new_node->right == aux && new_node->pai->left == new_node) {
        roda_esq(new_node);
        new_node->pai->color = BLACK;
        new_node->pai->pai->color = RED;
        roda_direita(new_node->pai->pai);
    } else if (new_node->pai->right && new_node->left && new_node->left == aux && new_node->pai->right == new_node) {
        roda_direita(new_node);
        new_node->pai->color = BLACK;
        new_node->pai->pai->color = RED;
        roda_esq(new_node->pai->pai);
    } else if (new_node->right && new_node->right == aux && new_node->right->pai == new_node && new_node->pai->right == new_node) {
        roda_esq(new_node->pai);
        new_node->color = BLACK;
        new_node->left->color = RED;
    } else if (new_node->left && new_node->left == aux && new_node->left->pai == new_node && new_node->pai->left) {
        roda_direita(new_node->pai);
        new_node->color = BLACK;
        new_node->right->color = RED;
    }
    return new_node;
}

palavra roda_direita(palavra no) {
    //num_rotacoes++; //Descomentar para obter numero de rotacoes
    palavra left_node = no->left;
    palavra left_right_node = left_node->right;


    left_node->right = no;
    no->left = left_right_node;

    if(no->pai) {
        if (no->pai->right == no)
            no->pai->right = left_node;
        else no->pai->left = left_node;
    }

    left_node->pai = no->pai;
    no->pai = left_node;
    if (left_right_node)
        left_right_node->pai = no;
    /*return sempre da raiz*/
    return left_node;
}

/*rotacao*/
palavra roda_esq(palavra no) {
    //num_rotacoes++; //Descomentar para obter numero de rotacoes
    palavra right_node = no->right;
    palavra right_left_node = right_node->left;


    right_node->left = no;
    no->right = right_left_node;

    if(no->pai) {
        if (no->pai->left == no)
            no->pai->left = right_node;
        else no->pai->right = right_node;
    }

    right_node->pai = no->pai;
    no->pai = right_node;
    if (right_left_node)
        right_left_node->pai = no;
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
    //FILE* fp = fopen("medicoes.txt", "a"); //Descomentar para correr testes de Carregamentos
    //t = clock(); //Descomentar para correr testes de Carregamentos
    while (1) {
        scanf("%[^ ().,;\n]", buffer);
        if (strcasecmp(buffer, "FIM") == 0) {
            scanf("%[.]", buffer2);
            if (buffer2[0] == '.') {
                //t = clock() - t; //Descomentar para correr testes de Carregamentos
                //miliseconds = 1000 * (float) t / CLOCKS_PER_SEC; //Descomentar para correr testes de Carregamentos
                //printf("%d\n", num_rotacoes); //Descomentar para obter numero de rotacoes
                //fprintf(fp, "%f\n", miliseconds); //Descomentar para correr testes de Carregamentos
                //fclose(fp); //Descomentar para correr testes de Carregamentos
                printf("GUARDADO.\n");
                //printf("Total palavras diferentes: %d\nTotal palavras: %d\n", num_palavras, num_total_palavas);  //Descomentar para obter numero de palavras
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
        //printf("NAO ENCONTRADA\n");
        return;
    }
    if (lista->num == num) {
        //printf("ENCONTRADA.\n");
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