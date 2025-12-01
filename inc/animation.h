#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "TXLib.h"

struct Node_t;

const double openning_delay = 2.0;
const double losing_delay = 8.0;
const double common_delay = 8.0;
const double explanation_delay = 3.0;
const double winning_delay = 3.0;
const double finding_delay = 8.0;
const double final_delay = 1.0;

const int openning_frames = 370;
const int explanation_frames = 150;
const int common_frames = 87;
const int finding_frames = 150;
const int losing_frames = 150;
const int winning_frames = 390;
const int final_frames = 500;

void WindowCreating();
void PrintText(const char* text);
void PrintVideo(const char* video_name, int frame_cnt, double frame_delay);
void PrintCharacter(Node_t* node);
void PrintIcon(const char* name);
void PrintImage(const char* image_name);
void CreateCharacter(const char* name);
void ClearScreen();

#endif // _ANIMATION_H_