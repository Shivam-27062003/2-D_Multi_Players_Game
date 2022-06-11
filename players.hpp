#ifndef PLAYERS_HPP
#define PLAYERS_HPP
#include  <SDL2/SDL.h>

#include <bits/stdc++.h>
#include <string>  
#include "tiling.hpp"
#include "texture.hpp"

using namespace std;

Mix_Chunk *coin; 
Mix_Chunk *meal;
Mix_Chunk *error;
Mix_Chunk  *hit;
Mix_Chunk *gLoad;
Mix_Chunk *gShot;
Mix_Chunk *vehicle;

class Player
{
    public:
		//Collision box of the dot
		SDL_Rect mBox;

		//The velocity of the dot
		int mVelX, mVelY;
        SDL_Texture *l_s[3],*r_s[3],*u_s[3],*d_s[3];
        // @@@@@@@@@@@@@@@@@@@@@@@@@
        SDL_Texture *l_a[3],*r_a[3],*u_a[3],*d_a[3];
        
        SDL_Texture *c_l,*c_r,*c_u,*c_d;
        bool visited[17]={false};
    

		//The dimensions of the dot
		static const int DOT_WIDTH = 20;
		static const int DOT_HEIGHT = 20;
        int count = 0;
        string face = "down";
        bool cycle;
        float energy;
        float health;
        bool punch;
        SDL_Texture *healthBar;
        SDL_Texture *energyBar;
        SDL_Texture *coinSack;
        LTexture statusTextTexture;
        int status;
        int dark_coin;
        bool gun;
        bool dead;
        bool fired;
        bool shot;
        bool transferCoins;
        int gun_price;
        int bullet;
        TTF_Font *font = NULL;
        
       

       

		//Maximum axis velocity of the dot
		int DOT_VEL = 1;
        Player(){}
		//Initializes the variables
		Player(SDL_Renderer* gRenderer)
        {
            //Initialize the collision box
            mBox.x = 200;
            mBox.y = 70;
            mBox.w = DOT_WIDTH;
            mBox.h = DOT_HEIGHT;
            cycle = false;
            punch=false;
            //Initialize the velocity
            mVelX = 0;
            mVelY = 0;
            loadmedia(gRenderer);
            energy = 1.0;
            health = 1.0;
            dark_coin = 0;
            coin = Mix_LoadWAV("./Sounds/coin.wav");
            meal = Mix_LoadWAV("./Sounds/food.wav");
            error = Mix_LoadWAV("./Sounds/error.wav");
            hit = Mix_LoadWAV("./Sounds/punch.wav");
            gLoad = Mix_LoadWAV("./Sounds/loadgun.wav");
            gShot = Mix_LoadWAV("./Sounds/fire.wav");
            vehicle = Mix_LoadWAV("./Sounds/car.wav");
            gun = false;
            dead = false;
            fired = false;
            shot = false;
            transferCoins=false;
            gun_price = 60;
            bullet = 0;

            SDL_Color color  = {255, 255, 255};
            font =  TTF_OpenFont("./Fonts/arial.ttf", 20);
            // loadfont(20, "arial.ttf", {255,255,255}, &statusTextTexture);
        }
        void RenderHPBar(int x, int y, int w, int h, float Percent, SDL_Color FGColor, SDL_Color BGColor, SDL_Renderer* Renderer) {
            Percent = Percent > 1.f ? 1.f : Percent < 0.f ? 0.f : Percent;
            SDL_Color old;
            SDL_GetRenderDrawColor(Renderer, &old.r, &old.g, &old.g, &old.a);
            SDL_Rect bgrect = { x, y, w, h };
            SDL_Rect renderQuad = { 0, 0, 700, 420 };
            SDL_Rect renderQuad_pos = { 16, 0, 180, 108 };
            SDL_SetRenderDrawColor(Renderer, FGColor.r, FGColor.g, FGColor.b, FGColor.a);
            int pw = (int)((float)w * Percent);
            int px = x + (w - pw);
            SDL_Rect fgrect = { x, y, pw, h };
            SDL_RenderFillRect(Renderer, &fgrect);
            SDL_RenderCopyEx(Renderer, healthBar, &renderQuad, &renderQuad_pos, 0.0, NULL, SDL_FLIP_NONE);
            SDL_SetRenderDrawColor(Renderer, old.r, old.g, old.b, old.a);
        }
        void RenderEnergyBar(int x, int y, int w, int h, float Percent, SDL_Color FGColor, SDL_Color BGColor, SDL_Renderer* Renderer) {
            Percent = Percent > 1.f ? 1.f : Percent < 0.f ? 0.f : Percent;
            SDL_Color old;
            SDL_GetRenderDrawColor(Renderer, &old.r, &old.g, &old.g, &old.a);
            SDL_Rect bgrect = { x, y, w, h };
            SDL_Rect renderQuad = { 0, 0, 700, 420 };
            SDL_Rect renderQuad_pos = { 16, 50, 180, 108 };
            SDL_SetRenderDrawColor(Renderer, FGColor.r, FGColor.g, FGColor.b, FGColor.a);
            int pw = (int)((float)w * Percent);
            int px = x + (w - pw);
            SDL_Rect fgrect = { x, y, pw, h };
            SDL_RenderFillRect(Renderer, &fgrect);
            SDL_RenderCopyEx(Renderer, energyBar, &renderQuad, &renderQuad_pos, 0.0, NULL, SDL_FLIP_NONE);
            SDL_SetRenderDrawColor(Renderer, old.r, old.g, old.b, old.a);
        }
        void RenderCoins(SDL_Renderer* Renderer) {
            
            SDL_Rect renderQuad = { 0, 0, 820, 512 }; //size of imported image
            SDL_Rect renderQuad_pos = { 1000, 50, 60, 38 };//size of redered image

            
            SDL_RenderCopyEx(Renderer, coinSack, &renderQuad, &renderQuad_pos, 0.0, NULL, SDL_FLIP_NONE);
            
        }

        

