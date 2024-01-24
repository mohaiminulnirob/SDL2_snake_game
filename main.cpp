#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include<bits/stdc++.h>

using namespace std;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int GRID_SIZE = 20;
int NEWGRID;
int consumeCount=0;
bool bonus=false;
bool start=false;

SDL_Window* window;
SDL_Renderer* renderer;
TTF_Font* font;

std::vector<int> snakeX, snakeY;
std::vector<int> obstacleX, obstacleY;
int foodX, foodY,bonusfoodX, bonusfoodY;
int score = 0,highscore=0;
int delay =120;
long long int prevtime,currtime;

int direction =3,level;

bool gameOver = false;
int obstacle1X=SCREEN_WIDTH-80,obstacle1Y=60,obstacle1W=80,obstacle2X=SCREEN_WIDTH-20,obstacle2Y=80,obstacle2H=60;
int obstacle3X=SCREEN_WIDTH-80,obstacle3Y=SCREEN_HEIGHT-40,obstacle4X=SCREEN_WIDTH-20,obstacle4Y=SCREEN_HEIGHT-100,obstacle4H=60;
int obstacle5X=SCREEN_WIDTH/2-60,obstacle5Y=SCREEN_HEIGHT/2-80,obstacle5W=120,obstacle6X=SCREEN_WIDTH/2-60,obstacle6Y=SCREEN_HEIGHT/2+80,obstacle6W=120;
int obsDirection=1;

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
                case SDLK_1:
                    if(start==false)
                    {
                        level=1;
                        start=true;
                    }
                    break;
                case SDLK_2:
                    if(start==false)
                    {
                        level=2;
                        start=true;
                    }
                    break;
                case SDLK_3:
                    if(start==false)
                    {
                        level=3;
                        start=true;
                    }
                    break;
                case SDLK_m:
                    if(start==true && gameOver==true)
                    {
                        start=false;
                        direction=4;
                    }
                    break;
                case SDLK_ESCAPE:
                    exit(0);
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
        if ((foodX == snakeX[i] && foodY == snakeY[i]) || foodY<=60 || foodY>=SCREEN_HEIGHT-40 || foodX>=SCREEN_WIDTH-100)
            regenerate=1;
        else if
       (
       foodX>=obstacle1X-20 && foodY>=obstacle1Y && foodY<=obstacle1Y+20 ||
       foodX>=obstacle2X-20 && foodY>=obstacle2Y-20 && foodY<=obstacle2Y+obstacle2H ||
       foodX>=obstacle3X-20 && foodY>=obstacle3X-20 && foodY<obstacle3X+20||
       foodX>=obstacle4X-20 && foodY>=obstacle4Y-20 && foodY<=obstacle4Y+60 ||
       foodX>=obstacle5X-20 && foodX<=obstacle5X+obstacle5W && foodY>=obstacle5Y-20 && foodY<=obstacle5Y+20 ||
       foodX>=obstacle6X-20 && foodX<=obstacle6X+obstacle6W && foodY>=obstacle6Y-20 && foodY<=obstacle6Y+20 
       )
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
        if ((bonusfoodX == snakeX[i] && bonusfoodY == snakeY[i]) || bonusfoodY<=60 || bonusfoodY>=SCREEN_HEIGHT-40 || bonusfoodX>=SCREEN_WIDTH-100)
            bonusregenerate=1;
        else if
       (
       bonusfoodX>=obstacle1X-20 && bonusfoodY>=obstacle1Y && bonusfoodY<=obstacle1Y+20 ||
       bonusfoodX>=obstacle2X-20 && bonusfoodY>=obstacle2Y-20 && bonusfoodY<=obstacle2Y+obstacle2H ||
       bonusfoodX>=obstacle3X-20 && bonusfoodY>=obstacle3X-20 && bonusfoodY<obstacle3X+20||
       bonusfoodX>=obstacle4X-20 && bonusfoodY>=obstacle4Y-20 && bonusfoodY<=obstacle4Y+60 ||
       bonusfoodX>=obstacle5X-20 && bonusfoodX<=obstacle5X+obstacle5W && bonusfoodY>=obstacle5Y-20 && bonusfoodY<=obstacle5Y+20 ||
       bonusfoodX>=obstacle6X-20 && bonusfoodX<=obstacle6X+obstacle6W && bonusfoodY>=obstacle6Y-20 && bonusfoodY<=obstacle6Y+20 
       )
          bonusregenerate=1;
        else bonusregenerate=0;
     }
    }
}

void moveObstacle()
{
    if(obstacle2Y>=SCREEN_HEIGHT/2+20-60)
    obsDirection=2;
    else if(obstacle2Y<=80)
    obsDirection=1;
    if(obsDirection==1)
    {
        obstacle1Y+=GRID_SIZE-10;
        obstacle2Y+=GRID_SIZE-10;
        obstacle3Y-=GRID_SIZE-10;
        obstacle4Y-=GRID_SIZE-10;
    }
    else
    {
    obstacle1Y-=GRID_SIZE-10;
    obstacle2Y-=GRID_SIZE-10;
    obstacle3Y+=GRID_SIZE-10;
    obstacle4Y+=GRID_SIZE-10;
    }
}

