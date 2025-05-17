#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../adt_map/map.h"
#include "../adt_pq/pq.h"


#define MAX_TREE_HT 100
#define CHAR_SET_SIZE 256

typedef struct Node
{
    char data;
    unsigned freq;
    struct Node *left, *right;
} Node;

int cmp_nodes(void *a, void *b){
    Node *n1 = (Node*)a;
    Node *n2 = (Node*)b;
    return n1->freq - n2->freq;
}

int convert_char(void *key){
    return *(( unsigned char *)key);
}

boolean compare_char(void *a, void *b){
    return *((unsigned char *)a) == *((unsigned char *)b);
}

map *getFrequencies(const char *text){
    map *freqMap = map_create(256, convert_char, compare_char);
    for (int i = 0; text[i] != '\0'; i++){
        char c = text[i];
        char *key = malloc(sizeof(char));
        *key = c;

        if (map_contains(freqMap, key))
        {
            int *count;
            count = (int*)map_get(freqMap, key);
            (*count)++;
            free(key);
        } else{
            int *count;
            count = malloc(sizeof(int));
            *count=1;
            map_put(freqMap, key, count);
        }
        
    }
    return freqMap;
    
}

Node *buildHuffmanTree(map *freqMap)
{
    pq *queue = pq_create(256, cmp_nodes);
    Node *left, *right, *top;
    
    for (int i = 0; i < CHAR_SET_SIZE; i++)
    {
        char c = i;
        if (map_contains(freqMap, &c))
        {
            int *freq = (int*)map_get(freqMap, &c);
            Node *n = malloc(sizeof(Node));
            n->data = c;
            n->left = NULL;
            n->right = NULL;
            n->freq = *freq;
            pq_offer(queue, n);
        }
    }
    

    while (pq_size(queue) > 1)
    {
        left = pq_poll(queue);
        right = pq_poll(queue);

        top = malloc(sizeof(Node));
        top->data = '\0';
        top->left = left;
        top->right = right;
        top->freq = left->freq + right->freq;

        pq_offer(queue, top);
    }

    return pq_poll(queue);
}

void storeCodes(Node *root, char *path, map *codeMap)
{
    if (root == NULL)
    {
        /* code */
        return;
    }
    
    if (root->left == NULL && root->right == NULL)
    {
        char *key = malloc(sizeof(char));
        *key = root->data;
        char *code = malloc(strlen(path)+1);
        strcpy(code, path);
        map_put(codeMap, key, code);
        return;
    }

    char leftpath[CHAR_SET_SIZE], rightpath[CHAR_SET_SIZE];
    int len = strlen(path);
    for (int i = 0; i < len; i++)
    {
        leftpath[i] = path[i];
    }
    leftpath[len] = '0';
    leftpath[len + 1] = '\0';
    storeCodes(root->left, leftpath, codeMap);

    int len2 = strlen(path);
    for (int i = 0; i < len2; i++)
    {
        rightpath[i] = path[i];
    }
    rightpath[len2] = '1';
    rightpath[len2 + 1] = '\0';
    storeCodes(root->right, rightpath, codeMap);
}

char *compressText(const char *text, map *codeMap) {
    char *compressed = malloc(strlen(text) * 10);
    compressed[0] = '\0';

    for (int i = 0; text[i]; i++) {
        char c = text[i];
        char *code = (char *)map_get(codeMap, &c);
        strcat(compressed, code);
    }

    return compressed;
}

void decompressText(Node *root, const char *compressed)
{
    Node *curr = root;
    printf("Texto descomprimido: ");
    for (int i = 0; compressed[i]; i++)
    {
        if (compressed[i] == '0'){
            curr = curr->left;
        } else{
            curr = curr->right;
        }
            

        if (curr->left == NULL && curr->right == NULL) {
            putchar(curr->data);
            curr = root;
        }
    }
    printf("\n");
}

int main()
{
    const char *text = "The sun is a huge ball of gases. It has a huge diameter. It is so huge that it can hold millions of planets inside it. The Sun is mainly made up of hydrogen and helium gas.";
    /* char characters[CHAR_SET_SIZE];
    int frequencies[CHAR_SET_SIZE]; */

    map *freqMap = getFrequencies(text);
    Node *root = buildHuffmanTree(freqMap);

    /*int arr[MAX_TREE_HT], top = 0;
    char *codes[CHAR_SET_SIZE] = {0};*/

    map *codeMap = map_create(CHAR_SET_SIZE, convert_char, compare_char);
    storeCodes(root, "", codeMap);

    printf("Tabla de códigos de Huffman:\n");
    for (int i = 0; i < 256; i++) {
        char c = i;
        if (map_contains(codeMap, &c)) {
            char *code = (char *)map_get(codeMap, &c);
            printf("'%c': %s\n", c, code);
        }
    }

  
    char *compressed = compressText(text, codeMap);
    printf("\nTexto comprimido:\n%s\n\n", compressed);

    decompressText(root, compressed);

    return 0;
}