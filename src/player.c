#include <player.h>

void CreateTexture(Player *player, Engine *engine, const char *path) {
  avformat_network_init();

  avformat_open_input(&player->formatContext, path, NULL, NULL);
  avformat_find_stream_info(player->formatContext, NULL);

  for (unsigned int i = 0; i < player->formatContext->nb_streams; i++) {
    if (player->formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
      player->streamIndex = i;
      player->codecParameters = player->formatContext->streams[i]->codecpar;
      break;
    }
  }
  
  if (player->streamIndex == -1 || player->codecParameters == NULL) return;
  player->videoCodec = avcodec_find_decoder(player->codecParameters->codec_id);

  if (!player->videoCodec) return;
  player->codecContext = avcodec_alloc_context3(player->videoCodec);

  if (!player->codecContext ||
      avcodec_parameters_to_context(player->codecContext, player->codecParameters) < 0 ||
      avcodec_open2(player->codecContext, player->videoCodec, NULL) < 0) return;
  player->frame = av_frame_alloc();

  engine->texture = SDL_CreateTexture(
    engine->renderer, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING,
    player->codecContext->width, player->codecContext->height);
}

void UpdateTexture(Player *player, Engine *engine) {
  SDL_RenderClear(engine->renderer);

  while (av_read_frame(player->formatContext, &player->packet) >= 0) {
    if (player->packet.stream_index == player->streamIndex) {
      avcodec_send_packet(player->codecContext, &player->packet);

      while (avcodec_receive_frame(player->codecContext, player->frame) == 0) {
        SDL_UpdateYUVTexture(engine->texture, NULL, player->frame->data[0], player->frame->linesize[0],
                             player->frame->data[1], player->frame->linesize[1], player->frame->data[2], player->frame->linesize[2]);
        SDL_RenderClear(engine->renderer);
        SDL_RenderCopy(engine->renderer, engine->texture, NULL, NULL);
        SDL_RenderPresent(engine->renderer);
        SDL_Delay(DELAY);
      }
    }

    av_packet_unref(&player->packet);
  }

  av_seek_frame(player->formatContext, player->streamIndex, 0, AVSEEK_FLAG_FRAME);

  SDL_SetRenderDrawColor(engine->renderer, 0, 0, 0, 255);
  SDL_RenderPresent(engine->renderer);
}

void RemoveTexture(Player *player) {
  av_frame_free(&player->frame);
  avcodec_close(player->codecContext);
  avformat_close_input(&player->formatContext);
  avformat_network_deinit();
}