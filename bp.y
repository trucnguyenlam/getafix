%{
int yylex();
%}
 %{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bpstruc.h"
#include "name.h"
#include "common.h"
#include "varOrd.h"

void yyerror(char*);
int line;

bp_ident_t bp_globals;
bp_ident_t bp_decls;
bp_ident_t bp_parms;  // For parsing only
bp_fun_t bp_functions;
int bp_numglobals;
int bp_maxrets;
int bp_maxlocs;
int bp_labelcount;
bp_ident_t bp_locids;
bp_ident_t bp_globids;
bp_ident_t bp_allglobids;
short bp_funcount;
int bp_declcount;

bp_tree_t bp_funtree;
bp_tree_t bp_nametree;
bp_tree_t bp_labeltree;
bp_ident_t *bp_dtail;

%}

%union {
bp_stmt_t stmt;
	bp_expr_t expr;
	bp_ident_t ident;
	bp_idref_t idref;
	bp_fun_t fun;
	char* c;
	int   i;
}

%token <c> IDENTIFIER
%token <i> INTEGER
%token NONDET
%token ASSIGN EQ_OP NE_OP IMPLIES
%token DECL
%token ENFORCE
%token AND OR
%token IF THEN ELSE ELSIF FI
%token WHILE DO OD
%token RETURN SKIP GOTO
%token BEG END
%token BOOL VOID
%token ASSERT ASSUME PRINT CHOOSE CONSTRAIN
%token ATOMIC_BEGIN ATOMIC_END

%type <ident> global_decls
%type <fun> fun_list function_definition
%type <i> fun_type parms
%type <idref> assign_id_list asgn_id
%type <idref> expression_list
%type <stmt> statement_list
%type <stmt> labelled_statement statement
%type <stmt> parallel_assign call assert print
%type <stmt> selection_statement elsif_list
%type <stmt> iteration_statement jump_statement
%type <stmt> atomic_statement
%type <expr> primary_expression unary_expression equality_expression
%type <expr> and_expression exclusive_or_expression expression
%type <expr> decider or_expression enforce choose_expression

%start program

%%

program	: pstart global_decls fun_list
	  {
		bp_globals = $2;
		bp_functions = $3;
	  } ;

pstart	: {
		bp_funcount = -1;
		bp_funtree = NULL;
		bp_labelcount = 0;

		bp_maxlocs = 0;
		bp_maxrets = 0;
		bp_locids = NULL;
		bp_globids = NULL;
		bp_allglobids = NULL;
	  } ;

/***** decls *****/

global_decls
	: decl_start decl_list
	  {
		$$ = bp_decls;
		bp_numglobals = bp_declcount;
	  } ;

decl_start
	: {
		bp_declcount = 0;
		bp_funcount++;
		bp_decls = NULL;
	  } ;

decl_local_start
	: {
		bp_declcount = 0;
		bp_decls = NULL;
	  } ;



decl_list
	: DECL identifier_list ';' decl_list
	| ;

identifier_list
	: decl_id
	| identifier_list ',' decl_id ;

decl_id	: IDENTIFIER
{
		if (Is_there_var(bp_decls, $1))
		{
			yyerror("global variable already exists");
			YYERROR;
		}
  	        Insert_var($1);
	  } ;

/***** functions *****/

fun_list
	: function_definition
	| function_definition fun_list
	  { $$ = $1; $$->next = $2; } ;

function_definition
	: fun_type IDENTIFIER parms
	  BEG decl_local_start  decl_list enforce statement_list END
	  {
		int i;
		$$ = alloc_bp_fun();
		$$->funname =$2;
		$$->num_returns = $1;
		$$->num_args = $3;
		$$->num_locals = bp_declcount;
		$$->num_globals = bp_numglobals;
		  //list of local variables
		$$->locals = bp_decls;
		  //list of parameters
		$$->parms = bp_parms;
		$$->enforce = $7;
		$$->stmt = $8;
		$$->next = NULL;
        $$->reachable=0;
        $$->useful=0;
	  } ;

fun_type
        : BOOL	{ $$ = 1; }
	| BOOL '<' INTEGER '>' { $$ = $3; }
        | VOID	{ $$ = 0; } ;

enforce	: ENFORCE expression ';' { $$ = $2; }
	| { $$ = NULL; } ;

