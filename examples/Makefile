.POSIX:
.SUFFIXES:
.PHONY: build clean

EXAMPLES = \
	life \
	dvd-screensaver

build:
	@for ex in $(EXAMPLES); do (cd $$ex && $(MAKE)); done

clean:
	@for ex in $(EXAMPLES); do (cd $$ex && $(MAKE) clean); done
