#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

const int SCREEN_WIDTH = 30;
const int SCREEN_HEIGHT = 20;
const string PLAYER_CHAR = "🚀";
const string ALIEN_CHAR = "👾";
const string BULLET_CHAR = "🔺";
const string EMPTY_CHAR = " ";

class Spaceship {
public:
    int x, y;

    Spaceship() {
        x = SCREEN_WIDTH / 2;
        y = SCREEN_HEIGHT - 1;
    }

    void moveLeft() {
        if (x > 0) x--;
    }

    void moveRight() {
        if (x < SCREEN_WIDTH - 1) x++;
    }

    void shoot(vector<pair<int, int>>& bullets) {
        bullets.push_back(make_pair(x, y - 1));
    }
};

class Alien {
public:
    int x, y;

    Alien(int startX, int startY) {
        x = startX;
        y = startY;
    }

    void moveDown() {
        y++;
    }
};

void clearScreen() {
    cout << "\033[2J\033[1;1H"; // ANSI escape code to clear screen
}

void drawScreen(const Spaceship& spaceship, const vector<Alien>& aliens, const vector<pair<int, int>>& bullets) {
    vector<vector<string>> screen(SCREEN_HEIGHT, vector<string>(SCREEN_WIDTH, EMPTY_CHAR));

    screen[spaceship.y][spaceship.x] = PLAYER_CHAR;

    for (const Alien& alien : aliens) {
        screen[alien.y][alien.x] = ALIEN_CHAR;
    }

    for (const auto& bullet : bullets) {
        if (bullet.second >= 0 && bullet.second < SCREEN_HEIGHT) {
            screen[bullet.second][bullet.first] = BULLET_CHAR;
        }
    }

    for (int i = 0; i < SCREEN_HEIGHT; ++i) {
        for (int j = 0; j < SCREEN_WIDTH; ++j) {
            cout << screen[i][j];
        }
        cout << endl;
    }
}

bool isCollision(pair<int, int> bullet, const Alien& alien) {
    return bullet.first == alien.x && bullet.second == alien.y;
}

void updateAliens(vector<Alien>& aliens, bool& gameOver) {
    for (Alien& alien : aliens) {
        alien.moveDown();
    }

    // Check if any alien has reached the bottom of the screen
    for (const Alien& alien : aliens) {
        if (alien.y >= SCREEN_HEIGHT - 1) {
            gameOver = true;
            return;
        }
    }
}

void updateBullets(vector<pair<int, int>>& bullets) {
    for (auto& bullet : bullets) {
        bullet.second--;
    }

    bullets.erase(remove_if(bullets.begin(), bullets.end(), [](const pair<int, int>& bullet) {
        return bullet.second < 0;
    }), bullets.end());
}

void checkCollisions(vector<Alien>& aliens, vector<pair<int, int>>& bullets) {
    for (auto it = aliens.begin(); it != aliens.end();) {
        bool collisionDetected = false;
        for (auto bulletIt = bullets.begin(); bulletIt != bullets.end();) {
            if (isCollision(*bulletIt, *it)) {
                bulletIt = bullets.erase(bulletIt);
                collisionDetected = true;
                break;
            } else {
                ++bulletIt;
            }
        }
        if (collisionDetected) {
            it = aliens.erase(it);
        } else {
            ++it;
        }
    }
}

void updateGame(Spaceship& spaceship, vector<Alien>& aliens, vector<pair<int, int>>& bullets, bool& gameOver, int& alienMoveCounter, int alienMoveThreshold) {
    // Move aliens down
    if (alienMoveCounter >= alienMoveThreshold) {
        updateAliens(aliens, gameOver);
        alienMoveCounter = 0;
    } else {
        alienMoveCounter++;
    }

    // Move bullets up and handle bullet-alien collisions
    updateBullets(bullets);
    checkCollisions(aliens, bullets);

    // Generate new aliens at the top of the screen less frequently
    if (rand() % 50 == 0) { // Further reduced frequency of alien generation
        aliens.push_back(Alien(rand() % SCREEN_WIDTH, 0));
    }
}

int main() {
    srand(static_cast<unsigned int>(time(0)));

    Spaceship spaceship;
    vector<Alien> aliens;
    vector<pair<int, int>> bullets;
    bool gameOver = false;

    int alienMoveCounter = 0;
    const int alienMoveThreshold = 15; // Slower alien movement

    struct termios originalT, modifiedT;
    tcgetattr(STDIN_FILENO, &originalT);
    modifiedT = originalT;
    modifiedT.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &modifiedT);

    int oldFcntl = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldFcntl | O_NONBLOCK);

    while (!gameOver) {
        char input = '\0';
        read(STDIN_FILENO, &input, sizeof(char));
        switch (input) {
            case 'a': spaceship.moveLeft(); break;
            case 'd': spaceship.moveRight(); break;
            case ' ': spaceship.shoot(bullets); break;
            default: break;
        }

        updateGame(spaceship, aliens, bullets, gameOver, alienMoveCounter, alienMoveThreshold);

        clearScreen();
        drawScreen(spaceship, aliens, bullets);

        this_thread::sleep_for(chrono::milliseconds(50)); // Faster game loop for smoother gameplay
    }

    cout << "Game Over! Aliens reached the bottom of the screen." << endl;

    tcsetattr(STDIN_FILENO, TCSANOW, &originalT);
    fcntl(STDIN_FILENO, F_SETFL, oldFcntl);

    return 0;
}
