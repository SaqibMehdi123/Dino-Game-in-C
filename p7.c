#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "raylib.h"

int score = 0;
int highScore = 0; // Variable to store the highest score
float scoreUpdateTime = 0;
float gameSpeed = 5.0; // Initial game speed, you can adjust this value

// Load the high score from a file (you can replace "highscore.txt" with your desired file name)
void loadHighScore() {
    FILE *file = fopen("HISCR.txt", "r");
    if (file != NULL) {
        fscanf(file, "%d", &highScore);
        fclose(file);
    }
}

void saveHighScore() {
    FILE *file = fopen("HISCR.txt", "w");
    if (file != NULL) {
        fprintf(file, "%d", highScore);
        fclose(file);
    }
}

typedef struct Sprite {
    Texture2D tex;
    Vector2 pos;
    Rectangle srect[4];

    int frames;
    int currentFrame;
    int actualWidth;
    int actualHeight;

    long double time;
    float animationTime;
} Sprite;

typedef struct Cactus {
    Texture2D tex;

    Rectangle srect[60];
    Vector2 pos[60];

    int size;
    int actualWidth;
    int actualHeight;
    int speed;
} Cactus;

// Function to animate the sprite
void sprite_animate(Sprite *s);

// Function to control the flying behavior of the bird sprite
bool dinobird_fly(Sprite *s, int speed);

