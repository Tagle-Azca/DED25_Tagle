#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int x, y;
    struct Node *previous, *next, *up, *down;
} Node;
// struct node test

int **loadMaze(const char *fileName, int *rows, int *cols)
{
    FILE *file = fopen(fileName, "r");
    if (!file)
        return NULL;

    if (fscanf(file, "%d %d", rows, cols) != 2)
    {
        fclose(file);
        return NULL;
    }

    int r = 0, c = 0;
    char line[1024];

    while (fgets(line, sizeof(line), file))
    {
        if (c == 0)
        {
            char *ptr = line;
            while (*ptr)
            {
                if (*ptr == '0' || *ptr == '1')
                    c++;
                while (*ptr && *ptr != ' ')
                    ptr++;
                if (*ptr)
                    ptr++;
            }
        }
        r++;
    }

    *rows = r;
    *cols = c;
    rewind(file);

    int **maze = malloc(*rows * sizeof(int *));
    for (int i = 0; i < *rows; i++)
        maze[i] = malloc(*cols * sizeof(int));

    for (int i = 0; i < *rows; i++)
    {
        for (int j = 0; j < *cols; j++)
        {
            fscanf(file, "%d", &maze[i][j]);
        }
    }

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
    return x >= 0 && x < rows && y >= 0 && y < cols && maze[x][y] == 0 && visited[x][y] == 0;
}

Node *createNode(int x, int y, Node *previous)
{
    Node *newNode = malloc(sizeof(Node));
    newNode->x = x;
    newNode->y = y;
    newNode->previous = previous;
    return newNode;
}

void freePath(Node *node)
{
    while (node)
    {
        Node *temp = node;
        node = node->previous;
        free(temp);
    }
}

int dfs(int **maze, int **visited, int x, int y, int rows, int cols, Node **path)
{
    if (!isValid(x, y, maze, visited, rows, cols))
        return 0;

    visited[x][y] = 1;
    Node *current = createNode(x, y, *path);
    *path = current;

    if (x == rows - 1 && y == cols - 1)
        return 1;

    if (dfs(maze, visited, x + 1, y, rows, cols, path))
        return 1;
    if (dfs(maze, visited, x, y + 1, rows, cols, path))
        return 1;
    if (dfs(maze, visited, x - 1, y, rows, cols, path))
        return 1;
    if (dfs(maze, visited, x, y - 1, rows, cols, path))
        return 1;

    Node *temp = *path;
    *path = (*path)->previous;
    free(temp);
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
    {
        visited[i] = calloc(cols, sizeof(int));
    }

    Node *path = NULL;
    if (dfs(maze, visited, 0, 0, rows, cols, &path))
    {
        Node *current = path;
        while (current)
        {
            printf("(%d, %d)\n", current->x, current->y);
            current = current->previous;
        }
    }
    else
    {
        printf("No path found.\n");
    }

    freePath(path);
    freeMaze(maze, rows);
    for (int i = 0; i < rows; i++)
        free(visited[i]);
    free(visited);

    return 0;
}
