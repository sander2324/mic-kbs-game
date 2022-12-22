.PHONY: clean

default: upload

buildfiles:
	cmake -B build -G "Unix Makefiles"

hex:
	cmake --build build --target hex

upload:
	cmake --build build --target upload

clean:
	rm -r build/
