#ifndef TILING_H
#define TILING_H
#include "texture.hpp"

//Screen dimension constants
const int SCREEN_WIDTH = 1120;
const int SCREEN_HEIGHT = 800;

//The dimensions of the level
const int LEVEL_WIDTH = 3360;
const int LEVEL_HEIGHT = 1920;

//Tile constants
const int TILE_WIDTH = 32;
const int TILE_HEIGHT = 32;
const int TOTAL_TILES = 6300;
const int TOTAL_TILE_SPRITES = 6300;

//The different tile sprites

const int TILE_UD = 217;
const int TILE_LR = 216;
const int TILE_J2 = 219;
const int TILE_J4 = 218;
const int YULU = 100;



LTexture gTileTexture;
SDL_Rect gTileClips[ TOTAL_TILE_SPRITES];
map<pair<int,int>,bool> yulu;
map<pair<int,int>,bool> food;
map<pair<int,int>,bool> jwala;
map<pair<int,int>,bool> ara;
map<pair<int,int>,bool> kara;
map<pair<int,int>,bool> nil;
map<pair<int,int>,bool> kumaon;
map<pair<int,int>,bool> vindy;
map<pair<int,int>,bool> satpura;
map<pair<int,int>,bool> shiva;
map<pair<int,int>,bool> zanskar;
map<pair<int,int>,bool> girnar;
map<pair<int,int>,bool> udai;
map<pair<int,int>,bool> kailash;
map<pair<int,int>,bool> himadri;
map<pair<int,int>,bool> library;
map<pair<int,int>,bool> bharti;
map<pair<int,int>,bool> hospital;
map<pair<int,int>,bool> mainbuilding;
map<pair<int,int>,bool> lecturehall;;

int coin_div[17];

    //The tile
class Tile
{
    public:
        //Initializes position and type
        Tile( int x, int y, int tileType );

        //Shows the tile
        void render( SDL_Rect& camera, SDL_Renderer* gRenderer );

        //Get the tile type
        int getType();

        //Get the collision box
        SDL_Rect getBox();

        

    private:
        //The attributes of the tile
        SDL_Rect mBox;

        //The tile type
        int mType;
};
// //Starts up SDL and creates window
// bool init();

//Loads media
bool LoadMedia( Tile* tiles[], SDL_Renderer* gRenderer );

// //Frees media and shuts down SDL
// void close( Tile* tiles[] );

//Box collision detector
bool checkCollision( SDL_Rect a, SDL_Rect b );

//Checks collision box against set of tiles
bool touchesWall( SDL_Rect box, Tile* tiles[] );

//Sets tiles from tile map
bool setTiles( Tile *tiles[] );

bool markTiles();

void divide_coin();

Tile::Tile( int x, int y, int tileType )
{
    //Get the offsets
    mBox.x = x;
    mBox.y = y;

    //Set the collision box
    mBox.w = TILE_WIDTH;
    mBox.h = TILE_HEIGHT;

    //Get the tile type
    mType = tileType;
}

void Tile::render( SDL_Rect& camera, SDL_Renderer* gRenderer )
{
    //If the tile is on screen
    // if (mBox.x - camera.x < 0) return;
    // if (mBox.y - camera.y < 0) return;
    // if (mBox.x - camera.x > SCREEN_WIDTH) return;
    // if (mBox.y - camera.y > SCREEN_HEIGHT) return;
    if( checkCollision( camera, mBox ) )
    {
        //Show the tile
        // gTileTexture.render( mBox.x - camera.x, mBox.y - camera.y, gRenderer, &gTileClips[ mType ] );   //################################################################################
        // int i = mType / 105;
        // int j = mType % 105;
        // for (int u = 0; u < 32; u++){
        //     for (int v = 0; v < 32; v++){
        //         gTileTexture.render( mBox.x - camera.x, mBox.y - camera.y, gRenderer, &gTileClips[32 * 105 * i + u + 32 * j + v]);   
        //     }
        // }
    }
}

int Tile::getType()
{
    return mType;
}

SDL_Rect Tile::getBox()
{
    return mBox;
}

