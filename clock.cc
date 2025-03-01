#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <cmath>
#include <ctime>
#include <string>
#include <vector>
#include <cstdlib>
#include <algorithm>
template <typename T>
T clamp(const T& value, const T& min_val, const T& max_val) {
    if (value < min_val) {
        return min_val;
    } else if (value > max_val) {
        return max_val;
    } else {
        return value;
    }
}
// Screen dimensions and clock settings
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int RADIUS = 200;
int cx = SCREEN_WIDTH / 2;
int cy = SCREEN_HEIGHT / 2;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

// Global variable for color scheme (0, 1, 2)
int colorScheme = 0;

// Global clock hand angles in degrees
float secondAngle = 0.0f;
float minuteAngle = 0.0f;
float hourAngle = 0.0f;


// Particle structure for dynamic background effect
struct Particle {
    float x, y;
    float vx, vy;
    float life; 
    SDL_Color color;
};

std::vector<Particle> particles;



void initParticles() {
    srand(static_cast<unsigned int>(time(nullptr)));
    const int numParticles = 20; 
    particles.resize(numParticles);
    for (int i = 0; i < numParticles; i++) {
        particles[i].x = rand() % SCREEN_WIDTH;
        particles[i].y = rand() % SCREEN_HEIGHT;
        // Random velocity between -1 and 1 
        particles[i].vx = ((rand() % 200) - 100) / 100.0f;
        particles[i].vy = ((rand() % 200) - 100) / 100.0f;
        // Life between 3 and 5 seconds
        particles[i].life = 3.0f + (rand() % 200) / 100.0f;
        particles[i].color = { static_cast<Uint8>(rand() % 256),
                               static_cast<Uint8>(rand() % 256),
                               static_cast<Uint8>(rand() % 256),
                               255 };
    }
}


// Update particle positions using deltaTime. Respawn particles when their life ends or they move out of bounds.
void updateParticles(float deltaTime) {
    for (Particle &p : particles) {
        p.x += p.vx * 100 * deltaTime; // Scale velocity by 100 for noticeable movement
        p.y += p.vy * 100 * deltaTime;
        p.life -= deltaTime;
        if (p.life <= 0 || p.x < 0 || p.x >= SCREEN_WIDTH || p.y < 0 || p.y >= SCREEN_HEIGHT) {
            // Respawn particle at a random position
            p.x = rand() % SCREEN_WIDTH;
            p.y = rand() % SCREEN_HEIGHT;
            p.vx = ((rand() % 200) - 100) / 100.0f;
            p.vy = ((rand() % 200) - 100) / 100.0f;
            p.life = 3.0f + (rand() % 200) / 100.0f;
            p.color = { static_cast<Uint8>(rand() % 256),
                        static_cast<Uint8>(rand() % 256),
                        static_cast<Uint8>(rand() % 256),
                        255 };
        }
    }
}


// Draw each particle as a very small point.
void drawParticles(SDL_Renderer* renderer) {
    for (const Particle &p : particles) {
        SDL_SetRenderDrawColor(renderer, p.color.r, p.color.g, p.color.b, 255);
        SDL_RenderDrawPoint(renderer, static_cast<int>(p.x), static_cast<int>(p.y));
    }
}



void drawFilledCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w;
            int dy = radius - h;
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, centerX + dx, centerY + dy);
            }
        }
    }
}


// Draw a smooth, gradient, thick line by stamping filled circles.
void drawSmoothGradientThickLine(SDL_Renderer* renderer, 
                                 float x0, float y0, 
                                 float x1, float y1, 
                                 float thickness,
                                 const SDL_Color& startColor, 
                                 const SDL_Color& endColor) {
    float dx = x1 - x0;
    float dy = y1 - y0;
    float length = std::sqrt(dx * dx + dy * dy);
    if (length == 0) return;
    for (float t = 0; t <= 1.0f; t += 1.0f / length) {
        float x = x0 + t * dx;
        float y = y0 + t * dy;
        SDL_Color currentColor = {
            static_cast<Uint8>(startColor.r * (1.0f - t) + endColor.r * t),
            static_cast<Uint8>(startColor.g * (1.0f - t) + endColor.g * t),
            static_cast<Uint8>(startColor.b * (1.0f - t) + endColor.b * t),
            255
        };
        SDL_SetRenderDrawColor(renderer, currentColor.r, currentColor.g, currentColor.b, 255);
        drawFilledCircle(renderer, static_cast<int>(x), static_cast<int>(y), static_cast<int>(thickness / 2));
    }
}


