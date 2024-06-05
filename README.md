# 2-D_Multi_Players_Game
This is a two player competitive game where you compete to collect coins from different location to buy a gun and kill the opponent. You can use services like food courts and yulu. The last man standing wins the game.

# Instructions for pre-requisites(SDL2 and other required libraries):
1. Install SDL2 using sudo apt-get install libsdl2-dev
2. Install SDL_Image for image rendering using sudo apt-get libsdl2-image-dev
3. Install SDL_ttf for text rendering using sudo apt-get install libsdl2-ttf-dev
4. Install SDL_mixer for audio using sudo apt-get install libsdl2-mixer-dev

# How to launch the game:

1. Clone the repository.
2. Edit the main.cpp to put the IP address of the device hosting server.
3. Run make after navigating to the Final Game folder of one device.
4. Run command ./server.out on the same device to launch the game server.
5. Run the command ./main.out on the same device to start the game for first player.
6. Run make command similarly on another device and run the command ./main.out to join the game.

# Acknowledgements
1. Lazy foo productions for some implementations.
2. Free stock images for sprites.
3. Tiled and Pixilart


