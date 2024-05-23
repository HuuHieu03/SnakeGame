#include "SDL.h"
#include "Graphics.h"
#include <bits/stdc++.h>
#include <windows.h>

#define S_WALL_WIDTH 760
#define S_WALL_HEIGHT 500
#define S_WIDTH 800
#define S_HEIGHT 600
#define S_FOOD_WIDTH 20
#define S_FOOD_HEIGHT 20
//40*27
using namespace std;
SDL_Point pos[40][27];
Graphics graphics;
pair<SDL_Rect,int> part;

SDL_Texture* Snake_head[5];
SDL_Texture* tail[5];
SDL_Texture* Body[5];

int startTime = 0;


int main(int argc, char** argv) {
    graphics.init();
    SDL_Texture* background_1 = graphics.loadTexture("images/Background_edited.png");
    graphics.prepareScene(background_1);
    SDL_RenderPresent(graphics.renderer);
    SDL_Delay(25);

    SDL_Event e;

    enum Direction {
        DOWN, LEFT, RIGHT, UP
    };

    bool running = true;
    int dir = 0, next_dir = 0;
    deque <pair<SDL_Rect,int>> rq;
    int score_num = 0;
    int val;
    int size;
    // speed = 1, 2, 4, 5 , 10, 20
    //
    int speed = 10;
    switch (speed){
        case 20:
            val = 1;
            break;
        case 10:
            val = 2;
            break;
        case 5:
            val = 4;
            break;
        case 4:
            val = 5;
            break;
        case 2:
            val = 10;
            break;
        case 1:
            val = 20;
            break;
    }

//set Size
    size = val;
    SDL_Rect head {600, 600, 20, 20};
    SDL_Rect apples {300, 300, S_FOOD_WIDTH, S_FOOD_HEIGHT};
    SDL_Rect golden_apples {0, 0, S_FOOD_WIDTH, S_FOOD_HEIGHT};
    SDL_Point applePos;
    SDL_Point golden_applePos;
    rq.push_front({head,dir});

    golden_applePos.x = rand() % 38+1;
    golden_applePos.y = rand() % 25+1;
    golden_apples.x = pos[golden_applePos.x][golden_applePos.y].x;
    golden_apples.y = pos[golden_applePos.x][golden_applePos.y].y;

    Snake_head[0] = graphics.loadTexture("images/snake2_D.png");
    Snake_head[3] = graphics.loadTexture("images/snake2_U.png");
    Snake_head[2] = graphics.loadTexture("images/snake2_R.png");
    Snake_head[1] = graphics.loadTexture("images/snake2_L.png");

    tail[0] = graphics.loadTexture("images/last_tail_D.png");
    tail[3] = graphics.loadTexture("images/last_tail_U.png");
    tail[1] = graphics.loadTexture("images/last_tail_L.png");
    tail[2] = graphics.loadTexture("images/last_tail_R.png");

    Body[0] = graphics.loadTexture("images/body2.png");
    Body[3] = graphics.loadTexture("images/body2.png");
    Body[1] = graphics.loadTexture("images/body.png");
    Body[2] = graphics.loadTexture("images/body.png");

//play music
//
    Mix_Music *gMusic = graphics.loadMusic("sounds\\themesong.mp3");
    graphics.play(gMusic);
    Mix_Chunk *eating_sound = graphics.loadSound("sounds\\eating.wav");

    for(int i=0;i<40;i++)
    {
        for(int j=0;j<27;j++)
        {
            pos[i][j].x=i*20;
            pos[i][j].y=j*20;
        }
    }

//    SDL_Texture* background_1 = graphics.loadTexture("images/Background_edited.png");
//    graphics.prepareScene(background_1);
    SDL_Rect Play {285, 230, 227, 50};
    SDL_Rect Exit {359, 300, 81, 50};
    int PlayGame = 0;
    bool live = false;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {running = false;}
            else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_DOWN && (dir == LEFT || dir == RIGHT))
                        next_dir = DOWN;
                else if (e.key.keysym.sym == SDLK_UP && (dir == LEFT || dir == RIGHT))
                        next_dir = UP;
                else if (e.key.keysym.sym == SDLK_LEFT && (dir == UP || dir == DOWN))
                        next_dir = LEFT;
                else if (e.key.keysym.sym == SDLK_RIGHT && (dir == UP || dir == DOWN))
                        next_dir = RIGHT;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN && PlayGame == 0) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if ((x >= Play.x) && (x <= Play.x + Play.w) && (y >= Play.y) && (y <= Play.y + Play.h)) {
                    PlayGame = 4;
                    live = true;
                }
                else if ((x >= Exit.x) && (x <= Exit.x + Exit.w) && (y >= Exit.y) && (y <= Exit.y + Exit.h)) {
                    running = false;

                }
            }
        }
        if (PlayGame && live) {
//            int Time = SDL_GetTicks() / 1000;
//            cout << '\n' << Time << '\n';
            if (next_dir != dir) {
                if ((dir == LEFT || dir == RIGHT) && head.x % 20 == 0) dir = next_dir;
                else if ((dir == UP || dir == DOWN) && head.y % 20 == 0) dir = next_dir;
            }
            switch(dir) {
                case DOWN:
                    head.y += speed;
                    break;
                case UP:
                    head.y -= speed;
                    break;
                case LEFT:
                    head.x -= speed;
                    break;
                case RIGHT:
                    head.x += speed;
                    break;

            }
            //Collision detection wth apple
            //
            if (head.x == apples.x && head.y == apples.y) {
                graphics.play(eating_sound);
                size += val;
                score_num += 10;
                applePos.x = rand() % 38+1 ;
                applePos.y = rand() % 25+1 ;
                apples.x = pos[applePos.x][applePos.y].x;
                apples.y = pos[applePos.x][applePos.y].y;
            }

            //Collision detection wth Golden apple
            //
            if (head.x == golden_apples.x && head.y == golden_apples.y) {
                graphics.play(eating_sound);
                size += val;
                score_num += 30;
                golden_applePos.x = rand() % 38+1 ;
                golden_applePos.y = rand() % 25+1 ;
                golden_apples.x = pos[golden_applePos.x][golden_applePos.y].x;
                golden_apples.y = pos[golden_applePos.x][golden_applePos.y].y;
                startTime = SDL_GetTicks() / 1000;
            }

            for_each(rq.begin(), rq.end(), [&](auto& snake_segment) {
                if(head.x == snake_segment.first.x && head.y == snake_segment.first.y) {
                    size = val;
                    score_num = 0;
                    PlayGame--;
                }
            });
            if (head.x > S_WALL_WIDTH || head.y > S_WALL_HEIGHT || head.x < 20 || head.y < 20) {
                size = val;
                score_num = 0;
                PlayGame--;
                head.x = S_WIDTH / 2;
                head.y = S_HEIGHT / 2;
            }
            //cout<<size<<"\n";
            rq.push_front({head,dir});
            while (rq.size() > size + 2)
                rq.pop_back();

     //*Draw background
            SDL_Texture* background = graphics.loadTexture("images/grey2.png");
            graphics.prepareScene(background);

    //*Draw walls
            SDL_Texture* border1 = graphics.loadTexture("images/border1.png");
            graphics.renderTexture(border1, 0, 0);
            graphics.renderTexture(border1, 780, 0);

            SDL_Texture* border = graphics.loadTexture("images/border.png");
            graphics.renderTexture(border, 0, 0);
            graphics.renderTexture(border, 0, 520);

    //Draw apple
    //
            SDL_Texture* fruit = graphics.loadTexture("images/fruit.png");
            graphics.renderTexture(fruit, apples.x, apples.y);

            //Draw body
            //
            for (int i = 1; i < rq.size(); i++) {

                    //continue;
                if (i == rq.size() - 1)
                    graphics.renderTexture(tail[rq[i].second], rq[i].first.x, rq[i].first.y);
                else
                    graphics.renderTexture(Body[rq[i].second], rq[i].first.x, rq[i].first.y);
                //SDL_RenderFillRect(graphics.renderer, &snake_segment);
            }
            graphics.renderTexture(Snake_head[dir], rq[0].first.x, rq[0].first.y);
          //SDL_RenderFillRect(renderer, &head);

        //Golden apple
        //
        int currentTime = SDL_GetTicks() / 1000;
        cout << currentTime - startTime << '\n';
        if (currentTime - startTime == 10) {
            golden_apples.x = pos[golden_applePos.x][golden_applePos.y].x;
            golden_apples.y = pos[golden_applePos.x][golden_applePos.y].y;
        }
        if (currentTime - startTime >= 10 && currentTime - startTime <= 20)
        {
            SDL_Texture* Gold = graphics.loadTexture("images/golden-apple.png");
            graphics.renderTexture(Gold, golden_apples.x, golden_apples.y);
        }
        else if (currentTime - startTime > 30) {
            startTime = SDL_GetTicks() / 1000;
        }

    //Writing
    //
            TTF_Font* font = graphics.loadFont("font/SVN-Times New Roman 2 bold.ttf", 40);
            SDL_Color color = {250, 250, 250, 0};
            string score_string = "Score: ";
            string Score_val = to_string(score_num);
            score_string += Score_val;
    //        cout << ' ' << score_num << '\n';
            SDL_Texture* Score = graphics.renderText(score_string.c_str(), font, color);
            graphics.renderTexture(Score, 100, 530);

            string life_string = "Life: ";
            string life_val = to_string(PlayGame);
            life_string += life_val;
    //        cout << ' ' << score_num << '\n';
            SDL_Texture* Life = graphics.renderText(life_string.c_str(), font, color);
            graphics.renderTexture(Life, 500, 530);

            SDL_RenderPresent(graphics.renderer);
            SDL_Delay(25);

            SDL_DestroyTexture( border1 );
            border1 = NULL;
            SDL_DestroyTexture( background );
            background = NULL;
            SDL_DestroyTexture( border );
            border = NULL;
            SDL_DestroyTexture( fruit );
            fruit = NULL;

            SDL_DestroyTexture( Score );
            TTF_CloseFont( font );
            Score = NULL;
            SDL_DestroyTexture( Life );
            Life = NULL;

        }
        else if (PlayGame == 0 && live) {
                    SDL_Rect Play_again {268, 197, 270, 128};
                    SDL_Rect Exit_2 {268, 375, 270, 128};
                   // running = false;
                    SDL_Texture* Gameover = graphics.loadTexture("images/Gameover_edited.png");
                    graphics.prepareScene(Gameover);
                    SDL_RenderPresent(graphics.renderer);
                    while (SDL_PollEvent(&e)) {
                        if (e.type == SDL_QUIT) {running = false;}
                        else if (e.type == SDL_MOUSEBUTTONDOWN) {
                            int x, y;
                            SDL_GetMouseState(&x, &y);
                            if ((x >= Play_again.x) && (x <= Play_again.x + Play_again.w) && (y >= Play_again.y) && (y <= Play_again.y + Play_again.h)) {
                                PlayGame = 3;
                                live = true;
                                startTime == SDL_GetTicks();
                            }
                            else if ((x >= Exit_2.x) && (x <= Exit_2.x + Exit_2.w) && (y >= Exit_2.y) && (y <= Exit_2.y + Exit_2.h)) {
                                running = false;
//                                break;
                            }
                        }
                    }

            }

    }
    return 0;

}
