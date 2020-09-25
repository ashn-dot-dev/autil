#define AUTIL_IMPLEMENTATION
#include "../../autil.h"
#define AENGN_IMPLEMENTATION
#include "../../aengn.h"

#define VERSION "0.2"

static struct rgba const black = {0x00, 0x00, 0x00, 0xff};
static struct rgba const white = {0xff, 0xff, 0xff, 0xff};
static struct rgba const iconc = {0xaa, 0xaa, 0xaa, 0x77};
static struct rgba const red = {0xff, 0x00, 0x00, 0xff};

#define CELLS_W 64
#define CELLS_H 48
#define PIXEL_SCALE 16
// OFFSET accounts for the always-dead 1-cell boundary around the visible
// region of cells on screen. Keeping this extra boundary allows for the
// calculation of cell neighbors to use (x|y)-1 and (x|y)+1 without ever
// indexing out of bounds.
#define OFFSET 1
// cells[x][y] == 1 => Cell is alive.
// cells[x][y] == 0 => Cell is dead.
static int cells[CELLS_W + 2 * OFFSET][CELLS_H + 2 * OFFSET];
// neighbors[x][y] => Number of neighbors that the cell at (x, y) has.
static int neighbors[CELLS_W + 2 * OFFSET][CELLS_H + 2 * OFFSET];

// Seconds per fixed-timestep update.
static double const TIMESTEP = 1.0 / 20.0;
// Updates per generation.
#define UPG_1 ((int)6)
#define UPG_2 ((int)3)
#define UPG_3 ((int)1)
static int upg = UPG_2;
// Updates until the next generation.
static int updates_remaining = 0;
// Advance generations rapidly if true.
static bool run = false;
// Advance one generation and then pause if true.
static bool step = false;
// Hide cursor and UI icons (play and pause) if true.
static bool hide = false;

static struct sprite* spr_cells = NULL;
static struct sprite* spr_mouse = NULL;
static struct sprite* spr_icon_play = NULL;
static struct sprite* spr_icon_pause = NULL;
static struct sprite* spr_icon_speed = NULL;

static void
usage(void);
static void
argparse(int argc, char** argv);

static int
main_init(void);
static void
main_fini(void);

static int
runtick(void* ctx);
static int
hinput(void);
static void
update(void);
static void
render(void);
static void
advance(void);

int
main(int argc, char** argv)
{
    argparse(argc, argv);

    if (aengn_init(CELLS_W, CELLS_H, PIXEL_SCALE) != 0) {
        return EXIT_FAILURE;
    }
    atexit(aengn_fini);

    if (main_init() != 0) {
        return EXIT_FAILURE;
    }
    atexit(main_fini);

    aengn_run(runtick, NULL);
    return EXIT_SUCCESS;
}

static void
usage(void)
{
    // clang-format off
    fputs(
        "Usage: life [OPTION]..."                                       "\n"
        "Options:"                                                      "\n"
        "  -h, --help       Display usage information and exit."        "\n"
        "      --version    Display version information and exit."      "\n"
        "Controls:"                                                     "\n"
        "   Left mouse button   => Turn cell under cursor alive."       "\n"
        "   Right mouse button  => Turn cell under cursor dead."        "\n"
        "   Middle mouse button => Turn cell under cursor dead."        "\n"
        "   R or RETURN         => Start / stop simulation."            "\n"
        "   S or SPACE          => Advance one generation."             "\n"
        "   C                   => Turn all cells on the board dead."   "\n"
        "   H                   => Hide / show cursor & UI icons."      "\n"
        "   1,2,3               => Set simulation speed to slow,"       "\n"
        "                          medium, or fast, respectively."      "\n"
        , stderr
    );
    // clang-format on
}

static void
argparse(int argc, char** argv)
{
    bool error = false;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            usage();
            exit(EXIT_SUCCESS);
        }
        if (strcmp(argv[i], "--version") == 0) {
            puts(VERSION);
            exit(EXIT_SUCCESS);
        }

        if (strncmp(argv[i], "-", 1) == 0 || strncmp(argv[i], "--", 2) == 0) {
            errorf("Unrecognized command line option '%s'", argv[i]);
            error = true;
            continue;
        }
    }

    if (error) {
        exit(EXIT_FAILURE);
    }
}

