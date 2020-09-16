.POSIX:
.SUFFIXES:
.PHONY: build clean

EXAMPLES = \
	life

build:
	@for ex in $(EXAMPLES); do (cd $$ex && $(MAKE)); done

clean:
	@for ex in $(EXAMPLES); do (cd $$ex && $(MAKE) clean); done
