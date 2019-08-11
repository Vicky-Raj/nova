makefile:

all:
	gcc  ./Lexer/lexer.c ./Lexer/delimlexer.c ./Lexer/helper.c ./Lexer/oplexer.c ./Lexer/wordlexer.c ./Lexer/constlexer.c \
	./Parser/parser.c ./Parser/expressParser.c ./Parser/helper.c  ./Parser/symbolParser.c ./Parser/compositeParser.c \
	./Parser/errors.c ./Parser/assignParser.c ./Parser/controlStatParser.c ./Parser/funcParser.c ./Semantic/helpers.c \
	./Semantic/semantic.c -o nova

clean:
	rm nova