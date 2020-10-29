/*
AENGN - ASHN'S GAME ENGINE
    Single header file containing the guts of a 2D engine for games and simple
    applications with pixel graphics.


DEPENDENCIES
    This library depends on definitions found in the header "autil.h".
    Be sure to #include the file "autil.h" before the #include for this file:
        #include "autil.h"
        #include "aengn.h"

    This library depends on and links against the development-libraries:
    1. libm
    2. libSDL2
    3. libSDL2_image

    Linker flags (gcc/clang):
        -lm -lSDL2 -lSDL2_image

    Installation (Debian/Ubuntu):
        $ apt-get install libsdl2-dev
        $ apt-get install libsdl2-image-dev


USAGE
    To create function and object definitions:
        #define AENGN_IMPLEMENTATION
    in exactly *ONE* translation unit before including this file:
        #define AENGN_IMPLEMENTATION
        #include "aengn.h"

    To restrict function and object definitions to a single translation unit:
        #define AENGN_API static
    before including this file:
        #define AENGN_IMPLEMENTATION
        #define AENGN_API static
        #include "aengn.h"


CHANGELOG
    v0.3.0 - 2020-10-29
    -------------------
    + Namespace library with AENGN_ and aengn_.
    + Add rendering functions: aengn_draw_point, aengn_draw_line, and
      aengn_draw_rect.
    + Rename draw_show to aengn_end_frame.

    v0.2.0 - 2020-09-16
    -------------------
    + Initial release.
    + Added engine initialization, finalization, execution, timing, and other
      misc. functions: aengn_init, aengn_fini, aengn_run, and aengn_now,
      screen_w, screen_h, pixel_scale.
    + Added input state retrieval functions: scankey_state, virtkey_state,
      mousebutton_state, mousepos_x, mousepos_y, and associated macros.
    + Added type sprite, type rgba, and associated functions / macros:
      sprite_new, sprite_del, sprite_w, sprite_h, sprite_set_pixel,
      sprite_get_pixel, sprite_update_texture, and RGBA_LOCAL_PTR.
    + Added resource loading functions: load_surface, load_texture, and
      load_sprite.
    + Added rendering functions: draw_clear, draw_texture, draw_sprite, and
      draw_show.


LICENSE
    Copyright (c) 2020 ashn <me@ashn.dev>

    Permission to use, copy, modify, and/or distribute this software for any
    purpose with or without fee is hereby granted.

    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
    SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
    OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
    CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// HEADER SECTION ////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#ifndef AENGN_H_INCLUDED
#define AENGN_H_INCLUDED

#ifndef AENGN_API
#    define AENGN_API extern
#endif

#ifndef AUTIL_H_INCLUDED
#    error "autil.h must be included before this file"
#endif

#include <stdbool.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// These macros are used in place of the SDL2 SDL_BUTTON_* value-like macros
// and SDL_BUTTON function-like macro to avoid the somewhat awkward use of
// SDL_BUTTON(SDL_BUTTON_WHATEVER) bitmask returned by SDL_GetMouseState.
// clang-format off
#define AENGN_MOUSEBUTTON_LEFT   ((int)0)
#define AENGN_MOUSEBUTTON_RIGHT  ((int)1)
#define AENGN_MOUSEBUTTON_MIDDLE ((int)2)
#define AENGN_MOUSEBUTTON_COUNT  ((size_t)3)
// clang-format on

// Returns non-zero if should quit.
typedef int (*aengn_runtick_fn)(void* ctx);

// Initialize the engine and create a window with screen_w and screen_h virtual
// pixels each composed of pixel_scale by pixel_scale physical pixels.
// Returns a non-zero value on error.
AENGN_API int
aengn_init(int screen_w, int screen_h, int pixel_scale);
// Finalize engine resources and close the window associated with the
// application. This function may be registered with atexit.
AENGN_API void
aengn_fini(void);
// Start the main application loop, calling the user-provided runtick function
// once per tick of the main loop.
AENGN_API void
aengn_run(aengn_runtick_fn runtick, void* ctx);
// Returns the number of seconds elapsed since the invocation of aengn_run.
AENGN_API double
aengn_now(void);
// Present the results of aengn_draw_* calls onto the screen.
// Calling this function conceptually "ends" the current frame.
AENGN_API void
aengn_end_frame(void);

// Returns the width of the screen in virtual pixels.
// The width of the display area in physical pixels can be calculated with:
//      int const disp_w = aengn_screen_w() * aengn_pixel_scale();
AENGN_API int
aengn_screen_w(void);
// Returns the height of the screen in virtual pixels.
// The height of the display area in physical pixels can be calculated with:
//      int const disp_h = aengn_screen_h() * aengn_pixel_scale();
AENGN_API int
aengn_screen_h(void);
// Returns the number of physical pixels per virtual pixel on the display.
AENGN_API int
aengn_pixel_scale(void);

struct aengn_button_state
{
    // Button was pressed this frame.
    bool pressed;
    // Button was released this frame.
    bool released;
    // Button is held this frame.
    // This value is set to true the same frame that pressed is set to true.
    // This value is set to false the same frame that releases is set to true.
    bool down;
};
// Returns the state of the provided SDL2 scancode (SDL_SCANCODE_*).
AENGN_API struct aengn_button_state const*
aengn_scankey_state(SDL_Scancode key);
// Returns the state of the provided SDL2 keycode (SDLK_*).
AENGN_API struct aengn_button_state const*
aengn_virtkey_state(SDL_Keycode key);
// Returns the state of the provided mouse button (AENGN_MOUSEBUTTON_*).
AENGN_API struct aengn_button_state const*
aengn_mousebutton_state(int button);

// Returns the mouse cursor position in the range [0, aengn_screen_w()-1].
AENGN_API int
aengn_mousepos_x(void);
// Returns the mouse cursor position in the range [0, aengn_screen_h()-1].
AENGN_API int
aengn_mousepos_y(void);

// Abstract two-dimensional bitmap.
struct aengn_sprite;
// Information for a 24-bit color depth value with 8-bit opacity (alpha) value
// as represented by the RGBA color model.
struct aengn_rgba
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

// Produce a pointer of type struct aengn_rgba* whose contents holds the
// provided red, blue, green, and alpha values.
// This pointer has automatic storage duration associated with the enclosing
// block.
#define AENGN_RGBA_LOCAL_PTR(red_, green_, blue_, alpha_)                      \
    (&(struct aengn_rgba){.r = red_, .g = green_, .b = blue_, .a = alpha_})

// Allocate and initialize a sprite with the provided width and height.
// Returns NULL on error.
AENGN_API struct aengn_sprite*
aengn_sprite_new(int w, int h);
// Deinitialize and free the sprite.
AENGN_API void
aengn_sprite_del(struct aengn_sprite* self);
// Return the width of the sprite in virtual pixels.
AENGN_API int
aengn_sprite_w(struct aengn_sprite const* self);
// Return the height of the sprite in virtual pixels.
AENGN_API int
aengn_sprite_h(struct aengn_sprite const* self);
// Set the pixel at position (x, y) to the RGBA value in color.
AENGN_API void
aengn_sprite_set_pixel(
    struct aengn_sprite* self, int x, int y, struct aengn_rgba const* color);
// Store the RGBA value for the pixel at position (x, y) in color.
AENGN_API void
aengn_sprite_get_pixel(
    struct aengn_sprite const* self, int x, int y, struct aengn_rgba* color);
// Update the GPU memory associated underlying SDL_Texture from sprite memory.
// This process is expensive, so calls to aengn_sprite_set_pixel will *not*
// update GPU memory until this function is called.
// This function is automatically called from within aengn_draw_sprite if the
// underlying SDL_Texture has not been explicitly updated since the last call to
// aengn_sprite_set_pixel.
// Returns a non-zero value on error.
AENGN_API int
aengn_sprite_update_texture(struct aengn_sprite* self);

// Load and create an SDL_Surface from a file with the provided path.
// This suface must be explicitly deinitialized with SDL_FreeSurface.
// Returns NULL on error.
AENGN_API SDL_Surface*
aengn_load_surface(char const* path);
// Load and create an SDL_Texture from a file with the provided path.
// This texture must be explicitly deinitialized with SDL_DestroyTexture.
// Returns NULL on error.
AENGN_API SDL_Texture*
aengn_load_texture(char const* path);
// Load and create a sprite from a file with the provided path.
// This sprite must be explicitly deinitialized with aengn_sprite_del.
// Returns NULL on error.
AENGN_API struct aengn_sprite*
aengn_load_sprite(char const* path);

// Clear the screen using the provided RGBA value in color.
// The call aengn_draw_clear(NULL) will clear the screen with an opaque black.
AENGN_API void
aengn_draw_clear(struct aengn_rgba const* color);
// Draw a point at (x, y).
// Returns a non-zero value on error.
AENGN_API int
aengn_draw_point(int x, int y, struct aengn_rgba const* color);
// Draw a line from (x1, y1) to (x2, y2).
// Returns a non-zero value on error.
AENGN_API int
aengn_draw_line(int x1, int y1, int x2, int y2, struct aengn_rgba const* color);
// Draw axis-aligned rectangle with one corner at (x1, y1) and the opposite
// corner at (x2, y2).
// Returns a non-zero value on error.
AENGN_API int
aengn_draw_rect(int x1, int y1, int x2, int y2, struct aengn_rgba const* color);
// Draw the provided SDL_Texture with top-left position at (x, y).
// Returns a non-zero value on error.
AENGN_API int
aengn_draw_texture(SDL_Texture* tex, int x, int y);
// Draw the provided sprite with top-left position at (x, y).
// Returns a non-zero value on error.
AENGN_API int
aengn_draw_sprite(struct aengn_sprite* sprite, int x, int y);

#endif // AENGN_H_INCLUDED

////////////////////////////////////////////////////////////////////////////////
//////////////////////////// IMPLEMENTATION SECTION ////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#ifdef AENGN_IMPLEMENTATION
#undef AENGN_IMPLEMENTATION

#ifdef __EMSCRIPTEN__
#    include <emscripten.h>
#endif

static int g_screen_w = 0;
static int g_screen_h = 0;
static int g_pixel_scale = 0;

// SDL_Scancode => struct aengn_button_state
static struct autil_map* g_scankey_map = NULL;
// SDL_Keycode  => struct aengn_button_state
static struct autil_map* g_virtkey_map = NULL;
// Array mapping AENGN-supported mouse buttons => struct aengn_button_state.
static struct aengn_button_state g_mousebutton_state[AENGN_MOUSEBUTTON_COUNT];
static struct aengn_button_state BUTTON_STATE_DEFAULT = {0};

static SDL_Window* g_window = NULL;
static SDL_Renderer* g_renderer = NULL;

static bool g_should_quit;
static Uint64 g_run_start; // SDL_GetPerformanceCounter
static Uint64 g_frame_start; // SDL_GetPerformanceCounter
static Uint64 g_fps_period_start; // SDL_GetPerformanceCounter
static size_t g_fps_period_count; // Frames in the fps period.

static int
scankey_map_vpcmp(void const* lhs, void const* rhs)
{
    assert(lhs != NULL);
    assert(rhs != NULL);
    SDL_Scancode const l = *(SDL_Scancode*)lhs;
    SDL_Scancode const r = *(SDL_Scancode*)rhs;
    if (l < r) {
        return -1;
    }
    if (l > r) {
        return +1;
    }
    return 0;
}

static int
virtkey_map_vpcmp(void const* lhs, void const* rhs)
{
    assert(lhs != NULL);
    assert(rhs != NULL);
    SDL_Keycode const l = *(SDL_Keycode*)lhs;
    SDL_Keycode const r = *(SDL_Keycode*)rhs;
    if (l < r) {
        return -1;
    }
    if (l > r) {
        return +1;
    }
    return 0;
}

AENGN_API int
aengn_init(int screen_w, int screen_h, int pixel_scale)
{
    if (screen_w <= 0) {
        autil_errorf(
            "[%s] Invalid screen width (%d pixels)", __func__, screen_w);
        goto error;
    }
    if (screen_h <= 0) {
        autil_errorf(
            "[%s] Invalid screen height (%d pixels)", __func__, screen_h);
        goto error;
    }
    if (pixel_scale <= 0) {
        autil_errorf("[%s] Invalid pixel scale (%d)", __func__, pixel_scale);
        goto error;
    }

    g_screen_w = screen_w;
    g_screen_h = screen_h;
    g_pixel_scale = pixel_scale;
    g_scankey_map = autil_map_new(
        sizeof(SDL_Scancode),
        sizeof(struct aengn_button_state),
        scankey_map_vpcmp);
    g_virtkey_map = autil_map_new(
        sizeof(SDL_Keycode),
        sizeof(struct aengn_button_state),
        virtkey_map_vpcmp);
    memset(g_mousebutton_state, 0x00, sizeof(g_mousebutton_state));

    SDL_version ver;
    SDL_GetVersion(&ver);
    autil_infof("Using SDL version %d.%d.%d", ver.major, ver.minor, ver.patch);

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        autil_errorf("[%s][SDL_Init] %s", __func__, SDL_GetError());
        goto error;
    }

    static char const* const window_title = "";
    static int const window_x = SDL_WINDOWPOS_CENTERED;
    static int const window_y = SDL_WINDOWPOS_CENTERED;
    int const window_w = g_screen_w * g_pixel_scale;
    int const window_h = g_screen_h * g_pixel_scale;
    g_window = SDL_CreateWindow(
        window_title, window_x, window_y, window_w, window_h, SDL_WINDOW_SHOWN);
    if (g_window == NULL) {
        autil_errorf("[%s][SDL_CreateWindow] %s", __func__, SDL_GetError());
        goto error;
    }

    g_renderer = SDL_CreateRenderer(
        g_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (g_renderer == NULL) {
        autil_errorf("[%s][SDL_CreateRenderer] %s", __func__, SDL_GetError());
        goto error;
    }
    SDL_SetRenderDrawBlendMode(g_renderer, SDL_BLENDMODE_BLEND);

    return 0;

error:
    aengn_fini();
    return -1;
}

AENGN_API void
aengn_fini(void)
{
    if (g_window != NULL) {
        SDL_DestroyWindow(g_window);
        g_window = NULL;
    }
    if (g_renderer != NULL) {
        SDL_DestroyRenderer(g_renderer);
        g_renderer = NULL;
    }

    if (SDL_WasInit(SDL_INIT_EVERYTHING)) {
        SDL_Quit();
    }

    g_screen_w = -1;
    g_screen_h = -1;
    g_pixel_scale = -1;

    if (g_scankey_map != NULL) {
        autil_map_del(g_scankey_map);
        g_scankey_map = NULL;
    }
    if (g_virtkey_map != NULL) {
        autil_map_del(g_virtkey_map);
        g_virtkey_map = NULL;
    }
    memset(g_mousebutton_state, 0x00, sizeof(g_mousebutton_state)); // scrub
}

// Variables needed to "pass" the update and ctx parameters to
// aengn_run_update in a way that is compatible with Emscripten.
// The Emscripten main loop can be initialized using:
//  1. emscripten_set_main_loop     => takes em_callback_func
//  2. emscripten_set_main_loop_arg => takes em_arg_callback_func
// With either function we need these adaptor variables to properly forward the
// update and ctx parameters to the callback.
// clang-format off
static aengn_runtick_fn g_run__main_loop_body__update = NULL;
static void*            g_run__main_loop_body__ctx    = NULL;
// clang-format on
// Compatible with em_callback_func (Emscripten v2.0.2+).
static void
aengn_run__main_loop_body(void)
{
    if (g_run__main_loop_body__update(g_run__main_loop_body__ctx)) {
        g_should_quit = true;
    }
}

AENGN_API void
aengn_run(aengn_runtick_fn update, void* ctx)
{
    g_run__main_loop_body__update = update;
    g_run__main_loop_body__ctx = ctx;

    Uint64 const now = SDL_GetPerformanceCounter();
    g_should_quit = false;
    g_run_start = now;
    g_frame_start = now;
    g_fps_period_start = now;
    g_fps_period_count = 0;

    aengn_draw_clear(NULL);
    aengn_end_frame();
#ifdef __EMSCRIPTEN__
    em_callback_func const main_loop_body = aengn_run__main_loop_body;
    emscripten_set_main_loop(main_loop_body, 0, 1);
#else
    while (!g_should_quit) {
        aengn_run__main_loop_body();
    }
#endif
}

AENGN_API double
aengn_now(void)
{
    Uint64 const now = SDL_GetPerformanceCounter();
    Uint64 const elapsed = now - g_run_start;
    return (double)elapsed / (double)SDL_GetPerformanceFrequency();
}

static void
process_event_(SDL_Event const* event)
{
    assert(event != NULL);

    // SDL_QuitEvent
    if (event->type == SDL_QUIT) {
        g_should_quit = true;
    }

    // SDL_KeyboardEvent
    SDL_Scancode scankey = (SDL_Scancode)0xDEADBEEF;
    SDL_Keycode virtkey = (SDL_Keycode)0xDEADBEEF;
    struct aengn_button_state* scanstate = NULL;
    struct aengn_button_state* virtstate = NULL;
    if (event->type == SDL_KEYDOWN || event->type == SDL_KEYUP) {
        scankey = event->key.keysym.scancode;
        virtkey = event->key.keysym.sym;
        scanstate = autil_map_lookup(g_scankey_map, &scankey);
        virtstate = autil_map_lookup(g_virtkey_map, &virtkey);
        if (scanstate == NULL) {
            autil_map_insert(
                g_scankey_map, &scankey, &BUTTON_STATE_DEFAULT, NULL, NULL);
            scanstate = autil_map_lookup(g_scankey_map, &scankey);
        }
        if (virtstate == NULL) {
            autil_map_insert(
                g_virtkey_map, &virtkey, &BUTTON_STATE_DEFAULT, NULL, NULL);
            virtstate = autil_map_lookup(g_virtkey_map, &virtkey);
        }
        assert(scanstate != NULL);
        assert(virtstate != NULL);
    }
    if (event->type == SDL_KEYDOWN) {
        assert(scanstate != NULL);
        assert(virtstate != NULL);
        scanstate->pressed = true;
        virtstate->pressed = true;
        scanstate->down = true;
        virtstate->down = true;
    }
    if (event->type == SDL_KEYUP) {
        assert(scanstate != NULL);
        assert(virtstate != NULL);
        scanstate->released = true;
        virtstate->released = true;
        scanstate->down = false;
        virtstate->down = false;
    }

    // SDL_MouseButtonEvent
    struct aengn_button_state* mousebtnstate = NULL;
    if (event->type == SDL_MOUSEBUTTONDOWN
        || event->type == SDL_MOUSEBUTTONUP) {
        if (event->button.button == SDL_BUTTON_LEFT) {
            mousebtnstate = &g_mousebutton_state[AENGN_MOUSEBUTTON_LEFT];
        }
        if (event->button.button == SDL_BUTTON_RIGHT) {
            mousebtnstate = &g_mousebutton_state[AENGN_MOUSEBUTTON_RIGHT];
        }
        if (event->button.button == SDL_BUTTON_MIDDLE) {
            mousebtnstate = &g_mousebutton_state[AENGN_MOUSEBUTTON_MIDDLE];
        }
    }
    if (mousebtnstate != NULL && event->type == SDL_MOUSEBUTTONDOWN) {
        mousebtnstate->pressed = true;
        mousebtnstate->down = true;
    }
    if (mousebtnstate != NULL && event->type == SDL_MOUSEBUTTONUP) {
        mousebtnstate->released = true;
        mousebtnstate->down = false;
    }
}

AENGN_API void
aengn_end_frame(void)
{
    // Render the frame.
    SDL_RenderPresent(g_renderer);

    // Since a frame has just ended, perform FPS calculations.
    Uint64 const ONE_SECOND = SDL_GetPerformanceFrequency();
    Uint64 const now = SDL_GetPerformanceCounter();
    Uint64 const fps_period_elapsed = now - g_fps_period_start;
    g_fps_period_count += 1;
    if (fps_period_elapsed >= ONE_SECOND) {
        double const sec = (double)fps_period_elapsed / (double)ONE_SECOND;
        double const fps = (double)g_fps_period_count / sec;
        char title[256] = {0};
        snprintf(title, AUTIL_ARRAY_COUNT(title) - 1, "FPS: %0.2f", fps);
        SDL_SetWindowTitle(g_window, title);
        g_fps_period_start = now;
        g_fps_period_count = 0;
    }
    // From this moment on we consider all work done part of the *next* frame
    // since any future work will have happened *after* the user was shown the
    // result of previous draw calls (i.e. the content of the frame).
    g_frame_start = now;

    // Clear previous frame's input state.
    struct autil_vec const* const scankeys = autil_map_vals(g_scankey_map);
    for (size_t i = 0; i < autil_vec_count(scankeys); ++i) {
        struct aengn_button_state* const sk = autil_vec_get(scankeys, i);
        assert(sk != NULL);
        sk->pressed = false;
        sk->released = false;
    }
    struct autil_vec const* const virtkeys = autil_map_vals(g_virtkey_map);
    for (size_t i = 0; i < autil_vec_count(virtkeys); ++i) {
        struct aengn_button_state* const vk = autil_vec_get(virtkeys, i);
        assert(vk != NULL);
        vk->pressed = false;
        vk->released = false;
    }
    for (size_t i = 0; i < AENGN_MOUSEBUTTON_COUNT; ++i) {
        struct aengn_button_state* const btn = &g_mousebutton_state[i];
        btn->pressed = false;
        btn->released = false;
    }

    // Process all new input state for the next frame.
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        process_event_(&event);
    }
}

AENGN_API int
aengn_screen_w(void)
{
    return g_screen_w;
}

AENGN_API int
aengn_screen_h(void)
{
    return g_screen_h;
}

AENGN_API int
aengn_pixel_scale(void)
{
    return g_pixel_scale;
}

AENGN_API struct aengn_button_state const*
aengn_scankey_state(SDL_Scancode key)
{
    struct aengn_button_state const* const state =
        autil_map_lookup(g_scankey_map, &key);
    return state != NULL ? state : &BUTTON_STATE_DEFAULT;
}

AENGN_API struct aengn_button_state const*
aengn_virtkey_state(SDL_Keycode key)
{
    struct aengn_button_state const* const state =
        autil_map_lookup(g_virtkey_map, &key);
    return state != NULL ? state : &BUTTON_STATE_DEFAULT;
}

AENGN_API struct aengn_button_state const*
aengn_mousebutton_state(int button)
{
    assert(0 <= button && button < (int)AENGN_MOUSEBUTTON_COUNT);
    return &g_mousebutton_state[button];
}

AENGN_API int
aengn_mousepos_x(void)
{
    int x;
    SDL_GetMouseState(&x, NULL);
    return x / g_pixel_scale;
}

AENGN_API int
aengn_mousepos_y(void)
{
    int y;
    SDL_GetMouseState(NULL, &y);
    return y / g_pixel_scale;
}

struct aengn_sprite
{
    int w;
    int h;
    SDL_Surface* surface;
    SDL_Texture* texture;
    bool texture_needs_update;
};

AENGN_API struct aengn_sprite*
aengn_sprite_new(int w, int h)
{
    assert(w > 0);
    assert(h > 0);

    struct aengn_sprite* self = NULL;
    SDL_Surface* surface = NULL;
    SDL_Texture* texture = NULL;
    static int const DEPTH = 32;

    self = autil_xalloc(NULL, sizeof(struct aengn_sprite));

    surface = SDL_CreateRGBSurfaceWithFormat(
        0, w, h, DEPTH, SDL_PIXELFORMAT_ARGB8888);
    if (surface == NULL) {
        autil_errorf(
            "[%s][SDL_CreateRGBSurfaceWithFormat] %s",
            __func__,
            SDL_GetError());
        goto error;
    }

    texture = SDL_CreateTexture(
        g_renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        w,
        h);
    if (texture == NULL) {
        autil_errorf("[%s][SDL_CreateTexture] %s", __func__, SDL_GetError());
        goto error;
    }
    // TODO: Using alpha blending on all sprites *may* lead to a performance
    //       hit. Do testing with SDL_BLENDMODE_NONE to see if there is a
    //       performance difference, and if so, maybe make the blend mode
    //       configurable.
    if (0 != SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND)) {
        autil_errorf(
            "[%s][SDL_SetTextureBlendMode] %s", __func__, SDL_GetError());
        goto error;
    }

    self->w = w;
    self->h = h;
    self->surface = surface;
    self->texture = texture;
    self->texture_needs_update = false;

    return self;

error:
    if (self != NULL) {
        autil_xalloc(self, AUTIL_XALLOC_FREE);
    }
    if (surface != NULL) {
        SDL_FreeSurface(surface);
    }
    if (texture != NULL) {
        SDL_DestroyTexture(texture);
    }
    return NULL;
}

AENGN_API void
aengn_sprite_del(struct aengn_sprite* self)
{
    assert(self != NULL);

    SDL_FreeSurface(self->surface);
    SDL_DestroyTexture(self->texture);
    memset(self, 0x00, sizeof(*self)); // scrub
    autil_xalloc(self, AUTIL_XALLOC_FREE);
}

AENGN_API int
aengn_sprite_w(struct aengn_sprite const* self)
{
    assert(self != NULL);

    return self->w;
}

AENGN_API int
aengn_sprite_h(struct aengn_sprite const* self)
{
    assert(self != NULL);

    return self->h;
}

AENGN_API int
aengn_sprite_update_texture(struct aengn_sprite* self)
{
    int const err = SDL_UpdateTexture(
        self->texture, NULL, self->surface->pixels, self->surface->pitch);
    if (err) {
        autil_errorf("[%s][SDL_UpdateTexture] %s", __func__, SDL_GetError());
        return -1;
    }

    self->texture_needs_update = false;
    return 0;
}

static Uint32*
sprite_locate_pixel_(struct aengn_sprite const* sprite, int x, int y)
{
    assert(0 <= x && x < sprite->w);
    assert(0 <= y && y < sprite->h);

    Uint32* const pixels = sprite->surface->pixels;
    int const rowlen =
        sprite->surface->pitch / sprite->surface->format->BytesPerPixel;
    return pixels + (y * rowlen) + x;
}

AENGN_API void
aengn_sprite_set_pixel(
    struct aengn_sprite* self, int x, int y, struct aengn_rgba const* color)
{
    assert(self != NULL);
    assert(color != NULL);

    Uint32* const px = sprite_locate_pixel_(self, x, y);
    Uint32 const rgba = SDL_MapRGBA(
        self->surface->format, color->r, color->g, color->b, color->a);
    *px = rgba;
    self->texture_needs_update = true;
}

AENGN_API void
aengn_sprite_get_pixel(
    struct aengn_sprite const* self, int x, int y, struct aengn_rgba* color)
{
    assert(self != NULL);
    assert(color != NULL);

    Uint32* const px = sprite_locate_pixel_(self, x, y);
    SDL_GetRGBA(
        *px, self->surface->format, &color->r, &color->g, &color->b, &color->a);
}

AENGN_API SDL_Surface*
aengn_load_surface(char const* path)
{
    SDL_Surface* const surface = IMG_Load(path);
    if (surface == NULL) {
        autil_errorf("[%s(%s)][IMG_Load] %s", __func__, path, IMG_GetError());
        return NULL;
    }
    return surface;
}

AENGN_API SDL_Texture*
aengn_load_texture(char const* path)
{
    SDL_Surface* const surface = aengn_load_surface(path);
    if (surface == NULL) {
        autil_errorf(
            "[%s(%s)][aengn_load_surface] Failed to load surface",
            __func__,
            path);
        return NULL;
    }

    SDL_Texture* const texture =
        SDL_CreateTextureFromSurface(g_renderer, surface);
    SDL_FreeSurface(surface);
    if (texture == NULL) {
        autil_errorf(
            "[%s(%s)][SDL_CreateTextureFromSurface] %s",
            __func__,
            path,
            SDL_GetError());
        return NULL;
    }

    return texture;
}

AENGN_API struct aengn_sprite*
aengn_load_sprite(char const* path)
{
    int err = 0;
    SDL_Surface* surface = NULL;
    struct aengn_sprite* sprite = NULL;

    surface = aengn_load_surface(path);
    if (surface == NULL) {
        autil_errorf(
            "[%s(%s)][aengn_load_surface] Failed to load surface",
            __func__,
            path);
        goto error;
    }

    sprite = aengn_sprite_new(surface->w, surface->h);
    if (sprite == NULL) {
        autil_errorf(
            "[%s(%s)][aengn_sprite_new] Failed create sprite", __func__, path);
        goto error;
    }

    err = SDL_BlitSurface(surface, NULL, sprite->surface, NULL);
    if (err) {
        autil_errorf(
            "[%s(%s)][SDL_BlitSurface] %s", __func__, path, SDL_GetError());
        goto error;
    }

    err = aengn_sprite_update_texture(sprite);
    if (err) {
        autil_errorf(
            "[%s(%s)][sprite_update_texture] Failed to update texture",
            __func__,
            path);
        goto error;
    }

    return sprite;

error:
    if (surface != NULL) {
        SDL_FreeSurface(surface);
    }
    if (sprite != NULL) {
        aengn_sprite_del(sprite);
    }
    return NULL;
}

AENGN_API int
aengn_draw_texture(SDL_Texture* tex, int x, int y)
{
    int w;
    int h;
    SDL_QueryTexture(tex, NULL, NULL, &w, &h);
    SDL_Rect dst = {.x = x * g_pixel_scale,
                    .y = y * g_pixel_scale,
                    .w = w * g_pixel_scale,
                    .h = h * g_pixel_scale};
    int const err = SDL_RenderCopy(g_renderer, tex, NULL, &dst);
    if (err) {
        autil_errorf("[%s][SDL_RenderCopy] %s", __func__, SDL_GetError());
    }
    return err;
}

AENGN_API int
aengn_draw_sprite(struct aengn_sprite* sprite, int x, int y)
{
    assert(sprite != NULL);

    if (sprite->texture_needs_update) {
        aengn_sprite_update_texture(sprite);
    }

    int const err = aengn_draw_texture(sprite->texture, x, y);
    if (err) {
        autil_errorf(
            "[%s][aengn_draw_texture] Failed to draw sprite texture", __func__);
    }
    return err;
}

AENGN_API void
aengn_draw_clear(struct aengn_rgba const* color)
{
    static struct aengn_rgba const black = {0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE};
    if (color == NULL) {
        color = &black;
    }
    SDL_SetRenderDrawColor(g_renderer, color->r, color->g, color->b, color->a);
    SDL_RenderClear(g_renderer);
}

AENGN_API int
aengn_draw_point(int x, int y, struct aengn_rgba const* color)
{
    assert(color != NULL);

    SDL_SetRenderDrawColor(g_renderer, color->r, color->g, color->b, color->a);
    SDL_Rect const rect = {.x = x * g_pixel_scale,
                           .y = y * g_pixel_scale,
                           .w = g_pixel_scale,
                           .h = g_pixel_scale};
    int const err = SDL_RenderFillRect(g_renderer, &rect);
    if (err) {
        autil_errorf("[%s][SDL_RenderFillRect] %s", __func__, SDL_GetError());
    }
    return err;
}

// Optimized line function from the article "Line drawing on a grid" written by
// Red Blob Games, ported from C# to C99.
// https://www.redblobgames.com/grids/line-drawing.html
//
// Vecs xpos and ypos are caller-allocated and should be initialized to hold
// elements of type int.
static void
line_pos_(
    int x1,
    int y1,
    int x2,
    int y2,
    struct autil_vec* xpos,
    struct autil_vec* ypos)
{
    assert(xpos != NULL);
    assert(ypos != NULL);

    autil_vec_resize(xpos, 0);
    autil_vec_resize(ypos, 0);

    // The number of steps to take is exactly the diagonal distance between
    // (x1, y1) and (x2, y2).
    int const dx = x2 - x1;
    int const dy = y2 - y1;
    int const abs_dx = abs(dx);
    int const abs_dy = abs(dy);
    int const nsteps = abs_dx > abs_dy ? abs_dx : abs_dy;

    double const nsteps_inverse = 1.0 / (double)nsteps;
    double const xstep = dx * nsteps_inverse;
    double const ystep = dy * nsteps_inverse;

    // Populate the xpos and ypos vectors with each (x, y) point from (x1, y1)
    // to (x2, y2). These points are connected by either a pixel edge, or a
    // corner between the two pixels.
    double x = (double)x1;
    double y = (double)y1;
    for (int step = 0; step <= nsteps; ++step) {
        int const xround = (int)round(x);
        int const yround = (int)round(y);
        autil_vec_insert(xpos, autil_vec_count(xpos), &xround);
        autil_vec_insert(ypos, autil_vec_count(ypos), &yround);

        x += xstep;
        y += ystep;
    }
}

AENGN_API int
aengn_draw_line(int x1, int y1, int x2, int y2, struct aengn_rgba const* color)
{
    assert(color != NULL);

    struct autil_vec* const xpos = autil_vec_new(sizeof(int));
    struct autil_vec* const ypos = autil_vec_new(sizeof(int));
    line_pos_(x1, y1, x2, y2, xpos, ypos);

    int err = 0;
    assert(autil_vec_count(xpos) == autil_vec_count(ypos));
    size_t const npoints = autil_vec_count(xpos);
    for (size_t i = 0; i < npoints; ++i) {
        err |= aengn_draw_point(
            AUTIL_DEREF_PTR(int, autil_vec_get(xpos, i)),
            AUTIL_DEREF_PTR(int, autil_vec_get(ypos, i)),
            color);
    }
    if (err) {
        autil_errorf(
            "[%s][aengn_draw_point] Failed to draw point(s)", __func__);
    }

    autil_vec_del(xpos);
    autil_vec_del(ypos);
    return err;
}

AENGN_API int
aengn_draw_rect(int x1, int y1, int x2, int y2, struct aengn_rgba const* color)
{
    assert(color != NULL);

    int err = 0;
    err |= aengn_draw_line(x1, y1, x2, y1, color);
    err |= aengn_draw_line(x1, y2, x2, y2, color);
    err |= aengn_draw_line(x1, y1, x1, y2, color);
    err |= aengn_draw_line(x2, y1, x2, y2, color);

    if (err) {
        autil_errorf(
            "[%s][aengn_draw_line] Failed to draw rect line(s)", __func__);
    }
    return err;
}

#endif // AENGN_IMPLEMENTATION