void update() {
    if (gameOver && direction==4) {
        score = 0;
        direction=3;
        delay = 120;
        consumeCount=0;
        gameOver = false;
        snakeX.clear();
        snakeY.clear();
        int x=-20;
       for(int i=0;i<3;i++)
        {
        snakeX.push_back(SCREEN_WIDTH / 2+x);
        snakeY.push_back(SCREEN_HEIGHT / 2+x);
        x+=20;
        }

    }
    else if(gameOver)
    {
        if(score>=highscore)
           highscore=score;
        return;
    }
    
    if(level==3)
    moveObstacle();

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
    

    snakeX.insert(snakeX.begin(), newHeadX);
    snakeY.insert(snakeY.begin(), newHeadY);

     if(newHeadY<60 || newHeadY>SCREEN_HEIGHT-40 )
           gameOver=true;
    else if(level==2 || level==3)
    {
    if(
       newHeadX>obstacle1X-20 && newHeadY>obstacle1Y-20 && newHeadY<obstacle1Y+20 ||
       newHeadX>obstacle2X-20 && newHeadY>obstacle2Y-20 && newHeadY<obstacle1Y+obstacle2H ||
       newHeadX>obstacle3X-20 && newHeadY>obstacle3Y-20 && newHeadY<obstacle3Y+20 ||
       newHeadX>obstacle4X-20 && newHeadY>obstacle4Y-20 && newHeadY<obstacle4Y+obstacle4H ||
       newHeadX>obstacle5X-20 && newHeadX<obstacle5X+obstacle5W && newHeadY>obstacle5Y-20 && newHeadY<obstacle5Y+20 ||
       newHeadX>obstacle6X-20 && newHeadX<obstacle6X+obstacle6W && newHeadY>obstacle6Y-20 && newHeadY<obstacle6Y+20
       )
           gameOver=true;
    }

    
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
        NEWGRID=GRID_SIZE;
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

    if(start==false)
    {
    SDL_Surface* surface = SDL_LoadBMP("snake_pic.bmp");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    SDL_Rect snakerect = {SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2-200 , 200, 150};
    SDL_RenderCopy(renderer, texture, nullptr, &snakerect);
    SDL_DestroyTexture(texture);

    SDL_Color startcolour = {75, 255, 255, 255};
    std::string start = "START SNAKE GAME";
    surface = TTF_RenderText_Solid(font, start.c_str(),startcolour );
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    SDL_Rect startrect = {SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2-30 , 400, 40};
    SDL_RenderCopy(renderer, texture, nullptr, &startrect);
    SDL_DestroyTexture(texture);

    SDL_Color level1colour = {100, 100, 255, 0};
    std::string level1 = "PRESS 1 FOR LEVEL 1";
    surface = TTF_RenderText_Solid(font, level1.c_str(),level1colour );
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    SDL_Rect level1rect = {SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2+20 , 300, 30};
    SDL_RenderCopy(renderer, texture, nullptr, &level1rect);
    SDL_DestroyTexture(texture);

    SDL_Color level2colour = {100, 100, 255, 0};
    std::string level2 = "PRESS 2 FOR LEVEL 2";
    surface = TTF_RenderText_Solid(font, level2.c_str(),level2colour );
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    SDL_Rect level2rect = {SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2+55 , 300, 30};
    SDL_RenderCopy(renderer, texture, nullptr, &level2rect);
    SDL_DestroyTexture(texture);

    SDL_Color level3colour = {100, 100, 255, 0};
    std::string level3 = "PRESS 3 FOR LEVEL 3";
    surface = TTF_RenderText_Solid(font, level3.c_str(),level1colour );
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    SDL_Rect level3rect = {SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2+90 , 300, 30};
    SDL_RenderCopy(renderer, texture, nullptr, &level3rect);
    SDL_DestroyTexture(texture);

     SDL_Color esccolour = {255, 0, 0, 255};
    std::string esc = "PRESS ESC TO EXIT";
    surface = TTF_RenderText_Solid(font, esc.c_str(),esccolour );
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    SDL_Rect escrect = {SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2+125 , 300, 30};
    SDL_RenderCopy(renderer, texture, nullptr, &escrect);
    SDL_DestroyTexture(texture);

    SDL_RenderPresent(renderer);
    return;
    }

    if (gameOver) {
        SDL_Delay(delay);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_Color textColor = {255, 255, 255, 255};
        std::string scoreText = "SCORE: " + std::to_string(score);
        SDL_Surface* surface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        SDL_Rect scoreRect = {SCREEN_WIDTH/2-50, 5, 100, 30};
        SDL_RenderCopy(renderer, texture, nullptr, &scoreRect);
        SDL_DestroyTexture(texture);
        
        SDL_Color highscoreColor = {100, 255, 50, 255};
        std::string highscoreText = "HIGH SCORE: "+ std::to_string(highscore);
        surface = TTF_RenderText_Solid(font, highscoreText.c_str(), highscoreColor);
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        SDL_Rect highscoreRect = {SCREEN_WIDTH / 2 - 100, 45 , 200, 40};
        SDL_RenderCopy(renderer, texture, nullptr, &highscoreRect);
        SDL_DestroyTexture(texture);

        if(score==highscore && score>0)
        {
            surface = SDL_LoadBMP("victory_pic.bmp");
            texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
            SDL_Rect victoryrect = {SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2-150 , 150, 150};
            SDL_RenderCopy(renderer, texture, nullptr, &victoryrect);
            SDL_DestroyTexture(texture);
        }

        SDL_Color gameOverColor = {255, 0, 0, 255};
        std::string gameOverText = "GAME OVER!";
        surface = TTF_RenderText_Solid(font, gameOverText.c_str(), gameOverColor);
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        SDL_Rect gameOverRect = {SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2-15 , 300, 30};
        SDL_RenderCopy(renderer, texture, nullptr, &gameOverRect);
        SDL_DestroyTexture(texture);

        SDL_Color RedoColor = {50, 150, 100, 255};
        std::string RedoText = "PRESS 'R' TO RETRY";
        surface = TTF_RenderText_Solid(font, RedoText.c_str(), RedoColor);
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        SDL_Rect redoRect = {SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 + 15, 300, 30};
        SDL_RenderCopy(renderer, texture, nullptr, &redoRect);
        SDL_DestroyTexture(texture);

        SDL_Color MenuColor = {100, 255, 100, 255};
        std::string MenuText = "PRESS 'M' FOR MAIN MENU";
        surface = TTF_RenderText_Solid(font, MenuText.c_str(), MenuColor);
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        SDL_Rect MenuRect = {SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 + 50, 300, 30};
        SDL_RenderCopy(renderer, texture, nullptr, &MenuRect);
        SDL_DestroyTexture(texture);
        SDL_RenderPresent(renderer);
        return;
    }

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect borderup = {0,40,SCREEN_WIDTH ,20 };
    SDL_RenderFillRect(renderer, &borderup);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect borderlow = {0,SCREEN_HEIGHT-20,SCREEN_WIDTH ,20};
    SDL_RenderFillRect(renderer, &borderlow);

    if(level==2 || level==3)
    {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect obstacle1rect = {obstacle1X, obstacle1Y, 80, 20};
    SDL_RenderFillRect(renderer, &obstacle1rect);
    
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect obstacle2rect = {obstacle2X, obstacle2Y, 20, 60};
    SDL_RenderFillRect(renderer, &obstacle2rect);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect obstacle3rect = {obstacle3X, obstacle3Y, 80, 20};
    SDL_RenderFillRect(renderer, &obstacle3rect);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect obstacle4rect = {obstacle4X, obstacle4Y, 20, 60};
    SDL_RenderFillRect(renderer, &obstacle4rect);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect obstacle5rect = {obstacle5X,obstacle5Y, 120, 20};
    SDL_RenderFillRect(renderer, &obstacle5rect);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect obstacle6rect = {obstacle6X, obstacle6Y, 120, 20};
    SDL_RenderFillRect(renderer, &obstacle6rect);
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_Rect rect = {snakeX[0], snakeY[0], GRID_SIZE, GRID_SIZE};
    SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderDrawColor(renderer, 50, 150, 150, 255);
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
    NEWGRID+=4;
    if(NEWGRID>24)
      NEWGRID=GRID_SIZE;
    SDL_Rect bonusfoodRect = {bonusfoodX, bonusfoodY, NEWGRID, NEWGRID};
    SDL_RenderFillRect(renderer, &bonusfoodRect);
    }

    SDL_Color textColor = {255, 255, 255, 255};
    std::string scoreText = "SCORE: " + std::to_string(score);
    SDL_Surface* surface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    SDL_Rect scoreRect = {SCREEN_WIDTH/2-50, 5, 100, 30};
    SDL_RenderCopy(renderer, texture, nullptr, &scoreRect);
    SDL_DestroyTexture(texture);

    SDL_RenderPresent(renderer);
}

void setup()
{
   SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Snake Game by Nirob", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    TTF_Init();
    font = TTF_OpenFont("font/Zebulon Bold.otf", 24);
    int x=-20;
       for(int i=0;i<3;i++)
        {
        snakeX.push_back(SCREEN_WIDTH / 2+x);
        snakeY.push_back(SCREEN_HEIGHT / 2+x);
        x+=20;
        }


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