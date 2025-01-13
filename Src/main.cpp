#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "players.hpp"
// #include "client.hpp"
#include "tiling.hpp"
#include "button.hpp"


#include "texture.hpp"
// Client side C/C++ program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#define PORT 8080
using namespace std;

// const char* SERVER_IP = "172.20.10.2";
const char* SERVER_IP = "127.0.0.1";
// const int SCREEN_WIDTH = 1120;
// const int SCREEN_HEIGHT = 900;

SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Surface* background = NULL;
SDL_Renderer* renderer = NULL;
TTF_Font* font = NULL;
TTF_Font* coinfont = NULL;

LTexture gTextTexture;
// LTexture statusTextTexture;
LTexture gDotTexture;
LTexture coinNumberTexture;
Mix_Chunk* start_music = NULL;
Mix_Chunk* step = NULL;
Mix_Chunk* win;
Mix_Chunk* lose;
int tempx, tempy, punches=0, bullets=0, pendingCoins=0;
bool hasGun=false, OpponentKilled=false;


 string statusText[21]{"Near YULU stand, Press <ENTER> to take/leave YULU"
,"Near food court, Press <ENTER> to eat"
,"Near hospital, Press <ENTER> to recover health"
,"Near Jwalamukhi hostel"
,"Near Aravali hostel"
,"Near Karakoram hostel"
,"Near Nilgiri hostel"
,"Near Kumaon hostel"
,"Near Vindhyanchal hostel"
,"Near Satpura hostel"
,"Near Shivalik hostel"
,"Near Zanskar hostel"
,"Near Girnar hostel"
,"Near Udaigiri hostel"
,"Near Kailash hostel"
,"Near Himadri hostel"
,"Near Bharti Building"
,"Near Library"
,"Near Main Building"
,"Near Lecture Hall","..."
};

LTexture Location[21];



SDL_Color color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
   SDL_Color col = {r,g,b,a};
   return col;
}
SDL_Color healthColor= color(200, 0, 0, 200);
SDL_Color energyColor= color(128, 0, 255, 200);
SDL_Color barBGColor= color(0, 0, 0, 100);


// SDL_Surface* mapSurface = NULL;
LTexture* mapTexture = new LTexture();
Player player;
// Player opponent;
map<int, Player> players;
map<int, LTexture> gDotTextures;

int sock = 0, valread;
sockaddr_in serv_addr;
string helloStr = "Hello from client";
bool Terminate = false;

vector<string> split(string s){
    vector<string> res;
    res.push_back("");
    for (char c: s){
        if (c == ' ') res.push_back("");
        else res[res.size() - 1] += c;
    }
    return res;
}

bool handle(string s){
    // Something to write here
    cerr << "Recieved " << s << endl;
    string temp = "";
    for (char c: s){
        if (c == '#') break;
        temp += c;
    }
    vector<string> data = split(temp);
    for (string d: data) cerr << d << " "; cerr << "\n";
    int sNumber = atoi(data[0].c_str());
    if (players.find(sNumber) == players.end()) {
        players[sNumber] = Player(renderer);
        gDotTextures[sNumber] = LTexture();
        gDotTextures[sNumber].loadFromFile("./Images/randi0l.bmp",renderer);
    }
    players[sNumber].mBox.x = atoi(data[1].c_str());
    tempx=atoi(data[1].c_str());
    players[sNumber].mBox.y = atoi(data[2].c_str());
    tempy=atoi(data[2].c_str());
    players[sNumber].mVelX = atoi(data[3].c_str());
    players[sNumber].mVelY = atoi(data[4].c_str());
    players[sNumber].face = data[5];
    players[sNumber].count = atoi(data[6].c_str());
    players[sNumber].cycle = data[7] == "1";
    players[sNumber].punch = data[8] == "1";
    players[sNumber].gun = data[9] == "1";
    players[sNumber].shot = data[10] == "1";
    players[sNumber].dead = data[11] == "1";
    players[sNumber].transferCoins = data[12] == "1";    
    players[sNumber].fired = data[13] == "1";

    if(data[13] == "1")bullets++;

    if(data[12] == "1")pendingCoins+=20;
    
    if(data[8] == "1")punches++;

    if(data[9] == "1")hasGun=true;

    if(data[11] == "1")OpponentKilled=true;
    

    players[sNumber].updateData(&gDotTextures[sNumber]);

    return false;
}