static int
main_init(void)
{
    spr_cells = sprite_new(screen_w() + OFFSET, screen_h() + OFFSET);
    if (spr_cells == NULL) {
        goto error;
    }

    spr_mouse = sprite_new(1, 1);
    if (spr_mouse == NULL) {
        goto error;
    }
    sprite_set_pixel(spr_mouse, 0, 0, &red);

    static int const PLAY_PAUSE_W = 3;
    static int const PLAY_PAUSE_H = 3;

    spr_icon_play = sprite_new(PLAY_PAUSE_W, PLAY_PAUSE_H);
    if (spr_icon_play == NULL) {
        goto error;
    }
    sprite_set_pixel(spr_icon_play, 0, 0, &iconc);
    sprite_set_pixel(spr_icon_play, 0, 1, &iconc);
    sprite_set_pixel(spr_icon_play, 0, 2, &iconc);
    sprite_set_pixel(spr_icon_play, 1, 0, &iconc);
    sprite_set_pixel(spr_icon_play, 1, 1, &iconc);
    sprite_set_pixel(spr_icon_play, 1, 2, &iconc);
    sprite_set_pixel(spr_icon_play, 2, 1, &iconc);

    spr_icon_pause = sprite_new(PLAY_PAUSE_W, PLAY_PAUSE_H);
    if (spr_icon_pause == NULL) {
        goto error;
    }
    sprite_set_pixel(spr_icon_pause, 0, 0, &iconc);
    sprite_set_pixel(spr_icon_pause, 0, 1, &iconc);
    sprite_set_pixel(spr_icon_pause, 0, 2, &iconc);
    sprite_set_pixel(spr_icon_pause, 2, 0, &iconc);
    sprite_set_pixel(spr_icon_pause, 2, 1, &iconc);
    sprite_set_pixel(spr_icon_pause, 2, 2, &iconc);

    spr_icon_speed = sprite_new(1, 1);
    sprite_set_pixel(spr_icon_speed, 0, 0, &iconc);

    return 0;

error:
    main_fini();
    return -1;
}

static void
main_fini(void)
{
#define SPRITE_DEL_IF_NOT_NULL(spr_)                                           \
    if (spr_ != NULL) {                                                        \
        sprite_del(spr_);                                                      \
    }
    SPRITE_DEL_IF_NOT_NULL(spr_cells);
    SPRITE_DEL_IF_NOT_NULL(spr_mouse);
    SPRITE_DEL_IF_NOT_NULL(spr_icon_play);
    SPRITE_DEL_IF_NOT_NULL(spr_icon_pause);
    SPRITE_DEL_IF_NOT_NULL(spr_icon_speed);
#undef SPRITE_DEL_IF_NOT_NULL
}

static int
runtick(void* ctx)
{
    (void)ctx;
    static double time_accumulator = 0.0;
    static double time_previous = 0.0;

    double const time_now = aengn_now();
    double const time_elapsed = time_now - time_previous;
    time_previous = time_now;
    time_accumulator += time_elapsed;

    int const quit = hinput();
    while (time_accumulator >= TIMESTEP) {
        update();
        time_accumulator -= TIMESTEP;
    }
    render();

    return quit;
}

