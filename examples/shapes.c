#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#define AENGN_IMPLEMENTATION
#include "../aengn.h"

#define VERSION "0.1"

#define PIXEL_SCALE 8
#define CELLS_W (640 / PIXEL_SCALE)
#define CELLS_H (480 / PIXEL_SCALE)

struct rgba const RGBA_WHITE = {0xff, 0xff, 0xff, 0xff};
struct rgba const RGBA_PURPLE = {0xff, 0x22, 0xff, 0xff};

struct shape
{
    enum shape_kind
    {
        SHAPE_POINT,
        SHAPE_LINE,
        SHAPE_RECT
    } kind;

    struct point
    {
        int x;
        int y;
    } p1, p2;
};
// List of all shapes to be drawn (in order) every frame.
struct vec* shapes = NULL;

// Currently selected shape.
enum shape_kind selected = SHAPE_POINT;
// Mouse position where the user started drawing a shape by clicking.
struct point tmp_shape_start = {0, 0};

static int
runtick(void* ctx);
static void
draw_shape(struct shape const* s);

int
main(void)
{
    if (aengn_init(CELLS_W, CELLS_H, PIXEL_SCALE) != 0) {
        return EXIT_FAILURE;
    }
    atexit(aengn_fini);
    shapes = vec_new(sizeof(struct shape));

    aengn_run(runtick, NULL);

    vec_del(shapes);
    return EXIT_SUCCESS;
}

static int
runtick(void* ctx)
{
    (void)ctx;

    // Escape => Quit.
    if (virtkey_state(SDLK_ESCAPE)->released) {
        return 1;
    }

    // MOUSEBUTTON_LEFT pressed => Start drawing a shape.
    if (mousebutton_state(MOUSEBUTTON_LEFT)->pressed) {
        tmp_shape_start = (struct point){.x = mousepos_x(), .y = mousepos_y()};
    }
    // MOUSEBUTTON_LEFT pressed => Finish drawing the currently selected shape.
    if (mousebutton_state(MOUSEBUTTON_LEFT)->released) {
        struct shape const s = {
            .kind = selected,
            .p1 = tmp_shape_start,
            .p2 = (struct point){.x = mousepos_x(), .y = mousepos_y()}};
        vec_insert(shapes, vec_count(shapes), &s);
    }

    // 1,2,3,... => Change the selected shape.
    if (virtkey_state(SDLK_1)->pressed) {
        infof("Selected 'point'");
        selected = SHAPE_POINT;
    }
    else if (virtkey_state(SDLK_2)->pressed) {
        infof("Selected 'line'");
        selected = SHAPE_LINE;
    }
    else if (virtkey_state(SDLK_3)->pressed) {
        infof("Selected 'rect'");
        selected = SHAPE_RECT;
    }

    // C => Clear the screen by removing existing shapes.
    if (virtkey_state(SDLK_c)->released) {
        vec_resize(shapes, 0);
    }

    // Render all existing shapes as well as a preview of the selected shape if
    // the user is currently drawing one with the mouse.
    draw_clear(NULL);
    for (size_t i = 0; i < vec_count(shapes); ++i) {
        draw_shape(vec_get(shapes, i));
    }
    if (mousebutton_state(MOUSEBUTTON_LEFT)->down) {
        struct shape const s = {
            .kind = selected,
            .p1 = tmp_shape_start,
            .p2 = (struct point){.x = mousepos_x(), .y = mousepos_y()}};
        // The shape itself.
        draw_shape(&s);
        // Start point.
        draw_point(tmp_shape_start.x, tmp_shape_start.y, &RGBA_PURPLE);
        // End point.
        if (selected != SHAPE_POINT) {
            draw_point(mousepos_x(), mousepos_y(), &RGBA_PURPLE);
        }
    }

    aengn_end_frame();
    return 0;
}

static void
draw_shape(struct shape const* s)
{
    if (s->kind == SHAPE_POINT) {
        draw_point(s->p1.x, s->p1.y, &RGBA_WHITE);
        return;
    }
    if (s->kind == SHAPE_LINE) {
        draw_line(s->p1.x, s->p1.y, s->p2.x, s->p2.y, &RGBA_WHITE);
        return;
    }
    if (s->kind == SHAPE_RECT) {
        draw_rect(s->p1.x, s->p1.y, s->p2.x, s->p2.y, &RGBA_WHITE);
        return;
    }
    fatalf("Unreachable!");
}
