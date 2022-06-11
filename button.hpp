#ifndef BUTTON_H
#define BUTTON_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <bits/stdc++.h>
using namespace std;

class Button
{
    public:
        SDL_Rect *rect;
        SDL_Surface *surface[2];
        int count;
        Button(string name,int x, int y){
            rect = new SDL_Rect;
            rect->w = 200;
            rect->h = 60;
            rect->x = x;
            rect->y = y;
            count = 0;
            surface[0] = SDL_LoadBMP(("./Images/"+name+"0.bmp").c_str());
            surface[1] = SDL_LoadBMP(("./Images/"+name+"1.bmp").c_str());
        }

        string control(SDL_Event &event){
            string s="";
            if(event.type == SDL_KEYDOWN){
                if(event.key.keysym.sym == SDLK_UP|| event.key.keysym.sym == SDLK_LEFT){
                    if(count==0){
                        count = 1;
                    }
                    else{
                        count = 0;
                    }
                }
                else if(event.key.keysym.sym == SDLK_DOWN|| event.key.keysym.sym == SDLK_RIGHT){
                    if(count==1){
                        count = 0;
                    }
                    else{
                        count = 1;
                    }
                }
                else if(event.key.keysym.sym == SDLK_RETURN){
                    if(count==0){
                        s="start";
                    }
                    else{
                        s="exit";
                    }
                }
            }

            cout<<count<<"\n";
            cout<<s<<"\n";
            return s;
        }

        void draw(SDL_Surface* screenSurface, SDL_Window* window){
            SDL_BlitSurface(surface[count], NULL, screenSurface, rect);
            SDL_UpdateWindowSurface(window);
        }
};

#endif
