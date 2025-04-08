#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int x, y;
    struct Node *next;
} Node;

Node *createNode(int x, int y)
{
    Node *newNode = malloc(sizeof(Node));
    newNode->x = x;
    newNode->y = y;
    newNode->next = NULL;
    return newNode;
}

void pushNode(Node **head, int x, int y)
{
    Node *newNode = createNode(x, y);
    newNode->next = *head;
    *head = newNode;
}

void popNode(Node **head)
{
    if (*head)
    {
        Node *temp = *head;
        *head = (*head)->next;
        free(temp);
    }
}

void freeList(Node *head)
{
    while (head)
    {
        Node *temp = head;
        head = head->next;
        free(temp);
    }
}

int **loadMaze(const char *fileName, int *rows, int *cols)
{
    FILE *file = fopen(fileName, "r");
    if (!file)
    {
        printf("Can't open the file.\n");
        return NULL;
    }

    if (fscanf(file, "%d %d", rows, cols) != 2)
    {
        printf("Can't read index.\n");
        fclose(file);
        return NULL;
    }

    int **maze = malloc(*rows * sizeof(int *));
    for (int i = 0; i < *rows; i++)
        maze[i] = malloc(*cols * sizeof(int));

    for (int i = 0; i < *rows; i++)
        for (int j = 0; j < *cols; j++)
            fscanf(file, "%d", &maze[i][j]);

    fclose(file);
    return maze;
}

void freeMaze(int **maze, int rows)
{
    for (int i = 0; i < rows; i++)
        free(maze[i]);
    free(maze);
}

int isValid(int x, int y, int **maze, int **visited, int rows, int cols)
{
    return x >= 0 && x < rows && y >= 0 && y < cols &&
           maze[x][y] == 0 && visited[x][y] == 0;
}

int dfs(int **maze, int **visited, int x, int y, int destX, int destY, int rows, int cols, Node **path)
{
    if (!isValid(x, y, maze, visited, rows, cols))
        return 0;

    visited[x][y] = 1;
    pushNode(path, x, y);

    if (x == destX && y == destY)
        return 1;

    if (dfs(maze, visited, x + 1, y, destX, destY, rows, cols, path))
        return 1;
    if (dfs(maze, visited, x, y + 1, destX, destY, rows, cols, path))
        return 1;
    if (dfs(maze, visited, x - 1, y, destX, destY, rows, cols, path))
        return 1;
    if (dfs(maze, visited, x, y - 1, destX, destY, rows, cols, path))
        return 1;

    visited[x][y] = 0;
    popNode(path);
    return 0;
}

int main()
{
    int rows, cols;
    int **maze = loadMaze("./Maze.txt", &rows, &cols);
    if (!maze)
        return 1;

    int **visited = malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++)
        visited[i] = calloc(cols, sizeof(int));

    int startX = 4, startY = 2;
    int endX = 0, endY = 1;

    if (maze[startX][startY] != 0 || maze[endX][endY] != 0)
    {
        printf("Entrada o salida bloqueada.\n");
        return 1;
    }

    Node *path = NULL;

    if (dfs(maze, visited, startX, startY, endX, endY, rows, cols, &path))
    {
        printf("Found path\n");
        Node *temp = path;
        while (temp)
        {
            printf("(%d, %d)\n", temp->x, temp->y);
            temp = temp->next;
        }
    }
    else
    {
        printf("No path found.\n");
    }

    freeList(path);
    freeMaze(maze, rows);
    for (int i = 0; i < rows; i++)
        free(visited[i]);
    free(visited);

    return 0;
}
