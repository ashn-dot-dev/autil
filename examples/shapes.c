#define AUTIL_IMPLEMENTATION
#include "../autil.h"
#define AENGN_IMPLEMENTATION
#include "../aengn.h"

#define VERSION "0.1"

#define PIXEL_SCALE 8
#define CELLS_W (640 / PIXEL_SCALE)
#define CELLS_H (480 / PIXEL_SCALE)

struct aengn_rgba const RGBA_WHITE = {0xff, 0xff, 0xff, 0xff};
struct aengn_rgba const RGBA_PURPLE = {0xff, 0x22, 0xff, 0xff};

struct shape {
    enum shape_kind { SHAPE_POINT, SHAPE_LINE, SHAPE_RECT } kind;

    struct point {
        int x;
        int y;
    } p1, p2;
};
// List of all shapes to be drawn (in order) every frame.
struct autil_vec* shapes = NULL;

// Currently selected shape.
enum shape_kind selected = SHAPE_POINT;
// Mouse position where the user started drawing a shape by clicking.
struct point tmp_shape_start = {0, 0};

static int
runtick(void* ctx);
static void
aengn_draw_shape(struct shape const* s);

int
main(void)
{
    if (aengn_init(CELLS_W, CELLS_H, PIXEL_SCALE) != 0) {
        return EXIT_FAILURE;
    }
    atexit(aengn_fini);
    shapes = autil_vec_new(sizeof(struct shape));

    aengn_run(runtick, NULL);

    autil_vec_del(shapes);
    return EXIT_SUCCESS;
}

static int
runtick(void* ctx)
{
    (void)ctx;

    // Escape => Quit.
    if (aengn_virtkey_state(SDLK_ESCAPE)->released) {
        return 1;
    }

    // AENGN_MOUSEBUTTON_LEFT pressed => Start drawing a shape.
    if (aengn_mousebutton_state(AENGN_MOUSEBUTTON_LEFT)->pressed) {
        tmp_shape_start =
            (struct point){.x = aengn_mousepos_x(), .y = aengn_mousepos_y()};
    }
    // AENGN_MOUSEBUTTON_LEFT pressed => Finish drawing the currently selected shape.
    if (aengn_mousebutton_state(AENGN_MOUSEBUTTON_LEFT)->released) {
        struct shape const s = {
            .kind = selected,
            .p1 = tmp_shape_start,
            .p2 = (struct point){
                .x = aengn_mousepos_x(), .y = aengn_mousepos_y()}};
        autil_vec_insert(shapes, autil_vec_count(shapes), &s);
    }

    // 1,2,3,... => Change the selected shape.
    if (aengn_virtkey_state(SDLK_1)->pressed) {
        autil_infof("Selected 'point'");
        selected = SHAPE_POINT;
    }
    else if (aengn_virtkey_state(SDLK_2)->pressed) {
        autil_infof("Selected 'line'");
        selected = SHAPE_LINE;
    }
    else if (aengn_virtkey_state(SDLK_3)->pressed) {
        autil_infof("Selected 'rect'");
        selected = SHAPE_RECT;
    }

    // C => Clear the screen by removing existing shapes.
    if (aengn_virtkey_state(SDLK_c)->released) {
        autil_vec_resize(shapes, 0);
    }

    // Render all existing shapes as well as a preview of the selected shape if
    // the user is currently drawing one with the mouse.
    aengn_draw_clear(NULL);
    for (size_t i = 0; i < autil_vec_count(shapes); ++i) {
        aengn_draw_shape(autil_vec_ref(shapes, i));
    }
    if (aengn_mousebutton_state(AENGN_MOUSEBUTTON_LEFT)->down) {
        struct shape const s = {
            .kind = selected,
            .p1 = tmp_shape_start,
            .p2 = (struct point){
                .x = aengn_mousepos_x(), .y = aengn_mousepos_y()}};
        // The shape itself.
        aengn_draw_shape(&s);
        // Start point.
        aengn_draw_point(tmp_shape_start.x, tmp_shape_start.y, &RGBA_PURPLE);
        // End point.
        if (selected != SHAPE_POINT) {
            aengn_draw_point(
                aengn_mousepos_x(), aengn_mousepos_y(), &RGBA_PURPLE);
        }
    }

    aengn_end_frame();
    return 0;
}

static void
aengn_draw_shape(struct shape const* s)
{
    if (s->kind == SHAPE_POINT) {
        aengn_draw_point(s->p1.x, s->p1.y, &RGBA_WHITE);
        return;
    }
    if (s->kind == SHAPE_LINE) {
        aengn_draw_line(s->p1.x, s->p1.y, s->p2.x, s->p2.y, &RGBA_WHITE);
        return;
    }
    if (s->kind == SHAPE_RECT) {
        aengn_draw_rect(s->p1.x, s->p1.y, s->p2.x, s->p2.y, &RGBA_WHITE);
        return;
    }
    autil_fatalf("Unreachable!");
}
