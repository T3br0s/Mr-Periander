#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <queue>
#include <algorithm>
#include <chrono>
#include <thread>

using namespace std;

class MazeGame {
private:
    vector<vector<char>> maze;
    int rows;
    int cols;
    int playerX;
    int playerY;
    int stoneX;
    int stoneY;
    int babisX;
    int babisY;
    vector<pair<int, int>> shortestPath;

public:
    MazeGame(const string& fileName);
    void playGame();
    void printMaze();
    void movePlayer(char direction);
    bool isValidMove(int x, int y);
    void moveBabis();
    void calculateShortestPath();
    void traverseShortestPath();
    int calculateDistance(int x1, int y1, int x2, int y2);
};

MazeGame::MazeGame(const string& fileName) {
    ifstream inputFile(fileName);

    if (inputFile.is_open()) {
        string line;
        while (getline(inputFile, line)) {
            vector<char> row;
            for (char ch : line) {
                row.push_back(ch);
            }
            maze.push_back(row);
        }
        inputFile.close();

        rows = maze.size();
        cols = maze[0].size();

        srand(time(0));
        playerX = rand() % (rows - 2) + 1;
        playerY = rand() % (cols - 2) + 1;

        do {
            stoneX = rand() % (rows - 2) + 1;
            stoneY = rand() % (cols - 2) + 1;
        } while (stoneX == playerX && stoneY == playerY);

        do {
            babisX = rand() % (rows - 2) + 1;
            babisY = rand() % (cols - 2) + 1;
        } while ((babisX == playerX && babisY == playerY) || (babisX == stoneX && babisY == stoneY));

        calculateShortestPath();
        traverseShortestPath();
    }
}

void MazeGame::printMaze() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (i == playerX && j == playerY) {
                cout << "M ";
            } else if (i == stoneX && j == stoneY) {
                cout << "S ";
            } else if (i == babisX && j == babisY) {
                cout << "L ";
            } else {
                cout << maze[i][j] << " ";
            }
        }
        cout << endl;
    }
}

bool MazeGame::isValidMove(int x, int y) {
    return (x >= 0 && x < rows && y >= 0 && y < cols && maze[x][y] == '.');
}

void MazeGame::movePlayer(char direction) {
    char playerMove;
    cout << "Κίνηση Periander (w/a/s/d): ";
    cin >> playerMove;
    int newX = playerX;
    int newY = playerY;

    switch (playerMove) {
        case 'w':
            newX--;
            break;
        case 's':
            newX++;
            break;
        case 'a':
            newY--;
            break;
        case 'd':
            newY++;
            break;
        default:
            return;
    }

    if (isValidMove(newX, newY)) {
        playerX = newX;
        playerY = newY;
    }
    if (newX == stoneX && newY == stoneY) {
        cout << "Congratulations! You found the stone and won the game." << endl;
        exit(0);
    }
}

void MazeGame::moveBabis() {
    if (babisX == -1 && babisY == -1)
        return;

    int newX = babisX;
    int newY = babisY;

    if (newX == stoneX && newY == stoneY) {
        cout << "Babis found the stone! You lost the game." << endl;
        exit(0);
    }

    int minDistance = calculateDistance(newX, newY, stoneX, stoneY);

    if (isValidMove(newX - 1, newY) && calculateDistance(newX - 1, newY, stoneX, stoneY) < minDistance) {
        newX--;
    } else if (isValidMove(newX + 1, newY) && calculateDistance(newX + 1, newY, stoneX, stoneY) < minDistance) {
        newX++;
    } else if (isValidMove(newX, newY - 1) && calculateDistance(newX, newY - 1, stoneX, stoneY) < minDistance) {
        newY--;
    } else if (isValidMove(newX, newY + 1) && calculateDistance(newX, newY + 1, stoneX, stoneY) < minDistance) {
        newY++;
    }

    babisX = newX;
    babisY = newY;

    if (newX == stoneX && newY == stoneY) {
        cout << "Babis reached the stone!" << endl;
        exit(0);
    }
}

int MazeGame::calculateDistance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

int dx[] = {-1, 0, 1, 0};
int dy[] = {0, 1, 0, -1};

void MazeGame::calculateShortestPath() {
    vector<vector<int>> distance(rows, vector<int>(cols, -1));
    vector<vector<int>> parent(rows, vector<int>(cols, -1));
    queue<pair<int, int>> q;

    distance[playerX][playerY] = 0;
    q.push(make_pair(playerX, playerY));

    while (!q.empty()) {
        pair<int, int> curr = q.front();
        q.pop();
        int currX = curr.first;
        int currY = curr.second;

        if (currX == stoneX && currY == stoneY)
            break;

        for (int i = 0; i < 4; i++) {
            int newX = currX + dx[i];
            int newY = currY + dy[i];

            if (isValidMove(newX, newY) && distance[newX][newY] == -1) {
                distance[newX][newY] = distance[currX][currY] + 1;
                parent[newX][newY] = i;
                q.push(make_pair(newX, newY));
            }
        }
    }
}

void MazeGame::traverseShortestPath() {
    for (int i = 0; i < shortestPath.size(); i++) {
        system("cls");
        int newX = shortestPath[i].first;
        int newY = shortestPath[i].second;

        if (isValidMove(newX, newY)) {
            playerX = newX;
            playerY = newY;
        }

        moveBabis();
        printMaze();

        if (i != shortestPath.size() - 1) {
            this_thread::sleep_for(chrono::milliseconds(500));
        }
    }
}

void MazeGame::playGame() {
    printMaze();

    while (true) {
        movePlayer('M');
        moveBabis();
        printMaze();
    }
}

int main() {
    int map;
    cout << "Pick your Map(1-10): ";
    cin >> map;
    if (map == 1){
            MazeGame game("Maze1.txt");
            game.playGame();
        
    }
    else if (map == 2){
            MazeGame game("Maze2.txt");
            game.playGame();
        
    }
    else if (map == 3){
            MazeGame game("Maze3.txt");
            game.playGame();
        
    }
    else if (map == 4){
            MazeGame game("Maze4.txt");
            game.playGame();
        
    }
    else if (map == 5){
            MazeGame game("Maze5.txt");
            game.playGame();
        
    }
    else if (map == 6){
            MazeGame game("Maze6.txt");
            game.playGame();
        
    }else if (map == 7){
            MazeGame game("Maze7.txt");
            game.playGame();
        
    }else if (map == 8){
            MazeGame game("Maze8.txt");
            game.playGame();
        
    }
    else if (map == 9){
            MazeGame game("Maze9.txt");
            game.playGame();
        
    }else if (map == 10){
            MazeGame game("Maze10.txt");
            game.playGame();
    }
    else {
        cout << "FATAL ERROR";
        return 0;
    }
    

    return 0;
}
