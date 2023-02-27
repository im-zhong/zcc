all: scanner parser
	mv src/location.hh include/ir/location.hh
	mv src/parser.hpp include/ir/parser.hpp
	sed -i 's/parser.hpp/ir\/parser.hpp/' src/parser.cpp

scanner: src/ir.lex
	flex -o src/scanner.cpp src/ir.lex

parser: src/ir.y
	bison -o src/parser.cpp src/ir.y

.PHONY: clean
clean:
	rm include/ir/location.hh include/ir/parser.hpp src/parser.cpp src/scanner.cpp result.ir test_ir

.PHONY: test
test:
	./test_ir test.ir > result.ir
	diff result.ir test.ir