parms	: decl_start '(' parameter_list ')'
	  { $$ = bp_declcount; bp_parms = bp_decls; } ;

parameter_list
	: identifier_list
	| ;

/***** statements *****/

statement_list
        : labelled_statement
	| labelled_statement statement_list
	  {	$$ = $1; $$->next = $2;		} ;

labelled_statement
        : IDENTIFIER ':' statement
	  {
		$$ = $3;
                $$->label = $1;
                $$->next = NULL;
                $$->next_stmt = NULL;
                $$->numlabel = -1;
                $$->visited = 0;
                $$->locals=NULL;
	  }
        | statement
	  {	$$ = $1;
                $$->label = NULL;
                $$->next = NULL;
                $$->next_stmt = NULL;
                $$->numlabel = -1;
                $$->visited = 0;
                $$->locals=NULL;
          } ;

statement
	: parallel_assign ';'
        | assert ';'
        | print ';'
	| call ';'
	| selection_statement
	| iteration_statement
	| jump_statement ';'
    | atomic_statement ';'
    ;

atomic_statement
    : ATOMIC_BEGIN {
        new_bp_stmt($$,BP_ATOMIC_BEGIN);
      }
    | ATOMIC_END {
        new_bp_stmt($$,BP_ATOMIC_BEGIN);
      }
    ;


parallel_assign
       	: assign_id_list ASSIGN expression_list

	  {
		bp_idref_t tmp1, tmp2, tmp3;

		new_bp_stmt($$,BP_ASSIGN);

		for (tmp1 = $1, tmp2 = $3; tmp1 && tmp2; tmp1 = tmp1->next)
		{
			tmp1->expr = tmp2->expr;
			tmp3 = tmp2->next;
			free(tmp2);
			tmp2 = tmp3;
		}

		if (tmp1 || tmp2)
		{
			yyerror("number of assign clauses doesn't match");
			YYERROR;
		}

		$$->e.a.asgnlist = $1;
	  }
       	| assign_id_list ASSIGN call
	  {
		$$ = $3;
		$$->e.a.asgnlist = $1;
	  } ;

assign_id_list
	: asgn_id
	| assign_id_list ',' asgn_id
          {  $$ = $1;
             bp_idref_t p;
  	     for (p=$$; p->next; p=p->next);
             p->next = $3;

	     //             $$ = $3;
	     //             $$->next = $1;
          } ;

asgn_id	: IDENTIFIER
	  {
		$$ = alloc_bp_idref();
                $$->var_name=$1;
                $$->expr =NULL;
		$$->primed = 1;
		$$->next = NULL;
	  } ;

assert	: ASSERT '(' decider ')'
	  {
		new_bp_stmt($$,BP_ASSERT);
		$$->e.c.expr = $3;
	  }
	| ASSUME '(' decider ')'
	  {
		new_bp_stmt($$,BP_ASSUME);
		$$->e.c.expr = $3;
	  }
	| CONSTRAIN '(' decider ')'
	  {
		new_bp_stmt($$,BP_CONSTRAIN);
		$$->e.c.expr = $3;
	  } ;

print	: PRINT '(' expression_list ')'
	  {
		new_bp_stmt($$,BP_PRINT);
		$$->e.a.asgnlist = $3;
	  } ;

call	: IDENTIFIER '(' expression_list ')'
	  {
		new_bp_stmt($$,BP_CALL);
		$$->e.a.label =$1;
		$$->e.a.asgnlist = NULL;
		$$->e.a.fmllist = $3;
	  }
	| IDENTIFIER '(' ')'
	  {
		new_bp_stmt($$,BP_CALL);
		$$->e.a.label =strdup($1);
		$$->e.a.asgnlist = NULL;
		$$->e.a.fmllist = NULL;
	  } ;

selection_statement
	: IF '(' decider ')' THEN statement_list elsif_list
	  {
		new_bp_stmt($$,BP_IF);
		$$->e.c.expr = $3;
		$$->e.c.thenstmt = $6;
		$$->e.c.elsestmt = $7;
	  } ;

