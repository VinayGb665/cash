all : lex yacc calc
lex : calc.l
	lex calc.l
yacc : calc.y
	yacc -d calc.y
calc : lex.yy.c y.tab.c y.tab.h
	gcc lex.yy.c y.tab.c -ly -ll -o calc
	rm lex.yy.c y.tab.c y.tab.h