void *getMessages(void *args){
    char buffer[1024];
    while(!Terminate){
        valread = read(sock, buffer, 1024);
        string temp(buffer);
        if (temp.length() < 6) continue;
        string init(temp.begin(), temp.begin() + 3);
        if (init != "###") continue;
        string s(temp.begin() + 3, temp.end());
        // cout << s << "\n";
        Terminate = handle(s);
        // if (s == "Terminate") {
        //     // cerr << "Uf not expected this\n";
        //     Terminate = true;
        // }
        for (int i = 0; i < 1024; i++) buffer[i] = 0;
    }
    return NULL;
}

void transmit(string s){
    s = "###" + s + "###";
    const char* dir = s.c_str();
    send(sock, dir, 1024, 0);
    // send(sock, dir, strlen(dir), 0);
    // printf("Message sent");
}

void init();
void action();
void close();


void init(){
    pthread_t thread;
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
    // SDL_SetRenderDrawColor( renderer, 0, 0, 0, 0 );
    TTF_Init();
    Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048);
    screenSurface = SDL_GetWindowSurface(window);
    // mapSurface = SDL_LoadBMP("map.bmp");
    mapTexture->loadFromFile("./Images/map.png", renderer);
    player = Player(renderer);

    // const char* hello = helloStr.c_str();
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return;
    }
 
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
 
    // cerr << "2\n";
    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr)
        <= 0) {
        printf(
            "\nInvalid address/ Address not supported \n");
        return;
    }
    // cerr << "2\n";
 
    if (connect(sock, (struct sockaddr*)&serv_addr,
                sizeof(serv_addr))
        < 0) {
        printf("\nConnection Failed \n");
        return;
    }
    Terminate = false;
    cerr << "3\n";
    // cerr << "4\n";
    pthread_create(&thread, NULL, getMessages, NULL);
    // opponent = Player(renderer);
    // if (mapSurface) cerr << "Yes it si loaded\n";
    // if (screenSurface) cerr << "Yes screen is also loaded\n";
}

