#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include<bits/stdc++.h>

using namespace std;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int TILE_SIZE = 10;
int snakeX = SCREEN_WIDTH / 2;
int snakeY = SCREEN_HEIGHT / 2;
int foodX, foodY;
int direction = 1; 
int snakeSize = 1;
int snakeXHistory[SCREEN_WIDTH * SCREEN_HEIGHT];
int snakeYHistory[SCREEN_WIDTH * SCREEN_HEIGHT];
bool gameOver = 0;

int delay=120;
int score=0;

SDL_Window* window;
SDL_bool bordered=SDL_TRUE;
SDL_Renderer* renderer;
TTF_Font* font =nullptr;
SDL_Surface* surface;
SDL_Texture* texture;

void PlaceFood() {
    int maxX = (SCREEN_WIDTH / TILE_SIZE);
    int maxY = (SCREEN_HEIGHT / TILE_SIZE) ;
    bool regenerate=1;
    while(regenerate)
    {
        int pass1,pass2;
    foodX = (rand() % maxX) * TILE_SIZE;
    foodY = (rand() % maxY) * TILE_SIZE;

     for (int i = 0; i < snakeSize; ++i) {
        if ((foodX == snakeXHistory[i] && foodY == snakeYHistory[i]) || foodY<70 )
            regenerate=1;
        else regenerate=0;
     }
    }
   
}

void CheckCollision() {
    if (snakeX < 0 || snakeY < 55 || snakeX >= SCREEN_WIDTH|| snakeY >= SCREEN_HEIGHT)
       gameOver=1;

    for (int i = 0; i < snakeSize; ++i) {
        if (snakeX == snakeXHistory[i] && snakeY == snakeYHistory[i])
            gameOver=1;

    }

}

void process_input() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT)
            exit(0);
        else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_UP:
                    direction = 0;
                    break;
                case SDLK_RIGHT:
                    direction = 1;
                    break;
                case SDLK_DOWN:
                    direction = 2;
                    break;
                case SDLK_LEFT:
                    direction = 3;
                    break;
                case SDLK_r:
                    if (gameOver) {
                        score = 0;
                        snakeSize=1;
                        direction=1;
                        delay = 120;
                        gameOver = 0;
                        snakeX = SCREEN_WIDTH / 2;
                       snakeY = SCREEN_HEIGHT / 2;
                    }
                    break;
            }
        }
    }
}

void Update() {
    if (gameOver) {
        return; 
    }
    int headX = snakeX;
    int headY = snakeY;

    switch (direction) {
        case 0:
            headY -= TILE_SIZE;
            break;
        case 1:
            headX += TILE_SIZE;
            break;
        case 2:
            headY += TILE_SIZE;
            break;
        case 3:
            headX -= TILE_SIZE;
            break;
    }

    snakeXHistory[0] = snakeX;
    snakeYHistory[0] = snakeY;

    snakeX = headX;
    snakeY = headY;

    if (headX == foodX && headY == foodY) {
        PlaceFood();
        snakeSize++;
        score+=10;
        delay-=5;
    }

    for (int i = snakeSize - 1; i > 0; --i) {
        snakeXHistory[i] = snakeXHistory[i - 1];
        snakeYHistory[i] = snakeYHistory[i - 1];
    }
}

void Render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect borderup = {0,60 ,SCREEN_WIDTH ,1 };
    SDL_RenderFillRect(renderer, &borderup);
    

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    for (int i = 0; i < snakeSize; ++i) {
        SDL_Rect rect = {snakeXHistory[i], snakeYHistory[i], TILE_SIZE, TILE_SIZE};
        SDL_RenderFillRect(renderer, &rect);
    }

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_Rect foodRect = {foodX, foodY, TILE_SIZE, TILE_SIZE};
    SDL_RenderFillRect(renderer, &foodRect);

    SDL_Color textColor = {255, 255, 255, 255};
    std::string scoreText = "Score: " + std::to_string(score);
    surface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    SDL_Rect scoreRect = {SCREEN_WIDTH / 2 - 50, 0 , 100, 50};
    SDL_RenderCopy(renderer, texture, nullptr, &scoreRect);
    SDL_DestroyTexture(texture);

    if (gameOver) {
        SDL_Color gameOverColor = {255, 0, 0, 255};
        std::string gameOverText = "Game Over!";
        surface = TTF_RenderText_Solid(font, gameOverText.c_str(), gameOverColor);
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect gameOverRect = {SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2-15 , 300, 30};
        SDL_RenderCopy(renderer, texture, nullptr, &gameOverRect);
        SDL_DestroyTexture(texture);
        SDL_Color RedoColor = {0, 255, 0, 255};
        std::string RedoText = "Press 'R' to Retry";
        surface = TTF_RenderText_Solid(font, RedoText.c_str(), RedoColor);
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_Rect gameOverRect1 = {SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 + 15, 300, 30};
        SDL_RenderCopy(renderer, texture, nullptr, &gameOverRect1);
        SDL_DestroyTexture(texture);
    }

    SDL_RenderPresent(renderer);
}

void setup()
{
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT,0);
    void SDL_SetWindowBordered(SDL_Window * window,
                           SDL_bool bordered);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    TTF_Init();
    font = TTF_OpenFont("font/Zebulon.otf", 24);

    PlaceFood();
}

void destroyWindow()
{
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


int main(int argc, char *argv[]) {

    setup();

    while (true) {
        process_input();
        Update();

        Render();

        SDL_Delay(delay);

        CheckCollision();
    }

    destroyWindow();  

    return 0;
}