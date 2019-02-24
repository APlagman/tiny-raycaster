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

    Player player({3.456f, 2.345f}, 1.523f, M_PIf32 / 3.0f);
    Map map;
    TextureSet wallTextures("content/walltext.png");
    TextureSet monsterTextures("content/monsters.png");
    if (0 == wallTextures.getCount() || 0 == monsterTextures.getCount()) {
        std::cerr << "Failed to load textures" << std::endl;
        return -1;
    }
    std::vector<Sprite> sprites{{{3.523f, 3.812f}, 2},
                                {{1.834f, 8.765f}, 0},
                                {{5.323f, 5.365f}, 1},
                                {{4.123f, 10.265f}, 1}};

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

    SDL_Event event;
    while (true) {
        if (SDL_PollEvent(&event)) {
            if (SDL_QUIT == event.type
                || (SDL_KEYDOWN == event.type
                    && SDLK_ESCAPE == event.key.keysym.sym)) {
                break;
            }
            if (SDL_KEYUP == event.type) {
                switch (event.key.keysym.sym) {
                    case 'a':
                    case 'd':
                        player.setTurningDirection(Direction::None);
                        break;
                    case 'w':
                    case 's':
                        player.setWalkingDirection(Direction::None);
                        break;
                    default:
                        break;
                }
            }
            if (SDL_KEYDOWN == event.type) {
                switch (event.key.keysym.sym) {
                    case 'a':
                        player.setTurningDirection(Direction::Left);
                        break;
                    case 'd':
                        player.setTurningDirection(Direction::Right);
                        break;
                    case 'w':
                        player.setWalkingDirection(Direction::Forward);
                        break;
                    case 's':
                        player.setWalkingDirection(Direction::Backward);
                        break;
                    default:
                        break;
                }
            }
        }

        player.setAngle(player.getAngle()
                        + static_cast<float>(player.getTurningDirection())
                              * 0.05f);
        const Point2D<float> newPlayerPos = {
            player.getX()
                + static_cast<float>(player.getWalkingDirection())
                      * std::cos(player.getAngle()) * 0.1f,
            player.getY()
                + static_cast<float>(player.getWalkingDirection())
                      * std::sin(player.getAngle()) * 0.1f};
        const Point2D<float> playerRectTopLeft =
            newPlayerPos - Point2D<float>{0.1f, 0.1f};

        if (newPlayerPos.x >= 0 && newPlayerPos.y < map.getWidth()
            && newPlayerPos.y >= 0 && newPlayerPos.y < map.getHeight()
            && map.isEmpty(cast<float, size_t>(playerRectTopLeft))
            && map.isEmpty(cast<float, size_t>(playerRectTopLeft
                                               + Point2D<float>{0.2f, 0.0f}))
            && map.isEmpty(cast<float, size_t>(playerRectTopLeft
                                               + Point2D<float>{0.0f, 0.2f}))
            && map.isEmpty(cast<float, size_t>(playerRectTopLeft
                                               + Point2D<float>{0.2f, 0.2f}))) {
            player.setX(newPlayerPos.x);
            player.setY(newPlayerPos.y);
        }

        render(fb, map, player, sprites, wallTextures, monsterTextures);
        SDL_UpdateTexture(frameBufferTexture,
                          NULL,
                          reinterpret_cast<const void*>(fb.getImage().data()),
                          fb.getWidth() * 4);

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