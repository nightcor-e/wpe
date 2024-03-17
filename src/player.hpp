#pragma once

extern "C" {
  #include <libavcodec/avcodec.h>
  #include <libavformat/avformat.h>
}

#include <SDL2/SDL.h>

#include <windows.h>
#include <string>

#define DELAY 1000 / 30

class Player {
public:
    Player();

    void init(std::string &path);
    void update();
    void clean();

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Event event;
    
    AVFrame *frame;
    AVPacket packet;
    AVCodecContext *codecContext;
    AVFormatContext *formatContext = nullptr;
    const AVCodec *videoCodec = nullptr;
    AVCodecParameters *codecParameters = nullptr;
    int videoStreamIndex = -1;

    bool isPlaying = true;
};