bool LoadMedia( Tile* tiles[] , SDL_Renderer* gRenderer)
{
    //Loading success flag
    bool success = true;
    SDL_Surface* temp = IMG_Load( "./Images/map.png" );

    //Load tile texture
    if( !gTileTexture.loadFromFile("./Images/map.png" , gRenderer))
    {
        printf( "Failed to load tile set texture!\n" );
        success = false;
    }

    //Load tile map
    if( !setTiles( tiles ) )
    {
        printf( "Failed to load tile set!\n" );
        success = false;
    }


    markTiles();
    divide_coin();

    return success;
}

void divide_coin(){
    for(int i=0;i<17; i++){
        coin_div[i] = rand()%11+5;
    }
}

bool setTiles(Tile* tiles[]){
    // Success flag
    bool tilesLoaded = true;

    //The tile offsets
    int x = 0, y = 0;

    //Open the map
    std::ifstream map( "map.txt" );

    //If the map couldn't be loaded
    if( map.fail() )
    {
        printf( "Unable to load map file!\n" );
        tilesLoaded = false;
    }
     else
    {
        //Initialize the tiles
        for( int i = 0; i < TOTAL_TILES; ++i )
        {
            //Determines what kind of tile will be made
            int tileType = -1;

            //Read tile from map file
            map >> tileType;


            //If the was a problem in reading the map
            if( map.fail() )
            {
                //Stop loading map
                printf( "Error loading map: Unexpected end of file!\n" );
                tilesLoaded = false;
                cout<<tilesLoaded;
                break;
            }

            //If the number is a valid tile number
            if( ( tileType >= -20 ) && ( tileType < 6300 ) )
            {
                tiles[ i ] = new Tile( x, y, tileType );
            }
            //If we don't recognize the tile type
            else
            {
                //Stop loading map
                printf( "Error loading map: Invalid tile type at %d!\n", i );
                tilesLoaded = false;
                break;
            }
            // Move to next tile spot
            x += TILE_WIDTH;

            //If we've gone too far
            if( x >= LEVEL_WIDTH )
            {
                //Move back
                x = 0;

                //Move to the next row
                y += TILE_HEIGHT;
            }
        }
        //Clip the sprite sheet
        if( tilesLoaded )
        {
            for(int i=0;i<60;i++){
                for(int j=0;j<105;j++){
                    gTileClips[i*105+j].x = j*TILE_WIDTH;
                    gTileClips[i*105+j].y = i*TILE_HEIGHT;
                    gTileClips[i*105+j].w = TILE_WIDTH;
                    gTileClips[i*105+j].h = TILE_HEIGHT;
                }
            }
        }
    }

    //Close the file
    map.close();

    //If the map was loaded fine
    return tilesLoaded;
}