        void take_yulu(LTexture *gDotTexture){
            cycle = true;
            SDL_Texture* temp;
            if(face=="up")
                temp = c_u;
            else if(face=="down")
                temp = c_d;
            else if(face=="left")
                temp = c_l;
            else if(face=="right")
                temp = c_r;
            gDotTexture->mTexture = temp;

        }

        void leave_yulu(LTexture *gDotTexture){
            cycle = false;
            count = 0;
            SDL_Texture* temp;
            if(face=="up")
                temp = u_s[count];
            else if(face=="down")
                temp = d_s[count];
            else if(face=="left")
                temp = l_s[count];
            else if(face=="right")
                temp = r_s[count];
            gDotTexture->mTexture = temp;
        }

		//Takes key presses and adjusts the dot's velocity
		void handleEvent(SDL_Event &e,LTexture *gDotTexture)
        {
            if( e.type == SDL_KEYDOWN )
            {
                if(e.key.repeat == 0){
                //Adjust the velocity
                    switch( e.key.keysym.sym )
                    {
                        case SDLK_UP: mVelY -= DOT_VEL;face = "up"; break;
                        case SDLK_DOWN: mVelY += DOT_VEL;face = "down";break;
                        case SDLK_LEFT: mVelX -= DOT_VEL;face = "left";break;
                        case SDLK_RIGHT: mVelX += DOT_VEL;face = "right";break;
                        case SDLK_SPACE: if(energy >= 0.2){punch=true; energy-=0.2; Mix_PlayChannel(-1,hit,0);}break;
                        case SDLK_RETURN:
                            mVelX +=0;
                            if(yulu.count(pair<int,int>((mBox.x+DOT_WIDTH/2)/32,(mBox.y+ DOT_HEIGHT/2)/32)) == true && dark_coin >=5){
                                if(cycle){cycle = false;}
                                else{cycle = true; dark_coin-=5;Mix_PlayChannel(-1, vehicle , 0);}

                            }
                            else if(food.count(pair<int,int>((mBox.x+DOT_WIDTH/2)/32,(mBox.y+DOT_HEIGHT/2)/32))){
                                if(cycle){Mix_PlayChannel(-1, error, 0);}
                                else{
                                    energy = 1.0;
                                    dark_coin -= 2;
                                    Mix_PlayChannel(-1, meal, 0);
                                }
                            }
                            else if(hospital.count(pair<int,int>((mBox.x+DOT_WIDTH/2)/32,(mBox.y+DOT_HEIGHT/2)/32))){
                                if(cycle){Mix_PlayChannel(-1, error, 0);}
                                else{health = 1.0;}
                            }
                            else if(jwala.count(pair<int,int>((mBox.x+DOT_WIDTH/2)/32,(mBox.y+DOT_HEIGHT/2)/32)) == true){
                                if(cycle == true || visited[0] == true){Mix_PlayChannel(-1, error, 0);}
                                else if(!cycle && !visited[0]){
                                    dark_coin+=coin_div[0];visited[0] = true;
                                    Mix_PlayChannel(-1, coin, 0);
                                }
                            }
                            else if(ara.count(pair<int,int>((mBox.x+DOT_WIDTH/2)/32,(mBox.y+DOT_HEIGHT/2)/32)) == true){
                                if(cycle == true || visited[1] == true){Mix_PlayChannel(-1, error, 0);}
                                else if(!cycle && !visited[1]){
                                    dark_coin+=coin_div[1];visited[1] = true;
                                    Mix_PlayChannel(-1, coin, 0);
                                }
                            }
                            else if(kara.count(pair<int,int>((mBox.x+DOT_WIDTH/2)/32,(mBox.y+DOT_HEIGHT/2)/32)) == true){
                                if(cycle == true || visited[2] == true){Mix_PlayChannel(-1, error, 0);}
                                else if(!cycle && !visited[2]){
                                    dark_coin+=coin_div[2];visited[2] = true;
                                    Mix_PlayChannel(-1, coin, 0);
                                }
                            }
                            else if(nil.count(pair<int,int>((mBox.x+DOT_WIDTH/2)/32,(mBox.y+DOT_HEIGHT/2)/32)) == true){
                                if(cycle == true || visited[3] == true){Mix_PlayChannel(-1, error, 0);}
                                else if(!cycle && !visited[3]){
                                    dark_coin+=coin_div[3];visited[3] = true;
                                    Mix_PlayChannel(-1, coin, 0);
                                }
                            }
                            else if(kumaon.count(pair<int,int>((mBox.x+DOT_WIDTH/2)/32,(mBox.y+DOT_HEIGHT/2)/32)) == true){
                                if(cycle == true || visited[4] == true){Mix_PlayChannel(-1, error, 0);}
                                else if(!cycle && !visited[4]){
                                    dark_coin+=coin_div[4];visited[4] = true;
                                    Mix_PlayChannel(-1, coin, 0);
                                }
                            }
                            else if(vindy.count(pair<int,int>((mBox.x+DOT_WIDTH/2)/32,(mBox.y+DOT_HEIGHT/2)/32)) == true){
                                if(cycle == true || visited[5] == true){Mix_PlayChannel(-1, error, 0);}
                                else if(!cycle && !visited[5]){
                                    dark_coin+=coin_div[5];visited[5] = true;
                                    Mix_PlayChannel(-1, coin, 0);
                                }
                            }
                            else if(satpura.count(pair<int,int>((mBox.x+DOT_WIDTH/2)/32,(mBox.y+DOT_HEIGHT/2)/32)) == true){
                                if(cycle == true || visited[6] == true){Mix_PlayChannel(-1, error, 0);}
                                else if(!cycle && !visited[6]){
                                    dark_coin+=coin_div[6];visited[6] = true;
                                    Mix_PlayChannel(-1, coin, 0);
                                }
                            }
                            else if(shiva.count(pair<int,int>((mBox.x+DOT_WIDTH/2)/32,(mBox.y+DOT_HEIGHT/2)/32)) == true){
                                if(cycle == true || visited[7] == true){Mix_PlayChannel(-1, error, 0);}
                                else if(!cycle && !visited[7]){
                                    dark_coin+=coin_div[7];visited[7] = true;
                                    Mix_PlayChannel(-1, coin, 0);
                                }
                            }
                            else if(zanskar.count(pair<int,int>((mBox.x+DOT_WIDTH/2)/32,(mBox.y+DOT_HEIGHT/2)/32)) == true){
                                if(cycle == true || visited[8] == true){Mix_PlayChannel(-1, error, 0);}
                                else if(!cycle && !visited[8]){
                                    dark_coin+=coin_div[8];visited[8] = true;
                                    Mix_PlayChannel(-1, coin, 0);
                                }
                            }
                            else if(girnar.count(pair<int,int>((mBox.x+DOT_WIDTH/2)/32,(mBox.y+DOT_HEIGHT/2)/32)) == true){
                                if(cycle == true || visited[9] == true){Mix_PlayChannel(-1, error, 0);}
                                else if(!cycle && !visited[9]){
                                    dark_coin+=coin_div[9];visited[9] = true;
                                    Mix_PlayChannel(-1, coin, 0);
                                }
                            }
                            else if(udai.count(pair<int,int>((mBox.x+DOT_WIDTH/2)/32,(mBox.y+DOT_HEIGHT/2)/32)) == true){
                                if(cycle == true || visited[10] == true){Mix_PlayChannel(-1, error, 0);}
                                else if(!cycle && !visited[10]){
                                    dark_coin+=coin_div[10];visited[10] = true;
                                    Mix_PlayChannel(-1, coin, 0);
                                }
                            }
                            else if(kailash.count(pair<int,int>((mBox.x+DOT_WIDTH/2)/32,(mBox.y+DOT_HEIGHT/2)/32)) == true){
                                if(cycle == true || visited[11] == true){Mix_PlayChannel(-1, error, 0);}
                                else if(!cycle && !visited[11]){
                                    dark_coin+=coin_div[11];visited[11] = true;
                                    Mix_PlayChannel(-1, coin, 0);
                                }
                            }
                            else if(himadri.count(pair<int,int>((mBox.x+DOT_WIDTH/2)/32,(mBox.y+DOT_HEIGHT/2)/32)) == true){
                                if(cycle == true || visited[12] == true){Mix_PlayChannel(-1, error, 0);}
                                else if(!cycle && !visited[12]){
                                    dark_coin+=coin_div[12];visited[12] = true;
                                    Mix_PlayChannel(-1, coin, 0);
                                }
                            }
                            else if(bharti.count(pair<int,int>((mBox.x+DOT_WIDTH/2)/32,(mBox.y+DOT_HEIGHT/2)/32)) == true){
                                if(cycle == true || visited[13] == true){Mix_PlayChannel(-1, error, 0);}
                                else if(!cycle && !visited[13]){
                                    dark_coin+=coin_div[13];visited[13] = true;
                                    Mix_PlayChannel(-1, coin, 0);
                                }
                            }
                            else if(library.count(pair<int,int>((mBox.x+DOT_WIDTH/2)/32,(mBox.y+DOT_HEIGHT/2)/32)) == true){
                                if(cycle == true || visited[14] == true){Mix_PlayChannel(-1, error, 0);}
                                else if(!cycle && !visited[14]){
                                    dark_coin+=coin_div[14];visited[14] = true;
                                    Mix_PlayChannel(-1, coin, 0);
                                }
                            }
                            else if(mainbuilding.count(pair<int,int>((mBox.x+DOT_WIDTH/2)/32,(mBox.y+DOT_HEIGHT/2)/32)) == true){
                                if(cycle == true || visited[15] == true){Mix_PlayChannel(-1, error, 0);}
                                else if(!cycle && !visited[15]){
                                    dark_coin+=coin_div[15];visited[15] = true;
                                    Mix_PlayChannel(-1, coin, 0);
                                }
                            }
                            else if(lecturehall.count(pair<int,int>((mBox.x+DOT_WIDTH/2)/32,(mBox.y+DOT_HEIGHT/2)/32)) == true){
                                if(cycle == true || visited[16] == true){Mix_PlayChannel(-1, error, 0);}
                                else if(!cycle && !visited[16]){
                                    dark_coin+=coin_div[16];visited[16] = true;
                                    Mix_PlayChannel(-1, coin, 0);
                                }
                            }
                            break;
                        case SDLK_b:
                            if(dark_coin>=gun_price && girnar.count(pair<int,int>((mBox.x+DOT_WIDTH/2)/32,(mBox.y+DOT_HEIGHT/2)/32)) == true){
                                dark_coin-=gun_price;
                                gun = true;
                                Mix_PlayChannel(-1, gLoad, 0);
                                bullet = 10;
                            }
                            break;
                        case SDLK_x:
                            if(gun == true && bullet>0){
                                Mix_PlayChannel(-1, gShot, 0);
                                bullet--;
                                gun = false;
                                fired = true;  
                            }    
                        break;    
                        default: break;
                    }
                    
                }
                if(yulu.count(pair<int,int>((mBox.x+DOT_WIDTH/2)/32,(mBox.y+ DOT_HEIGHT/2)/32))){
                    cerr<<"#########################"<<"yulu"<<"#########################"<<endl;
                    status=0;
                }
                else if(food.count(pair<int,int>((mBox.x+DOT_WIDTH/2)/32,(mBox.y+DOT_HEIGHT/2)/32))){
                    cerr<<"#########################"<<"food"<<"#########################"<<endl;
                    status=1;
                }
                else if(hospital.count(pair<int,int>((mBox.x+DOT_WIDTH/2)/32,(mBox.y+DOT_HEIGHT/2)/32))){
                    cerr<<"#########################"<<"hospital"<<"#########################"<<endl;
                    status=2;
                }
                else if(jwala.count(pair<int,int>((mBox.x+DOT_WIDTH/2)/32,(mBox.y+DOT_HEIGHT/2)/32))){
                    cerr<<"#########################"<<"jwala"<<"#########################"<<endl;
                    status=3;

                }
                else if(ara.count(pair<int,int>((mBox.x+DOT_WIDTH/2)/32,(mBox.y+DOT_HEIGHT/2)/32))){
                    cerr<<"#########################"<<"aravali"<<"#########################"<<endl;
                    status=4;
                }
                else if(kara.count(pair<int,int>((mBox.x+DOT_WIDTH/2)/32,(mBox.y+DOT_HEIGHT/2)/32))){
                    cerr<<"#########################"<<"karakoram"<<"#########################"<<endl;
                    status=5;
                }
                else if(nil.count(pair<int,int>((mBox.x+DOT_WIDTH/2)/32,(mBox.y+DOT_HEIGHT/2)/32))){
                    cerr<<"#########################"<<"nilgiri"<<"#########################"<<endl;
                    status=6;
                }
                else if(kumaon.count(pair<int,int>((mBox.x+DOT_WIDTH/2)/32,(mBox.y+DOT_HEIGHT/2)/32))){
                    cerr<<"#########################"<<"kumaon"<<"#########################"<<endl;
                    status=7;
                }
                else if(vindy.count(pair<int,int>((mBox.x+DOT_WIDTH/2)/32,(mBox.y+DOT_HEIGHT/2)/32))){
                    cerr<<"#########################"<<"vindhyanchal"<<"#########################"<<endl;
                    status=8;
                }
                else if(satpura.count(pair<int,int>((mBox.x+DOT_WIDTH/2)/32,(mBox.y+DOT_HEIGHT/2)/32))){
                    cerr<<"#########################"<<"satpura"<<"#########################"<<endl;
                    status=9;
                }
                else if(shiva.count(pair<int,int>((mBox.x+DOT_WIDTH/2)/32,(mBox.y+DOT_HEIGHT/2)/32))){
                    cerr<<"#########################"<<"shivalik"<<"#########################"<<endl;
                    status=10;
                }
                else if(zanskar.count(pair<int,int>((mBox.x+DOT_WIDTH/2)/32,(mBox.y+DOT_HEIGHT/2)/32))){
                    cerr<<"#########################"<<"zanskar"<<"#########################"<<endl;
                    status=11;
                }
                else if(girnar.count(pair<int,int>((mBox.x+DOT_WIDTH/2)/32,(mBox.y+DOT_HEIGHT/2)/32))){
                    cerr<<"#########################"<<"girnar"<<"#########################"<<endl;
                    status=12;
                }
                else if(udai.count(pair<int,int>((mBox.x+DOT_WIDTH/2)/32,(mBox.y+DOT_HEIGHT/2)/32))){
                    cerr<<"#########################"<<"udaigiri"<<"#########################"<<endl;
                    status=13;
                }
                else if(kailash.count(pair<int,int>((mBox.x+DOT_WIDTH/2)/32,(mBox.y+DOT_HEIGHT/2)/32))){
                    cerr<<"#########################"<<"kailash"<<"#########################"<<endl;
                    status=14;
                }
                else if(himadri.count(pair<int,int>((mBox.x+DOT_WIDTH/2)/32,(mBox.y+DOT_HEIGHT/2)/32))){
                    cerr<<"#########################"<<"himadri"<<"#########################"<<endl;
                    status=15;
                }
                else if(bharti.count(pair<int,int>((mBox.x+DOT_WIDTH/2)/32,(mBox.y+DOT_HEIGHT/2)/32))){
                    cerr<<"#########################"<<"Bharti Building"<<"#########################"<<endl;
                    status=16;
                }
                else if(library.count(pair<int,int>((mBox.x+DOT_WIDTH/2)/32,(mBox.y+DOT_HEIGHT/2)/32))){
                    cerr<<"#########################"<<"library"<<"#########################"<<endl;
                    status=17;
                }
                else if(mainbuilding.count(pair<int,int>((mBox.x+DOT_WIDTH/2)/32,(mBox.y+DOT_HEIGHT/2)/32))){
                    cerr<<"#########################"<<"main building"<<"#########################"<<endl;
                    status=18;
                }
                else if(lecturehall.count(pair<int,int>((mBox.x+DOT_WIDTH/2)/32,(mBox.y+DOT_HEIGHT/2)/32))){
                    cerr<<"#########################"<<"lecture hall"<<"#########################"<<endl;
                    status=19;
                }
                else status=20;
                if(!cycle)energy-=0.001;

                updateData(gDotTexture);
            }
            //If a key was released
            else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
            {
                //Adjust the velocity
                switch( e.key.keysym.sym )
                {
                    case SDLK_UP: mVelY += DOT_VEL;face = "up";break;
                    case SDLK_DOWN: mVelY -= DOT_VEL;face = "down"; break;
                    case SDLK_LEFT: mVelX += DOT_VEL;face = "left";break;
                    case SDLK_RIGHT: mVelX -= DOT_VEL;face = "right";break;
                    

                }
                if(!cycle){
                    if(energy>0)DOT_VEL = 3;
                    else DOT_VEL=3;
                }
                else{DOT_VEL = 6;}
                
            }    
        }

    
        void loadmedia(SDL_Renderer* gRenderer){
            string image  = "randi";
            for(int i = 0; i < 3; i++){
                l_s[i] = SDL_CreateTextureFromSurface(gRenderer,SDL_LoadBMP(("./Images/"+image+to_string(i)+"l.bmp").c_str()));
                r_s[i] = SDL_CreateTextureFromSurface(gRenderer,SDL_LoadBMP(("./Images/"+image+to_string(i)+"r.bmp").c_str()));
                u_s[i] = SDL_CreateTextureFromSurface(gRenderer,SDL_LoadBMP(("./Images/"+image+to_string(i)+"u.bmp").c_str()));
                d_s[i] = SDL_CreateTextureFromSurface(gRenderer,SDL_LoadBMP(("./Images/"+image+to_string(i)+"d.bmp").c_str()));
            }
// new@@@@@@@@@@@@@@@@@@@@


            image = "arm";
            for(int i = 0; i < 3; i++){
                l_a[i] = SDL_CreateTextureFromSurface(gRenderer,SDL_LoadBMP(("./Images/"+image+to_string(i)+"l.bmp").c_str()));
                r_a[i] = SDL_CreateTextureFromSurface(gRenderer,SDL_LoadBMP(("./Images/"+image+to_string(i)+"r.bmp").c_str()));
                u_a[i] = SDL_CreateTextureFromSurface(gRenderer,SDL_LoadBMP(("./Images/"+image+to_string(i)+"u.bmp").c_str()));
                d_a[i] = SDL_CreateTextureFromSurface(gRenderer,SDL_LoadBMP(("./Images/"+image+to_string(i)+"d.bmp").c_str()));
            }
            

            image = "cycle";
            c_l = SDL_CreateTextureFromSurface(gRenderer,SDL_LoadBMP(("./Images/"+image+"l.bmp").c_str()));
            c_r = SDL_CreateTextureFromSurface(gRenderer,SDL_LoadBMP(("./Images/"+image+"r.bmp").c_str()));
            c_u = SDL_CreateTextureFromSurface(gRenderer,SDL_LoadBMP(("./Images/"+image+"u.bmp").c_str()));
            c_d = SDL_CreateTextureFromSurface(gRenderer,SDL_LoadBMP(("./Images/"+image+"d.bmp").c_str()));
            
            healthBar= IMG_LoadTexture(gRenderer, "./Images/healthBar_new.png");
            energyBar= IMG_LoadTexture(gRenderer, "./Images/energyBar.png");
            coinSack = IMG_LoadTexture(gRenderer, "./Images/coinSack.png");
        }


