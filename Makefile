.PHONY: build
build:
	ninja -C build/

.PHONY: test
test:
	ninja -C build/ test

.PHONY: run_test
run_test:
	./build/test/unimail_test

.PHONY: clean
clean:
	rm -rf build/