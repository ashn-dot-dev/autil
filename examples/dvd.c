#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#define AENGN_IMPLEMENTATION
#include "../aengn.h"

#define VERSION "0.1"

#define SCREEN_W 540
#define SCREEN_H 360
#define PIXEL_SCALE 2

struct aengn_sprite* dvd = NULL;
struct Mix_Chunk* bump = NULL;
static int x = 0;
static int y = 20;
static int dx = 1;
static int dy = 1;

static void
update_color(struct aengn_sprite* spr, struct aengn_rgba const* color)
{
    for (int x = 0; x < aengn_sprite_w(spr); ++x) {
        for (int y = 0; y < aengn_sprite_h(spr); ++y) {
            struct aengn_rgba pixel;
            aengn_sprite_get_pixel(spr, x, y, &pixel);
            if (pixel.a == 0xff) {
                aengn_sprite_set_pixel(spr, x, y, color);
            }
        }
    }
}

static uint8_t
rand_rgb(void)
{
    uint8_t const val = (uint8_t)(rand() % UINT8_MAX);
    static uint8_t const min = 0x20;
    return val > min ? val : min;
}

static void
update_color_random(struct aengn_sprite* spr)
{
    uint8_t const r = rand_rgb();
    uint8_t const g = rand_rgb();
    uint8_t const b = rand_rgb();
    update_color(spr, AENGN_RGBA_LOCAL_PTR(r, g, b, 0xff));
}

static int
runtick(void* ctx)
{
    (void)ctx;
    static double time = 0.0;
    static double prev = 0.0;
    double const now = aengn_now();
    time += now - prev;
    prev = now;

    static double const TIMESTEP = 1.0 / 60.0;
    if (time < TIMESTEP) {
        return 0;
    }
    time -= TIMESTEP;

    // Escape => Close the application.
    if (aengn_virtkey_state(SDLK_ESCAPE)->released) {
        return 1;
    }
    // Z => Change the dvd logo to white.
    if (aengn_virtkey_state(SDLK_z)->down) {
        update_color(dvd, AENGN_RGBA_LOCAL_PTR(0xff, 0xff, 0xff, 0xff));
    }
    // X => Change the dvd logo to a random color.
    if (aengn_virtkey_state(SDLK_x)->down) {
        update_color_random(dvd);
    }

    x += dx;
    y += dy;
    if (x <= 0 || (x + aengn_sprite_w(dvd)) >= aengn_screen_w()) {
        dx *= -1;
        update_color_random(dvd);
        aengn_play_chunk(bump);
    }
    if (y <= 0 || (y + aengn_sprite_h(dvd)) >= aengn_screen_h()) {
        dy *= -1;
        update_color_random(dvd);
        aengn_play_chunk(bump);
    }

    aengn_draw_clear(NULL);
    aengn_draw_sprite(dvd, x, y);

    aengn_end_frame();
    return 0;
}

int
main(void)
{
    if (aengn_init(SCREEN_W, SCREEN_H, PIXEL_SCALE) != 0) {
        return EXIT_FAILURE;
    }

    dvd = aengn_load_sprite("dvd-assets/dvd-logo-90x40.png");
    if (dvd == NULL) {
        return EXIT_FAILURE;
    }
    bump = aengn_load_chunk("dvd-assets/bump.ogg");
    if (bump == NULL) {
        return EXIT_FAILURE;
    }

    aengn_run(runtick, NULL);

    aengn_sprite_del(dvd);
    Mix_FreeChunk(bump);
    return EXIT_SUCCESS;
}