		//Moves the dot and check collision against tiles
		void move( Tile *tiles[])
        {
            //Move the dot left or right
            mBox.x += mVelX;

            //If the dot went too far to the left or right or touched a wall
            if( ( mBox.x < 0 ) || ( mBox.x + DOT_WIDTH > LEVEL_WIDTH ) || touchesWall( mBox, tiles ) )
            {
                //move back
                mBox.x -= mVelX;
            }

            mBox.y += mVelY;

            //If the dot went too far up or down or touched a wall
            if( ( mBox.y < 0 ) || ( mBox.y + DOT_HEIGHT > LEVEL_HEIGHT ) || touchesWall( mBox, tiles ) )
            {
                //move back
                mBox.y -= mVelY;
            }

        }

		//Centers the camera over the dot
		void setCamera( SDL_Rect& camera )
        {
            //Center the camera over the dot
            camera.x = ( mBox.x + DOT_WIDTH / 2 ) - SCREEN_WIDTH / 2;
            camera.y = ( mBox.y + DOT_HEIGHT / 2 ) - SCREEN_HEIGHT / 2;

            //Keep the camera in bounds
            if( camera.x < 0 )
            { 
                camera.x = 0;
            }
            if( camera.y < 0 )
            {
                camera.y = 0;
            }
            if( camera.x > LEVEL_WIDTH - camera.w )
            {
                camera.x = LEVEL_WIDTH - camera.w;
            }
            if( camera.y > LEVEL_HEIGHT - camera.h )
            {
                camera.y = LEVEL_HEIGHT - camera.h;
            }
        }

