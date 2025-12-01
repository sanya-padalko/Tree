#include "animation.h"
#include "tree.h"

void WindowCreating() {
    txCreateWindow (600, 500);
    txSelectFont("Arial", 20);
    txSetFillColor (RGB (0, 0, 0));
}

void PrintText(const char* text) {
    txTextOut (250, 0, text);
}

void PrintVideo(const char* video_name, int frame_cnt, double frame_delay) {

    ClearScreen();

    for (int i = 1; i <= frame_cnt; ++i) {
        char image_name[100];
        snprintf(image_name, 100, "C:/Users/user/Documents/GitHub/Tree/animation/%s/%s_%d.bmp", video_name, video_name, i);

        PrintImage(image_name);

        txSleep(frame_delay);
    }

    txSleep(500);
}

void PrintCharacter(Node_t* node) {
    if (!node)
        return;

    PrintIcon(node->message);
}

void PrintIcon(const char* name) {
    if (!name)
        return;

    char* real_name = strdup(name);
    int i = 0;
    while (real_name[i] != '\0') {
        if (real_name[i] == ' ')
            real_name[i] = '_';

        ++i;
    }

    char image_name[100];
    snprintf(image_name, 100, "C:/Users/user/Documents/GitHub/Tree/icons/%s.bmp", real_name);

    PrintImage(image_name);

    txSleep(2000);
}

void PrintImage(const char* image_name) {
    if (!image_name) 
        return;

    HDC image = txLoadImage(image_name);
    POINT center = { txGetExtentX() / 2, txGetExtentY() / 2 };
    POINT size = txGetExtent (image);
    txBitBlt(center.x - size.x / 2, center.y - size.y / 2, image);
    txDeleteDC(image);
}

void CreateCharacter(const char* name) {
    HDC new_image = txLoadImage("C:/Users/user/Documents/Github/Tree/icons/empty_icon.bmp");
    char image_path[100];
    snprintf(image_path, 100, "C:/Users/user/Documents/Github/Tree/icons/%s.bmp", name);
    txSaveImage(image_path, new_image);

    txDeleteDC(new_image);
}

void ClearScreen() {
    txClear();
    txClearConsole();
}
