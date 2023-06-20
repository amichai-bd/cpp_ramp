
#include <SDL.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>

void drawLine(SDL_Renderer* renderer, int x, int value, int maxValue, int windowHeight) {
    int height = (int)(((double)value / maxValue) * windowHeight);
    SDL_Rect rect = {x, windowHeight - height, 50, height};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White color
    SDL_RenderFillRect(renderer, &rect);
}

void bubbleSort(std::vector<int>& data, SDL_Renderer* renderer, int windowHeight) {
    for(int i = 0; i < data.size() - 1; i++) {
        for(int j = 0; j < data.size() - i - 1; j++) {
            if(data[j] > data[j + 1]) {
                std::swap(data[j], data[j + 1]);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Set draw color to black
                SDL_RenderClear(renderer); // Clear the screen

                // Find maximum value in data for normalization
                int maxValue = *std::max_element(data.begin(), data.end());

                // Draw a line for each data point
                for (int k = 0; k < data.size(); k++) {
                    drawLine(renderer, k * 60, data[k], maxValue, 600);
                }

                SDL_RenderPresent(renderer); // Present the result
                SDL_Delay(100); // Add delay to visualize sorting
            }
        }
    }
}

void shuffleData(std::vector<int>& data) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);

    std::shuffle(data.begin(), data.end(), generator);
}

int main(int argc, char* argv[]) {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    window = SDL_CreateWindow("Sorting Visualizer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cout << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        std::cout << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Some example data
    std::vector<int> data(10);
    for (int i = 0; i < 10; i++) {
        data[i] = i;
    }

    shuffleData(data); // Randomize data

    // Define "button"
    SDL_Rect button = {600, 20, 100, 50};

    // Main program loop
    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (x >= button.x && x <= button.x + button.w && y >= button.y && y <= button.y + button.h) {
                    shuffleData(data); // Re-randomize data on button press
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Set draw color to black
        SDL_RenderClear(renderer); // Clear the screen

        // Find maximum value in data for normalization
        int maxValue = *std::max_element(data.begin(), data.end());

        // Draw a line for each data point
        for (int i = 0; i < data.size(); i++) {
            drawLine(renderer, i * 60, data[i], maxValue, 600);
        }

        // Draw "button"
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color
        SDL_RenderFillRect(renderer, &button);

        SDL_RenderPresent(renderer); // Present the result

        // Start bubble sort
        bubbleSort(data, renderer, 600);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
