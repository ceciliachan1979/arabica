app          = arabica
test         = test_$(app)
cc           = clang
cxx          = clang++
src_app      = app
src_emulator = arabica
src_test     = test
src          = $(src_app) $(src_emulator) $(src_test)
dir_build    = build

default: build execute

fmt:
	for dir in $(src); do                                                                      \
		find $$dir -type f -iname "*.cpp" -o -iname "*.hpp" | xargs clang-format -i -style=file; \
	done

build: clean
	mkdir $(dir_build);                                                                                            
	cd $(dir_build); cmake -DCMAKE_C_COMPILER="$(cc)" -DCMAKE_CXX_COMPILER="$(cxx)" -DBUILD_APP=ON -GNinja ..; ninja; 

execute:
	./$(dir_build)/$(app).out

clean:
	rm -rf $(dir_build)

test: clean 
	mkdir $(dir_build);                                                                                                \
	cd $(dir_build); cmake -DCMAKE_C_COMPILER="$(cc)" -DCMAKE_CXX_COMPILER="$(cxx)" -DBUILD_TEST=ON -GNinja ..; ninja; \
	./$(test).out

.PHONY: default fmt build execute clean