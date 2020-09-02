# This will run by default with command `make`.
all: build/bin/larson.bin

build/bin/larson.bin: src/apps/larson.s | build
	arm-none-eabi-as src/apps/larson.s -o build/obj/larson.o
	arm-none-eabi-objcopy build/obj/larson.o -O binary build/bin/larson.bin

build:
	mkdir -p build/bin build/obj

# Upload to the Pi.
install: build/bin/larson.bin
	rpi-install.py build/bin/larson.bin

# Remove binary files with `make clean`.
clean:
	rm -rf build

# Check for clean build.
check:
	@! make clean all 2>&1 > /dev/null | grep 'error\|warning'