bool loadMedia(string message, int size, char* font_name, SDL_Color textColor, LTexture *texture )
{
	//Loading success flag
	bool success = true;

	//Open the font
	font = TTF_OpenFont( font_name, size);
	if( font == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	else
	{
		if( !texture->loadFromRenderedText( message, textColor ,renderer, font) )
		{
			printf( "Failed to render text texture!\n" );
			success = false;
		}
	}

	return success;
}
bool loadfont( int size, char* font_name, SDL_Color textColor, LTexture *texture )
{
	//Loading success flag
	bool success = true;

	//Open the font
	font = TTF_OpenFont( font_name, size);
	if( font == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	

	return success;
}

void rules(){
    Button button("rules",530,430);
    button.draw(screenSurface,window);
    // SDL_UpdateWindowSurface(window);
    SDL_Event e;
    while(true){
        // cout<<"loopint"<<'\n';
        bool b = true;
        while(SDL_PollEvent(&e)){
            
            if(e.type == SDL_QUIT){
                close();
                return;
            }
            else if(e.type == SDL_KEYDOWN && b==true){
                string s = button.control(e);
                button.draw(screenSurface,window);
                // SDL_UpdateWindowSurface(window);
                cout<<s<<'\n';
                if(s=="exit"){
                    close();
                    return;
                }
                else if(s=="start"){
                    Mix_PlayChannel( -1, start_music, 0 );
                	// background = SDL_LoadBMP("bg.bmp");
                    // SDL_BlitSurface( background, NULL, screenSurface, NULL );
                    // SDL_UpdateWindowSurface(window);
                    return;
                } 
                //cout<<"event"<<'\n';
                
                b=false;
            }
            else if(e.type == SDL_KEYUP){
                
                b = true;
            }
        }    
        
    }
    
}

void start(){
    init();
    background = SDL_LoadBMP("./Images/home.bmp");
    SDL_BlitSurface( background, NULL, screenSurface, NULL );
    Button button("home",430,330);
    button.draw(screenSurface,window);
    SDL_UpdateWindowSurface(window);
    start_music = Mix_LoadWAV("./Sounds/start.wav");
    SDL_Event e;
    bool cont = true;
    Mix_PlayChannel( -1, start_music, 0 );
    SDL_SetRenderDrawColor( renderer, 0,0, 0, 0 );
    SDL_RenderClear( renderer );
    loadMedia("IIT DELHI",80,"lazy.ttf",{255,255,255}, &gTextTexture);
    gTextTexture.render( ( SCREEN_WIDTH - gTextTexture.getWidth()) / 2, 280, renderer ,NULL);
    loadMedia("WELCOMES YOU TO THE",80,"lazy.ttf",{255,255,255}, &gTextTexture);
    gTextTexture.render( ( SCREEN_WIDTH - gTextTexture.getWidth()) / 2, 380, renderer ,NULL);
    loadMedia("GAME OF DEATH",80,"lazy.ttf",{255,255,255}, &gTextTexture);
    gTextTexture.render( ( SCREEN_WIDTH - gTextTexture.getWidth()) / 2, 480, renderer ,NULL);
    loadMedia("PRESS ANY TO KEY",50,"lazy.ttf",{255,0,0}, &gTextTexture);
    gTextTexture.render( ( SCREEN_WIDTH - gTextTexture.getWidth()) / 2, 580, renderer ,NULL);
    SDL_RenderPresent( renderer );
    while(true){
        while(SDL_PollEvent(&e)){
            
            if(e.type == SDL_QUIT){
                transmit("Terminate");
                close();
                return;
            }
            else if(e.type == SDL_KEYDOWN){
                string s = button.control(e);
                button.draw(screenSurface,window);
                cout<<s<<'\n';
                if(s=="exit"){
                    close();
                    return;

                }
                else if(s=="start"){
                    Mix_PlayChannel( -1, start_music, 0 );
                	background = SDL_LoadBMP("./Images/rules.bmp");
                    SDL_BlitSurface( background, NULL, screenSurface, NULL );
                    SDL_UpdateWindowSurface(window);
                    return;
                }     
            }
        }    
        
    }
    
}





void won(){
    sleep(1);
    background = SDL_LoadBMP("./Images/won.bmp");
    SDL_BlitSurface( background, NULL, screenSurface, NULL );
    SDL_UpdateWindowSurface(window);
    win = Mix_LoadWAV("./Sounds/won.wav");
    Mix_PlayChannel( -1, win, 0 );
    SDL_Event e;
    while(true){
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                close();
                return;
            }
            else if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE){
                background = SDL_LoadBMP("./Images/rules.bmp");
                SDL_BlitSurface( background, NULL, screenSurface, NULL );
                SDL_UpdateWindowSurface(window);
                return;
            }
        } 
    }
}

void lost(){
    sleep(1);
    background = SDL_LoadBMP("./Images/lose.bmp");
    SDL_BlitSurface( background, NULL, screenSurface, NULL );
    SDL_UpdateWindowSurface(window);
    lose = Mix_LoadWAV("./Sounds/lose.wav");
    Mix_PlayChannel(-1,lose, 0);
    SDL_Event e;
    while(true){
        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                close();
                return;
            }
            else if(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE){
                background = SDL_LoadBMP("./Images/rules.bmp");
                SDL_BlitSurface( background, NULL, screenSurface, NULL );
                SDL_UpdateWindowSurface(window);
                return;
            }
        } 
    }
}




void close(){
    SDL_DestroyWindow(window);window=NULL;
    SDL_Quit();
    gTextTexture.free();
    TTF_Quit();
}

void load_location(){
    font = TTF_OpenFont( "arial.ttf", 20);
    for(int i=0;i<21;i++){
        Location[i].loadFromRenderedText("Location: "+statusText[i],{255,255,255},renderer,font);
    }
}

