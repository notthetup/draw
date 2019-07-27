all: draw
	@true

draw: libopencm3
	make -C draw

libopencm3:
	git submodule init
	git submodule update
	make -C libopencm3

flash: draw
		dfu-util -d 1209:70b1 -D draw/draw.bin

clean:
	make -C libopencm3 clean
	make -C draw clean

.PHONY: libopencm3 flash draw
