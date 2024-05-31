build_all:
	mkdir build
	cd build && cmake ..  && make -j16

clean_all:
	rm -rf build


