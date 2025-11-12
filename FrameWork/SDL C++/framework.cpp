#include <SDL.h>
#include <SDL_ttf.h>
#include <chrono>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <sstream>

using namespace std;

const int WINDOW_W = 1200;
const int WINDOW_H = 640;
const float MAP_FRACTION = 0.70f;
const int AXIS_PADDING = 30;

struct Color { Uint8 r, g, b, a; };

struct Category {
    string name;
    Color color;
    vector<pair<int, int>> points;
    bool selected = false;
};

enum UIState { MAIN_VIEW, VENDING_VIEW };

// moved variables to allow functions to acess them 
SDL_Window* win = nullptr;
SDL_Renderer* ren = nullptr;
TTF_Font* font = nullptr;

UIState state = MAIN_VIEW;
vector<Category> categories;
int selectedCategoryIndex = -1;

int windowW = WINDOW_W, windowH = WINDOW_H;
int mapX, mapY, mapW, mapH;
int mapInnerW, mapInnerH;

int pointRadius = 8;
SDL_Rect sliderRect{ 20, 0, 0, 24 };
bool draggingSlider = false;

bool textInputActive = false;
string textBuffer;
SDL_Rect textInputRect{ 60, 120, 240, 32 };

string message;
Uint32 messageTimer = 0;


bool awaitingMapClickForAddPoint = false;
bool awaitingMapClickForRemove = false;
bool awaitingMapClickForSearch = false;
int lastSearchIdx = -1;

static SDL_Texture* createTextTexture(SDL_Renderer* rend, TTF_Font* font, const string& text, SDL_Color col, int& w, int& h) {
    if (!font) return nullptr;
    SDL_Surface* surf = TTF_RenderUTF8_Blended(font, text.c_str(), col);
    if (!surf) return nullptr;
    SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, surf);
    w = surf->w; h = surf->h;
    SDL_FreeSurface(surf);
    return tex;
}

static string trim(const string& s) { size_t a = s.find_first_not_of(" \t\n\r"); if (a == string::npos) return ""; size_t b = s.find_last_not_of(" \t\n\r"); return s.substr(a, b - a + 1); }

static double dist2(int x1, int y1, int x2, int y2) { double dx = x1 - x2; double dy = y1 - y2; return dx * dx + dy * dy; }

void drawFilledCircle(SDL_Renderer* ren, int cx, int cy, int radius) {
    for (int dx = -radius; dx <= radius; ++dx) {
        for (int dy = -radius; dy <= radius; ++dy) {
            if (dx * dx + dy * dy <= radius * radius) {
                SDL_RenderDrawPoint(ren, cx + dx, cy + dy);
            }
        }
    }
}

// made lambda functions from main normal so other functions can use them
Color makeColor() { 
    return Color{ (Uint8)(rand() % 156 + 80), (Uint8)(rand() % 156 + 80), (Uint8)(rand() % 156 + 80), 255 }; 
}

pair<int, int> worldToGraph(int mx, int my) {
    int axisX = mapX + AXIS_PADDING;
    int axisY = mapY + mapH - AXIS_PADDING;
    int graphX = mx - axisX;
    int graphY = axisY - my; 
    return { graphX, graphY };
}

pair<int, int> graphToWorld(int gx, int gy) {
    int axisX = mapX + AXIS_PADDING;
    int axisY = mapY + mapH - AXIS_PADDING;
    int worldX = axisX + gx;
    int worldY = axisY - gy; 
    return { worldX, worldY };
}

void computeLayout(int w, int h) {
    windowW = w; windowH = h;
    mapW = (int)(w * MAP_FRACTION);
    mapH = h;
    mapX = w - mapW;
    mapY = 0;
    sliderRect.x = 20;
    sliderRect.y = h - 100;
    sliderRect.w = mapX - 40;
    mapInnerW = mapW - 2 * AXIS_PADDING;
    mapInnerH = mapH - 2 * AXIS_PADDING;
}

