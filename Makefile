app          = arabica
test         = test_$(app)
cc           = clang
cxx          = clang++
src_app      = app
src_emulator = arabica
src_test     = test
src          = $(src_app) $(src_emulator) $(src_test)
dir_build    = build
dir_rom      = rom
game         = Tetris_Fran_Dachille_1991.ch8

default: build execute

fmt:
	for dir in $(src); do                                                                      \
		find $$dir -type f -iname "*.cpp" -o -iname "*.hpp" | xargs clang-format -i -style=file; \
	done

build: clean
	mkdir $(dir_build);                                                                                            
	cd $(dir_build); cmake -DCMAKE_C_COMPILER="$(cc)" -DCMAKE_CXX_COMPILER="$(cxx)" -DBUILD_APP=ON ..; make; 

execute:
	./$(dir_build)/$(app).out $(dir_rom)/$(game)

clean:
	rm -rf $(dir_build)

test: clean 
	mkdir $(dir_build);                                                                                                \
	cd $(dir_build); cmake -DCMAKE_C_COMPILER="$(cc)" -DCMAKE_CXX_COMPILER="$(cxx)" -DBUILD_TEST=ON ..; make; \
	./$(test).out

debug: clean build
	gdb -x commands.gdb --args ./$(dir_build)/$(app).out $(dir_rom)/$(game).ch8

.PHONY: default fmt build execute clean debug
