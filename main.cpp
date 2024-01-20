#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include<bits/stdc++.h>

using namespace std;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int GRID_SIZE = 20;
int consumeCount=0;
bool bonus=false;

SDL_Window* window;
SDL_Renderer* renderer;
TTF_Font* font;

std::vector<int> snakeX, snakeY;
std::vector<int> obstacleX, obstacleY;
int foodX, foodY,bonusfoodX, bonusfoodY;
int score = 0;
int delay = 120;
long long int prevtime,currtime;

int direction =3;

bool gameOver = false;
bool obstacleMovingRight = true;

void handleInput() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            exit(0);
        } else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_UP:
                    if (direction != 1) direction = 0;
                    break;
                case SDLK_DOWN:
                    if (direction != 0) direction = 1;
                    break;
                case SDLK_LEFT:
                    if (direction != 3) direction = 2;
                    break;
                case SDLK_RIGHT:
                    if (direction != 2) direction = 3;
                    break;
                case SDLK_r:
                    direction=4;
                    break;
             }
        }
    }
}

void spawnFood() {
    int maxX = (SCREEN_WIDTH / GRID_SIZE);
    int maxY = (SCREEN_HEIGHT / GRID_SIZE) ;
    bool regenerate=1;
    while(regenerate)
    {
    foodX = (rand() % maxX) * GRID_SIZE;
    foodY = (rand() % maxY) * GRID_SIZE;

     for (int i = 0; i < snakeX.size(); ++i) {
        if ((foodX == snakeX[i] && foodY == snakeY[i]) || foodY<41 || foodY>SCREEN_HEIGHT-21)
            regenerate=1;
        else regenerate=0;
     }
    }
}

void spawnbonusFood() {
    int maxX = (SCREEN_WIDTH / GRID_SIZE);
    int maxY = (SCREEN_HEIGHT / GRID_SIZE) ;
    bool bonusregenerate=1;
    while(bonusregenerate)
    {
    bonusfoodX = (rand() % maxX) * GRID_SIZE;
    bonusfoodY = (rand() % maxY) * GRID_SIZE;

     for (int i = 0; i < snakeX.size(); ++i) {
        if ((bonusfoodX == snakeX[i] && bonusfoodY == snakeY[i]) || bonusfoodY<41 || bonusfoodY>SCREEN_HEIGHT-21)
            bonusregenerate=1;
        else bonusregenerate=0;
     }
    }
}

void update() {
    if (gameOver && direction==4) {
        score = 0;
        direction=3;
        delay = 120;
        gameOver = false;
        snakeX.clear();
        snakeY.clear();
        snakeX.push_back(SCREEN_WIDTH / 2);
        snakeY.push_back(SCREEN_HEIGHT / 2);
    }
    else if(gameOver)
        return;

    int newHeadX = snakeX.front();
    int newHeadY = snakeY.front();

    switch (direction) {
        case 0:
            newHeadY -= GRID_SIZE;
            break;
        case 1:
            newHeadY += GRID_SIZE;
            break;
        case 2:
            newHeadX -= GRID_SIZE;
            break;
        case 3:
            newHeadX += GRID_SIZE;
            break;
    }

    newHeadX = (newHeadX + SCREEN_WIDTH) % SCREEN_WIDTH;
    
    if(newHeadY<=41 || newHeadY>=SCREEN_HEIGHT-20)
           gameOver=true;

    snakeX.insert(snakeX.begin(), newHeadX);
    snakeY.insert(snakeY.begin(), newHeadY);

    
    for (int i = 1; i < snakeX.size(); ++i) {
        if (newHeadX == snakeX[i] && newHeadY == snakeY[i]) {
            gameOver = true;
        }
    }

    if (newHeadX == foodX && newHeadY == foodY) {
        score += 10;
        consumeCount++;
        spawnFood();
    } 
    else {
        snakeX.pop_back();
        snakeY.pop_back();
    }
    if(consumeCount%5==0 && consumeCount>0)
    {
        prevtime=SDL_GetTicks();
        consumeCount=0;
        spawnbonusFood();
        bonus=true;
    }
    currtime=SDL_GetTicks();
    if(currtime-prevtime>=4000 && bonus==true)
    bonus =false;
    if(bonus==true && newHeadX == bonusfoodX && newHeadY==bonusfoodY)
        {
            score+=20;
            bonus=false;
        }
    
}

void render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect borderup = {0,40,SCREEN_WIDTH ,1 };
    SDL_RenderFillRect(renderer, &borderup);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect borderlow = {0,SCREEN_HEIGHT-2,SCREEN_WIDTH ,2};
    SDL_RenderFillRect(renderer, &borderlow);
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_Rect rect = {snakeX[0], snakeY[0], GRID_SIZE, GRID_SIZE};
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 1; i < snakeX.size(); ++i) {
        SDL_Rect rect = {snakeX[i], snakeY[i], GRID_SIZE, GRID_SIZE};
        SDL_RenderFillRect(renderer, &rect);
    }

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_Rect foodRect = {foodX, foodY, GRID_SIZE, GRID_SIZE};
    SDL_RenderFillRect(renderer, &foodRect);

    if(bonus==true)
    {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect bonusfoodRect = {bonusfoodX, bonusfoodY, GRID_SIZE, GRID_SIZE};
    SDL_RenderFillRect(renderer, &bonusfoodRect);
    }

    SDL_Color textColor = {255, 255, 255, 255};
    std::string scoreText = "Score: " + std::to_string(score);
    SDL_Surface* surface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_Rect scoreRect = {SCREEN_WIDTH/2-50, 5, 100, 30};
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
    window = SDL_CreateWindow("Snake Game with Moving Obstacles", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    TTF_Init();
    font = TTF_OpenFont("font/Zebulon Bold.otf", 24);

    snakeX.push_back(SCREEN_WIDTH / 2);
    snakeY.push_back(SCREEN_HEIGHT / 2);

    spawnFood();
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
        handleInput();
        update();
        render();

        SDL_Delay(delay); 
    }
    
    destroyWindow();

    return 0;
}