void drawButton(const string& text, SDL_Rect rect, Color bg, bool glow) {
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(ren, bg.r, bg.g, bg.b, bg.a);
    SDL_RenderFillRect(ren, &rect);

    SDL_SetRenderDrawColor(ren, 40, 40, 40, 255);
    SDL_RenderDrawRect(ren, &rect);

    if (glow) {
        SDL_SetRenderDrawColor(ren, bg.r, bg.g, bg.b, 80);
        SDL_Rect glowR = { rect.x - 6, rect.y - 6, rect.w + 12, rect.h + 12 };
        SDL_RenderFillRect(ren, &glowR);
    }

    if (font) {
        int tw, th; SDL_Color white = { 255,255,255,255 };
        SDL_Texture* tex = createTextTexture(ren, font, text, white, tw, th);
        if (tex) {
            SDL_Rect dst{ rect.x + 6, rect.y + (rect.h - th) / 2, tw, th };
            if (dst.w > rect.w - 12) { dst.w = rect.w - 12; }
            SDL_RenderCopy(ren, tex, nullptr, &dst);
            SDL_DestroyTexture(tex);
        }
    }
}

void handleInput(bool& running) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) running = false;
        else if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED) {
            computeLayout(e.window.data1, e.window.data2);
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN) {
            int mx = e.button.x, my = e.button.y;

            if (mx < mapX && state == MAIN_VIEW) {
                int bx = 20, by = 20, bw = mapX - 40, bh = 40, spacing = 8;
                bool handled = false;

                for (size_t i = 0; i < categories.size(); ++i) {
                    SDL_Rect b{ bx, by + (int)i * (bh + spacing), bw, bh };
                    if (mx >= b.x && mx <= b.x + b.w && my >= b.y && my <= b.y + b.h) {
                        selectedCategoryIndex = (int)i;
                        state = VENDING_VIEW;
                        handled = true;
                        break;
                    }
                }
                if (handled) continue;

                SDL_Rect addBtn{ 20, 240, mapX - 40, 40 };
                if (mx >= addBtn.x && mx <= addBtn.x + addBtn.w && my >= addBtn.y && my <= addBtn.y + addBtn.h) {
                    textInputActive = true; textBuffer.clear(); SDL_StartTextInput();
                    textInputRect.y = addBtn.y - 42;
                    continue;
                }

                SDL_Rect removeAllBtn{ 20, 290, mapX - 40, 40 };
                if (mx >= removeAllBtn.x && mx <= removeAllBtn.x + removeAllBtn.w && my >= removeAllBtn.y && my <= removeAllBtn.y + removeAllBtn.h) {
                    categories.clear(); selectedCategoryIndex = -1; state = MAIN_VIEW;
                    message = "Wiped the slate clean!"; messageTimer = SDL_GetTicks();
                    continue;
                }

                if (mx >= sliderRect.x && mx <= sliderRect.x + sliderRect.w && my >= sliderRect.y && my <= sliderRect.y + sliderRect.h) {
                    draggingSlider = true;
                    float t = float(mx - sliderRect.x) / float(sliderRect.w);
                    pointRadius = int(4 + t * 20);
                    continue;
                }
            }
            else if (mx >= mapX) { 
                if (state == MAIN_VIEW) {
                    auto gp = worldToGraph(e.button.x, e.button.y);
                    double bestDist2 = 1e12; int bestCat = -1;
                    for (size_t i = 0; i < categories.size(); ++i) {
                        for (auto& p : categories[i].points) {
                            double d2 = dist2(gp.first, gp.second, p.first, p.second);
                            if (d2 < bestDist2) { bestDist2 = d2; bestCat = (int)i; }
                        }
                    }
                    if (bestCat != -1 && bestDist2 < (pointRadius + 10) * (pointRadius + 10)) {
                        categories[bestCat].selected = !categories[bestCat].selected;
                    }
                } 
                else if (state == VENDING_VIEW && selectedCategoryIndex >= 0) {
                    auto gp = worldToGraph(e.button.x, e.button.y);
                    auto& pts = categories[selectedCategoryIndex].points;

                    if (awaitingMapClickForAddPoint) {
                        pts.push_back({ gp.first, gp.second });
                        message = "New point logged at (" + to_string(gp.first) + ", " + to_string(gp.second) + ").";
                        awaitingMapClickForAddPoint = false;
                    } 
                    else if (awaitingMapClickForRemove) {
                        if (pts.empty()) {
                            message = "This group is empty!";
                        } else {
                            double best = 1e12; int bi = -1;
                            for (size_t i = 0; i < pts.size(); ++i) {
                                double d = dist2(gp.first, gp.second, pts[i].first, pts[i].second);
                                if (d < best) { best = d; bi = (int)i; }
                            }
                            string coords = "(" + to_string(pts[bi].first) + ", " + to_string(pts[bi].second) + ")";
                            pts.erase(pts.begin() + bi);
                            message = "Removed nearest point at " + coords + ".";
                        }
                        awaitingMapClickForRemove = false;
                    } 
                    else if (awaitingMapClickForSearch) {
                        if (pts.empty()) {
                            message = "This group is empty!";
                            lastSearchIdx = -1;
                        } else {
                            auto start = std::chrono::high_resolution_clock::now();

                            double best = 1e12; int bi = -1;
                            for (size_t i = 0; i < pts.size(); ++i) {
                                double d = dist2(gp.first, gp.second, pts[i].first, pts[i].second);
                                if (d < best) { best = d; bi = (int)i; }
                            }
                            auto end = std::chrono::high_resolution_clock::now();
                            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

                            lastSearchIdx = bi;
                            message = "Found point at (" + to_string(pts[bi].first) + ", " + to_string(pts[bi].second) + ").";
                            message += " Time: " + to_string(duration) + " us.";
                        }
                        awaitingMapClickForSearch = false;
                    }
                    messageTimer = SDL_GetTicks();
                }
            }
        }
        else if (e.type == SDL_MOUSEBUTTONUP) {
            draggingSlider = false;
        }
        else if (e.type == SDL_MOUSEMOTION) {
            if (draggingSlider) {
                int mx = e.motion.x;
                float t = float(mx - sliderRect.x) / float(sliderRect.w);
                t = max(0.0f, min(1.0f, t));
                pointRadius = int(4 + t * 20);
            }
        }
        else if (e.type == SDL_KEYDOWN) {
            if (textInputActive) {
                if (e.key.keysym.sym == SDLK_RETURN) {
                    string name = trim(textBuffer);
                    if (!name.empty()) {
                        categories.push_back({ name, makeColor(), {} });
                        message = "Added the group: " + name; messageTimer = SDL_GetTicks();
                    }
                    else {
                        message = "Name can't be blank!"; messageTimer = SDL_GetTicks();
                    }
                    textBuffer.clear(); textInputActive = false; SDL_StopTextInput();
                }
                else if (e.key.keysym.sym == SDLK_ESCAPE) {
                    textBuffer.clear(); textInputActive = false; SDL_StopTextInput();
                }
                else if (e.key.keysym.sym == SDLK_BACKSPACE && !textBuffer.empty()) {
                    textBuffer.pop_back();
                }
            }
            else if (state == VENDING_VIEW && e.key.keysym.sym == SDLK_BACKSPACE) {
                state = MAIN_VIEW; selectedCategoryIndex = -1; awaitingMapClickForAddPoint = false; lastSearchIdx = -1;
                awaitingMapClickForRemove = false; 
                awaitingMapClickForSearch = false;
            }
        }
        else if (e.type == SDL_TEXTINPUT) {
            if (textInputActive) { textBuffer += e.text.text; }
        }
    }

    int mx, my; Uint32 mouseState = SDL_GetMouseState(&mx, &my);
    bool isClicking = (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;

    if (state == VENDING_VIEW && selectedCategoryIndex != -1) {
        int bx = 20; int by = 20; int bw = mapX - 60; int bh = 36; int gap = 12;
        by += 40 + gap; 
        SDL_Rect backBtn{ bx, by, 42, 36 };
        SDL_Rect addPointBtn{ bx + 54, by, bw - 54, bh };
        by += (bh + gap);
        SDL_Rect remPointBtn{ bx, by, bw, bh };
        by += (bh + gap);
        SDL_Rect searchBtn{ bx, by, bw, bh };
        by += (bh + gap);
        SDL_Rect deleteCatBtn{ bx, by, bw, bh };
        by += (bh + gap);
        SDL_Rect addRandomBtn{ bx, by, bw, bh }; 

        static bool wasClicking = false;
        if (isClicking && !wasClicking) {
            if (mx >= backBtn.x && mx <= backBtn.x + backBtn.w && my >= backBtn.y && my <= backBtn.y + backBtn.h) {
                state = MAIN_VIEW; selectedCategoryIndex = -1; awaitingMapClickForAddPoint = false; lastSearchIdx = -1;
                awaitingMapClickForRemove = false;
                awaitingMapClickForSearch = false;
            }
            else if (mx >= addPointBtn.x && mx <= addPointBtn.x + addPointBtn.w && my >= addPointBtn.y && my <= addPointBtn.y + addPointBtn.h) {
                awaitingMapClickForAddPoint = !awaitingMapClickForAddPoint;
                awaitingMapClickForRemove = false;
                awaitingMapClickForSearch = false;
                message = awaitingMapClickForAddPoint ? "Click map to place a new point now." : "Point placement mode off.";
                messageTimer = SDL_GetTicks();
            }
            else if (mx >= remPointBtn.x && mx <= remPointBtn.x + remPointBtn.w && my >= remPointBtn.y && my <= remPointBtn.y + remPointBtn.h) {
                awaitingMapClickForRemove = !awaitingMapClickForRemove;
                awaitingMapClickForAddPoint = false;
                awaitingMapClickForSearch = false;
                message = awaitingMapClickForRemove ? "Click on map to remove nearest point." : "Remove mode off.";
                messageTimer = SDL_GetTicks();
                lastSearchIdx = -1;
            }
            else if (mx >= searchBtn.x && mx <= searchBtn.x + searchBtn.w && my >= searchBtn.y && my <= searchBtn.y + searchBtn.h) {
                awaitingMapClickForSearch = !awaitingMapClickForSearch;
                awaitingMapClickForAddPoint = false;
                awaitingMapClickForRemove = false;
                message = awaitingMapClickForSearch ? "Click on map to find nearest point." : "Search mode off.";
                messageTimer = SDL_GetTicks();
                lastSearchIdx = -1;
            }
            else if (mx >= deleteCatBtn.x && mx <= deleteCatBtn.x + deleteCatBtn.w && my >= deleteCatBtn.y && my <= deleteCatBtn.y + deleteCatBtn.h) {
                string n = categories[selectedCategoryIndex].name;
                categories.erase(categories.begin() + selectedCategoryIndex);
                selectedCategoryIndex = -1; state = MAIN_VIEW;
                message = "Deleted group: " + n; messageTimer = SDL_GetTicks();
                awaitingMapClickForAddPoint = false;
                awaitingMapClickForRemove = false;
                awaitingMapClickForSearch = false;
            }
            else if (mx >= addRandomBtn.x && mx <= addRandomBtn.x + addRandomBtn.w && my >= addRandomBtn.y && my <= addRandomBtn.y + addRandomBtn.h) {
                if (selectedCategoryIndex >= 0) {
                    auto& pts = categories[selectedCategoryIndex].points;
                    for (int i = 0; i < 100; ++i) {
                        int rx = rand() % mapInnerW;
                        int ry = rand() % mapInnerH;
                        pts.push_back({rx, ry});
                    }
                    message = "Added 100 random points.";
                    messageTimer = SDL_GetTicks();
                }
            }
        }
        wasClicking = isClicking;
    }
}