bool markTiles()
{
    //Success flag
    bool tilesLoaded = true;

    //The tile offsets
    int x = 0, y = 0;

    //Open the map
    std::ifstream map( "map.txt" );

    //If the map couldn't be loaded
    if( map.fail() )
    {
        printf( "Unable to load map file!\n" );
        tilesLoaded = false;
    }
     else
    {
        //Initialize the tiles
        for( int i = 0; i < TOTAL_TILES; ++i )
        {
            //Determines what kind of tile will be made
            int tileType = -1;

            //Read tile from map file
            map >> tileType;


            //If the was a problem in reading the map
            if( map.fail() )
            {
                //Stop loading map
                printf( "Error loading map: Unexpected end of file!\n" );
                tilesLoaded = false;
                cout<<tilesLoaded;
                break;
            }

            //If the number is a valid tile number
            // if(tileType == 1296){
            //     for(int i=x;i<x+TILE_WIDTH;i++)
            //     {
            //         for(int j=y;j<y+TILE_HEIGHT;j++)
            //         {
            //             yulu[make_pair(i,j)]=true;
            //         }
            //     }
            // }


            if(tileType == 1296){
                yulu[make_pair(x/32,y/32)]=true;
            }

            if(tileType == 0){
                jwala[make_pair(x/32,y/32)]=true;
            }

            if(tileType == 1){
                        ara[make_pair(x/32,y/32)]=true;
            }

            if(tileType == 2 ){
                 kara[make_pair(x/32,y/32)]=true;
            }
            

            if(tileType == 3 ){
                nil[make_pair(x/32,y/32)]=true;
            }

            if(tileType == 4 ){
                kumaon[make_pair(x/32,y/32)]=true;
            }

            if(tileType == 5 ){
                vindy[make_pair(x/32,y/32)]=true;
            }

            if(tileType == 6 ){
                satpura[make_pair(x/32,y/32)]=true;
            }

            if(tileType == 7 ){
                shiva[make_pair(x/32,y/32)]=true;
            }

            if(tileType == 8 ){
                zanskar[make_pair(x/32,y/32)]=true;
            }

            if(tileType == 9 ){
                girnar[make_pair(x/32,y/32)]=true;
            }

            if(tileType == 10 ){
                udai[make_pair(x/32,y/32)]=true;
            }


            if(tileType == 11 ){
                kailash[make_pair(x/32,y/32)]=true;
            }

            if(tileType == 12 ){
                himadri[make_pair(x/32,y/32)]=true;
            }

            if(tileType == 13 ){
                hospital[make_pair(x/32,y/32)]=true;
            }

            if(tileType == 14 ){
                bharti[make_pair(x/32,y/32)]=true;
            }

            if(tileType == 15 ){
                library[make_pair(x/32,y/32)]=true;
            }

            if(tileType == 16 ){
                mainbuilding[make_pair(x/32,y/32)]=true;
            }

            if(tileType == 17 ){
                lecturehall[make_pair(x/32,y/32)]=true;
            }

            if(tileType == 18 ){
                food[make_pair(x/32,y/32)]=true;
            }

            // Move to next tile spot
            x += TILE_WIDTH;

            //If we've gone too far
            if( x >= LEVEL_WIDTH )
            {
                //Move back
                x = 0;

                //Move to the next row
                y += TILE_HEIGHT;
            }
        }
    }
        //Clip the sprite sheet
    //Close the file
    map.close();

    //If the map was loaded fine
    return tilesLoaded;
}


/*The touchesWall function checks a given collision box against tiles of type TILE_CENTER, TILE_TOP, TILE_TOPRIGHT, TILE_RIGHT, TILE_BOTTOMRIGHT, TILE_BOTTOM, TILE_BOTTOMLEFT, TILE_LEFT, and TILE_TOPLEFT which are all wall tiles. If you check back when we defined these constants, you'll see that these are numbered right next to each other so all we have to do is check if the type is between TILE_CENTER and TILE_TOPLEFT.

If the given collision box collides with any tile that is a wall this function returns true,*/


bool touchesWall( SDL_Rect box, Tile* tiles[] )
{
    //Go through the tiles
    for( int i = 0; i < TOTAL_TILES; ++i )
    {
        //If the tile is a wall type tile
        if(tiles[i]->getType() == 1296 && checkCollision(box, tiles[i]->getBox()))return false;
        if(tiles[i]->getType()>=0 && tiles[i]->getType()<=18 && checkCollision(box, tiles[i]->getBox()))return false;
        if(tiles[i]->getType()>=216 && tiles[i]->getType()<=219 && checkCollision(box, tiles[i]->getBox()))return false;
        if(checkCollision(box, tiles[i]->getBox()))return true;
        // if( tiles[i]->getType() <216 && tiles[i]->getType() > 18)
        // {
        //     //If the collision box touches the wall tile
        //     if( checkCollision( box, tiles[ i ]->getBox() ) )
        //     {
        //         ans = true;
        //     }
        // }
        // if(tiles[i]->getType() >219 && checkCollision(box, tiles[i]->getBox())){
        //     ans true;
        // }
        // if(tiles[i]->getType() == 1296){
        //     return false;
        // }
    }
    return false;
}

bool checkCollision( SDL_Rect a, SDL_Rect b )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x+10;
    rightA = a.x + a.w+10;
    topA = a.y+45;
    bottomA = a.y + a.h+45;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if( bottomA<= topB )
    {
        return false;
    }

    if( topA >= bottomB )
    {
        return false;
    }

    if( rightA <= leftB )
    {
        return false;
    }

    if( leftA >= rightB )
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}



#endif

