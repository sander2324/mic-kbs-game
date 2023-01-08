.PHONY: clean

default: upload

buildfiles:
	cmake -B build -G "Unix Makefiles"

upload:
	cmake --build build --target upload

clean:
	rm -r build/