static int
hinput(void)
{
    int quit = 0;

    // Escape => Quit.
    if (virtkey_state(SDLK_ESCAPE)->released) {
        quit = 1;
    }

    // Left mouse button => Turn cell under mouse alive.
    if (mousebutton_state(MOUSEBUTTON_LEFT)->down) {
        cells[mousepos_x() + OFFSET][mousepos_y() + OFFSET] = 1;
    }
    // Middle mouse button => Turn cell under mouse dead.
    // Right mouse button => Turn cell under mouse dead.
    else if (
        mousebutton_state(MOUSEBUTTON_MIDDLE)->down
        || mousebutton_state(MOUSEBUTTON_RIGHT)->down) {
        cells[mousepos_x() + OFFSET][mousepos_y() + OFFSET] = 0;
    }

    // R or RETURN => Start / stop simulation.
    if (scankey_state(SDL_SCANCODE_RETURN)->pressed
        || virtkey_state(SDLK_r)->pressed) {
        run = !run;
        step = false;
    }

    // S or SPACE => Advance one generation.
    if (scankey_state(SDL_SCANCODE_SPACE)->pressed
        || virtkey_state(SDLK_s)->pressed) {
        run = false;
        step = true;
    }

    // C => Turn all cells on the board dead.
    if (virtkey_state(SDLK_c)->pressed) {
        run = false;
        memset(cells, 0x00, sizeof(cells));
    }

    // H => Hide / show cursor & UI icons.
    if (virtkey_state(SDLK_h)->pressed) {
        hide = !hide;
    }

    // 1 => Set simulation speed to slow.
    if (virtkey_state(SDLK_1)->pressed) {
        upg = UPG_1;
        updates_remaining = upg;
    }
    // 2 => Set simulation speed to medium.
    else if (virtkey_state(SDLK_2)->pressed) {
        upg = UPG_2;
        updates_remaining = upg;
    }
    // 3 => Set simulation speed to fast.
    else if (virtkey_state(SDLK_3)->pressed) {
        upg = UPG_3;
        updates_remaining = upg;
    }

    return quit;
}

static void
update(void)
{
    // Update the  cell-grid sprite to match the cells array.
    // The grid will get drawn regardless of whether the simulation is paused
    // or not, so it should be updated every frame.
    for (int x = OFFSET; x < CELLS_W + OFFSET; ++x) {
        for (int y = OFFSET; y < CELLS_H + OFFSET; ++y) {
            struct rgba const* color = cells[x][y] != 0 ? &white : &black;
            sprite_set_pixel(spr_cells, x, y, color);
        }
    }

    bool const paused = !run && !step;
    if (paused) {
        return;
    }

    if (step || updates_remaining == 0) {
        advance();
        updates_remaining = upg;
        step = false;
    }
    updates_remaining -= 1;
}

static void
render(void)
{
    draw_clear(NULL);
    draw_sprite(spr_cells, -OFFSET, -OFFSET);
    if (!hide) {
        static int const ICON_X = 1;
        static int const ICON_Y = 1;

        int const playing = run || step;
        draw_sprite(playing ? spr_icon_play : spr_icon_pause, ICON_X, ICON_Y);

        assert(sprite_h(spr_icon_play) == sprite_h(spr_icon_pause));
#define MAYBE_DRAW_SPEED_ICON(num_)                                            \
    if (upg <= UPG_##num_) {                                                   \
        draw_sprite(                                                           \
            spr_icon_speed,                                                    \
            ICON_X + (num_ - 1) * sprite_w(spr_icon_speed) + (num_ - 1),       \
            screen_h() - sprite_h(spr_icon_speed) - 1);                        \
    }
        MAYBE_DRAW_SPEED_ICON(1); // Should always be drawn.
        MAYBE_DRAW_SPEED_ICON(2);
        MAYBE_DRAW_SPEED_ICON(3);
#undef MAYBE_DRAW_SPEED_ICON

        draw_sprite(spr_mouse, mousepos_x(), mousepos_y());
    }
    draw_show();
}

static void
advance(void)
{
    memset(neighbors, 0x00, sizeof(neighbors));
    for (int x = OFFSET; x < CELLS_W + OFFSET; ++x) {
        for (int y = OFFSET; y < CELLS_H + OFFSET; ++y) {
            if (cells[x][y] == 0) {
                continue;
            }
            neighbors[x - 1][y - 1] += 1;
            neighbors[x + 0][y - 1] += 1;
            neighbors[x + 1][y - 1] += 1;

            neighbors[x - 1][y + 0] += 1;
            neighbors[x + 1][y + 0] += 1;

            neighbors[x - 1][y + 1] += 1;
            neighbors[x + 0][y + 1] += 1;
            neighbors[x + 1][y + 1] += 1;
        }
    }
    for (int x = OFFSET; x < CELLS_W + OFFSET; ++x) {
        for (int y = OFFSET; y < CELLS_H + OFFSET; ++y) {
            cells[x][y] = cells[x][y] != 0
                ? neighbors[x][y] == 2 || neighbors[x][y] == 3
                : neighbors[x][y] == 3;
        }
    }
}