void render() {
    SDL_SetRenderDrawColor(ren, 240, 240, 240, 255);
    SDL_RenderClear(ren);

    SDL_Rect mapRect{ mapX, mapY, mapW, mapH };
    SDL_SetRenderDrawColor(ren, 220, 220, 220, 255);
    SDL_RenderFillRect(ren, &mapRect);

    SDL_Rect leftRect{ 0,0,mapX, windowH };
    SDL_SetRenderDrawColor(ren, 32, 32, 32, 255);
    SDL_RenderFillRect(ren, &leftRect);

    SDL_SetRenderDrawColor(ren, 160, 160, 160, 255);
    int axisX = mapX + AXIS_PADDING;
    int axisY = mapY + mapH - AXIS_PADDING;

    SDL_RenderDrawLine(ren, axisX, axisY, mapX + mapW - AXIS_PADDING, axisY);
    SDL_RenderDrawLine(ren, axisX, mapY + AXIS_PADDING, axisX, axisY);

    if (font) {
        for (int t = 0; t <= 4; ++t) {
            int tx = axisX + mapInnerW * t / 4;
            SDL_RenderDrawLine(ren, tx, axisY - 4, tx, axisY + 4);
            int w, h; string label = to_string(mapInnerW * t / 4);
            SDL_Texture* tex = createTextTexture(ren, font, label, { 0,0,0,255 }, w, h);
            if (tex) { SDL_Rect dst{ tx - 8, axisY + 8, w, h }; SDL_RenderCopy(ren, tex, nullptr, &dst); SDL_DestroyTexture(tex); }

            int ty = axisY - mapInnerH * t / 4;
            SDL_RenderDrawLine(ren, axisX - 4, ty, axisX + 4, ty);
            label = to_string(mapInnerH * t / 4);
            tex = createTextTexture(ren, font, label, { 0,0,0,255 }, w, h);
            if (tex) { SDL_Rect dst{ axisX - 30, ty - 8, w, h }; SDL_RenderCopy(ren, tex, nullptr, &dst); SDL_DestroyTexture(tex); }
        }
    }

    for (size_t i = 0; i < categories.size(); ++i) {
        auto& cat = categories[i];
        bool isHighlighted = cat.selected || (state == VENDING_VIEW && (int)i == selectedCategoryIndex);

        for (size_t j = 0; j < cat.points.size(); ++j) {
            auto p_graph = cat.points[j];
            auto p_world = graphToWorld(p_graph.first, p_graph.second);
            int wx = p_world.first;
            int wy = p_world.second;

            if (isHighlighted) {
                SDL_SetRenderDrawColor(ren, cat.color.r, cat.color.g, cat.color.b, 120);
                drawFilledCircle(ren, wx, wy, pointRadius + 4);
            }

            if (state == VENDING_VIEW && (int)i == selectedCategoryIndex && (int)j == lastSearchIdx) {
                SDL_SetRenderDrawColor(ren, 255, 32, 32, 255);
                drawFilledCircle(ren, wx, wy, pointRadius + 8);
            }

            SDL_SetRenderDrawColor(ren, cat.color.r, cat.color.g, cat.color.b, 255);
            drawFilledCircle(ren, wx, wy, pointRadius);
        }
    }

    if (state == MAIN_VIEW) {
        int bx = 20, by = 20, bw = mapX - 40, bh = 40, spacing = 8;
        for (size_t i = 0; i < categories.size(); ++i) {
            SDL_Rect b{ bx, by + (int)i * (bh + spacing), bw, bh };
            drawButton(categories[i].name, b, categories[i].color, categories[i].selected);
        }

        SDL_Rect addBtn{ 20, 240, mapX - 40, 40 }; drawButton("Add Category", addBtn, Color{ 70,130,180,255 }, textInputActive);
        SDL_Rect removeAllBtn{ 20, 290, mapX - 40, 40 }; drawButton("Remove All", removeAllBtn, Color{ 180,70,70,255 }, false);

        int sw = sliderRect.w; int sx = sliderRect.x; int sy = sliderRect.y; int sh = sliderRect.h;

        if (font) {
            int labelW, labelH;
            SDL_Texture* labtex = createTextTexture(ren, font, string("Point Size: ") + to_string(pointRadius), { 255,255,255,255 }, labelW, labelH);
            if (labtex) { SDL_Rect ld{ sx, sy - 26, labelW, labelH }; SDL_RenderCopy(ren, labtex, nullptr, &ld); SDL_DestroyTexture(labtex); }
        }
        SDL_SetRenderDrawColor(ren, 100, 100, 100, 255); SDL_Rect track{ sx, sy + sh / 2 - 4, sw, 8 }; SDL_RenderFillRect(ren, &track);
        float t = float(pointRadius - 4) / float(20);
        int thumbX = sx + int(t * sw);
        SDL_SetRenderDrawColor(ren, 200, 200, 200, 255); SDL_Rect thumb{ thumbX - 8, sy, 16, sh }; SDL_RenderFillRect(ren, &thumb);

        if (textInputActive) {
            SDL_Rect tin{ textInputRect.x, textInputRect.y, textInputRect.w, textInputRect.h };
            SDL_SetRenderDrawColor(ren, 255, 255, 255, 230); SDL_RenderFillRect(ren, &tin);
            SDL_SetRenderDrawColor(ren, 0, 0, 0, 255); SDL_RenderDrawRect(ren, &tin);
            if (font) {
                int tw, th;
                string display_text = textBuffer.empty() ? string("Type category name...") : textBuffer;
                SDL_Color text_col = textBuffer.empty() ? SDL_Color{ 150,150,150,255 } : SDL_Color{ 0,0,0,255 };
                SDL_Texture* ttex = createTextTexture(ren, font, display_text, text_col, tw, th);
                if (ttex) { 
                    SDL_Rect dst{ tin.x + 6, tin.y + (tin.h - th) / 2, tw, th }; 
                    SDL_RenderCopy(ren, ttex, nullptr, &dst); 
                    SDL_DestroyTexture(ttex); 
                }
                bool cursorVisible = (SDL_GetTicks() / 500) % 2 == 0;

                if (cursorVisible) {
                    int textWidth, textHeight;
                    TTF_SizeUTF8(font, textBuffer.c_str(), &textWidth, &textHeight);
                    SDL_Rect cursorRect;
                    cursorRect.x = tin.x + 6 + textWidth; 
                    cursorRect.y = tin.y + 4;                       
                    cursorRect.w = 2;                               
                    cursorRect.h = tin.h - 8;                       
                    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
                    SDL_RenderFillRect(ren, &cursorRect);
                }
            }
        }
    }

    if (state == VENDING_VIEW && selectedCategoryIndex >= 0 && selectedCategoryIndex < (int)categories.size()) {
        SDL_SetRenderDrawColor(ren, 48, 48, 48, 220);
        SDL_Rect panel{ 10, 10, mapX - 20, 380 }; SDL_RenderFillRect(ren, &panel);

        int bx = 20; int by = 20; int bw = mapX - 60; int bh = 36; int gap = 12;
        auto& currentCat = categories[selectedCategoryIndex];

        if (font) {
            int tw, th;
            string title = "Editing: " + currentCat.name;
            SDL_Texture* ttex = createTextTexture(ren, font, title, { 255,255,255,255 }, tw, th);
            if (ttex) { SDL_Rect dst{ bx, by, tw, th }; SDL_RenderCopy(ren, ttex, nullptr, &dst); SDL_DestroyTexture(ttex); }
        }
        by += 40 + gap;

        SDL_Rect backBtn{ bx, by, 42, 36 }; drawButton("<-", backBtn, Color{ 100,100,100,255 }, false);
        SDL_Rect addPointBtn{ bx + 54, by, bw - 54, bh };
        drawButton("Add point (click map)", addPointBtn, Color{ 80,160,80,255 }, awaitingMapClickForAddPoint);

        by += (bh + gap);

        SDL_Rect remPointBtn{ bx, by, bw, bh }; 
        drawButton("Remove nearest point (click map)", remPointBtn, Color{ 200,100,80,255 }, awaitingMapClickForRemove);

        by += (bh + gap);

        SDL_Rect searchBtn{ bx, by, bw, bh };
        drawButton("Search nearest point to mouse", searchBtn, Color{ 100,140,220,255 }, awaitingMapClickForSearch || lastSearchIdx != -1);

        by += (bh + gap);

        SDL_Rect deleteCatBtn{ bx, by, bw, bh }; drawButton("Delete Category", deleteCatBtn, Color{ 180,60,60,255 }, false);

        by += (bh + gap);
        SDL_Rect addRandomBtn{ bx, by, bw, bh }; 
        drawButton("Add 100 Random Points", addRandomBtn, Color{ 200, 140, 40, 255 }, false);


        if (font) {
            int tw, th;
            string info = "Total Points: " + to_string(currentCat.points.size());
            SDL_Texture* ttex = createTextTexture(ren, font, info, { 200,200,200,255 }, tw, th);
            if (ttex) { 
                SDL_Rect dst{ bx, by + (bh + gap), tw, th }; // This 'by' is now correct
                SDL_RenderCopy(ren, ttex, nullptr, &dst); 
                SDL_DestroyTexture(ttex); 
            }
        }
    }

    if (!message.empty()) {
        if (SDL_GetTicks() - messageTimer < 3000) {
            if (font) {
                int tw, th;
                SDL_Texture* tex = createTextTexture(ren, font, message, { 0,0,0,255 }, tw, th);
                if (tex) {
                    int padding = 10;
                    SDL_Rect dst{ mapX + 20, 20, tw, th };
                    SDL_Rect bgRect{ dst.x - padding, dst.y - padding, dst.w + 2 * padding, dst.h + 2 * padding };
                    SDL_SetRenderDrawColor(ren, 255, 255, 200, 220);
                    SDL_RenderFillRect(ren, &bgRect);
                    SDL_RenderCopy(ren, tex, nullptr, &dst);
                    SDL_DestroyTexture(tex);
                }
            }
        }
        else {
            message.clear();
        }
    }

    if (font) {
        int lx = 20;
        int ly = windowH - 140;
        int idx = 0;
        int padding = 4;
        for (auto& c : categories) {
            if (c.selected) {
                int tw, th;
                SDL_Texture* tex = createTextTexture(ren, font, c.name, { 255,255,255,255 }, tw, th);
                if (tex) {
                    SDL_Rect dst{ lx + padding * 2, ly + idx * (th + padding * 2) + padding, tw, th };
                    SDL_Rect bgRect{ lx, ly + idx * (th + padding * 2), tw + padding * 4, th + padding * 2 };

                    SDL_SetRenderDrawColor(ren, c.color.r, c.color.g, c.color.b, 255);
                    SDL_RenderFillRect(ren, &bgRect);
                    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
                    SDL_RenderDrawRect(ren, &bgRect);

                    SDL_RenderCopy(ren, tex, nullptr, &dst);
                    SDL_DestroyTexture(tex);
                    idx++;
                }
            }
        }
        if (idx > 0) {
            int tw, th;
            SDL_Texture* titleTex = createTextTexture(ren, font, "Active Groups:", { 255, 255, 255, 255 }, tw, th);
            if (titleTex) {
                SDL_Rect dst{ lx, ly - th - 5, tw, th };
                SDL_RenderCopy(ren, titleTex, nullptr, &dst);
                SDL_DestroyTexture(titleTex);
            }
        }
    }

    if ((awaitingMapClickForAddPoint || awaitingMapClickForRemove || awaitingMapClickForSearch) && font) {
        int mx, my;
        SDL_GetMouseState(&mx, &my); 

        if (mx >= mapX) { 
            auto gp = worldToGraph(mx, my);
            string coordText = "(" + to_string(gp.first) + ", " + to_string(gp.second) + ")";

            int tw, th;
            SDL_Texture* tex = createTextTexture(ren, font, coordText, { 0, 0, 0, 255 }, tw, th);
            if (tex) {
                SDL_Rect dst{ mx + 15, my + 10, tw, th }; 
                
                SDL_Rect bgRect{ dst.x - 4, dst.y - 2, dst.w + 8, dst.h + 4 };
                SDL_SetRenderDrawColor(ren, 255, 255, 255, 200);
                SDL_RenderFillRect(ren, &bgRect);

                SDL_RenderCopy(ren, tex, nullptr, &dst);
                SDL_DestroyTexture(tex);
            }
        }
    }

    SDL_RenderPresent(ren);
}

