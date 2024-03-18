#pragma once

#include <engine.h>

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>

#define DELAY 1000 / 30

typedef struct Player {
  const AVCodec *videoCodec;

  AVFrame *frame;
  AVCodecContext *codecContext;
  AVCodecParameters *codecParameters;
  AVFormatContext *formatContext;
  AVPacket packet;

  int streamIndex;
} Player;

void CreateTexture(Player *player, Engine *engine, const char *path);
void UpdateTexture(Player *player, Engine *engine);
void RemoveTexture(Player *player);