// Draw a dynamic vertical gradient background that diffuses over time.
void drawGradientBackground(SDL_Renderer* renderer) {

    float time = SDL_GetTicks() / 1000.0f;
    
    // Base colors per scheme.
    SDL_Color baseStart, baseEnd;
    if (colorScheme == 0) {
        baseStart = {20, 0, 60, 255};   // dark purple
        baseEnd   = {0, 0, 0, 255};      // black
    } else if (colorScheme == 1) {
        baseStart = {0, 50, 150, 255};   // dark blue
        baseEnd   = {0, 0, 30, 255};     // very dark blue
    } else if (colorScheme == 2) {
        baseStart = {255, 140, 0, 255};  // orange
        baseEnd   = {50, 0, 0, 255};     // dark red
    }
    
    // Dynamically modulate the base colors.
    SDL_Color dynamicStart, dynamicEnd;
    dynamicStart.r = static_cast<Uint8>(clamp(int(baseStart.r + 20 * sin(time + 0)), 0, 255));
    dynamicStart.g = static_cast<Uint8>(clamp(int(baseStart.g + 20 * sin(time + 2)), 0, 255));
    dynamicStart.b = static_cast<Uint8>(clamp(int(baseStart.b + 20 * sin(time + 4)), 0, 255));
    dynamicStart.a = 255;
    
    dynamicEnd.r = static_cast<Uint8>(clamp(int(baseEnd.r + 20 * sin(time + 1)), 0, 255));
    dynamicEnd.g = static_cast<Uint8>(clamp(int(baseEnd.g + 20 * sin(time + 3)), 0, 255));
    dynamicEnd.b = static_cast<Uint8>(clamp(int(baseEnd.b + 20 * sin(time + 5)), 0, 255));
    dynamicEnd.a = 255;
    
    // Draw vertical gradient using dynamic colors.
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        float t = static_cast<float>(y) / SCREEN_HEIGHT;
        Uint8 r = static_cast<Uint8>(dynamicStart.r + t * (dynamicEnd.r - dynamicStart.r));
        Uint8 g = static_cast<Uint8>(dynamicStart.g + t * (dynamicEnd.g - dynamicStart.g));
        Uint8 b = static_cast<Uint8>(dynamicStart.b + t * (dynamicEnd.b - dynamicStart.b));
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderDrawLine(renderer, 0, y, SCREEN_WIDTH, y);
    }
}


// Draw the outer circle of the clock using gradient thick lines.
void drawCircle(){
    const float thickness = 4.0f;
    SDL_Color startColor, endColor;
    if (colorScheme == 0) {
        startColor = {255, 0, 255, 255}; // magenta
        endColor   = {0, 255, 255, 255};   // cyan
    } else if (colorScheme == 1) {
        startColor = {255, 255, 255, 255}; // white
        endColor   = {0, 191, 255, 255};   // deep sky blue
    } else if (colorScheme == 2) {
        startColor = {255, 105, 180, 255}; // hot pink
        endColor   = {255, 69, 0, 255};     // orange red
    }
    for (int i = 1; i <= 360; i++){
        int px  = cx + RADIUS * std::cos((M_PI / 180) * i);
        int py  = cy - RADIUS * std::sin((M_PI / 180) * i);
        int px2 = cx + RADIUS * std::cos((M_PI / 180) * (i - 1));
        int py2 = cy - RADIUS * std::sin((M_PI / 180) * (i - 1));
        drawSmoothGradientThickLine(renderer, px, py, px2, py2, thickness, startColor, endColor);
    }
}


// Draw tick marks (minute and hour markers) around the clock face.
void drawClockHours(){
    int r = RADIUS - 20;
    int len_h = r - 20;
    int len_m = r - 10;
    for (int i = 0; i <= 360; i += 6){
        float angle = (M_PI / 180) * i;
        int px = cx + r * std::sin(angle);
        int py = cy + r * std::cos(angle);
        int px2, py2;
        if (i % 30 == 0) {
            px2 = cx + len_h * std::sin(angle);
            py2 = cy + len_h * std::cos(angle);
            SDL_Color startColor = {255, 255, 255, 255};
            SDL_Color endColor   = {255, 255, 0, 255};
            drawSmoothGradientThickLine(renderer, px, py, px2, py2, 3.0f, startColor, endColor);
        } else {
            px2 = cx + len_m * std::sin(angle);
            py2 = cy + len_m * std::cos(angle);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawLine(renderer, px, py, px2, py2);
        }
    }
}


