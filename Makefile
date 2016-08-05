getafix:
	# Generate lexical
	flex bplex.l
	# Generate bp.tab.c (parser)
	bison bp.y
	gcc -o program -O2 common.c bptree.c varOrd.c bp.tab.c translate_Mucke.c

getafix_mubdd:
	# Generate lexical
	flex bplex.l
	# Generate bp.tab.c (parser)
	bison bp.y
	gcc -o program_mu -O2 common.c bptree.c varOrd.c bp.tab.c translate_Mucke_mubdd.c

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
	rm -rf program_mu
	rm -rf label_program
