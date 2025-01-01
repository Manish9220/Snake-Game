#include <stdio.h>
#include <stdlib.h>
#include <conio.h>  // For _kbhit() and _getch()
#include <time.h>
#include <windows.h>  // For Sleep()

#define HEIGHT 15
#define WIDTH 30

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    STOP
};

enum Direction dir;
int score;
int fruit_x, fruit_y;
int head_x, head_y;
int tail_length;
int tail_x[100];
int tail_y[100];

void setup();
void clear_screen();
void draw();
void input();
void game_play();

int main() {
    srand(time(NULL));  // Seed random number generator
    setup();            // Initialize game variables
    while (1) {
        draw();         // Render the game
        input();        // Handle user input
        game_play();    // Update game state
        Sleep(200);     // Control game speed (200 ms per frame)
    }
}

void setup() {
    // Initialize snake's position and direction
    head_x = WIDTH / 2;
    head_y = HEIGHT / 2;
    fruit_x = rand() % WIDTH;
    fruit_y = rand() % HEIGHT;
    dir = STOP;  // Snake waits for player input to start
    score = 0;
    tail_length = 0;
}

void input() {
    if (_kbhit()) {  // Check if a key is pressed
        char ch = _getch();  // Get the key pressed
        switch (ch) {
            case '4':
                if (dir != RIGHT) dir = LEFT;  // Prevent reversing into itself
                break;
            case '5':
                if (dir != UP) dir = DOWN;
                break;
            case '6':
                if (dir != LEFT) dir = RIGHT;
                break;
            case '8':
                if (dir != DOWN) dir = UP;
                break;
            case '0':  // Exit game
                printf("\nGame Over! Final Score: %d\n", score);
                exit(0);
                break;
            default:
                break;
        }
    }
}

void game_play() {
    // Move the tail
    for (int i = tail_length - 1; i > 0; i--) {
        tail_x[i] = tail_x[i - 1];
        tail_y[i] = tail_y[i - 1];
    }
    if (tail_length > 0) {
        tail_x[0] = head_x;
        tail_y[0] = head_y;
    }

    // Move the head in the current direction
    switch (dir) {
        case UP:
            head_y--;
            break;
        case DOWN:
            head_y++;
            break;
        case LEFT:
            head_x--;
            break;
        case RIGHT:
            head_x++;
            break;
        case STOP:
            break;  // Snake remains stationary until a key is pressed
    }

    // Check collision with walls
    if (head_x < 0 || head_x >= WIDTH || head_y < 0 || head_y >= HEIGHT) {
        printf("\nYou hit the wall! GAME OVER! Final Score: %d\n", score);
        exit(0);
    }

    // Check collision with tail
    for (int i = 0; i < tail_length; i++) {
        if (tail_x[i] == head_x && tail_y[i] == head_y) {
            printf("\nYou hit your tail! GAME OVER! Final Score: %d\n", score);
            exit(0);
        }
    }

    // Check if the snake eats the fruit
    if (head_x == fruit_x && head_y == fruit_y) {
        score += 10;
        tail_length++;
        fruit_x = rand() % WIDTH;
        fruit_y = rand() % HEIGHT;
    }
}

void draw() {
    clear_screen();
    printf("\t\tWelcome to The Snake Game!!!\n");
    // Draw the top border
    for (int i = 0; i < WIDTH + 2; i++) printf("#");
    printf("\n");

    // Draw the game area
    for (int i = 0; i < HEIGHT; i++) {
        printf("#");  // Left border
        for (int j = 0; j < WIDTH; j++) {
            if (i == head_y && j == head_x) {
                printf("O");  // Snake head
            } else if (i == fruit_y && j == fruit_x) {
                printf("F");  // Fruit
            } else {
                int tail_found = 0;
                for (int k = 0; k < tail_length; k++) {
                    if (tail_x[k] == j && tail_y[k] == i) {
                        printf("o");  // Snake tail
                        tail_found = 1;
                        break;
                    }
                }
                if (!tail_found) printf(" ");
            }
        }
        printf("#\n");  // Right border
    }

    // Draw the bottom border
    for (int i = 0; i < WIDTH + 2; i++) printf("#");
    printf("\n");

    // Display score
    printf("Score: %d\n", score);
}

void clear_screen() {
    system("cls");  // Clear the screen on Windows
}