// Draw hour labels (1 to 12) around the clock face using SDL_ttf.
void drawHourLabels(TTF_Font* font) {
    for (int hour = 1; hour <= 12; hour++) {
        float angle = (M_PI / 6) * hour - (M_PI / 2);
        int labelRadius = RADIUS - 50;
        int x = cx + labelRadius * std::cos(angle);
        int y = cy + labelRadius * std::sin(angle);
        std::string hourStr = std::to_string(hour);
        SDL_Color textColor = {255, 215, 0, 255}; // gold color
        SDL_Surface* textSurface = TTF_RenderText_Blended(font, hourStr.c_str(), textColor);
        if (textSurface) {
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            if (textTexture) {
                int texW = 0, texH = 0;
                SDL_QueryTexture(textTexture, nullptr, nullptr, &texW, &texH);
                SDL_Rect dstRect = {x - texW / 2, y - texH / 2, texW, texH};
                SDL_RenderCopy(renderer, textTexture, nullptr, &dstRect);
                SDL_DestroyTexture(textTexture);
            }
            SDL_FreeSurface(textSurface);
        }
    }
}


// Draw the clock hands (hour, minute, second) using global angles.
void drawHandle(){
    const int hour_radius = 90;
    const int minute_radius = 120;
    const int second_radius = 150;
    
    // Convert from degrees to radians (offset by -90° so 0° is at the top)
    float angle_s = (secondAngle - 90.0f) * M_PI / 180.0f;
    float angle_m = (minuteAngle - 90.0f) * M_PI / 180.0f;
    float angle_h = (hourAngle - 90.0f) * M_PI / 180.0f;
    
    int hourX   = cx + hour_radius * cos(angle_h);
    int hourY   = cy + hour_radius * sin(angle_h);
    int minuteX = cx + minute_radius * cos(angle_m);
    int minuteY = cy + minute_radius * sin(angle_m);
    int secondX = cx + second_radius * cos(angle_s);
    int secondY = cy + second_radius * sin(angle_s);
    
    // Hour hand
    SDL_Color hourStart = {255, 100, 100, 255};
    SDL_Color hourEnd   = {255, 0, 0, 255};
    drawSmoothGradientThickLine(renderer, cx, cy, hourX, hourY, 8.0f, hourStart, hourEnd);
    
    // Minute hand
    SDL_Color minuteStart = {0, 255, 255, 255};
    SDL_Color minuteEnd   = {0, 150, 255, 255};
    drawSmoothGradientThickLine(renderer, cx, cy, minuteX, minuteY, 6.0f, minuteStart, minuteEnd);
    
    // Second hand
    SDL_Color secondStart = {0, 255, 0, 255};
    SDL_Color secondEnd   = {50, 255, 50, 255};
    drawSmoothGradientThickLine(renderer, cx, cy, secondX, secondY, 4.0f, secondStart, secondEnd);
}


// Draw a small pulsating dot at the center of the clock.
void drawCenterDot(){
    Uint8 r = 255;
    Uint8 g = 255;
    Uint8 b = static_cast<Uint8>(128 + 127 * std::sin(SDL_GetTicks() / 500.0));
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    drawFilledCircle(renderer, cx, cy, 5);
}


// Render the digital clock (HH:MM:SS) at the bottom using SDL_ttf.
void renderDigitalClock(SDL_Renderer* renderer, TTF_Font* font) {
    std::time_t rawTime;
    std::tm* timeInfo;
    std::time(&rawTime);
    timeInfo = std::localtime(&rawTime);
    char timeStr[9];
    std::strftime(timeStr, sizeof(timeStr), "%H:%M:%S", timeInfo);
    std::string timeText(timeStr);
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, timeText.c_str(), textColor);
    if (textSurface) {
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (textTexture) {
            int texW = 0, texH = 0;
            SDL_QueryTexture(textTexture, nullptr, nullptr, &texW, &texH);
            SDL_Rect dstRect = { (SCREEN_WIDTH - texW) / 2, SCREEN_HEIGHT - texH - 10, texW, texH };
            SDL_RenderCopy(renderer, textTexture, nullptr, &dstRect);
            SDL_DestroyTexture(textTexture);
        }
        SDL_FreeSurface(textSurface);
    }
}


