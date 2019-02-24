#include "framebuffer.h"
#include "map.h"
#include "player.h"
#include "raycaster.h"
#include "rgba.h"
#include "sprite.h"
#include "textures.h"
#include "utils.h"

#include "SDL.h"

#include <iostream>
#include <vector>

int main()
{
    const Dimension2D<size_t> windowSize = {1024, 512};
    FrameBuffer fb(std::move(windowSize));
    fb.clear(packColor(RGBA::white()));

    Player player({3.456, 2.345}, 1.523, M_PI / 3.0);
    Map map;
    TextureSet wallTextures("content/walltext.png");
    TextureSet monsterTextures("content/monsters.png");
    if (0 == wallTextures.getCount() || 0 == monsterTextures.getCount()) {
        std::cerr << "Failed to load textures" << std::endl;
        return -1;
    }
    std::vector<Sprite> sprites{{{3.523, 3.812}, 2},
                                {{1.834, 8.765}, 0},
                                {{5.323, 5.365}, 1},
                                {{4.123, 10.265}, 1}};

    render(fb, map, player, sprites, wallTextures, monsterTextures);

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    if (SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "Couldn't initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    if (SDL_CreateWindowAndRenderer(fb.getWidth(),
                                    fb.getHeight(),
                                    SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS,
                                    &window,
                                    &renderer)) {
        std::cerr << "Couldn't create window and renderer: " << SDL_GetError()
                  << std::endl;
        return -1;
    }

    SDL_Texture* frameBufferTexture =
        SDL_CreateTexture(renderer,
                          SDL_PIXELFORMAT_ABGR8888,
                          SDL_TEXTUREACCESS_STREAMING,
                          fb.getWidth(),
                          fb.getHeight());
    SDL_UpdateTexture(frameBufferTexture,
                      NULL,
                      reinterpret_cast<const void*>(fb.getImage().data()),
                      fb.getWidth() * 4);

    SDL_Event event;
    while (true) {
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) {
            break;
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, frameBufferTexture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(frameBufferTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    // writePPMImage("./out.ppm", fb);

    return 0;
}