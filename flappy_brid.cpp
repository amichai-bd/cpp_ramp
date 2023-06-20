#include <SDL.h>
#include <string>

#include <cmath>
#include <vector>

// Define a simple feed-forward network with 2 inputs, 1 hidden layer with 3 neurons, and 1 output
struct NeuralNetwork {
    // Initialize weights and biases with some random values. 
    // These should be adjusted using some sort of learning algorithm.
    std::vector<std::vector<float>> weights1 = {{0, -10}, {0, -10}, {0, -10}};
    std::vector<float> biases1 = {5, 5, 5};
    std::vector<std::vector<float>> weights2 = {{1, 1, 1}};
    std::vector<float> biases2 = {-1.5};

    float sigmoid(float x) {
        return 1 / (1 + std::exp(-x));
    }

    std::vector<float> sigmoid(const std::vector<float>& x) {
        std::vector<float> y(x.size());
        for (int i = 0; i < x.size(); i++) {
            y[i] = sigmoid(x[i]);
        }
        return y;
    }

    std::vector<float> feedforward(const std::vector<float>& inputs) {
        // Hidden layer
        std::vector<float> hidden_layer(biases1);
        for (int i = 0; i < hidden_layer.size(); i++) {
            for (int j = 0; j < inputs.size(); j++) {
                hidden_layer[i] += weights1[i][j] * inputs[j];
            }
        }
        // Apply sigmoid activation function
        hidden_layer = sigmoid(hidden_layer);

        // Output layer
        std::vector<float> output_layer(biases2);
        for (int i = 0; i < output_layer.size(); i++) {
            for (int j = 0; j < hidden_layer.size(); j++) {
                output_layer[i] += weights2[i][j] * hidden_layer[j];
            }
        }
        // Apply sigmoid activation function
        output_layer = sigmoid(output_layer);

        return output_layer;
    }
};


const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const int SQUARE_SIZE = 40;
const int OBSTACLE_WIDTH = 20;
const int OBSTACLE_HEIGHT = 120;
const float GRAVITY = 0.7;
const int FLOOR = WINDOW_HEIGHT - SQUARE_SIZE;
const int OBSTACLE_SPEED = 7;
const int OBSTACLE_SPACE = 120;
const int JUMP_VELOCITY = -12;

SDL_Window* initSDL() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow(
        "first game",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    return window;
}

void resetObstacles(SDL_Rect& obstacle, SDL_Rect& obstacle_top) {
    obstacle.h     = rand() % (WINDOW_HEIGHT - OBSTACLE_HEIGHT);
    obstacle.y     = WINDOW_HEIGHT - obstacle.h;
    obstacle_top.y = 0;
    obstacle_top.h = obstacle.y - OBSTACLE_SPACE;
    obstacle.x     = WINDOW_WIDTH;
    obstacle_top.x = WINDOW_WIDTH;
}

void handleEvents(SDL_Event& event, bool& quit, float& velocity) {
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT) {
            quit = true;
        }
        else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_SPACE:
                    velocity = JUMP_VELOCITY;  // Apply an upward velocity when spacebar is pressed
                    break;
            }
        }
    }
}

void updateObstacles(SDL_Rect& obstacle, SDL_Rect& obstacle_top) {
    obstacle.x -= OBSTACLE_SPEED;
    obstacle_top.x -= OBSTACLE_SPEED;
    if (obstacle.x + obstacle.w < 0) {
        resetObstacles(obstacle, obstacle_top);
    }
}

void updateSquare(SDL_Rect& square, float& velocity) {
    square.y += velocity; // Apply vertical velocity
    velocity += GRAVITY;  // Apply gravity

    if (square.y >= FLOOR) {
        square.y = FLOOR;  // Ensure the square stays at the floor level
        velocity = 0;      // Stop the square from falling
    }
    if (square.y < 0) {
        square.y = 0;  // Ensure the square stays at the floor level
        velocity = 0;      // Stop the square from falling
    }
}

void render(SDL_Surface* screenSurface, SDL_Rect& obstacle, SDL_Rect& obstacle_top, SDL_Rect& square, bool isCollision) {
    SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
    SDL_FillRect(screenSurface, &obstacle, SDL_MapRGB(screenSurface->format, 0x00, 0xFF, 0x00));  // Draw the obstacle as green
    SDL_FillRect(screenSurface, &obstacle_top, SDL_MapRGB(screenSurface->format, 0x00, 0xFF, 0x00));  // Draw the obstacle as green
    SDL_FillRect(screenSurface, &square, isCollision ? SDL_MapRGB(screenSurface->format, 0xFF, 0x00, 0x00) : SDL_MapRGB(screenSurface->format, 0x00, 0x00, 0xFF));  // Change square's color to red if collision occurred
}

int main(int argc, char* args[])
{
    SDL_Window* window = initSDL();


    if (!window) {
        return -1;
    }

    SDL_Surface* screenSurface = SDL_GetWindowSurface(window);

    SDL_Rect square = {WINDOW_WIDTH / 8 - SQUARE_SIZE / 2, 0, SQUARE_SIZE, SQUARE_SIZE};
    SDL_Rect obstacle = {WINDOW_WIDTH, WINDOW_HEIGHT - OBSTACLE_HEIGHT, OBSTACLE_WIDTH, OBSTACLE_HEIGHT};
    SDL_Rect obstacle_top = {WINDOW_WIDTH, 0, OBSTACLE_WIDTH, WINDOW_HEIGHT - OBSTACLE_HEIGHT - OBSTACLE_SPACE};

    int score = 0;
    float velocity = 0;
    SDL_Event event;
    bool quit = false;

    // Instantiate the neural network
    NeuralNetwork nn;
    while (!quit) {
        handleEvents(event, quit, velocity);

        // Feed the current x and y distances to the neural network
        float dx = obstacle.x - square.x;
        float dy = obstacle.y - square.y;
        std::vector<float> nn_output = nn.feedforward({dx, dy});

        // Make the square "jump" if the output of the neural network is above 0.5
       // if (nn_output[0] > 0.5) {
       //     velocity = JUMP_VELOCITY;
       // }

        updateSquare(square, velocity);
        updateObstacles(obstacle, obstacle_top);

        bool isCollision = SDL_HasIntersection(&square, &obstacle) || SDL_HasIntersection(&square, &obstacle_top);

        // if there was no collision & the square is past the obstacle increase the score
        if (!isCollision && square.x > obstacle.x + obstacle.w) {
            score++;
        }

        render(screenSurface, obstacle, obstacle_top, square, isCollision);

        SDL_UpdateWindowSurface(window);
        SDL_Delay(20); // Delay for 20 milliseconds
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
