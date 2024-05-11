#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <stack>
#include <fstream>

enum CellState { Close, Open };

class Cell
{
public:
    int x;
    int y;
    CellState Left;
    CellState Right;
    CellState Top;
    CellState Bottom;
    bool Visited;
};

const int width = 10, height = 10;
Cell labyrinth[width][height];

void drawMaze()
{
    if (!glfwInit())
        return;

    GLFWwindow* window = glfwCreateWindow(600, 600, "Labirint", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, width * 100, height * 100, 0, -1, 1);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                if (labyrinth[x][y].Top == Open)
                {
                    glBegin(GL_LINES);
                    glVertex2f(x * 100, y * 100);
                    glVertex2f((x + 1) * 100, y * 100);
                    glEnd();
                }

                if (labyrinth[x][y].Left == Open)
                {
                    glBegin(GL_LINES);
                    glVertex2f(x * 100, y * 100);
                    glVertex2f(x * 100, (y + 1) * 100);
                    glEnd();
                }

                if (labyrinth[x][y].Right == Open)
                {
                    glBegin(GL_LINES);
                    glVertex2f((x + 1) * 100, y * 100);
                    glVertex2f((x + 1) * 100, (y + 1) * 100);
                    glEnd();
                }

                if (labyrinth[x][y].Bottom == Open)
                {
                    glBegin(GL_LINES);
                    glVertex2f(x * 100, (y + 1) * 100);
                    glVertex2f((x + 1) * 100, (y + 1) * 100);
                    glEnd();
                }
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}

void generateMaze(Cell maze[][height], int x, int y)
{
    maze[x][y].Visited = true;

    std::vector<std::pair<int, int>> neighbors;

    if (x > 0 && !maze[x - 1][y].Visited)
        neighbors.push_back(std::make_pair(x - 1, y));
    if (x < width - 1 && !maze[x + 1][y].Visited)
        neighbors.push_back(std::make_pair(x + 1, y));
    if (y > 0 && !maze[x][y - 1].Visited)
        neighbors.push_back(std::make_pair(x, y - 1));
    if (y < height - 1 && !maze[x][y + 1].Visited)
        neighbors.push_back(std::make_pair(x, y + 1));

    while (!neighbors.empty())
    {
        int randomIndex = rand() % neighbors.size();
        int nx = neighbors[randomIndex].first;
        int ny = neighbors[randomIndex].second;

        if (!maze[nx][ny].Visited)
        {
            if (nx == x)
            {
                if (ny > y)
                {
                    maze[x][y].Bottom = Open;
                    maze[nx][ny].Top = Open;
                }
                else
                {
                    maze[x][y].Top = Open;
                    maze[nx][ny].Bottom = Open;
                }
            }
            else
            {
                if (nx > x)
                {
                    maze[x][y].Right = Open;
                    maze[nx][ny].Left = Open;
                }
                else
                {
                    maze[x][y].Left = Open;
                    maze[nx][ny].Right = Open;
                }
            }

            generateMaze(maze, nx, ny);
        }

        neighbors.erase(neighbors.begin() + randomIndex);
    }
}
/*Эта функция открывает файл для записи, а затем записывает информацию
о каждой ячейке лабиринта в формате
"Top Right Bottom Left" (например, 1 0 1 1)*/

void saveMazeToFile(Cell maze[][height], const std::string& filename)
{
    std::ofstream file(filename);

    if (file.is_open())
    {
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                file << maze[x][y].Top << " " << maze[x][y].Right << " " << maze[x][y].Bottom << " " << maze[x][y].Left << "\n";
            }
        }

        file.close();
        std::cout << "Лабиринт сохранен в " << filename << std::endl;
    }
    else
    {
        std::cerr << "Не получилось открыть файл для записи" << std::endl;
    }
}

int main()
{
    setlocale(LC_ALL, "ru");
    srand(time(NULL));

    // Инициализация лабиринта
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            labyrinth[x][y].x = x;
            labyrinth[x][y].y = y;
            labyrinth[x][y].Left = Close;
            labyrinth[x][y].Right = Close;
            labyrinth[x][y].Top = Close;
            labyrinth[x][y].Bottom = Close;
            labyrinth[x][y].Visited = false;
        }
    }

    // Генерация лабиринта
    generateMaze(labyrinth, 0, 0);

    drawMaze();

    saveMazeToFile(labyrinth, "maze.txt");

    return 0;
}
