#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TREE_HT 100
#define CHAR_SET_SIZE 256

typedef struct Node
{
    char data;
    unsigned freq;
    struct Node *left, *right;
} Node;

typedef struct MinHeap
{
    unsigned size;
    unsigned capacity;
    Node **array;
} MinHeap;

Node *newNode(char data, unsigned freq)
{
    Node *temp = (Node *)malloc(sizeof(Node));
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
    return temp;
}

MinHeap *createMinHeap(unsigned capacity)
{
    MinHeap *minHeap = (MinHeap *)malloc(sizeof(MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (Node **)malloc(minHeap->capacity * sizeof(Node *));
    return minHeap;
}

void swapNodes(Node **a, Node **b)
{
    Node *t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(MinHeap *minHeap, int idx)
{
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx)
    {
        swapNodes(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

int isSizeOne(MinHeap *minHeap)
{
    return (minHeap->size == 1);
}

Node *extractMin(MinHeap *minHeap)
{
    Node *temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[--minHeap->size];
    minHeapify(minHeap, 0);
    return temp;
}

void insertMinHeap(MinHeap *minHeap, Node *node)
{
    ++minHeap->size;
    int i = minHeap->size - 1;
    while (i && node->freq < minHeap->array[(i - 1) / 2]->freq)
    {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = node;
}

void buildMinHeap(MinHeap *minHeap)
{
    int n = minHeap->size - 1;
    for (int i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

int isLeaf(Node *node)
{
    return !(node->left) && !(node->right);
}

MinHeap *createAndBuildMinHeap(char data[], int freq[], int size)
{
    MinHeap *minHeap = createMinHeap(size);
    for (int i = 0; i < size; ++i)
        minHeap->array[i] = newNode(data[i], freq[i]);
    minHeap->size = size;
    buildMinHeap(minHeap);
    return minHeap;
}

Node *buildHuffmanTree(char data[], int freq[], int size)
{
    Node *left, *right, *top;
    MinHeap *minHeap = createAndBuildMinHeap(data, freq, size);

    while (!isSizeOne(minHeap))
    {
        left = extractMin(minHeap);
        right = extractMin(minHeap);
        top = newNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        insertMinHeap(minHeap, top);
    }

    return extractMin(minHeap);
}

void storeCodes(Node *root, int arr[], int top, char *codes[CHAR_SET_SIZE])
{
    if (root->left)
    {
        arr[top] = 0;
        storeCodes(root->left, arr, top + 1, codes);
    }

    if (root->right)
    {
        arr[top] = 1;
        storeCodes(root->right, arr, top + 1, codes);
    }

    if (isLeaf(root))
    {
        codes[(unsigned char)root->data] = (char *)malloc(top + 1);
        for (int i = 0; i < top; i++)
            codes[(unsigned char)root->data][i] = arr[i] + '0';
        codes[(unsigned char)root->data][top] = '\0';
    }
}

int getFrequencies(const char *text, char *characters, int *frequencies)
{
    int freq[CHAR_SET_SIZE] = {0};
    int length = strlen(text);
    for (int i = 0; i < length; i++)
        freq[(unsigned char)text[i]]++;

    int count = 0;
    for (int i = 0; i < CHAR_SET_SIZE; i++)
    {
        if (freq[i])
        {
            characters[count] = (char)i;
            frequencies[count] = freq[i];
            count++;
        }
    }

    return count;
}

void compressText(const char *text, char *codes[CHAR_SET_SIZE], char *output)
{
    output[0] = '\0';
    for (int i = 0; text[i]; i++)
        strcat(output, codes[(unsigned char)text[i]]);
}

void decompressText(Node *root, const char *compressed)
{
    Node *curr = root;
    printf("Texto descomprimido: ");
    for (int i = 0; compressed[i]; i++)
    {
        if (compressed[i] == '0')
            curr = curr->left;
        else
            curr = curr->right;

        if (isLeaf(curr))
        {
            putchar(curr->data);
            curr = root;
        }
    }
    printf("\n");
}

int main()
{
    const char *text = "The sun is a huge ball of gases. It has a huge diameter. It is so huge that it can hold millions of planets inside it. The Sun is mainly made up of hydrogen and helium gas.";
    char characters[CHAR_SET_SIZE];
    int frequencies[CHAR_SET_SIZE];

    int size = getFrequencies(text, characters, frequencies);

    Node *root = buildHuffmanTree(characters, frequencies, size);

    int arr[MAX_TREE_HT], top = 0;
    char *codes[CHAR_SET_SIZE] = {0};

    storeCodes(root, arr, top, codes);

    printf("Tabla de códigos de Huffman:\n");
    for (int i = 0; i < CHAR_SET_SIZE; i++)
    {
        if (codes[i])
            printf("'%c': %s\n", i, codes[i]);
    }

    char compressed[10000];
    compressText(text, codes, compressed);

    printf("\nTexto comprimido:\n%s\n\n", compressed);

    decompressText(root, compressed);

    return 0;
}