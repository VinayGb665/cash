COMPILER = gcc
HEADER_FILES = common.h simple_command.h command.h y.tab.h
FILESYSTEM_FILES = command.c simple_command.c shell.c y.tab.c lex.yy.c
all : lex yacc build
lex : lexer.l
	lex lexer.l
yacc : parser.y
	yacc -d parser.y
build: $(FILESYSTEM_FILES) $(HEADER_FILES)
	$(COMPILER) $(FILESYSTEM_FILES) -ly -ll -o cash
parsertest: yacc lex test
test: lex.yy.c y.tab.c
	$(COMPILER) lex.yy.c y.tab.c -ly -ll -o cash
clean:
	rm cash