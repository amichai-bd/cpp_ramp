#include <SDL.h>
#include <vector>
#include <algorithm>

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;
constexpr int RECT_WIDTH = 10;

class Quadrant {
private:
    SDL_Renderer* renderer;
    std::vector<int> data;
    int x, y, width, height, rectWidth;
    SDL_Rect viewport;

public:
    Quadrant(SDL_Renderer* renderer, int x, int y, int width, int height, int rectWidth) :
        renderer(renderer),
        x(x),
        y(y),
        width(width),
        height(height),
        rectWidth(rectWidth) {

        viewport = { x, y, width, height };

        // Initialize data with random values
        for (int i = 0; i < width / rectWidth; i++) {
            data.push_back(rand() % height);
        }
    }

    void drawData() {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Set draw color to black
        SDL_RenderClear(renderer); // Clear the screen

        // Draw data as white rectangles
        for (int i = 0; i < data.size(); i++) {
            draw_rect(i * rectWidth, data[i], height);
        }
    }

    void bubbleSort() {
        for (int i = 0; i < data.size() - 1; i++) {
            for (int j = 0; j < data.size() - i - 1; j++) {
                if (data[j] > data[j + 1]) {
                    std::swap(data[j], data[j + 1]);

                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Set draw color to black
                    SDL_RenderClear(renderer); // Clear the screen

                    // Draw data as white rectangles
                    for (int k = 0; k < data.size(); k++) {
                        draw_rect(k * rectWidth, data[k], height);
                    }

                    SDL_RenderPresent(renderer); // Present the result
                    SDL_Delay(10); // Add delay to visualize sorting
                }
            }
        }
    }

    void draw_rect(int x, int value, int maxValue) {
        int height = value * this->height / maxValue;
        SDL_Rect rect = {this->x + x, this->y + this->height - height, rectWidth * 0.9, height};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White color
        SDL_RenderFillRect(renderer, &rect);
    }

    SDL_Rect* getViewport() {
        return &viewport;
    }
};

void cleanUp(SDL_Renderer* renderer, SDL_Window* window) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


void drawButton(SDL_Renderer* renderer, SDL_Rect button) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Set color to red
    SDL_RenderFillRect(renderer, &button); // Draw the button
}

bool checkResetButtonClicked(SDL_Event& event, SDL_Rect button) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        return (x >= button.x) && (x <= button.x + button.w) &&
               (y >= button.y) && (y <= button.y + button.h);
    }
    return false;
}


bool isRunning(SDL_Event& event) {
    // If the event is SDL_QUIT return false
    if (event.type == SDL_QUIT) {
        return false;
    }
    return true;
}


void initialize(SDL_Window** window, SDL_Renderer** renderer, Quadrant** quadrant1, SDL_Rect* button) {
    SDL_Init(SDL_INIT_VIDEO);

    *window = SDL_CreateWindow("Visual Sort",
                               SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED,
                               WINDOW_WIDTH,
                               WINDOW_HEIGHT,
                               SDL_WINDOW_SHOWN);

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);

    *quadrant1 = new Quadrant(*renderer, 0, 0, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, RECT_WIDTH);

    // define button
    button->x = 0; // Button's x position
    button->y = 0; // Button's y position
    button->w = 100; // Button's width
    button->h = 100; // Button's height
}

void handleEvents(SDL_Event& event, bool& running, bool& sorting, SDL_Rect& button, Quadrant* quadrant1, SDL_Renderer* renderer) {
    while (SDL_PollEvent(&event)) {
        running = isRunning(event);

        if (checkResetButtonClicked(event, button)) {
            *quadrant1 = Quadrant(renderer, 0, 0, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, RECT_WIDTH);
            sorting = true;
        }
    }
}

void render(SDL_Renderer* renderer, Quadrant* quadrant1, SDL_Rect& button) {
    SDL_RenderSetViewport(renderer, quadrant1->getViewport());
    quadrant1->drawData();

    // Reset viewport to default for button rendering
    SDL_RenderSetViewport(renderer, NULL);
    drawButton(renderer, button);

    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    SDL_Window* window;
    SDL_Renderer* renderer;
    Quadrant* quadrant1;
    SDL_Rect button;

    initialize(&window, &renderer, &quadrant1, &button);

    SDL_Event event;
    bool running = true;
    bool sorting = true;
    while (running) {
        handleEvents(event, running, sorting, button, quadrant1, renderer);
        render(renderer, quadrant1, button);
        if (sorting) {
            quadrant1->bubbleSort();
            sorting = false;
        }
    }

    cleanUp(renderer, window);
    delete quadrant1; //don't forget to delete the dynamically allocated memory

    return 0;
}