// Render the digital date (YYYY-MM-DD) above the digital clock.
void renderDigitalDate(SDL_Renderer* renderer, TTF_Font* font) {
    std::time_t rawTime;
    std::tm* timeInfo;
    std::time(&rawTime);
    timeInfo = std::localtime(&rawTime);
    char dateStr[11];
    std::strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", timeInfo);
    std::string dateText(dateStr);
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, dateText.c_str(), textColor);
    if (textSurface) {
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (textTexture) {
            int texW = 0, texH = 0;
            SDL_QueryTexture(textTexture, nullptr, nullptr, &texW, &texH);
            SDL_Rect dstRect = { (SCREEN_WIDTH - texW) / 2, SCREEN_HEIGHT - texH - 50, texW, texH };
            SDL_RenderCopy(renderer, textTexture, nullptr, &dstRect);
            SDL_DestroyTexture(textTexture);
        }
        SDL_FreeSurface(textSurface);
    }
}


// Update clock hand angles using delta time (in seconds).
void updateClockAngles(float deltaTime) {
    // Second hand: 360°/60 sec = 6° per second.
    secondAngle += 6.0f * deltaTime;
    if (secondAngle >= 360.0f) secondAngle -= 360.0f;
    
    // Minute hand: 360°/3600 sec = 0.1° per second.
    minuteAngle += 0.1f * deltaTime;
    if (minuteAngle >= 360.0f) minuteAngle -= 360.0f;
    
    // Hour hand: 360°/43200 sec ≈ 0.00833° per second.
    hourAngle += (360.0f / 43200.0f) * deltaTime;
    if (hourAngle >= 360.0f) hourAngle -= 360.0f;
}


// Initialize SDL and create the window and renderer.
bool SDLINIT() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL Initialization Error: " << SDL_GetError() << "\n";
        return false;
    }
    window = SDL_CreateWindow("Advanced Clock", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window Creation Error: " << SDL_GetError() << "\n";
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer Creation Error: " << SDL_GetError() << "\n";
        return false;
    }
    return true;
}



void RenderScreen(TTF_Font* font, bool showDigitalClock, bool showDigitalDate) {
    // Draw the dynamic gradient background.
    drawGradientBackground(renderer);
    // Draw dynamic particles in the background.
    drawParticles(renderer);
    
    // Draw clock components.
    drawCircle();
    drawClockHours();
    drawHourLabels(font);
    drawHandle();
    drawCenterDot();
    if (showDigitalClock)
        renderDigitalClock(renderer, font);
    if (showDigitalDate)
        renderDigitalDate(renderer, font);
    SDL_RenderPresent(renderer);
}



void DestroyScreen(TTF_Font* font) {
    if (font)
        TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}



int main() {
    if (!SDLINIT())
        return -1;
    
    if (TTF_Init() < 0) {
        std::cerr << "TTF Initialization Error: " << TTF_GetError() << "\n";
        return -1;
    }
    
    // Load font
    TTF_Font* font = TTF_OpenFont("ARIAL.TTF", 28);
    if (!font) {
        std::cerr << "Failed to load font: " << TTF_GetError() << "\n";
        return -1;
    }
    
    // Initialize toggles for digital clock and date.
    bool showDigitalClock = true;
    bool showDigitalDate  = true;
    
    // Initialize clock hand angles from current system time.
    std::time_t rawTime;
    std::tm* localTime;
    std::time(&rawTime);
    localTime = std::localtime(&rawTime);
    secondAngle = localTime->tm_sec * 6.0f;
    minuteAngle = localTime->tm_min * 6.0f + (localTime->tm_sec / 60.0f) * 6.0f;
    hourAngle   = (localTime->tm_hour % 12) * 30.0f + (localTime->tm_min / 60.0f) * 30.0f;
    
    // Initialize particle system.
    initParticles();
    
    SDL_Event e;
    bool quit = false;
    Uint32 lastTime = SDL_GetTicks();
    
    while (!quit) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;
        
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT)
                quit = true;
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE)
                    quit = true;
                if (e.key.keysym.sym == SDLK_d)
                    showDigitalClock = !showDigitalClock;
                if (e.key.keysym.sym == SDLK_t)
                    showDigitalDate = !showDigitalDate;
                if (e.key.keysym.sym == SDLK_c) // Toggle color scheme.
                    colorScheme = (colorScheme + 1) % 3;
            }
        }
        
        // Update particle system.
        updateParticles(deltaTime);
        // Update clock hands based on elapsed time.
        updateClockAngles(deltaTime);
        // Render the scene.
        RenderScreen(font, showDigitalClock, showDigitalDate);
    }
    
    DestroyScreen(font);
    return 0;
}
