# cpp_ramp
cpp_ramp is a learning project in C++ that explores various concepts and technologies, including the SDL2 library for GUI applications.  
This repository contains a simple Flappy Bird game implemented using SDL2 and a basic neural network for playing the game.  

currently the compile knows how to compile only the main!!
Need to be fixed
## Prerequisites

For Windows:  
- Install C++ compiler using MSYS2.
- Install the SDL2 library for Windows by following the instructions at the following link: [SDL2](https://github.com/libsdl-org/SDL/releases/tag/release-2.26.5)

For Linux/WSL (Ubuntu):
```bash
sudo apt update
sudo apt install libsdl2-dev
```


# Flappy Bird Game
Wrote a very basic Flappy Bird game using SDL2. In the game, a single obstacle is generated at a time. The bird (a red rectangle) is controlled by the space bar. The bird is affected by gravity and falls down if the space bar is not pressed. Pressing the space bar makes the bird jump.

# Neural Networks for Flappy Bird
Created a simple neural network with 3 layers: input, hidden, and output. The input layer has 2 neurons representing the x and y distances of the bird from the obstacle. The hidden layer has 3 neurons, and the output layer has 1 neuron representing the jump action.

As a starting point, the neural network is tuned to make the bird jump when it is below the obstacle. However, this is not a good strategy since the x distance of the bird needs to be considered. It serves as a starting point for further development and training.

Currently, both the space bar and the neural network are used to control the bird.


# Quick Start - incase you installed C++ and SDL2 on your windows machine using MSYS2
Compile the code using the provided tasks.json file:  
``` <Ctrl> + <Shift> + B ```

Alternatively, compile using the following command:  
```g++ -g main.cpp -o flappy_bird.exe -IC:/msys64/SDL2-2.26.5/x86_64-w64-mingw32/include/SDL2 -LC:/msys64/SDL2-2.26.5/x86_64-w64-mingw32/lib -w -Wl,-subsystem,windows -lmingw32 -lSDL2main -lSDL2 -Wl,-rpath,C:/msys64/SDL2-2.26.5-win32-x64/api_ms_win```

Run the game:  
```./flappy_bird.exe ```
