.PHONY: clean

default: upload

buildfiles:
	cmake -B build -G "Unix Makefiles"

hex:
	cmake --build build --target hex --no-print-directory

upload:
	cmake --build build --target upload

clean:
	rm -r build/