        void updateData(LTexture* gDotTexture){
            if(cycle){
                SDL_Texture* temp;
                if(face=="up")
                    temp = c_u;
                else if(face=="down")
                    temp = c_d;
                else if(face=="left")
                    temp = c_l;
                else if(face=="right")
                    temp = c_r;
                gDotTexture->mTexture = temp;
            }
            else if(!gun){
                count++;
                count=count%3;
                SDL_Texture* temp;
                if(face=="up")
                    temp = u_s[count];
                else if(face=="down")
                    temp = d_s[count];
                else if(face=="left")
                    temp = l_s[count];
                else if(face=="right")
                    temp = r_s[count];
                gDotTexture->mTexture = temp;
            }
            else{
                count++;
                count++;
                count=count%3;
                SDL_Texture* temp;
                if(face=="up")
                    temp = u_a[count];
                else if(face=="down")
                    temp = d_a[count];
                else if(face=="left")
                    temp = l_a[count];
                else if(face=="right")
                    temp = r_a[count];
                gDotTexture->mTexture = temp;
                
            }
        }

		//Shows the dot on the screen
		void render( SDL_Rect& camera, LTexture *gDotTexture ,SDL_Renderer* gRenderer)
        {
            //Show the dot
            
            gDotTexture->render( mBox.x - camera.x, mBox.y - camera.y, gRenderer, NULL);
        }

        string dataToSend(){
            stringstream ss;
            ss << mBox.x << " " << mBox.y << " " << mVelX << " " << mVelY << " " << face << " " << count << " " << cycle <<" "<<punch<<" "<<gun<<" "<<shot<< " "<<dead<< " "<<transferCoins <<" "<<fired;
            punch=false;
            fired=false;
            transferCoins=false;
            return ss.str();
        }

    
};

#endif