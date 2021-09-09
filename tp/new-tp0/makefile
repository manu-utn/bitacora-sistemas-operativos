all: clean
	make -C client
	make -C server

clean:
	make clean -C client
	make clean -C server

.PHONY: watch
watch:
	while true; do $(MAKE) -q || $(MAKE); sleep 1; done
