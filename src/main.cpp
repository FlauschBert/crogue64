#include <cstdint>
#include <cstdlib>

// C64 memory locations
constexpr uint16_t SCREEN_MEMORY = 0x0400;
constexpr uint16_t COLOR_MEMORY = 0xD800;

constexpr uint8_t SCREEN_WIDTH = 40;
constexpr uint8_t SCREEN_HEIGHT = 25;

inline uint8_t clamp(uint8_t value, uint8_t min, uint8_t max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

void clearScreen() {
    volatile uint8_t* screenPtr = (volatile uint8_t*)SCREEN_MEMORY;
    uint16_t constexpr screenSize = SCREEN_WIDTH * SCREEN_HEIGHT;
    for (uint16_t i = 0; i < screenSize; ++i) {
        screenPtr[i] = ' ';
    }
}

void drawText(uint8_t const posX, uint8_t const posY, char const* text,
              uint8_t const size, uint8_t const color) {
    volatile uint8_t* screenPtr = (volatile uint8_t*)SCREEN_MEMORY;
    volatile uint8_t* colorPtr = (volatile uint8_t*)COLOR_MEMORY;
    for (uint8_t i = 0; i < size; ++i) {
        uint8_t const screenX = posX + i - size / 2;
        uint16_t const pos = posY * (uint16_t)SCREEN_WIDTH + screenX;
        if (screenX < SCREEN_WIDTH && posY < SCREEN_HEIGHT) {
            screenPtr[pos] = text[i];
            colorPtr[pos] = color;
        }
    }
}

void drawToScreen(const uint8_t* const src, uint8_t const srcWidth,
                  uint8_t const srcHeight, uint8_t const srcOffsetX,
                  uint8_t const srcOffsetY, uint8_t const screenX,
                  uint8_t const screenY, uint8_t const rectWidth,
                  uint8_t const rectHeight, uint8_t const rectColor) {
    volatile uint8_t* screenPtr = (volatile uint8_t*)SCREEN_MEMORY;
    volatile uint8_t* colorPtr = (volatile uint8_t*)COLOR_MEMORY;

    uint8_t screenXClamp = clamp(screenX, 0, SCREEN_WIDTH - 1);
    uint8_t screenYClamp = clamp(screenY, 0, SCREEN_HEIGHT - 1);
    uint8_t screenXEndClamp =
        clamp(screenX + rectWidth - 1, 0, SCREEN_WIDTH - 1);
    uint8_t screenYEndClamp =
        clamp(screenY + rectHeight - 1, 0, SCREEN_HEIGHT - 1);

    if (screenXClamp > screenXEndClamp || screenYClamp > screenYEndClamp) {
        return;
    }

    uint8_t const screenWidthClamped = screenXEndClamp - screenXClamp + 1;
    uint8_t const screenHeightClamped = screenYEndClamp - screenYClamp + 1;

    uint8_t const srcOffsetXAdjusted = srcOffsetX + (screenXClamp - screenX);
    uint8_t const srcOffsetYAdjusted = srcOffsetY + (screenYClamp - screenY);

    for (uint8_t y = 0; y < screenHeightClamped; ++y) {
        for (uint8_t x = 0; x < screenWidthClamped; ++x) {
            // we assume that src is large enough to hold the requested data
            uint8_t const srcX = x + srcOffsetXAdjusted;
            uint8_t const srcY = y + srcOffsetYAdjusted;
            uint16_t const srcPos = srcY * (uint16_t)srcWidth + srcX;

            if (srcPos >= srcWidth * srcHeight || srcPos < 0) {
                continue;
            }

            uint8_t const screenXPos = x + screenXClamp;
            uint8_t const screenYPos = y + screenYClamp;
            uint16_t const screenPos =
                screenYPos * (uint16_t)SCREEN_WIDTH + screenXPos;

            screenPtr[screenPos] = src[srcPos];
            colorPtr[screenPos] = rectColor;
        }
    }
}

void fillCheckBoard(uint8_t* arrayPtr, uint8_t const back, uint8_t const fore,
                    uint8_t const width, uint8_t const height) {
    for (uint8_t y = 0; y < height; ++y) {
        for (uint8_t x = 0; x < width; ++x) {
            arrayPtr[y * width + x] = ((x + y) % 2) ? fore : back;
        }
    }
}

int main() {
    constexpr uint8_t ARRAY_WIDTH = 60;
    constexpr uint8_t ARRAY_HEIGHT = 50;

    uint8_t gameArray[ARRAY_HEIGHT][ARRAY_WIDTH];

    clearScreen();

    // Only uppercase letters are correctly shown in PETSCII
    drawText(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4, "GENERATING TEST DATA ...",
             24, 1);

    fillCheckBoard(&gameArray[0][0], ' ', '#', ARRAY_WIDTH, ARRAY_HEIGHT);

    clearScreen();

    // Use raster line counter as seed
    srand(*(volatile uint8_t*)0xD012);

#if 1
    for (uint8_t i = 0; i < 250; ++i) {
        uint8_t const rectWidth = 2 + (rand() % 16);
        uint8_t const rectHeight = 2 + (rand() % 9);
        uint8_t const screenX = rand() % SCREEN_WIDTH;
        uint8_t const screenY = rand() % SCREEN_HEIGHT;
        uint8_t const color = 1 + (rand() % 15);

        drawToScreen(&gameArray[0][0], ARRAY_WIDTH, ARRAY_HEIGHT, 0, 0, screenX,
                     screenY, rectWidth, rectHeight, color);
    }
#endif

#if 0
    drawToScreen(&gameArray[0][0], ARRAY_WIDTH, ARRAY_HEIGHT, 10, 10,
                 5, 5, 20, 15,
                 2);

    drawToScreen(&gameArray[0][0], ARRAY_WIDTH, ARRAY_HEIGHT, 10, 10,
                 15, 15, 20, 15,
                 3);

    drawToScreen(&gameArray[0][0], ARRAY_WIDTH, ARRAY_HEIGHT, 10, 10,
                 25, 2, 20, 15,
                 4);
#endif

    // Only uppercase letters are correctly shown in PETSCII
    drawText(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4, "DONE", 4, 1);

    return 0;
}