.POSIX:
.SUFFIXES:
.PHONY: default desktop browser clean

CC = c99
CFLAGS = -DNDEBUG
LDLIBS = -lm -lSDL2 -lSDL2_image
TARGET = life
SRCS = $(TARGET).c

EMSHELL = --shell-file ../emscripten-shell.html
EMEMBED = # None (normally something like => --embed-file assets)
EMFLAGS = \
	$(EMSHELL) $(EMEMBED) \
	-s USE_SDL=2 \
	-s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]'

default: desktop
desktop: $(TARGET)
browser: $(TARGET).html $(TARGET).bundle.html

$(TARGET): $(SRCS)
	$(CC) -o $@ $(SRCS) $(CFLAGS) $(LDLIBS)

$(TARGET).html: $(SRCS)
	emcc -o $@ $(SRCS) $(CFLAGS) $(EMFLAGS)

$(TARGET).bundle.html: $(SRCS)
	emcc -o $@ $(SRCS) $(CFLAGS) $(EMFLAGS) -s SINGLE_FILE=1

clean:
	rm -f \
		$(TARGET) $(OBJS) \
		$(TARGET).html $(TARGET).js $(TARGET).wasm \
		$(TARGET).bundle.html