elsif_list
	: FI
	  {	$$ = NULL;	}
	| ELSE statement_list FI
	  {	$$ = $2;	}
	| ELSIF '(' decider ')' THEN statement_list elsif_list
	  {
		new_bp_stmt($$,BP_ELSEIF);
		//		$$->numlabel = ++bp_labelcount;
		$$->e.c.expr = $3;
		$$->e.c.thenstmt = $6;
		$$->e.c.elsestmt = $7;
                $$->label = NULL;
                $$->next = NULL;
                $$->numlabel = -1;
                $$->visited = 0;
	  } ;

iteration_statement
	: WHILE '(' decider ')' DO statement_list OD
	  {
		new_bp_stmt($$,BP_WHILE);
		$$->e.c.expr = $3;
		$$->e.c.thenstmt = $6;
		$$->e.c.elsestmt = NULL;
	  } ;

jump_statement
	: RETURN
	  {
		new_bp_stmt($$,BP_RETURN);
		$$->e.a.asgnlist = NULL;
	  }
	| RETURN expression_list
	  {
		new_bp_stmt($$,BP_RETURN);
		$$->e.a.asgnlist = $2;
	  }
	| SKIP
	  {
		new_bp_stmt($$,BP_SKIP);
	  }
	| GOTO IDENTIFIER
	  {
		new_bp_stmt($$,BP_GOTO);
		$$->e.a.label = strdup($2);
	  } ;

/***** expressions *****/

primary_expression
	: IDENTIFIER
	  {
		if ((*$1 == 'T' || *$1 == 'F') && !$1[1])
		{
			new_bp_expr($$,BP_CONST,(bp_expr_t)(*$1 == 'T'),NULL);
		}
		else
		{
			new_bp_expr($$,BP_IDENT,NULL,NULL);
                        $$->var_name=strdup($1);
                        $$->primed = 0;
		}
	  }
	| '\'' IDENTIFIER
	  {

		new_bp_expr($$,BP_IDENT,NULL,NULL);
                $$->var_name=strdup($2);
                $$->primed = 1;

	  }
	| INTEGER
	  {
		if ($1 == 1 || $1 ==0) {
			new_bp_expr($$,BP_CONST,(bp_expr_t)$1,NULL);
		} else {
		    yyerror("Boolean constants are 0 and 1\n");
		    YYERROR;
		}
	  }
	| '(' expression ')'
	  {	$$ = $2;	} ;

unary_expression
	: primary_expression
	| unary_operator primary_expression
	  {	new_bp_expr($$,BP_NOT,$2,NULL);		} ;

unary_operator
	: '~'
	| '!' ;

equality_expression
	: unary_expression
	| equality_expression EQ_OP unary_expression
	  {	new_bp_expr($$,BP_EQ,$1,$3);	}
	| equality_expression NE_OP unary_expression
	  {	new_bp_expr($$,BP_NE,$1,$3);	} ;

and_expression
	: equality_expression
	| and_expression AND equality_expression
	  {	new_bp_expr($$,BP_AND,$1,$3);	} ;

exclusive_or_expression
	: and_expression
	| exclusive_or_expression '^' and_expression
	  {	new_bp_expr($$,BP_NE,$1,$3);	} ;

or_expression
	: exclusive_or_expression
	| or_expression OR exclusive_or_expression
	  {	new_bp_expr($$,BP_OR,$1,$3);	} ;

expression
	: or_expression
	| expression IMPLIES or_expression
	  {	new_bp_expr($$,BP_IMP,$1,$3);	} ;

decider
	: NONDET
	  {	new_bp_expr($$,BP_ND,NULL,NULL);	}
        | expression ;

choose_expression
	: expression
        | CHOOSE '[' expression ',' expression ']'
	  {	new_bp_expr($$,BP_CHOOSE,$3,$5);	} ;

expression_list
	: choose_expression
	  {
		$$ = alloc_bp_idref();
                $$->var_name = NULL;
		$$->expr = $1;
		$$->next = NULL;
	  }
	| expression_list ',' choose_expression
	  {

                bp_idref_t p1, p2;

		$$ = $1;

                p2 = alloc_bp_idref();
                p2->var_name = NULL;
                p2->expr = $3;
                p2->next = NULL;

  	        for (p1=$$; p1->next; p1=p1->next);
                p1->next = p2;

	  } ;

%%

#include "lex.yy.c"

void yyerror(char *s)
{
    fflush(stdout);
    printf("Line %d: syntax error.\n", line);
}

