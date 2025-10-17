#include <cstdint>
#include <cstdlib>

// C64 memory locations
constexpr uint16_t SCREEN_MEMORY = 0x0400;
constexpr uint16_t COLOR_MEMORY = 0xD800;

constexpr int SCREEN_WIDTH = 40;
constexpr int SCREEN_HEIGHT = 25;

inline int clamp(int value, int min, int max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

void clearScreen() {
    volatile uint8_t* screenPtr = (volatile uint8_t*)SCREEN_MEMORY;
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; ++i) {
        screenPtr[i] = ' ';
    }
}

void drawText(int posX, int posY, char const* text, uint8_t const size, uint8_t const color) {
    volatile uint8_t* screenPtr = (volatile uint8_t*)SCREEN_MEMORY;
    volatile uint8_t* colorPtr = (volatile uint8_t*)COLOR_MEMORY;
    for (int i = 0; i < size; ++i) {
        int const screenX = posX + i - size / 2;
        int const pos = posY * SCREEN_WIDTH + screenX;
        if (screenX < SCREEN_WIDTH && posY < SCREEN_HEIGHT) {
            screenPtr[pos] = text[i];
            colorPtr[pos] = color;
        }
    }
}

void drawToScreen(const uint8_t* src, int const srcWidth, int const srcHeight,
                  int const srcOffsetX, int const srcOffsetY, int const screenX,
                  int const screenY, int const rectWidth, int const rectHeight,
                  uint8_t const rectColor) {
    volatile uint8_t* screenPtr = (volatile uint8_t*)SCREEN_MEMORY;
    volatile uint8_t* colorPtr = (volatile uint8_t*)COLOR_MEMORY;

    int screenXClamp = clamp(screenX, 0, SCREEN_WIDTH - 1);
    int screenYClamp = clamp(screenY, 0, SCREEN_HEIGHT - 1);
    int screenXEndClamp = clamp(screenX + rectWidth - 1, 0, SCREEN_WIDTH - 1);
    int screenYEndClamp = clamp(screenY + rectHeight - 1, 0, SCREEN_HEIGHT - 1);
    
    if (screenXClamp > screenXEndClamp || screenYClamp > screenYEndClamp) {
        return;
    }

    int screenWidthClamped = screenXEndClamp - screenXClamp + 1;
    int screenHeightClamped = screenYEndClamp - screenYClamp + 1;

    int srcOffsetXAdjusted = srcOffsetX + (screenXClamp - screenX);
    int srcOffsetYAdjusted = srcOffsetY + (screenYClamp - screenY);

    for (int y = 0; y < screenHeightClamped; ++y) {
        for (int x = 0; x < screenWidthClamped; ++x) {
            int screenXPos = x + screenXClamp;
            int screenYPos = y + screenYClamp;
            // we assume that src is large enough to hold the requested data
            int srcX = x + srcOffsetXAdjusted;
            int srcY = y + srcOffsetYAdjusted;

            int screenPos = screenYPos * SCREEN_WIDTH + screenXPos;
            int srcPos = srcY * srcWidth + srcX;

            if (srcPos >= srcWidth * srcHeight || srcPos < 0) {
                continue;
            }

            screenPtr[screenPos] = src[srcPos];
            colorPtr[screenPos] = rectColor;
        }
    }
}

void fillCheckBoard(uint8_t* arrayPtr, uint8_t const back, uint8_t const fore,
                    int const width, int const height) {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            arrayPtr[y * width + x] = ((x + y) % 2) ? fore : back;
        }
    }
}

int main() {
    constexpr int ARRAY_WIDTH = 60;
    constexpr int ARRAY_HEIGHT = 50;

    uint8_t gameArray[ARRAY_HEIGHT][ARRAY_WIDTH];

    clearScreen();

    // Only uppercase letters are correctly shown in PETSCII
    drawText(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4, "GENERATING TEST DATA ...", 24, 1);

    fillCheckBoard(&gameArray[0][0], ' ', '#', ARRAY_WIDTH, ARRAY_HEIGHT);

    clearScreen();

    // srand(12345);

#if 1
    for (int i = 0; i < 100; ++i) {
        int rectWidth = 5 + (rand() % 16);
        int rectHeight = 5 + (rand() % 11);
        int screenX = rand() % (SCREEN_WIDTH - rectWidth);
        int screenY = rand() % (SCREEN_HEIGHT - rectHeight);
        uint8_t color = 1 + (rand() % 15);

        drawToScreen(&gameArray[0][0], ARRAY_WIDTH, ARRAY_HEIGHT, 0, 0,
                     screenX, screenY, rectWidth, rectHeight,
                     color);
    }
#endif

#if 0
    drawToScreen(&gameArray[0][0], ARRAY_WIDTH, ARRAY_HEIGHT, 10, 10,
                 5, 5, 20, 15,
                 2);

    drawToScreen(&gameArray[0][0], ARRAY_WIDTH, ARRAY_HEIGHT, 10, 10,
                 15, 15, 20, 15,
                 3);
#endif

    // Only uppercase letters are correctly shown in PETSCII
    drawText(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4, "DONE", 4, 1);

    return 0;
}