getafix:
	# Generate lexical
	flex bplex.l
	# Generate bp.tab.c (parser)
	bison bp.y
	gcc -o program -O2 common.c bptree.c varOrd.c bp.tab.c translate_Mucke.c

labeler:
	# Generate lexical
	flex bplex.l
	# Generate bp.tab.c (parser)
	bison bp.y
	gcc -o label_program -O2 common.c bptree.c varOrd.c bp.tab.c labeler.c

clean:
	rm -rf lex.yy.c
	rm -rf bp.tab.c
	rm -rf program
	rm -rf label_program
