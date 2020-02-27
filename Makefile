FLAGS = -Wall -Werror -fsanitize=address
ROOT = src
TAIL = out/lexer.o out/parser.o

all: runner

runner: ${ROOT}/runner.c ${TAIL}
	gcc ${FLAGS} ${ROOT}/runner.c -lm -o out/runner.out ${TAIL}
	./out/runner.out ./examples/program.sm
	make clean

out/lexer.o: ${ROOT}/lexer.h ${ROOT}/lexer.c
	gcc ${FLAGS} -o out/lexer.o -c ${ROOT}/lexer.c

out/parser.o: ${ROOT}/parser.h ${ROOT}/parser.c
	gcc ${FLAGS} -o out/parser.o -c ${ROOT}/parser.c

clean: out/
	rm out/*.o*
