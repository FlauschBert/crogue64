#include <cstdint>
#include <cstdio>

// C64 screen memory location (0x0400)
constexpr uint16_t SCREEN_MEMORY = 0x0400;

constexpr int SCREEN_WIDTH = 40;
constexpr int SCREEN_HEIGHT = 25;

void drawToScreen(const uint8_t* src, int const srcWidth, int const srcHeight,
                  int const screenX, int const screenY, int const rectWidth, int const rectHeight) {
    volatile uint8_t* screen = (volatile uint8_t*)SCREEN_MEMORY;

    for (int y = 0; y < rectHeight && y < srcHeight; ++y) {
        for (int x = 0; x < rectWidth && x < srcWidth; ++x) {
            int screenPos = (screenY + y) * SCREEN_WIDTH + (screenX + x);
            int srcPos = y * srcWidth + x;

            if (screenY + y < SCREEN_HEIGHT && screenX + x < SCREEN_WIDTH) {
                screen[screenPos] = src[srcPos];
            }
        }
    }
}

void fillCheckBoard(uint8_t* array, uint8_t const back, uint8_t const fore, int const width, int const height) {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            array[y * width + x] = ((x + y) % 2) ? fore : back;
        }
    }
}

int main() {
    constexpr int ARRAY_WIDTH = 80;
    constexpr int ARRAY_HEIGHT = 50;

    uint8_t gameArray[ARRAY_HEIGHT][ARRAY_WIDTH];
    fillCheckBoard(&gameArray[0][0], ' ', '#', ARRAY_WIDTH, ARRAY_HEIGHT);

    // Copy 10x5 rectangle to screen position 5,5 from gameArray offset 0,0
    drawToScreen(&gameArray[0][0], ARRAY_WIDTH, ARRAY_HEIGHT, 5, 5, 10, 5);

    return 0;
}