int main(int argc, char** argv) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) { SDL_Log("SDL Setup Failed: %s", SDL_GetError()); return 1; }
    if (TTF_Init() != 0) { SDL_Log("Font Setup Failed: %s", TTF_GetError()); SDL_Quit(); return 1; }

    win = SDL_CreateWindow("Map & Categories UI", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, SDL_WINDOW_RESIZABLE);
    if (!win) { SDL_Log("Window Error: %s", SDL_GetError()); TTF_Quit(); SDL_Quit(); return 1; }
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!ren) { SDL_Log("Renderer Error: %s", SDL_GetError()); SDL_DestroyWindow(win); TTF_Quit(); SDL_Quit(); return 1; }

    font = TTF_OpenFont("arial.ttf", 16);
    if (!font) { SDL_Log("Warning: 'arial.ttf' not found. Text may look weird."); }

    srand((unsigned)time(nullptr));

    categories.push_back({ "Vending", makeColor(), {{100,100},{200,150},{180,80}} });
    categories.push_back({ "Dustbin", makeColor(), {{300,200},{360,220}} });
    categories.push_back({ "GDFGHJ", makeColor(), {{120,320},{220,300},{420,120}} });
    computeLayout(WINDOW_W, WINDOW_H);

    bool running = true;
    while (running) {
        handleInput(running); 
        render();             
        SDL_Delay(10);        
    }
    
    if (font) TTF_CloseFont(font);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    TTF_Quit();
    SDL_Quit();
    return 0;
}