void control_player(){
    Tile* tileSet[ TOTAL_TILES ];
    Tile* tile;

		//Load media
		if( !LoadMedia( tileSet,renderer) )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//The dot that will be moving around on the screen

            gDotTexture.loadFromFile("./Images/randi0l.bmp",renderer);

			//Level camera
			SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

            // loadfont(20, "arial.ttf", {255,255,255}, &statusTextTexture);
			
            //load coinfont
            coinfont = TTF_OpenFont( "arial.ttf", 30);
            load_location();
            
            //While application is running
			while( !quit )
			{    SDL_Delay(20);
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
                    // if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) transmit(player.dataToSend());
					//Handle input for the dot
					player.handleEvent(e,&gDotTexture );
				}

				//Move the dot
				player.move( tileSet);
                
                //player.punch=false;

                //handle punches
                if(punches>0){
                    if(abs(tempx-player.mBox.x)+(tempy-player.mBox.y)<64){
                        player.health-=.15;
                    }
                    punches--;
                    if(player.health<=0){
                        player.mBox.x=1775;
                        player.mBox.y=945;
                        player.health=1.0;
                        player.energy=1.0;
                        player.dark_coin-=20;
                        player.transferCoins=true;
                        

                    }
                }
                if(bullets>0){
                    if(abs(tempx-player.mBox.x)+(tempy-player.mBox.y)<500){
                        cout<<"Bullet hit!";
                        player.dead=true;
                        player.health=0;
                    }
                    else{
                        cout<<"Bullet miss!";
                    }
                    bullets--;
                }
                if(pendingCoins>0){
                    player.dark_coin+=pendingCoins;
                    pendingCoins=0;
                }
                if(OpponentKilled==true)player.shot=true;
                transmit(player.dataToSend());

				player.setCamera( camera );
                //cout<<tempx<<" "<<tempy<<"\n";
                //cout<<"POSITION DIFFERENCE IS : ( "<<tempx-player.mBox.x<<", "<<tempy-player.mBox.y<<" )\n";
                if(player.dead == true){
                    player=Player(renderer);
                    hasGun=false; 
                    OpponentKilled=false;
                    punches=0;
                    bullets=0; 
                    pendingCoins=0;
                    lost();
                    return;
                }

                if(player.shot == true){
                    player=Player(renderer);
                    hasGun=false; 
                    OpponentKilled=false;
                    punches=0;
                    bullets=0; 
                    pendingCoins=0;
                    won();
                    return;
                }
				//Clear screen
				SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( renderer );

				//Render level
				for( int i = 0; i < TOTAL_TILES; ++i )
				{
					tileSet[ i ]->render( camera, renderer );
				}

                SDL_Rect* mapRect = new SDL_Rect();
                mapRect->x = 0;
                mapRect->y = 0;
                mapRect->w = 3360;
                mapRect->h = 1920;
                mapTexture->render(tileSet[0]->getBox().x - camera.x, tileSet[0]->getBox().y - camera.y, renderer,mapRect);
				player.render( camera, &gDotTexture,renderer);
                for (pair<int, Player> p: players) p.second.render(camera, &gDotTextures[p.first],renderer);
				
                //Health bar render
                player.RenderHPBar(60, 50, 120, 16  , player.health, healthColor, barBGColor, renderer);

                //Energy bar render
                player.RenderEnergyBar(60, 100, 120, 16  , player.energy, energyColor, barBGColor, renderer);

                //Render player coins
                player.RenderCoins(renderer);
                coinNumberTexture.loadFromRenderedText(to_string(player.dark_coin), {255,223,0} ,renderer, coinfont);
                coinNumberTexture.render( SCREEN_WIDTH - 65, 50, renderer, NULL);


                //rendering surface for message
                SDL_Color old;
                SDL_GetRenderDrawColor(renderer, &old.r, &old.g, &old.g, &old.a);
                SDL_Rect fgrect = { 0, 775, 1120, 225};
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderFillRect(renderer, &fgrect);
                SDL_SetRenderDrawColor(renderer, old.r, old.g, old.b, old.a);

                // Player location status render
                // statusTextTexture.loadFromRenderedText("Location: "+statusText[status], {255,255,255} ,renderer, font);
                // statusTextTexture.render((SCREEN_WIDTH - statusTextTexture.getWidth()) / 2, 775, renderer, NULL);
                // player.location(renderer);
                Location[player.status].render((SCREEN_WIDTH - Location[player.status].getWidth()) / 2, 775, renderer, NULL);
                
                cout<<player.mBox.x<<", "<<player.mBox.y<<"\n";

                SDL_RenderPresent( renderer );
			}
		}
		
		//Free resources and close SDL
}





void game(){
    rules();
    control_player();
    game();
}

int main(int argc,const char* argv){
    start();
    game();
    close();
    return 0;
}