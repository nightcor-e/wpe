#include <player.hpp>

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
  if (FindWindowEx(hwnd, nullptr, "SHELLDLL_DefView", nullptr)) {
    *(HWND *)lParam = FindWindowEx(NULL, hwnd, "WorkerW", nullptr);
    return false;
  }

  return true;
}

HWND getDesktop() {
  HWND progman = FindWindow("ProgMan", nullptr);
  HWND desktop = nullptr;

  SendMessageTimeout(progman, 0x052C, 0, 0, SMTO_NORMAL, 1000, nullptr);
  EnumWindows(EnumWindowsProc, (LPARAM)&desktop);

  return desktop;
}

Player::Player() {}

void Player::init(std::string &path) {
  if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
    window = SDL_CreateWindowFrom((void *)getDesktop());
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (renderer) {
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    }
  }

  avformat_network_init();

  avformat_open_input(&formatContext, path.c_str(), nullptr, nullptr);
  avformat_find_stream_info(formatContext, nullptr);

  for (unsigned int i = 0; i < formatContext->nb_streams; i++) {
    if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
      videoStreamIndex = i;
      codecParameters = formatContext->streams[i]->codecpar;
      break;
    }
  }

  if (videoStreamIndex != -1 && codecParameters != nullptr) {
    videoCodec = avcodec_find_decoder(codecParameters->codec_id);

    if (videoCodec) {
      codecContext = avcodec_alloc_context3(videoCodec);

      if (codecContext && avcodec_parameters_to_context(codecContext, codecParameters) >= 0) {
        if (avcodec_open2(codecContext, videoCodec, nullptr) >= 0) {
          frame = av_frame_alloc();

          texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_YV12,
            SDL_TEXTUREACCESS_STREAMING, codecContext->width, codecContext->height);
        }
      }
    }
  }
}

void Player::update() {
  SDL_RenderClear(renderer);

  while (av_read_frame(formatContext, &packet) >= 0) {
    if (packet.stream_index == videoStreamIndex) {
      avcodec_send_packet(codecContext, &packet);
      
      while (avcodec_receive_frame(codecContext, frame) == 0) {
        SDL_PollEvent(&event);

        if (event.type == SDL_QUIT) {
          isPlaying = false;
          return;
        }

        SDL_UpdateYUVTexture(texture, nullptr, frame->data[0], frame->linesize[0],
          frame->data[1], frame->linesize[1], frame->data[2], frame->linesize[2]);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
        SDL_Delay(DELAY);
      }
    }
    av_packet_unref(&packet);
  }

  av_seek_frame(formatContext, videoStreamIndex, 0, AVSEEK_FLAG_FRAME);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderPresent(renderer);
}

void Player::clean() {
  av_frame_free(&frame);
  avcodec_close(codecContext);
  avformat_close_input(&formatContext);
  avformat_network_deinit();

  SDL_DestroyTexture(texture);
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
}