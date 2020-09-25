.POSIX:
.SUFFIXES:
.PHONY: build clean

EXAMPLES = \
	aengn-life \
	aengn-dvd-screensaver \
	aengn-shapes

build:
	@for ex in $(EXAMPLES); do (cd $$ex && $(MAKE)); done

clean:
	@for ex in $(EXAMPLES); do (cd $$ex && $(MAKE) clean); done