int main(void) {
    const int screenWidth = 1300;
    const int screenHeight = 650;

    srand(time(NULL));

    // Initialization
    InitWindow(screenWidth, screenHeight, "CLang Dino Run");
    // Texture2D background = LoadTexture("ff.png");
    Image backgroundImage = LoadImage("R.png");
    Texture2D backgroundTexture = LoadTextureFromImage(backgroundImage);
    UnloadImage(backgroundImage); 

    SetTraceLogLevel(LOG_NONE);

    SetTargetFPS(40);

    //this function allows the access of speaker to the program to run the music
    InitAudioDevice();

    // Load sounds
    Sound jumpSound = LoadSound("jump.wav");
    Sound scoreSound = LoadSound("point.wav");
    Sound gameOverSound = LoadSound("die.wav");

    // Buttons
    Rectangle playButton = {screenWidth / 2 - 80, screenHeight / 2 - 50, 170, 40};
    Rectangle highScoreButton = {screenWidth / 2 - 80, screenHeight / 2, 170, 40};
    Rectangle soundButton = {screenWidth / 2 - 80, screenHeight / 2 + 50, 170, 40};
    Rectangle soundonButton = {screenWidth / 2 - 80, screenHeight / 2 + 50, 170, 40};
    Rectangle soundoffButton = {screenWidth / 2 - 80, screenHeight / 2 + 50, 170, 40};
    Rectangle exitButton = {screenWidth / 2 - 80, screenHeight / 2 + 100, 170, 40};
    Rectangle restartButton = {screenWidth / 2 - 80, screenHeight / 2 + 50, 170, 40};
    Rectangle backButton = {screenWidth / 2 - 80, screenHeight / 2 + 120, 170, 40};

    loadHighScore();

    bool showMenu = true;
    bool soundOn = true;

    while (!WindowShouldClose()) {
        
        // Main Menu
        if (showMenu) {
            // Draw buttons
            BeginDrawing();
            ClearBackground(BLACK);
            // DrawTextureEx(background, (Vector2){0, 0}, 0, 1.0, WHITE);
            DrawTexture(backgroundTexture, 0, 0, WHITE);

            // DrawText("Dino Run", screenWidth / 2 - MeasureText("Dino Run", 60) / 2, screenHeight / 7, 60, WHITE);

            // // Set button dimensions
            // const int buttonWidth = 200;
            // const int buttonHeight = 50; // Maintained button height for consistency
            // const int buttonSpacing = 10; // Reduced button spacing for minimal separation

            // // Calculate button positions
            // playButton.x = screenWidth / 2 - buttonWidth / 2;
            // playButton.y = screenHeight / 2 - buttonHeight - buttonSpacing; // Slight adjustment to button spacing

            // exitButton.x = screenWidth / 2 - buttonWidth / 2;
            // exitButton.y = screenHeight / 2 + buttonSpacing; // Slight adjustment to button spacing

            // // Draw buttons
            // DrawRectangleRec(playButton, GREEN);
            // DrawRectangleLinesEx(playButton, 2, WHITE);
            // DrawText("Play Game", playButton.x + buttonWidth / 5, playButton.y + buttonHeight / 4, 20, DARKGRAY);

            // DrawRectangleRec(exitButton, MAROON);
            // DrawRectangleLinesEx(exitButton, 2, WHITE);
            // DrawText("Exit", exitButton.x + buttonWidth / 3.5, exitButton.y + buttonHeight / 4, 20, LIGHTGRAY);



            //Assume screenWidth and screenHeight are variables representing the screen dimensions.
            //DrawText for "Dino Run"
            DrawText("Dino Run", screenWidth / 2 - MeasureText("Dino Run", 60) / 2, screenHeight / 6, 60, WHITE);

            // Set button dimensions
            const int buttonWidth = 200;
            const int buttonHeight = 40;
            const int buttonSpacing = 20;

            // Calculate button positions
            playButton.x = screenWidth / 2 - buttonWidth / 2;
            playButton.y = screenHeight / 2 - buttonHeight * 2 - buttonSpacing;

            highScoreButton.x = screenWidth / 2 - buttonWidth / 2;
            highScoreButton.y = screenHeight / 2 - buttonHeight - buttonSpacing / 2;

            soundButton.x = screenWidth / 2 - buttonWidth / 2;
            soundButton.y = screenHeight / 2 + buttonSpacing / 2;

            exitButton.x = screenWidth / 2 - buttonWidth / 2;
            exitButton.y = screenHeight / 2 + buttonHeight + buttonSpacing;

            // Draw buttons
            DrawRectangleRec(playButton, GREEN);
            DrawRectangleLinesEx(playButton, 2, WHITE);
            DrawText("Play Game", playButton.x + buttonWidth / 5, playButton.y + buttonHeight / 4, 20, DARKGRAY);

            // DrawRectangleRec(highScoreButton, GOLD);
            // DrawRectangleLinesEx(highScoreButton, 2, WHITE);
            // DrawText("High Score", highScoreButton.x + buttonWidth / 7, highScoreButton.y + buttonHeight / 4, 20, DARKBROWN);

            // DrawRectangleRec(soundButton, SKYBLUE);
            // DrawRectangleLinesEx(soundButton, 2, WHITE);
            // DrawText("Sound", soundButton.x + buttonWidth / 4.5, soundButton.y + buttonHeight / 4, 20, DARKBLUE);

            DrawRectangleRec(exitButton, MAROON);
            DrawRectangleLinesEx(exitButton, 2, WHITE);
            DrawText("Exit", exitButton.x + buttonWidth / 3.5, exitButton.y + buttonHeight / 4, 20, LIGHTGRAY);

            // DrawRectangleLinesEx(soundonButton, 2, LIGHTGRAY);  // Border width: 2, Button color: LIGHTGRAY
            // DrawText("On", soundonButton.x + buttonWidth / 3.5, soundonButton.y + buttonHeight / 4, 20, WHITE);

            // DrawRectangleLinesEx(soundoffButton, 2, LIGHTGRAY);  // Border width: 2, Button color: LIGHTGRAY
            // DrawText("Off", soundoffButton.x + buttonWidth / 3.5, soundoffButton.y + buttonHeight / 4, 20, WHITE);

            EndDrawing();

            // Check button clicks
            if (CheckCollisionPointRec(GetMousePosition(), playButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                showMenu = false;
                // Initialize game variables here
                score = 0;
            } else if (CheckCollisionPointRec(GetMousePosition(), highScoreButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                // Show high score logic here
                // You can use saved high score variable
            } else if (CheckCollisionPointRec(GetMousePosition(), soundButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                soundOn = !soundOn;
                // Toggle sound logic here
            } else if (CheckCollisionPointRec(GetMousePosition(), exitButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                CloseWindow();
                return 0;
            }
        } 
        else 
        {
            // Game Logic
            Sprite dino;
            dino.tex = LoadTexture("dino.png");
            dino.frames = 3;
            dino.currentFrame = 0;
            dino.actualWidth = 64;
            dino.actualHeight = 64;
            dino.pos = (Vector2){150, screenHeight * 0.7 - dino.actualHeight};
            for (int i = 0; i < dino.frames; i++) {
                dino.srect[i] = (Rectangle){dino.actualWidth * i, 0, dino.actualWidth, dino.actualHeight};
            }
            dino.time = 0;
            dino.animationTime = 0.16;

            Sprite dinobird;
            dinobird.tex = LoadTexture("dinobird.png");
            dinobird.frames = 4;
            dinobird.currentFrame = 0;
            dinobird.actualWidth = 32;
            dinobird.actualHeight = 18;
            dinobird.pos = (Vector2){700, screenHeight * 0.6};
            for (int i = 0; i < dinobird.frames; i++) {
                dinobird.srect[i] = (Rectangle){dinobird.actualWidth * i, 0, dinobird.actualWidth, dinobird.actualHeight};
            }
            dinobird.time = 0;
            dinobird.animationTime = 0.07;

            Cactus cactus;
            cactus.tex = LoadTexture("cactus.png");
            cactus.size = sizeof(cactus.srect) / sizeof(cactus.srect[0]);
            cactus.actualWidth = cactus.actualHeight = 64;
            cactus.pos[0] = (Vector2){screenWidth, screenHeight * 0.7 - cactus.actualHeight};
            cactus.srect[0] = (Rectangle){(rand() % 3) * cactus.actualWidth, 0, cactus.actualWidth, cactus.actualHeight};
            for (int i = 1; i < cactus.size; i++) {
                cactus.pos[i] = (Vector2){cactus.pos[i - 1].x + rand() % screenWidth + screenWidth / 2,
                                          screenHeight * 0.7 - cactus.actualHeight};
                cactus.srect[i] = (Rectangle){(rand() % 3) * cactus.actualWidth, 0, cactus.actualWidth, cactus.actualHeight};
            }
            cactus.speed = 9;

            // variables
            float jump_v = 24;
            float dv = jump_v;
            float dg = 2.2;

            int birdSpeed = cactus.speed;
            long double speedUpTime = 0;
            long double birdFlyTime = 0;
            long double scoreUpdateTime = 0;

            bool jump = false;
            bool birdfly = false;
            bool gameOver = false;

            while (!WindowShouldClose() && !showMenu) {
                // Process events
                if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
                    jump = true;
                }

                // Update
                if (!gameOver) {
                    sprite_animate(&dino);
                    sprite_animate(&dinobird);

                    // Jumping
                    if (jump) {
                        dino.pos.y -= dv;
                        dv -= dg;

                        if (dino.pos.y > screenHeight * 0.7 - dino.actualHeight) {
                            dino.pos.y = screenHeight * 0.7 - dino.actualHeight;
                            dv = jump_v;
                            jump = false;
                        }
                        // Play jump sound when the dino jumps
                        PlaySound(jumpSound);
                    }

                    if (birdfly) {
                        birdfly = dinobird_fly(&dinobird, birdSpeed);
                        birdFlyTime = 0;
                    }

                    for (int i = 0; i < cactus.size; i++) {
                        if (cactus.pos[i].x < -cactus.actualWidth * 2) {
                            continue;
                        }
                        cactus.pos[i].x -= cactus.speed;
                    }

                    gameSpeed += 0.1 * GetFrameTime(); // Increase speed by 0.1 units per second

                    // Speeding up the game
                    speedUpTime += GetFrameTime();
                    if (speedUpTime > 1.0) {
                        cactus.speed += 1.2;
                        speedUpTime = 0;
                    }
                    

                    birdFlyTime += GetFrameTime();
                    if (birdFlyTime > 4) {
                        birdSpeed = cactus.speed + 1;
                        birdfly = true;
                    }

                    scoreUpdateTime += GetFrameTime();
                    if (scoreUpdateTime >= 1.0) { // Update the score every 1 second
                        score++;
                        scoreUpdateTime = 0; // Reset the timer
                    }

                    if (score > highScore) {
                        highScore = score;
        
                        // Save the high score to a file
                        FILE *file = fopen("highscore.txt", "w");
                        if (file != NULL) {
                            fprintf(file, "%d", highScore);
                            fclose(file);
                        }
                    }

                    if (CheckCollisionPointRec(GetMousePosition(), highScoreButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        // Display the highest score
                        printf("High Score: %d\n", highScore);
                    }

                    // Collision detection
                    for (int i = 0; i < cactus.size; i++) {
                        if (cactus.pos[i].x > 0 && cactus.pos[i].x < screenWidth) {
                            if (CheckCollisionRecs((Rectangle){dino.pos.x, dino.pos.y, dino.actualWidth, dino.actualHeight},
                                                  (Rectangle){cactus.pos[i].x, cactus.pos[i].y, cactus.actualWidth, cactus.actualHeight})) {
                                gameOver = true;
                                // Play game over sound when the game is over
                                PlaySound(gameOverSound);
                            }
                        }
                    }

                    if (birdfly) {
                        if (CheckCollisionRecs((Rectangle){dino.pos.x, dino.pos.y, dino.actualWidth, dino.actualHeight},
                                               (Rectangle){dinobird.pos.x, dinobird.pos.y, dinobird.actualWidth, dinobird.actualHeight})) {
                            gameOver = true;
                            // Play game over sound when the game is over
                            PlaySound(gameOverSound);
                        }
                    }
                }

                // Draw
                BeginDrawing();
                ClearBackground(WHITE);
                DrawLineEx((Vector2){0, screenHeight * 0.7}, (Vector2){screenWidth, screenHeight * 0.7}, 5, RED);

                DrawTextureRec(dino.tex, dino.srect[dino.currentFrame], dino.pos, WHITE);

                if (birdfly) {
                    DrawTextureRec(dinobird.tex, dinobird.srect[dinobird.currentFrame], dinobird.pos, WHITE);
                }

                for (int i = 0; i < cactus.size; i++) {
                    if (cactus.pos[i].x > -cactus.actualWidth && cactus.pos[i].x < screenWidth) {
                        DrawTextureRec(cactus.tex, cactus.srect[i], cactus.pos[i], WHITE);
                    }
                }

                DrawText(TextFormat("Score: %04i", score), 10, 10, 20, DARKGRAY);

                if (gameOver) {
                    DrawText("GAME OVER", screenWidth * 0.35, screenHeight * 0.3, 60, RED);

                    // Draw buttons
                    const int buttonWidth = 170;
                    const int buttonHeight = 40;
                    const int buttonSpacing = 20; // Adjust this value for proper spacing

                    // Calculate button positions
                    restartButton.x = screenWidth / 2 - buttonWidth / 2;
                    restartButton.y = screenHeight / 2 - buttonHeight - buttonSpacing / 2;

                    backButton.x = screenWidth / 2 - buttonWidth / 2;
                    backButton.y = screenHeight / 2 + buttonSpacing / 2;

                    DrawRectangleRec(restartButton, GRAY);  // Filled color
                    DrawRectangleLinesEx(restartButton, 2, WHITE);  // Border
                    DrawText("Restart", restartButton.x + restartButton.width / 2 - MeasureText("Restart", 20) / 2, restartButton.y + restartButton.height / 2 - 10, 20, WHITE);

                    DrawRectangleRec(backButton, DARKBLUE);  // Filled color
                    DrawRectangleLinesEx(backButton, 2, WHITE);  // Border
                    DrawText("Menu", backButton.x + backButton.width / 2 - MeasureText("Menu", 20) / 2, backButton.y + backButton.height / 2 - 10, 20, WHITE);


                    // Check button clicks
                    if (CheckCollisionPointRec(GetMousePosition(), restartButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        // Reset game variables and restart
                        score = 0;
                        gameOver = false;
                        jump = false;
                        birdfly = false;

                        dino.pos = (Vector2){150, screenHeight * 0.7 - dino.actualHeight};

                        for (int i = 0; i < cactus.size; i++) {
                            cactus.pos[i] = (Vector2){screenWidth + i * screenWidth / 2, screenHeight * 0.7 - cactus.actualHeight};
                            cactus.srect[i] = (Rectangle){(rand() % 3) * cactus.actualWidth, 0, cactus.actualWidth, cactus.actualHeight};
                        }

                        cactus.speed = 9;
                        birdSpeed = cactus.speed;
                        speedUpTime = 0;
                        birdFlyTime = 0;
                        scoreUpdateTime = 0;
                    } else if (CheckCollisionPointRec(GetMousePosition(), backButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        // Go back to the main menu
                        showMenu = true;
                        score = 0; // Reset score
                    }
                    else if (CheckCollisionPointRec(GetMousePosition(), exitButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                        CloseWindow();
                        return 0;
                    }
                    // Play game over sound only once when the game is over
                    static bool playedGameOverSound = false;
                    if (!playedGameOverSound) {
                        PlaySound(gameOverSound);
                        playedGameOverSound = true;
                    }
                }

                EndDrawing();
            }

            // Free memory
            UnloadTexture(cactus.tex);
            UnloadTexture(dinobird.tex);
            UnloadTexture(dino.tex);

            // Unload sounds
            UnloadSound(jumpSound);
            UnloadSound(scoreSound);
            UnloadSound(gameOverSound);

        }
        if (score > highScore) {
            highScore = score;
            // Save the high score to a file
            saveHighScore();
        }
    }
        // UnloadTexture(background);
    UnloadTexture(backgroundTexture); // Unload texture
    
    // Quit
    CloseWindow();
    return 0;
}

// Function to animate the sprite
void sprite_animate(Sprite *s) {
    s->time += GetFrameTime();
    if (s->time > s->animationTime) {
        s->currentFrame++;
        s->time = 0;
    }

    if (s->currentFrame >= s->frames) {
        s->currentFrame = 0;
    }
}

// Function to control the flying behavior of the bird sprite
bool dinobird_fly(Sprite *s, int speed) {
    s->pos.x -= speed;

    if (s->pos.x < -s->actualWidth) {
        int range = GetScreenHeight() * 0.7 - GetScreenHeight() * 0.5;
        int y = rand() % range + GetScreenHeight() * 0.45;
        s->pos = (Vector2){GetScreenWidth(), y};
        return false;
    }

    return true;
}