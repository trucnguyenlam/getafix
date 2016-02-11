#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "bpstruc.h"
#include "name.h"
#include "common.h"
#include "varOrd.h"

/**
 * Conversion from Concurrent Boolean Program into Mucke
 *
 * ChangeLog:
 *     2016.02.06   Add function call (atomic) to the translation
 *
 */



extern FILE *yyin;

extern int yyparse(void);

extern int line;
extern int column;

bp_ident_t bp_all_locals;
int pc_return;
int dead_pc;
char *target_label;
int maxindexlocvar;
int num_thread;
int CS;
int n_bit_pc;


/******************************************************************/

static void print_PC(char *str, int pc, int num_bits) {
    int i = 1, j, k;

    // printf("(");
    printf("(/* pc=%d */ ", pc);

    while (pc > 0) {
        j = pc % 2;
        pc = pc / 2;
        if (i > 1) printf(" & ");
        printf("%s.b%d=%d", str, i, j);
        i++;
    }

    for (k = i; k <= num_bits; k++) {
        if (k > 1) printf(" & ");
        printf("%s.b%d=0", str, k);
    }

    printf(")");

}


/******************************************************************/

static int isThreadFunction(bp_fun_t f) {
    if (strcasecmp(f->funname, "main") == 0)  // Main function
    {
        return 1;
    }
    if (strcasecmp(f->funname, "init") == 0)
    {
        return 1;
    }
    // begin with thread (or whatever this word in mixed-case)
    if (
        strlen(f->funname) > 6 &&
        ((f->funname[0] == 't') || (f->funname[0] == 'T')) &&
        ((f->funname[1] == 'h') || (f->funname[1] == 'H')) &&
        ((f->funname[2] == 'r') || (f->funname[2] == 'R')) &&
        ((f->funname[3] == 'e') || (f->funname[3] == 'E')) &&
        ((f->funname[4] == 'a') || (f->funname[4] == 'A')) &&
        ((f->funname[5] == 'd') || (f->funname[5] == 'D'))
    ) {
        return 1;
    }

    return 0;
}


/********************************************************************/

static bp_stmt_t find_label(bp_stmt_t p, char *label) {
    bp_stmt_t temp;

    if (!p) return NULL;

    if ((p->label) != NULL) if (strcmp(p->label, label) == 0) return (p);

    switch (p->type) {
    case BP_IF:
    case BP_ELSEIF:
    case BP_WHILE: {
        temp = find_label(p->e.c.thenstmt, label);
        if (temp != NULL) return temp;

        temp = find_label(p->e.c.elsestmt, label);
        if (temp != NULL) return temp;

        break;
    }
    }

    return (find_label(p->next, label));
}


static bp_fun_t find_function(char *varname) {
    bp_fun_t y;

    for (y = bp_functions; y; y = y->next)
        if (strcmp(varname, y->funname) == 0) return (y);
    common_error("error: the name of the called function does not exist: %s\n", varname);
}


static bp_fun_t find_module(bp_fun_t funlist, char *name) {
    for (; funlist; funlist = funlist->next)
        if (strcmp(funlist->funname, name) == 0) return funlist;

    return NULL;
}


/******************************************************************/

static void reset_flag_visited(bp_stmt_t p) {
    if (!p) return;

    p->visited = 0;

    switch (p->type) {
    case BP_IF:
    case BP_ELSEIF:
    case BP_WHILE: {
        reset_flag_visited(p->e.c.thenstmt);
        reset_flag_visited(p->e.c.elsestmt);
        break;
    }
    }

    reset_flag_visited(p->next);
}

/********************************************************************/

static int Max(int a, int b) {
    if (a < b) return (b);
    return (a);
}

/********************************************************************/

static void print_d_pc(bp_stmt_t p) {
    printf("&");
    if (p->next_stmt) {
        //     printf("&dp=%d", (p->next_stmt)->numlabel );
        print_PC("dp", (p->next_stmt)->numlabel, n_bit_pc);
    } else {
        //     printf("&dp=%d", pc_return );
        print_PC("dp", pc_return, n_bit_pc);
    }
}

/********************************************************************/

static bp_ident_t All_locals(void) {
    bp_fun_t fun;
    bp_ident_t q;
    bp_ident_t bp_decls_temp;


    bp_decls = NULL;
    bp_declcount = 0;

    for (fun = bp_functions; fun; fun = fun->next) {
        for (q = fun->locals; q; q = q->next)
            if (!Is_there_var(bp_decls, q->varname))
                Insert_var(q->varname);
        for (q = fun->parms; q; q = q->next)
            if (!Is_there_var(bp_decls, q->varname))
                Insert_var(q->varname);
    }

    bp_all_locals = bp_decls;

    // Insert parameter to locals variable
    for (fun = bp_functions; fun; fun = fun->next) {
        bp_decls = fun->locals;

        for (q = fun->parms; q; q = q->next)
            if (!Is_there_var(bp_decls, q->varname))
                Insert_var(q->varname);

        fun->locals = bp_decls;
    }

    return (bp_all_locals);
}

/********************************************************************/
static void print_Mucke_expression(bp_expr_t p, char *prefix, char constrain) {
    bp_ident_t y, q;

    if (!p) return;


    switch (p->token) {
    case BP_CONST: {
        if (p->left)
            printf("true ");
        else
            printf("false ");
        return;
    }
    case BP_IDENT: {
        y = Is_there_var(bp_globals, p->var_name);

        if (y) {
            if (constrain == 0) {
                printf("%sG.v%d", prefix, y->varname_replace);
            } else {
                if (p->primed == 1) {
                    printf("dG.v%d", y->varname_replace);
                } else {
                    printf("G.v%d", y->varname_replace);
                }
            }
        }
        else {
            y = Is_there_var(bp_all_locals, p->var_name);
            if (!y) common_error("error in prin_Mucke_expression name var:%s\n", p->var_name);


            if (constrain == 0) {
                printf("%sL.v%d", prefix, y->varname_replace);
            } else {
                if (p->primed == 1) {
                    printf("dL.v%d", y->varname_replace);
                } else {
                    printf("L.v%d", y->varname_replace);
                }
            }
        }
        return;
    }
    case BP_NOT: {
        printf("(!");
        print_Mucke_expression(p->left, prefix, constrain);
        printf(")");
        return;
    }
    case BP_EQ: {
        printf("(");
        print_Mucke_expression(p->left, prefix, constrain);
        printf("<->");
        print_Mucke_expression(p->right, prefix, constrain);
        printf(")");
        return;
    }
    case BP_NE: {
        printf("(!(");
        print_Mucke_expression(p->left, prefix, constrain);
        printf("<->");
        print_Mucke_expression(p->right, prefix, constrain);
        printf("))");
        return;
    }
    case BP_AND: {
        printf("(");
        print_Mucke_expression(p->left, prefix, constrain);
        printf("&");
        print_Mucke_expression(p->right, prefix, constrain);
        printf(")");
        return;
    }
    case BP_OR: {
        printf("(");
        print_Mucke_expression(p->left, prefix, constrain);
        printf("|");
        print_Mucke_expression(p->right, prefix, constrain);
        printf(")");
        return;
    }
    case BP_IMP: {

        if ((p->left->token == BP_CONST) && (p->left->left)) {
            print_Mucke_expression(p->right, prefix, constrain);
            return;
        }

        if ((p->left->token == BP_CONST) && (!p->left->left)) {
            printf("true ");
            return;
        }

        printf("(");
        print_Mucke_expression(p->left, prefix, constrain);
        printf("->");
        print_Mucke_expression(p->right, prefix, constrain);
        printf(")");
        return;
    }

    }

}

/******************************************************************/
static void print_recursive_statement_program_Int1(bp_stmt_t p, bp_fun_t fun, short *head) {
    bp_ident_t y;
    bp_idref_t q;
    bp_ident_t z = NULL, temp_var;
    int i, tmp;
    bp_fun_t callee;

    if ((!p) || (p->visited != 0)) return;
    p->visited = 1;

    switch (p->type) {
    case BP_ASSIGN: {
        for (z = bp_globals; z; z = z->next) z->mark = 0;
        for (z = bp_all_locals; z; z = z->next) z->mark = 0;
        for (z = fun->locals; z; z = z->next) z->mark = 0;

        if ((*head) == 0) {
            printf("| ((cm=%d)&(false", fun->funname_replace);
            (*head) = 1;
        }

        printf("|(\n");
        printf("                    /* ASSIGN */ \n");
        //      printf(" cp=%d", p->numlabel);
        print_PC("cp", p->numlabel, n_bit_pc);

        print_d_pc(p);

        // For each variable in assignment
        for (q = (p->e.a.asgnlist); q; q = q->next) {
            z = Is_there_var(bp_globals, q->var_name);
            if (z) i = 0;
            else {
                i = 1;
                z = Is_there_var(bp_all_locals, q->var_name);
                if (!Is_there_var(fun->locals, q->var_name))
                    common_error("error var not found in the locals of the module %s\n", q->var_name);
            }

            if (!z) common_error("error in assign %s\n", q->var_name);

            z->mark = 1;

            if (((q->expr)->token) == BP_CHOOSE) {
                if (q->expr->left->token == BP_CONST) {
                    if (q->expr->left->left) {
                        if (i == 0) printf("&dG.v%d", z->varname_replace);
                        else printf("&dL.v%d", z->varname_replace);
                    } else {

                        // ( (!a & f2 ) | (! f2))
                        printf(" &((!");
                        if (i == 0) printf("dG.v%d", z->varname_replace);
                        else printf("dL.v%d", z->varname_replace);
                        printf("&(");
                        print_Mucke_expression(q->expr->right, "", 0);
                        printf("))|(!");
                        print_Mucke_expression(q->expr->right, "", 0);
                        printf("))\n");
                    }
                } else {
                    if (q->expr->right->token == BP_CONST) {
                        // ((a&f1)|(!f1))
                        printf(" &(( ");
                        if (i == 0) printf("dG.v%d", z->varname_replace);
                        else printf("dL.v%d", z->varname_replace);
                        printf("&(");
                        print_Mucke_expression(q->expr->left, "", 0);
                        printf("))|(!");
                        print_Mucke_expression(q->expr->left, "", 0);
                        printf("))\n");
                    } else {
                        // ((a&f1) | (!a & !f1 & f2 ) | (!f1 & ! f2))
                        printf(" &(( ");
                        if (i == 0) printf("dG.v%d", z->varname_replace);
                        else printf("dL.v%d", z->varname_replace);
                        printf("&(");
                        print_Mucke_expression(q->expr->left, "", 0);
                        printf("))|((!");
                        if (i == 0) printf("dG.v%d", z->varname_replace);
                        else printf("dL.v%d", z->varname_replace);
                        printf(")&(!");
                        print_Mucke_expression(q->expr->left, "", 0);
                        printf(")&(");
                        print_Mucke_expression(q->expr->right, "", 0);
                        printf("))|((!");
                        print_Mucke_expression(q->expr->left, "", 0);
                        printf(")&(!");
                        print_Mucke_expression(q->expr->right, "", 0);
                        printf(")))\n");
                    }
                }

            } else {
                if (i == 0) printf("&(dG.v%d", z->varname_replace);
                else printf("&(dL.v%d", z->varname_replace);
                if ((q->expr->token == BP_IDENT) || (q->expr->token == BP_CONST))
                    printf("=");
                else printf("<->");

                print_Mucke_expression(q->expr, "", 0);
                printf(")\n");
            }
        }

        printf(")");
        break;
    }

    // TRUC
    case BP_CALL: {
        // for (z = bp_globals;     z; z = z->next) z->mark = 0;
        // for (z = bp_all_locals;  z; z = z->next) z->mark = 0;
        // for (z = fun->locals; z; z = z->next) z->mark = 0;

        if ((*head) == 0) {
            printf("| ((cm=%d)&(false", fun->funname_replace);
            (*head) = 1;
        }

        printf("|(\n");
        printf("                    /* CALL */ \n");
        print_PC("cp", p->numlabel, n_bit_pc);
        print_d_pc(p);
        printf("\n");
        callee = find_function(p->e.a.label);
        printf("& (exists\n");
        printf("    Local   u_ENTRY_CL.\n");
        printf("    (\n");
        printf("      programCall(cm, %d, cp, L, u_ENTRY_CL, G)\n", callee->funname_replace);
        printf("    & (exists\n");
        printf("         PrCount u_pc,\n");
        printf("         Local   u_CL,\n");
        printf("         Global  u_CG.\n");
        printf("         (\n");
        printf("           (\n");
        printf("              %s(%d, u_pc, u_CL, u_CG, u_ENTRY_CL, G)\n", callee->funname, callee->funname_replace);
        printf("             & Exit(%d, u_pc)\n", callee->funname_replace);
        printf("           )\n");
        printf("          & SetReturnTS(cm, %d, cp, u_pc, L, dL, G, dG)\n", callee->funname_replace);
        printf("          & SetReturnUS(cm, %d, cp, u_pc, u_CL, dL, u_CG, dG)\n", callee->funname_replace);
        printf("         )\n");
        printf("      )\n");
        printf("    )\n");
        printf("  )\n");  // Close exists
        printf(")");   // Close or
        break;
    }

    case BP_IF:
    case BP_ELSEIF:
    case BP_WHILE: {
        print_recursive_statement_program_Int1(p->next_then, fun, head);
        print_recursive_statement_program_Int1(p->next_else, fun, head);
        break;
    }
    }

    print_recursive_statement_program_Int1(p->next_stmt, fun, head);
    return;
}


/************************************************************************/
static void print_recursive_statement_program_Int3(bp_stmt_t p, bp_fun_t fun, short *flag) {
    bp_ident_t y;
    bp_idref_t q;
    bp_ident_t z = NULL, temp_var;
    int i, tmp;

    if ((!p) || (p->visited != 0)) return;
    p->visited = 1;

    switch (p->type) {

    case BP_CONSTRAIN: {

        if (*flag == 0) {
            printf("| ((cm=%d)&((false", fun->funname_replace);
            *flag = 1;
        }

        printf("|(\n");
        printf("                      /* CONSTRAIN */\n");
        //      printf(" cp=%d", p->numlabel);
        print_PC("cp", p->numlabel, n_bit_pc);

        print_d_pc(p);

        if (((p->e.c.expr)->token) != BP_ND) {
            printf(" & ");
            print_Mucke_expression(p->e.c.expr, "", 1);
            printf("\n");
        }

        printf("\n)");

        break;
    }

    case BP_IF:
    case BP_ELSEIF:
    case BP_WHILE: {
        print_recursive_statement_program_Int3(p->next_then, fun, flag);
        print_recursive_statement_program_Int3(p->next_else, fun, flag);
        break;
    }
    }

    print_recursive_statement_program_Int3(p->next_stmt, fun, flag);
    return;
}

/************************************************************************/
static void print_recursive_statement_program_Int2(bp_stmt_t p, bp_fun_t fun, short *flag) {
    bp_ident_t y;
    bp_idref_t q;
    bp_ident_t z = NULL, temp_var;
    int i, tmp;

    if ((!p) || (p->visited != 0)) return;

    p->visited = 1;

    switch (p->type) {
    case BP_ASSERT:
    case BP_ASSUME: {
        if (*flag == 0) {
            *flag = 1;
            printf("| ((cm=%d)&\n", fun->funname_replace);
            printf("((false\n");
        }
        printf("|(\n");
        printf("                      /* ASSUME ASSERT 1 */\n");
        //      printf(" cp=%d\n", p->numlabel);
        print_PC("cp", p->numlabel, n_bit_pc);
        printf("\n");

        print_d_pc(p);

        if (((p->e.c.expr)->token) != BP_ND) {
            printf("&");
            print_Mucke_expression(p->e.c.expr, "", 0);
            printf("\n");
        }

        printf("\n");

        printf(")");

        if ((p->type == BP_ASSERT) && (((p->e.c.expr)->token) != BP_ND)) {
            printf("|(\n");
            printf("                    /* ASSERT 2 */\n");
            //         printf(" cp=%d\n", p->numlabel);
            print_PC("cp", p->numlabel, n_bit_pc);
            printf("\n");

            //         printf(" & dp=%d\n",dead_pc);
            printf(" & ");
            print_PC("dp", dead_pc, n_bit_pc);
            printf("\n");

            printf(" & ");
            printf("(!");
            print_Mucke_expression(p->e.c.expr, "", 0);
            printf(")\n");
            printf("\n");

            printf(")");
        }
        break;
    }

    case BP_IF:
    case BP_ELSEIF: {
        if (*flag == 0) {
            *flag = 1;
            printf("| ((cm=%d)&\n", fun->funname_replace);
            printf("((false\n");
        }

        if (((p->e.c.expr)->token) != BP_ND) {

            if (p->next_then) {
                printf("|(\n");
                printf("                    /* IF 1*/\n");
                //      printf(" cp=%d ", p->numlabel);
                print_PC("cp", p->numlabel, n_bit_pc);

                //      printf(" & dp=%d\n", (p->next_then)->numlabel );
                printf(" & ");
                print_PC("dp", (p->next_then)->numlabel, n_bit_pc);
                printf("\n");


                printf(" & ");
                print_Mucke_expression(p->e.c.expr, "", 0);
                printf("\n)");
            }

            printf("|(\n");
            printf("                    /* ELSE */\n");
            //  printf(" cp=%d", p->numlabel);
            print_PC("cp", p->numlabel, n_bit_pc);
            if (p->next_else) {
                //          printf(" & dp=%d", (p->next_else)->numlabel);
                printf(" & ");
                print_PC("dp", (p->next_else)->numlabel, n_bit_pc);
            }
            else {
                if (p->e.c.elsestmt) {
                    //             printf(" & dp=%d", pc_return );
                    printf(" & ");
                    print_PC("dp", pc_return, n_bit_pc);
                }
                else print_d_pc(p);
            }
            printf("\n");
            printf(" &");
            printf("(!");
            print_Mucke_expression(p->e.c.expr, "", 0);
            printf(")\n");

            printf(")");
        } else {
            printf("|(  /* IF 2*/\n");
            //  printf(" cp=%d", p->numlabel);
            print_PC("cp", p->numlabel, n_bit_pc);

            if (p->next_then) {
                //          printf(" & (dp=%d", (p->next_then)->numlabel );
                printf(" & (");
                print_PC("dp", (p->next_then)->numlabel, n_bit_pc);
            }
            else {
                //          printf(" & (dp=%d", pc_return );
                printf(" & (");
                print_PC("dp", pc_return, n_bit_pc);
            }
            //  printf("|dp=");
            printf("|");

            if (p->next_else) {
                //             printf("%d)\n", (p->next_else)->numlabel);
                print_PC("dp", (p->next_else)->numlabel, n_bit_pc);
                printf(")\n");
            }
            else {
                if (p->e.c.elsestmt) {
                    //      printf("%d)\n", pc_return );
                    print_PC("dp", pc_return, n_bit_pc);
                    printf(")\n");
                }
                else {
                    if (p->next_stmt) {
                        //                printf("%d)\n", (p->next_stmt)->numlabel );
                        print_PC("dp", (p->next_stmt)->numlabel, n_bit_pc);
                        printf(")\n");
                    } else {
                        //                printf("%d)\n", pc_return );
                        print_PC("dp", pc_return, n_bit_pc);
                        printf(")\n");
                    }
                }
            }

            printf(")");
        }

        //recursive calls
        print_recursive_statement_program_Int2(p->next_then, fun, flag);
        print_recursive_statement_program_Int2(p->next_else, fun, flag);

        break;
    }

    case BP_WHILE: {
        if (*flag == 0) {
            *flag = 1;
            printf("| ((cm=%d)&\n", fun->funname_replace);
            printf("((false\n");
        }
        if (p->next_then) {
            printf("|(\n");
            printf("                    /* WHILE */\n");
            //         printf(" cp=%d\n", p->numlabel);
            print_PC("cp", p->numlabel, n_bit_pc);
            printf("\n");

            //         printf(" & dp=%d\n", (p->next_then)->numlabel );
            printf(" & ");
            print_PC("dp", (p->next_then)->numlabel, n_bit_pc);
            printf("\n");

            if (((p->e.c.expr)->token) != BP_ND) {
                printf(" &");
                print_Mucke_expression(p->e.c.expr, "", 0);
                printf("\n");
            }

            printf("\n)");
        }

        printf("|(\n");
        printf("                    /* WHILE EXIT */\n");
        //      printf(" cp=%d\n", p->numlabel);
        print_PC("cp", p->numlabel, n_bit_pc);
        printf("\n");


        print_d_pc(p);

        if (((p->e.c.expr)->token) != BP_ND) {
            printf(" &");
            printf(" (!");
            print_Mucke_expression(p->e.c.expr, "", 0);
            printf(")\n");
        }

        printf(")");

        print_recursive_statement_program_Int2(p->next_then, fun, flag);

        break;
    }

    case BP_PRINT:
    case BP_SKIP: {
        if (*flag == 0) {
            *flag = 1;
            printf("| ((cm=%d)&\n", fun->funname_replace);
            printf("((false\n");
        }
        printf("|(\n");
        printf("                    /* SKIP */\n");
        //      printf(" cp=%d\n", p->numlabel);
        print_PC("cp", p->numlabel, n_bit_pc);
        printf("\n");

        print_d_pc(p);
        printf(")");

        break;
    }
    case BP_GOTO: {
        if (*flag == 0) {
            *flag = 1;
            printf("| ((cm=%d)&\n", fun->funname_replace);
            printf("((false\n");
        }
        printf("|(\n");
        printf("                     /* GOTO */\n");
        //      printf(" cp=%d\n", p->numlabel);
        print_PC("cp", p->numlabel, n_bit_pc);
        printf("\n");


        print_d_pc(p);
        printf(")");

        break;
    }
    }

    print_recursive_statement_program_Int2(p->next_stmt, fun, flag);
    return;
}

/************************************************************************/

static void print_recursive_statement_Copy_Var_Assign(bp_stmt_t p, bp_fun_t fun, bp_ident_t var) {
    bp_idref_t q;
    bp_ident_t z;


    if ((!p) || (p->visited == 1)) return;
    p->visited = 1;

    switch (p->type) {
    case BP_ASSIGN: {
        //&&&&&&
        for (q = (p->e.a.asgnlist); q; q = q->next) {
            z = Is_there_var(bp_globals, q->var_name);
            if (!z) z = Is_there_var(bp_all_locals, q->var_name);
            if (!z) common_error("error in assign %s\n", q->var_name);
            if (z == var) {
                //             printf("|((m=%d)&(p=%d))", fun->funname_replace, p->numlabel);
                printf("|((m=%d)&", fun->funname_replace);
                print_PC("p", p->numlabel, n_bit_pc);
                printf(")");

            }
        }
        break;
    }

    case BP_IF:
    case BP_ELSEIF:
    case BP_WHILE: {
        print_recursive_statement_Copy_Var_Assign(p->next_then, fun, var);
        print_recursive_statement_Copy_Var_Assign(p->next_else, fun, var);
        break;
    }
    }

    print_recursive_statement_Copy_Var_Assign(p->next_stmt, fun, var);
    return;
}

static void print_recursive_statement_Copy_Var_Locals_Assign(bp_stmt_t p, bp_fun_t fun, int varnum, int *flag) {
    bp_idref_t q;
    bp_ident_t z;

    if ((!p) || (p->visited != 0)) return;
    p->visited = 1;

    switch (p->type) {
    case BP_ASSIGN: {
        for (q = (p->e.a.asgnlist); q; q = q->next) {
            z = Is_there_var(bp_all_locals, q->var_name);
            if (z && (z->varname_replace == varnum)) {
                //           printf("|((m=%d)&(p=%d))", fun->funname_replace, p->numlabel);
                printf("|((m=%d)&", fun->funname_replace);
                print_PC("p", p->numlabel, n_bit_pc);
                printf(")");
            }
        }
        break;
    }

    case BP_IF:
    case BP_ELSEIF:
    case BP_WHILE: {
        print_recursive_statement_Copy_Var_Locals_Assign(p->next_then, fun, varnum, flag);
        print_recursive_statement_Copy_Var_Locals_Assign(p->next_else, fun, varnum, flag);
        break;
    }
    }

    print_recursive_statement_Copy_Var_Locals_Assign(p->next_stmt, fun, varnum, flag);
    return;
}

static void print_recursive_statement_Exemption_CopyVariables(bp_stmt_t p) {
    bp_idref_t q;
    bp_ident_t z;

    if ((!p) || (p->visited != 0)) return;
    p->visited = 1;

    switch (p->type) {
    case BP_CALL:
    {
        printf(" | ");
        print_PC("p", p->numlabel, n_bit_pc);
        break;
    }

    case BP_IF:
    case BP_ELSEIF:
    case BP_WHILE: {
        print_recursive_statement_Exemption_CopyVariables(p->next_then);
        print_recursive_statement_Exemption_CopyVariables(p->next_else);
        break;
    }
    }

    print_recursive_statement_Exemption_CopyVariables(p->next_stmt);
    return;
}

static void print_Exemption_CopyVariables(void)
{
    bp_fun_t fun;
    for (fun = bp_functions; fun; fun = fun->next) {
        if (isThreadFunction(fun))
        {
            printf("|((m=%d)&(false ", fun->funname_replace);
            reset_flag_visited(fun->stmt);
            print_recursive_statement_Exemption_CopyVariables(fun->stmt);
            printf("))\n");
        }
    }
}

static void CopyVariables_ProgramInt(void) {
    bp_fun_t fun;
    bp_ident_t z;
    int flag, varnum;


    printf("bool CopyVariables_ProgramInt(\n");
    printf(" Module  m,\n");
    printf(" PrCount p,\n");
    printf(" Local   cL,\n");
    printf(" Local   dL,\n");
    printf(" Global  cG,\n");
    printf(" Global  dG\n");
    printf(")\n");
    printf(" m <  p,\n");
    printf(" p  <  cL,\n");
    printf(" cL  ~+ dL,\n");
    printf(" cL  <  cG,\n");
    printf(" cG  ~+ dG\n");
    printf("(false \n");
    // TRUC's modification
    print_Exemption_CopyVariables();
    printf("| (true\n");
    for (varnum = 0; varnum <= maxindexlocvar; varnum++) {
        flag = 0;
        printf("&((dL.v%d=cL.v%d)", varnum, varnum);
        for (fun = bp_functions; fun; fun = fun->next) {
            for (z = fun->locals; z; z = z->next)
                if (z->varname_replace == varnum) break;
            if (z) {
                reset_flag_visited(fun->stmt);
                print_recursive_statement_Copy_Var_Locals_Assign(fun->stmt, fun, varnum, &flag);
            } else printf("|(m=%d)", fun->funname_replace);

        }

        printf(")\n");
    }


    for (z = bp_globals; z; z = z->next) {
        printf("&((dG.v%d=cG.v%d) ", z->varname_replace, z->varname_replace);
        for (fun = bp_functions; fun; fun = fun->next) {
            if (Is_there_var(fun->globals, z->varname)) {
                reset_flag_visited(fun->stmt);
                print_recursive_statement_Copy_Var_Assign(fun->stmt, fun, z);
            }//else printf("|(m=%d)", fun->funname_replace);
        }
        printf(")\n");
    }

    printf("));\n");

}

//  ##    ##   #  #  ###   ####  #  #  ###          ##   #  #  ###   ###    ##   #  #
// #  #  #  #  ## #   #    #     #  #   #          #  #  #  #   #     #    #  #  #  #
// #     #  #  ## #   #    ###    ##    #           #    #  #   #     #    #     ####
// #     #  #  # ##   #    #      ##    #            #   ####   #     #    #     #  #
// #  #  #  #  # ##   #    #     #  #   #          #  #  ####   #     #    #  #  #  #
//  ##    ##   #  #   #    ####  #  #   #           ##   #  #  ###    #     ##   #  #

static void print_recursive_statement_programContextSwitch(bp_stmt_t p, bp_fun_t fun, short *head) {
    bp_ident_t y;
    bp_idref_t q;
    bp_ident_t z = NULL, temp_var;
    int i, tmp;

    if ((!p) || (p->visited != 0)) return;
    p->visited = 1;

    if (p->cs > 0) {
        printf("| /*CS_%d_%d*/ ", fun->funname_replace, p->cs);
        print_PC("cp", p->numlabel, n_bit_pc);
        printf("\n");
    }

    switch (p->type) {
    case BP_IF:
    case BP_ELSEIF:
    case BP_WHILE: {
        print_recursive_statement_programContextSwitch(p->next_then, fun, head);
        print_recursive_statement_programContextSwitch(p->next_else, fun, head);
        break;
    }
    }

    print_recursive_statement_programContextSwitch(p->next_stmt, fun, head);
    return;
}


static void print_Mucke_programContextSwitch(void) {
    bp_fun_t fun;
    short head;
    for (fun = bp_functions; fun; fun = fun->next) {
        if (
            (strcmp(fun->funname, "init") != 0)
            && isThreadFunction(fun)
        ) {
            head = 0;
            reset_flag_visited(fun->stmt);
            printf("bool ContextSwitch_%d(// %s\n", fun->funname_replace, fun->funname);
            printf(" PrCount cp\n");
            printf(")\n");
            printf("(false\n");
            printf("| /*CS_%d_0*/ ", fun->funname_replace);
            print_PC("cp", 0, n_bit_pc);
            printf("\n");
            print_recursive_statement_programContextSwitch(fun->stmt, fun, &head);
            printf("| /*CS_%d_%d*/ ", fun->funname_replace, fun->last_cs);
            print_PC("cp", pc_return, n_bit_pc);
            printf("\n");
            printf(");\n#size ContextSwitch_%d;\n\n\n", fun->funname_replace);
        }
    }

}


// ###   #  #  ###   ####  ###   #  #   ##   #           ###   ###    ##   #  #   ##   ###   ###   ###    ##   #  #
//  #    ## #   #    #     #  #  ## #  #  #  #            #    #  #  #  #  ## #  #  #   #     #     #    #  #  ## #
//  #    ## #   #    ###   #  #  ## #  #  #  #            #    #  #  #  #  ## #   #     #     #     #    #  #  ## #
//  #    # ##   #    #     ###   # ##  ####  #            #    ###   ####  # ##    #    #     #     #    #  #  # ##
//  #    # ##   #    #     # #   # ##  #  #  #            #    # #   #  #  # ##  #  #   #     #     #    #  #  # ##
// ###   #  #   #    ####  #  #  #  #  #  #  ####         #    #  #  #  #  #  #   ##   ###    #    ###    ##   #  #

static void print_Mucke_programInt1(void) {
    bp_fun_t fun;
    short head;

    printf("bool programInt1(\n");
    printf(" Module  cm,\n");
    printf(" PrCount cp,\n");
    printf(" PrCount dp,\n");
    printf(" Local   L,\n");
    printf(" Local   dL,\n");
    printf(" Global  G,\n");
    printf(" Global  dG\n");
    printf(")\n");
    printf(" cm <  cp,\n");
    printf(" cp  ~+ dp,\n");
    printf(" cp  <  L,\n");
    printf(" L   ~+ dL,\n");
    printf(" L   <  G,\n");
    printf(" G   ~+ dG\n");
    printf("(false\n");

    for (fun = bp_functions; fun; fun = fun->next) {
        if (isThreadFunction(fun))
        {
            head = 0;
            reset_flag_visited(fun->stmt);
            print_recursive_statement_program_Int1(fun->stmt, fun, &head);
            if (head == 1) printf("))\n");
        }
    }
    printf(");\n");

    return;
}


static void print_Mucke_programInt2(void) {
    bp_fun_t fun;
    short flag;

    printf("bool programInt2(\n");
    printf(" Module  cm,\n");
    printf(" PrCount cp,\n");
    printf(" PrCount dp,\n");
    printf(" Local   L, \n");
    printf(" Global  G  \n");
    printf(")          \n");
    printf(" cm <  cp, \n");
    printf(" cp  ~+ dp,\n");
    printf(" cp  <  L, \n");
    printf(" L  <  G   \n");
    printf("(false    \n");

    for (fun = bp_functions; fun; fun = fun->next) {
        if (isThreadFunction(fun))
        {
            flag = 0;
            /*
            printf("| ((cm=%d)&\n",fun->funname_replace);
            printf("((false\n");
            */
            reset_flag_visited(fun->stmt);
            print_recursive_statement_program_Int2(fun->stmt, fun, &flag);
            if (flag == 1) printf(")))\n");
        }
    }
    printf(");\n");

    return;
}

static void print_Mucke_programInt3(void) {
    bp_fun_t p;
    short flag;

    printf("bool programInt3(\n");
    printf(" Module  cm,\n");
    printf(" PrCount cp,\n");
    printf(" PrCount dp,\n");
    printf(" Local   L,\n");
    printf(" Local   dL,\n");
    printf(" Global  G,\n");
    printf(" Global  dG\n");
    printf(")\n");
    printf(" cm <  cp,\n");
    printf(" cp  ~+ dp,\n");
    printf(" cp  <  L,\n");
    printf(" L  ~+ dL,\n");
    printf(" L  <  G,\n");
    printf(" G  ~+ dG\n");
    printf("(false\n");

    for (p = bp_functions; p; p = p->next) {
        if (isThreadFunction(p))
        {   flag = 0;
            reset_flag_visited(p->stmt);
            /*
             printf("| ( (cm=%d) &\n",p->funname_replace);
             printf("    (\n");

               printf("( false\n");
            */
            reset_flag_visited(p->stmt);
            print_recursive_statement_program_Int3(p->stmt, p, &flag);

            if (flag == 1) printf(")))");

            //      printf(")");

            //      printf(")\n");
        }
    }
    printf(");\n");

    return;
}

/********************************************************************************************/
/********************************************************************************************/

static void print_Mucke_programInt1_nonthread(void) {
    bp_fun_t fun;
    short head;

    printf("bool programInt1nonthread(\n");
    printf(" Module  cm,\n");
    printf(" PrCount cp,\n");
    printf(" PrCount dp,\n");
    printf(" Local   L,\n");
    printf(" Local   dL,\n");
    printf(" Global  G,\n");
    printf(" Global  dG\n");
    printf(")\n");
    printf(" cm <  cp,\n");
    printf(" cp  ~+ dp,\n");
    printf(" cp  <  L,\n");
    printf(" L   ~+ dL,\n");
    printf(" L   <  G,\n");
    printf(" G   ~+ dG\n");
    printf("(false\n");

    for (fun = bp_functions; fun; fun = fun->next) {
        if (!isThreadFunction(fun))
        {   head = 0;
            reset_flag_visited(fun->stmt);
            print_recursive_statement_program_Int1(fun->stmt, fun, &head);
            if (head == 1) printf("))\n");
        }
    }
    printf(");\n");

    return;
}


static void print_Mucke_programInt2_nonthread(void) {
    bp_fun_t fun;
    short flag;

    printf("bool programInt2nonthread(\n");
    printf(" Module  cm,\n");
    printf(" PrCount cp,\n");
    printf(" PrCount dp,\n");
    printf(" Local   L, \n");
    printf(" Global  G  \n");
    printf(")          \n");
    printf(" cm <  cp, \n");
    printf(" cp  ~+ dp,\n");
    printf(" cp  <  L, \n");
    printf(" L  <  G   \n");
    printf("(false    \n");

    for (fun = bp_functions; fun; fun = fun->next) {
        if (!isThreadFunction(fun))
        {   flag = 0;
            /*
            printf("| ((cm=%d)&\n",fun->funname_replace);
            printf("((false\n");
            */
            reset_flag_visited(fun->stmt);
            print_recursive_statement_program_Int2(fun->stmt, fun, &flag);
            if (flag == 1) printf(")))\n");
        }
    }
    printf(");\n");

    return;
}

static void print_Mucke_programInt3_nonthread(void) {
    bp_fun_t p;
    short flag;

    printf("bool programInt3nonthread(\n");
    printf(" Module  cm,\n");
    printf(" PrCount cp,\n");
    printf(" PrCount dp,\n");
    printf(" Local   L,\n");
    printf(" Local   dL,\n");
    printf(" Global  G,\n");
    printf(" Global  dG\n");
    printf(")\n");
    printf(" cm <  cp,\n");
    printf(" cp  ~+ dp,\n");
    printf(" cp  <  L,\n");
    printf(" L  ~+ dL,\n");
    printf(" L  <  G,\n");
    printf(" G  ~+ dG\n");
    printf("(false\n");

    for (p = bp_functions; p; p = p->next) {
        if (!isThreadFunction(p))
        {   flag = 0;
            reset_flag_visited(p->stmt);
            /*
             printf("| ( (cm=%d) &\n",p->funname_replace);
             printf("    (\n");

               printf("( false\n");
            */
            reset_flag_visited(p->stmt);
            print_recursive_statement_program_Int3(p->stmt, p, &flag);

            if (flag == 1) printf(")))");

            //      printf(")");

            //      printf(")\n");

        }
    }
    printf(");\n");

    return;
}


/******************************************************************/
static void print_recursive_statement_program_Call(bp_stmt_t p, bp_fun_t fun) {
    bp_fun_t y;
    bp_idref_t q;
    bp_ident_t z = NULL, t, tmp;
    int i;

    if ((!p) || (p->visited != 0)) return;
    p->visited = 1;

    switch (p->type) {
    case BP_CALL: {
        // fun:  is caller
        // y : is callee
        printf("|(");
        printf("cm=%d", fun->funname_replace);
        // printf("&cp=%d\n", p->numlabel);
        printf(" & ");
        print_PC("cp", p->numlabel, n_bit_pc);

        y = find_function(p->e.a.label);

        printf("&dm=%d", y->funname_replace);


        // Parameter of callee
        t = y->parms;
        for (q = (p->e.a.fmllist); q; q = q->next) {
            z = Is_there_var(bp_all_locals, t->varname);
            if (!z) common_error("error in ProgramCall: local variable not found\n");
            //          t->varname_replace = tmp->varname_replace;

            if (((q->expr)->token) == BP_CHOOSE) {
                // ((a&f1) | (!a & !f1 & f2 ) | (!f1 & ! f2))
                printf("&((");
                if (i == 0) printf("dG.v%d", z->varname_replace);
                else printf("dL.v%d", z->varname_replace);
                printf("&(");
                print_Mucke_expression(q->expr->left, "c", 0);
                printf("))|((!");
                if (i == 0) printf("dG.v%d", z->varname_replace);
                else printf("dL.v%d", z->varname_replace);
                printf(")&(!");
                print_Mucke_expression(q->expr->left, "c", 0);
                printf(")&(");
                print_Mucke_expression(q->expr->right, "c", 0);
                printf("))|((!");
                print_Mucke_expression(q->expr->left, "c", 0);
                printf(")&(!");
                print_Mucke_expression(q->expr->right, "c", 0);
                printf(")))\n");
            }
            else {
                printf("&(dL.v%d", z->varname_replace);
                if ((q->expr->token == BP_IDENT) || (q->expr->token == BP_CONST))
                    printf("=");
                else printf("<->");

                print_Mucke_expression(q->expr, "c", 0);
                printf(")\n");
            }
            t = t->next;    // Next parameter of callee
        }
        if (t || q) {
            common_error("error in ProgramCall: the list of parameters and arguments are of different lenght\n");
        }

        printf(")");

        break;
    }

    case BP_IF:
    case BP_ELSEIF:
    case BP_WHILE: {
        print_recursive_statement_program_Call(p->next_then, fun);
        print_recursive_statement_program_Call(p->next_else, fun);
        break;
    }
    }

    print_recursive_statement_program_Call(p->next_stmt, fun);
    return;

}

static void print_statements_programCall(void) {
    bp_fun_t p;

    for (p = bp_functions; p; p = p->next) {
        reset_flag_visited(p->stmt);
        print_recursive_statement_program_Call(p->stmt, p);
    }
}

/**
 * This is to set parameter of a function call
 */
static void print_Mucke_programCall(void) {
    printf("bool programCall( \n");
    printf(" Module  cm,    \n");                // Caller
    printf(" Module  dm,    \n");                // Callee
    printf(" PrCount cp,     \n");               // Caller PC
    printf(" Local   cL,      \n");              // Caller Local
    printf(" Local   dL,      \n");              // Callee Local (Parameter)
    printf(" Global  cG       \n");
    printf(")                 \n");
    printf(" cm ~+ dm,    \n");
    printf(" cm < cp,      \n");
    printf(" cp  < cL,       \n");
    printf(" cL  ~+ dL,       \n");
    printf(" cL  < cG         \n");
    printf("(false\n");
    print_statements_programCall();
    printf("\n);\n");
}

/***********************************************************************/
static void print_recursive_statement_Calling(bp_stmt_t p, bp_fun_t fun) {
    if ((!p) || (p->visited != 0)) return;
    p->visited = 1;

    switch (p->type) {
    case BP_CALL: {
        printf("| (m=%d", fun->funname_replace);
        printf(" & ");
        print_PC("p", p->numlabel, n_bit_pc);
        printf(")\n");
        break;
    }
    case BP_IF:
    case BP_ELSEIF:
    case BP_WHILE: {
        print_recursive_statement_Calling(p->next_then, fun);
        print_recursive_statement_Calling(p->next_else, fun);
        break;
    }
    }

    print_recursive_statement_Calling(p->next_stmt, fun);
    return;
}


static void print_statements_Calling(void) {
    bp_fun_t p;

    for (p = bp_functions; p; p = p->next) {
        reset_flag_visited(p->stmt);
        print_recursive_statement_Calling(p->stmt, p);
    }
}

/**
 * Calling is where a module get call
 */
static void print_Mucke_Calling(void) {
    printf("bool Calling(Module m, PrCount p)\n");
    printf("(false\n");

    print_statements_Calling();

    printf(");\n");
}


/***********************************************************************/
static void print_recursive_SkipCall(bp_stmt_t p, bp_fun_t fun) {
    int index, i = 0;
    bp_idref_t q;
    bp_ident_t z = NULL, y;

    if ((!p) || (p->visited != 0)) return;
    p->visited = 1;

    switch (p->type) {
    case BP_CALL: {
        printf("|(");
        printf("cm=%d", fun->funname_replace);
        // printf("&cp=%d", p->numlabel);
        printf(" & ");
        print_PC("cp", p->numlabel, n_bit_pc);
        print_d_pc(p);
        printf(")\n");
        break;
    }

    case BP_IF:
    case BP_ELSEIF:
    case BP_WHILE: {
        print_recursive_SkipCall(p->next_then, fun);
        print_recursive_SkipCall(p->next_else, fun);
        break;
    }
    }

    print_recursive_SkipCall(p->next_stmt, fun);
    return;
}


static void print_statements_SkipCall(void) {
    bp_fun_t p;

    for (p = bp_functions; p; p = p->next) {
        reset_flag_visited(p->stmt);
        print_recursive_SkipCall(p->stmt, p);
    }
}

/**
 * Change program counter of caller
 */
static void print_Mucke_SkipCall(void) {
    printf("bool SkipCall(\n");
    printf(" Module  cm,\n");    // Current module of caller
    printf(" PrCount cp, \n");   // program counter
    printf(" PrCount dp  \n");   // Next program coutner
    printf(")             \n");
    printf("cm < cp, \n");
    printf("dp ~+ dp \n");
    printf("(false\n");

    print_statements_SkipCall();

    printf("\n);\n\n");
}

/***************************************************************/
static void print_recursive_Exit(bp_stmt_t p, bp_fun_t fun) {
    int index, i;
    bp_idref_t q;
    bp_ident_t z = NULL;

    if ((!p) || (p->visited != 0)) return;
    p->visited = 1;

    switch (p->type) {
    case BP_RETURN: {
        printf("|(cm=%d", fun->funname_replace);
        printf(" & ");
        print_PC("cp", p->numlabel, n_bit_pc);
        printf(")\n");

        break;
    }
    case BP_IF:
    case BP_ELSEIF:
    case BP_WHILE: {
        print_recursive_Exit(p->next_then, fun);
        print_recursive_Exit(p->next_else, fun);
        break;
    }
    }
    print_recursive_Exit(p->next_stmt, fun);
    return;

}

static void print_statements_Exit(void) {
    bp_fun_t p;

    for (p = bp_functions; p; p = p->next) {
        reset_flag_visited(p->stmt);
        print_recursive_Exit(p->stmt, p);
        if (p->num_returns == 0)    /// If there is no return
        {   printf("|( cm=%d & ", p->funname_replace);
            print_PC("cp", pc_return, n_bit_pc);
            printf(")\n");
        }
    }
}

/**
 * Set exist point of callee
 */
static void print_Mucke_Exit(void) {

    printf(" bool Exit( Module cm, PrCount cp )\n");
    printf("(false\n");

    print_statements_Exit();

    //  printf("|( cp = %d)\n", pc_return);

    printf("); \n");
}


/***************************************************************/
static void print_recursive_SetReturnUS_FromCall(bp_stmt_t p, bp_fun_t fun_call, bp_stmt_t p_return,
        bp_fun_t fun_return, short *flag) {
    int i = 0, j = 0;
    bp_ident_t z, y, temp_var;
    bp_idref_t assign_call, list_return_expr;


    if (!p) return;

    switch (p->type) {
    case BP_CALL: {
        if ((strcmp(p->e.a.label, fun_return->funname) == 0) && ((p->numlabel) >= 0)) {

            if (*flag == 0) {
                *flag = 1;
                printf("|(\n");
                printf("   um=%d", fun_return->funname_replace);
                // printf("&up=%d\n", p_return->numlabel);
                printf(" & ");
                print_PC("up", p_return->numlabel, n_bit_pc);
                printf("\n");
                printf(" & (false\n");

            }

            printf("    |(");
            printf("tm=%d", fun_call->funname_replace);
            // printf("&tp=%d\n", p->numlabel);
            printf(" & ");
            print_PC("tp", p->numlabel, n_bit_pc);
            printf("\n");


            list_return_expr = p_return->e.a.asgnlist;

            for (assign_call = (p->e.a.asgnlist); assign_call; assign_call = assign_call->next) {
                z = Is_there_var(bp_globals, assign_call->var_name);
                if (z)
                    printf("       &(sG.v%d", z->varname_replace);
                else {
                    z = Is_there_var(bp_all_locals, assign_call->var_name);
                    if (!z) common_error("error in SetReturn: variable not found\n");

                    printf("       &(sL.v%d", z->varname_replace);
                }

                if ((list_return_expr->expr->token == BP_IDENT) || (list_return_expr->expr->token == BP_CONST))
                    printf("=");
                else printf("<->");

                print_Mucke_expression(list_return_expr->expr, "u", 0);
                printf(")\n");

                list_return_expr = list_return_expr->next;
            }

            printf("     )\n");
        }

        break;
    }

    case BP_IF:
    case BP_ELSEIF:
    case BP_WHILE: {
        print_recursive_SetReturnUS_FromCall(p->e.c.thenstmt, fun_call, p_return, fun_return, flag);
        print_recursive_SetReturnUS_FromCall(p->e.c.elsestmt, fun_call, p_return, fun_return, flag);
        break;
    }
    }

    print_recursive_SetReturnUS_FromCall(p->next, fun_call, p_return, fun_return, flag);

    return;
}


static void print_recursive_SetReturnUS(bp_stmt_t p, bp_fun_t fun) {
    bp_fun_t fun_p;
    short flag;

    if ((!p) || (p->visited != 0)) return;
    p->visited = 1;

    switch (p->type) {
    case BP_RETURN: {
        flag = 0;
        for (fun_p = bp_functions; fun_p; fun_p = fun_p->next)
            print_recursive_SetReturnUS_FromCall(fun_p->stmt, fun_p, p, fun, &flag);

        if (flag == 1) printf("   )\n)\n");

        break;
    }

    case BP_IF:
    case BP_ELSEIF:
    case BP_WHILE: {
        print_recursive_SetReturnUS(p->next_then, fun);
        print_recursive_SetReturnUS(p->next_else, fun);
        break;
    }
    }

    print_recursive_SetReturnUS(p->next_stmt, fun);
}


static void print_statements_SetReturnUS(void) {
    bp_fun_t fun;

    for (fun = bp_functions; fun; fun = fun->next) {
        reset_flag_visited(fun->stmt);
        print_recursive_SetReturnUS(fun->stmt, fun);
    }
}

static void print_recursive_SetReturnUS_FromCallglobals(bp_stmt_t p, bp_fun_t fun_call, bp_stmt_t p_return,
        bp_fun_t fun_return, bp_ident_t var, short *head) {
    bp_ident_t z, y, temp_var;
    bp_idref_t assign_call, list_return_expr;
    int flag;

    if (!p) return;

    switch (p->type) {
    case BP_CALL: {
        if ((strcmp(p->e.a.label, fun_return->funname) == 0) && ((p->numlabel) >= 0)) {
            list_return_expr = p_return->e.a.asgnlist;

            flag = 0;
            for (assign_call = (p->e.a.asgnlist); assign_call; assign_call = assign_call->next) {
                z = Is_there_var(bp_globals, assign_call->var_name);

                if (z && (z == var)) {
                    flag = 1;
                    if ((*head) == 1) {
                        *head = 0;
                        printf("|(");
                        printf(" (um=%d)", fun_return->funname_replace);
                        // printf(" & (up=%d)", p_return->numlabel);
                        printf(" & ");
                        print_PC("up", p_return->numlabel, n_bit_pc);
                        printf(" & (false");
                    }

                    printf("    |(");
                    printf("         (tm=%d)", fun_call->funname_replace);
                    // printf("       & (tp=%d))", p->numlabel);
                    printf("       & ");
                    print_PC("tp", p->numlabel, n_bit_pc);
                    printf(")");

                }

                list_return_expr = list_return_expr->next;
            }

        }

        break;
    }

    case BP_IF:
    case BP_ELSEIF:
    case BP_WHILE: {
        print_recursive_SetReturnUS_FromCallglobals(p->e.c.thenstmt, fun_call, p_return, fun_return, var, head);
        print_recursive_SetReturnUS_FromCallglobals(p->e.c.elsestmt, fun_call, p_return, fun_return, var, head);
        break;
    }
    }

    print_recursive_SetReturnUS_FromCallglobals(p->next, fun_call, p_return, fun_return, var, head);

    return;
}


static void print_recursive_SetReturnUSglobals(bp_stmt_t p, bp_fun_t fun, bp_ident_t var) {
    bp_fun_t fun_p;
    short head;

    if ((!p) || (p->visited != 0)) return;
    p->visited = 1;

    switch (p->type) {
    case BP_RETURN: {
        head = 1;
        for (fun_p = bp_functions; fun_p; fun_p = fun_p->next)
            print_recursive_SetReturnUS_FromCallglobals(fun_p->stmt, fun_p, p, fun, var, &head);

        if (head == 0) printf("   ))\n");

        break;
    }

    case BP_IF:
    case BP_ELSEIF:
    case BP_WHILE: {
        print_recursive_SetReturnUSglobals(p->next_then, fun, var);
        print_recursive_SetReturnUSglobals(p->next_else, fun, var);
        break;
    }
    }

    print_recursive_SetReturnUSglobals(p->next_stmt, fun, var);
}

/**
 * Set return from the callee
 *
 */
static void print_SetReturnUS(void) {
    bp_fun_t fun;
    bp_ident_t z;

    printf("bool SetReturnUS(\n");
    printf(" Module  tm,\n");
    printf(" Module  um,\n");
    printf(" PrCount tp,\n");
    printf(" PrCount up,\n");
    printf(" Local   uL,\n");
    printf(" Local   sL,\n");
    printf(" Global  uG,\n");
    printf(" Global  sG \n");
    printf(")           \n");
    printf(" tm   ~+ um,\n");
    printf(" tm   <  tp,\n");
    printf(" tp  ~+ up, \n");
    printf(" tp  <  uL, \n");
    printf(" uL  ~+ sL, \n");
    printf(" uL  <  uG, \n");
    printf(" uG  ~+ sG  \n");
    printf("((false\n");
    print_statements_SetReturnUS();
    printf(")");


    for (z = bp_globals; z; z = z->next) {
        printf("&((sG.v%d=uG.v%d)", z->varname_replace, z->varname_replace);
        /*
              for(fun=bp_functions; fun; fun=fun->next)
              {
                 if (!Is_there_var(fun->globals, z->varname))
                    printf("|(um=%d)",fun->funname_replace);

              }
        */

        for (fun = bp_functions; fun; fun = fun->next) {
            reset_flag_visited(fun->stmt);
            print_recursive_SetReturnUSglobals(fun->stmt, fun, z);
        }
        printf(")\n");
    }


    printf(");\n\n\n");
}

///////////////////////////////////////////////////////////////////////////////////////////
static void print_recursive_SetReturnTS_FromCall_globals(bp_stmt_t p, bp_fun_t fun_call,
        bp_stmt_t p_return, bp_fun_t fun_return, bp_ident_t var,
        short *head) {
    bp_ident_t z, y, temp_var;
    bp_idref_t assign_call, list_return_expr;

    if (!p) return;

    switch (p->type) {
    case BP_CALL: {
        if ((strcmp(p->e.a.label, fun_return->funname) == 0) && ((p->numlabel) >= 0)) {
            list_return_expr = p_return->e.a.asgnlist;

            for (assign_call = (p->e.a.asgnlist); assign_call; assign_call = assign_call->next) {
                z = Is_there_var(bp_globals, assign_call->var_name);

                if (z && (z == var)) {
                    if ((*head) == 1) {
                        *head = 0;
                        printf("|(");
                        printf(" (um=%d)", fun_return->funname_replace);
                        // printf(" &(up=%d)", p_return->numlabel);
                        printf(" & ");
                        print_PC("up", p_return->numlabel, n_bit_pc);
                        printf(" &(false");
                    }
                    printf("    |(");
                    printf("         (tm=%d)", fun_call->funname_replace);
                    // printf("       & (tp=%d))", p->numlabel);
                    printf("       &  ");
                    print_PC("tp", p->numlabel, n_bit_pc);
                    printf(")");
                }

                list_return_expr = list_return_expr->next;
            }

        }

        break;
    }

    case BP_IF:
    case BP_ELSEIF:
    case BP_WHILE: {
        print_recursive_SetReturnTS_FromCall_globals(p->e.c.thenstmt, fun_call, p_return, fun_return, var, head);
        print_recursive_SetReturnTS_FromCall_globals(p->e.c.elsestmt, fun_call, p_return, fun_return, var, head);
        break;
    }
    }

    print_recursive_SetReturnTS_FromCall_globals(p->next, fun_call, p_return, fun_return, var, head);

    return;
}


static void print_recursive_SetReturnTS_globals(bp_stmt_t p, bp_fun_t fun, bp_ident_t var) {
    bp_fun_t fun_p;
    short head;


    if ((!p) || (p->visited != 0)) return;
    p->visited = 1;

    switch (p->type) {
    case BP_RETURN: {

        head = 1;
        for (fun_p = bp_functions; fun_p; fun_p = fun_p->next)
            if (Is_there_var(fun_p->globals, var->varname))
                print_recursive_SetReturnTS_FromCall_globals(fun_p->stmt, fun_p, p, fun, var, &head);

        if (head == 0) printf("   ))\n");

        break;
    }

    case BP_IF:
    case BP_ELSEIF:
    case BP_WHILE: {
        print_recursive_SetReturnTS_globals(p->next_then, fun, var);
        print_recursive_SetReturnTS_globals(p->next_else, fun, var);
        break;
    }
    }

    print_recursive_SetReturnTS_globals(p->next_stmt, fun, var);
}


/*******************************************************************************/

static void print_recursive_SetReturnTS_FromCall_locals(bp_stmt_t p, bp_fun_t fun_call, bp_stmt_t p_return,
        bp_fun_t fun_return, int varnum, short *head) {
    bp_ident_t z, y, temp_var;
    bp_idref_t assign_call, list_return_expr;

    if (!p) return;

    switch (p->type) {
    case BP_CALL: {
        if ((strcmp(p->e.a.label, fun_return->funname) == 0) && ((p->numlabel) >= 0)) {
            list_return_expr = p_return->e.a.asgnlist;

            for (assign_call = (p->e.a.asgnlist); assign_call; assign_call = assign_call->next) {
                z = Is_there_var(bp_all_locals, assign_call->var_name);
                if (z && (z->varname_replace == varnum)) {
                    if ((*head) == 1) {
                        *head = 0;
                        printf("|(");
                        printf(" (um=%d)", fun_return->funname_replace);
                        // printf(" &(up=%d)", p_return->numlabel);
                        printf(" & ");
                        print_PC("up", p_return->numlabel, n_bit_pc);
                        printf(" &(false");
                    }

                    printf("|(");
                    printf("(tm=%d)", fun_call->funname_replace);
                    // printf("& (tp=%d))", p->numlabel);
                    printf(" & ");
                    print_PC("tp", p->numlabel, n_bit_pc);
                    printf(")");
                }

                list_return_expr = list_return_expr->next;
            }

        }

        break;
    }

    case BP_IF:
    case BP_ELSEIF:
    case BP_WHILE: {
        print_recursive_SetReturnTS_FromCall_locals(p->e.c.thenstmt, fun_call, p_return, fun_return, varnum, head);
        print_recursive_SetReturnTS_FromCall_locals(p->e.c.elsestmt, fun_call, p_return, fun_return, varnum, head);
        break;
    }
    }

    print_recursive_SetReturnTS_FromCall_locals(p->next, fun_call, p_return, fun_return, varnum, head);

    return;
}


static void print_recursive_SetReturnTS_locals(bp_stmt_t p, bp_fun_t fun, int varnum) {
    bp_fun_t fun_p;
    short head;

    if ((!p) || (p->visited != 0)) return;
    p->visited = 1;

    switch (p->type) {
    case BP_RETURN: {

        head = 1;
        for (fun_p = bp_functions; fun_p; fun_p = fun_p->next)
            //        if (Is_there_var(fun_p->locals, var->varname))
            print_recursive_SetReturnTS_FromCall_locals(fun_p->stmt, fun_p, p, fun, varnum, &head);

        if (head == 0) printf("   ))\n");

        break;
    }

    case BP_IF:
    case BP_ELSEIF:
    case BP_WHILE: {
        print_recursive_SetReturnTS_locals(p->next_then, fun, varnum);
        print_recursive_SetReturnTS_locals(p->next_else, fun, varnum);
        break;
    }
    }

    print_recursive_SetReturnTS_locals(p->next_stmt, fun, varnum);
}


/**********************************************************************************/

/**
 * Return from calling function (T->S)
 * T:   point when calling
 * S:   point when returning
 * U:   callee
 */
static void print_SetReturnTS(void) {
    bp_ident_t z;
    bp_fun_t fun;
    int varnum;
    printf("bool SetReturnTS( \n");
    printf(" Module  tm,\n");  // Caller
    printf(" Module  um,\n");  // Callee
    printf(" PrCount tp,\n");  // Caller PC
    printf(" PrCount up,\n");  // Callee PC (when returning)
    printf(" Local   tL,\n");  // Local of Caller
    printf(" Local   sL,\n");  // Local of Caller after returning
    printf(" Global  tG,\n");  // Global of Caller
    printf(" Global  sG \n");  // Global of Caller after returning
    printf(")           \n");
    printf(" tm ~+ um,  \n");
    printf(" tm <  tp,  \n");
    printf(" tp  ~+ up, \n");
    printf(" tp  <  tL, \n");
    printf(" tL  ~+ sL, \n");
    printf(" tL  <  tG, \n");
    printf(" tG  ~+ sG  \n");
    printf("(false \n");

    printf("| (true\n");

    for (varnum = 0; varnum <= maxindexlocvar; varnum++) {
        printf("&((sL.v%d=tL.v%d)", varnum, varnum);   // All local variable will be copied exept that

        for (fun = bp_functions; fun; fun = fun->next) {
            reset_flag_visited(fun->stmt);
            print_recursive_SetReturnTS_locals(fun->stmt, fun, varnum);
        }
        /*
              for(fun=bp_functions; fun; fun=fun->next)
              {
          for (z=fun->locals; z; z=z->next){
                    if (z->varname_replace == varnum) break;
          }

              if (!z) printf("|(tm=%d)",fun->funname_replace);
              }
        */
        printf(")\n");
    }

    for (z = bp_globals; z; z = z->next) {
        printf("&((sG.v%d=tG.v%d)", z->varname_replace, z->varname_replace);
        for (fun = bp_functions; fun; fun = fun->next) {
            reset_flag_visited(fun->stmt);
            print_recursive_SetReturnTS_globals(fun->stmt, fun, z);

            if (Is_there_var(fun->globals, z->varname))
                printf("|(um=%d)", fun->funname_replace);
            else
                printf("|(tm=%d)", fun->funname_replace);

        }
        printf(")\n");
    }
    printf("));\n");
}


/******************************************************************/

static int put_pc_to_statements(bp_stmt_t p, int pc) {

    if ((!p) || ((p->numlabel) > -1)) return pc;

    pc++;

    (p->numlabel) = pc;

    switch (p->type) {
    case BP_IF:
    case BP_ELSEIF:
    case BP_WHILE: {
        pc = put_pc_to_statements(p->next_then, pc);
        pc = put_pc_to_statements(p->next_else, pc);
        break;
    }
    }

    pc = put_pc_to_statements(p->next_stmt, pc);
    return pc;
}


/********************************************************************/

static void discover_new_locals_in_assign_id_list(bp_idref_t p, bp_fun_t fun) {
    bp_ident_t y;

    for (; p; p = p->next) {
        y = Is_there_var(bp_globals, p->var_name);
        if (!y) {
            y = Is_there_var(fun->locals, p->var_name);
            if (!y) {
                bp_decls = fun->locals;
                Insert_var(p->var_name);
                fun->locals = bp_decls;
            }
        }
    }
}

static void discover_new_locals_in_expression(bp_expr_t p, bp_fun_t fun) {
    bp_ident_t y, q;

    if (!p) return;

    switch (p->token) {
    case BP_CONST: {
        return;
    }
    case BP_IDENT: {
        y = Is_there_var(bp_globals, p->var_name);
        if (y) return;
        y = Is_there_var(fun->locals, p->var_name);
        if (!y) {
            bp_decls = fun->locals;
            Insert_var(p->var_name);
            fun->locals = bp_decls;
        }
        return;
    }
    }

    discover_new_locals_in_expression(p->left, fun);
    discover_new_locals_in_expression(p->right, fun);
    return;
}

static void discover_new_locals_in_decider(bp_expr_t p, bp_fun_t fun) {
    if (p->token != BP_ND)
        discover_new_locals_in_expression(p, fun);
}

static void discover_new_locals_in_expression_list(bp_idref_t p, bp_fun_t fun) {
    for (; p; p = p->next)
        discover_new_locals_in_expression(p->expr, fun);
}

static void discover_new_locals_in_statements(bp_stmt_t p, bp_fun_t fun) {
    if ((!p) || (p->visited != 0)) return;

    p->visited = 1;

    switch (p->type) {
    case BP_ASSIGN: {
        discover_new_locals_in_assign_id_list(p->e.a.asgnlist, fun);
        discover_new_locals_in_expression_list(p->e.a.asgnlist, fun);
        break;
    }
    case BP_ASSERT:
    case BP_CONSTRAIN:
    case BP_ASSUME: {
        discover_new_locals_in_decider(p->e.c.expr, fun);
        break;
    }

    case BP_CALL: {
        discover_new_locals_in_assign_id_list(p->e.a.asgnlist, fun);
        discover_new_locals_in_expression_list(p->e.a.fmllist, fun);
        break;
    }
    case BP_IF:
    case BP_ELSEIF:
    case BP_WHILE: {
        discover_new_locals_in_decider(p->e.c.expr, fun);
        discover_new_locals_in_statements(p->next_then, fun);
        discover_new_locals_in_statements(p->next_else, fun);
        break;
    }
    case BP_RETURN: {
        discover_new_locals_in_expression_list(p->e.a.asgnlist, fun);
        break;
    }
    }

    discover_new_locals_in_statements(p->next_stmt, fun);
}

static void discover_new_locals(void) {
    bp_fun_t p;

    for (p = bp_functions; p; p = p->next) {
        discover_new_locals_in_statements(p->stmt, p);
    }

}


/********************************************************************/

static void recursive_set_next_statement(bp_stmt_t start, bp_stmt_t p, bp_stmt_t next_p) {

    bp_stmt_t temp;

    if (!p) return;

    if (p->next) p->next_stmt = p->next;
    else p->next_stmt = next_p;


    switch (p->type) {
    case BP_IF:
    case BP_ELSEIF: {
        recursive_set_next_statement(start, p->e.c.thenstmt, p->next_stmt);
        recursive_set_next_statement(start, p->e.c.elsestmt, p->next_stmt);
        break;
    }
    case BP_WHILE: {
        recursive_set_next_statement(start, p->e.c.thenstmt, p);
        break;
    }
    case BP_GOTO: {
        p->next_stmt = find_label(start, p->e.a.label);
        if (p->next_stmt == NULL)
            common_error("error: label of goto not found: %s\n", p->e.a.label);

        break;
    }
    }

    recursive_set_next_statement(start, p->next, next_p);
}

static void set_next_statement(void) {
    bp_fun_t p;

    for (p = bp_functions; p; p = p->next) {
        recursive_set_next_statement(p->stmt, p->stmt, NULL);
    }
}


/********************************************************************/

static bp_stmt_t next_no_skip_goto_skip(bp_stmt_t p, bp_fun_t fun) {
    int i;
    bp_stmt_t z;


    if ((!p) || (p->visited != 0)) return NULL;

    if ((p->label) != NULL) {
        if (strcmp(p->label, target_label) == 0) return p;
        if (strcmp(p->label, "endinit") == 0) return p;
    }
    p->visited = 1;

    switch (p->type) {
    case BP_ASSUME: {
        if ((((p->e.c.expr)->token) == BP_CONST) && ((p->e.c.expr)->left)) {
            z = next_no_skip_goto_skip(p->next, fun);

            p->visited = 0;
            return (z);
        }

        break;
    }

    case BP_GOTO: {
        z = next_no_skip_goto_skip(find_label(fun->stmt, p->e.a.label), fun);

        p->visited = 0;
        return (z);
    }
    case BP_PRINT:
    case BP_SKIP: {
        z = next_no_skip_goto_skip(p->next_stmt, fun);

        p->visited = 0;
        return (z);
    }
    }
    p->visited = 0;
    return (p);
}

static void recursive_eliminate_goto_print_skip(bp_stmt_t p, bp_fun_t fun) {
    int tmp;
    bp_stmt_t z;

    if (!p) return;

    switch (p->type) {
    case BP_IF:
    case BP_ELSEIF:
    case BP_WHILE: {
        p->next_then = next_no_skip_goto_skip(p->e.c.thenstmt, fun);
        recursive_eliminate_goto_print_skip(p->e.c.thenstmt, fun);

        p->next_else = next_no_skip_goto_skip(p->e.c.elsestmt, fun);
        recursive_eliminate_goto_print_skip(p->e.c.elsestmt, fun);

        break;
    }
    }

    if ((p->type) == BP_GOTO)
        p->next_stmt = next_no_skip_goto_skip(find_label(fun->stmt, p->e.a.label), fun);
    else
        p->next_stmt = next_no_skip_goto_skip(p->next_stmt, fun);

    recursive_eliminate_goto_print_skip(p->next, fun);
}

static void eliminate_goto_print_skip(void) {
    bp_fun_t p;

    for (p = bp_functions; p; p = p->next) {
        reset_flag_visited(p->stmt);
        recursive_eliminate_goto_print_skip(p->stmt, p);

        //remove the first statement in case it is a skip, goto ...

        //    /*
        if (((p->stmt->label) == NULL) || (strcmp(p->stmt->label, target_label) != 0))
            switch ((p->stmt)->type) {
            case BP_ASSUME: {
                if ((((p->stmt->e.c.expr)->token) == BP_CONST) && ((p->stmt->e.c.expr)->left))
                    p->stmt = p->stmt->next_stmt;
                break;
            }
            //         case BP_GOTO:
            case BP_PRINT:
            case BP_SKIP: {
                p->stmt = (p->stmt)->next_stmt;
                break;
            }
            }
        //    */
    }
}


/********************************************************************/
static void put_name_numb_fun_locals(void) {
    bp_fun_t p;
    bp_ident_t z, t;

    for (p = bp_functions; p; p = p->next) {
        for (z = p->locals; z; z = z->next) {
            t = Is_there_var(bp_all_locals, z->varname);
            if (t == NULL) common_error("error: local variable not found %s\n", z->varname);

            z->varname_replace = t->varname_replace;
        }
    }
}


/********************************************************************/
static void print_Iterative_CopyLocals(void) {
    bp_fun_t fun;
    bp_ident_t z, y;

    for (fun = bp_functions; fun; fun = fun->next) {
        printf(" |(m=%d", fun->funname_replace);
        for (z = fun->parms; z; z = z->next) {
            y = Is_there_var(bp_all_locals, z->varname);
            if (!y) common_error("the variable does not exist.\n");
            printf("\n&  d.v%d=c.v%d", y->varname_replace, y->varname_replace);
        }
        printf(")\n");
    }
}


static void print_CopyLocals(void) {

    printf("bool CopyLocals(\n");
    printf("Module m,\n");
    printf("Local c,\n");
    printf("Local d\n");
    printf(")\n");
    printf("m < c,\n");
    printf("c ~+ d\n");
    printf("(false\n");
    print_Iterative_CopyLocals();
    printf(");\n");

    //  printf("#size CopyLocals;\n");

}


/********************************************************************/

static void print_CopyGlobals(void) {
    bp_fun_t fun;
    bp_ident_t z, temp_var;


    printf("bool CopyGlobals(\n");
    printf("Module m,\n");
    printf("Global c,\n");
    printf("Global d\n");
    printf(")\n");
    printf("m < c,\n");
    printf("c ~+ d\n");
    printf("(false\n");


    for (fun = bp_functions; fun; fun = fun->next) {
        printf(" |(m=%d", fun->funname_replace);

        for (z = bp_globals; z; z = z->next) z->mark = -1;
        for (z = fun->globals; z; z = z->next) {
            temp_var = Is_there_var(bp_globals, z->varname);
            temp_var->mark = 0;
        }
        for (z = bp_globals; z; z = z->next)
            if (z->mark == 0) printf("\n  &d.v%d=c.v%d", z->varname_replace, z->varname_replace);
        printf(")\n");
    }


    printf(");\n");


    printf("#size CopyGlobals;\n");

}




/********************************************************************/

/********************************************************************/


static void print_Global_init() {

    bp_stmt_t p;
    bp_idref_t q;
    bp_ident_t z;
    bp_fun_t fun;


    printf("bool GlobalInit(Global CG)( true ");

    for (fun = bp_functions; fun; fun = fun->next) {
        if (
            (strlen(fun->funname) == 4) &&
            (fun->funname[0] == 'i') &&
            (fun->funname[1] == 'n') &&
            (fun->funname[2] == 'i') &&
            (fun->funname[3] == 't')
        ) {
            p = fun->stmt;
            if ((!p) ||/* (p->next!=NULL) ||*/ (p->type != BP_ASSIGN))
                common_error("error init \n");


            for (q = (p->e.a.asgnlist); q; q = q->next) {
                z = Is_there_var(bp_globals, q->var_name);
                if (!z) common_error("error init-only globals can be assigned\n");

                if (((q->expr)->token) == BP_CHOOSE)
                    common_error("error init choose is not allowed\n");
                else {
                    printf("&(CG.v%d", z->varname_replace);

                    if (q->expr->token == BP_CONST)
                        printf("=");
                    else
                        common_error("error init only constants can be assigned\n");

                    print_Mucke_expression(q->expr, "C", 0);
                    printf(")");
                }
            }
        }
    }
    printf(");\n\n");
    return;
}


static int convert_power(int pc) {
    int i = 1;

    while (pc >= i) {
        i = i * 2;
    }

    return (i * 2);

}

static int NumBits(int pc) {
    int i = 1, bit = 1;

    while (pc >= i) {
        i = i * 2;
        bit++;
    }

    return (bit);
}


static void print_PC_0() {
    int j;

    printf("bool initPC( PrCount pc)(/* pc=0 */\n");
    // printf("bool initPC( PrCount pc)(\n");
    printf("true\n");
    for (j = 1; j <= n_bit_pc; j++) printf("& !pc.b%d\n", j);

    printf(");\n\n");
    return;
}



/********************************************************************/
/********************************************************************/
/***                    VARIABLE ORDERING                        ****/
/********************************************************************/
/********************************************************************/


/********************************************************************/
/***           VARIABLE ORDERING for globals                     ****/
/********************************************************************/


/*****************************/
/*    for an expression      */
/*****************************/

static void recursive_add_edges_dependency_graph_EXPRESSIONGLOBALS(bp_expr_t p, int a, graph *g) {
    bp_ident_t z, q;

    if (!p) return;

    switch (p->token) {
    case BP_CONST: {
        return;
    }
    case BP_IDENT: {
        z = Is_there_var(bp_globals, p->var_name);
        if (z) addEdge(g, a, z->varname_replace);

        return;
    }
    case BP_NOT: {
        recursive_add_edges_dependency_graph_EXPRESSIONGLOBALS(p->left, a, g);
        return;
    }

    case BP_NE:
    case BP_EQ:
    case BP_AND:
    case BP_OR:
    case BP_IMP: {
        recursive_add_edges_dependency_graph_EXPRESSIONGLOBALS(p->left, a, g);
        recursive_add_edges_dependency_graph_EXPRESSIONGLOBALS(p->right, a, g);
        return;
    }
    }

}

/*************************************************/

static void recursive_add_edges_dependency_graph_RETURNSGLOBALS(bp_stmt_t p, bp_fun_t fun_call, bp_stmt_t p_return,
        bp_fun_t fun_return, graph *g) {
    bp_ident_t z;
    bp_idref_t assign_call, list_return_expr;

    if (!p) return;
    switch (p->type) {
    case BP_CALL: {

        if ((strcmp(p->e.a.label, fun_return->funname) == 0) && ((p->numlabel) >= 0)) {

            list_return_expr = p_return->e.a.asgnlist;


            for (assign_call = (p->e.a.asgnlist); assign_call; assign_call = assign_call->next) {

                z = Is_there_var(bp_globals, assign_call->var_name);

                if (z)
                    recursive_add_edges_dependency_graph_EXPRESSIONGLOBALS(list_return_expr->expr,
                            z->varname_replace, g);

                list_return_expr = list_return_expr->next;

            }

        }

        break;
    }

    case BP_IF:
    case BP_ELSEIF:
    case BP_WHILE: {
        recursive_add_edges_dependency_graph_RETURNSGLOBALS(p->e.c.thenstmt, fun_call, p_return, fun_return, g);
        recursive_add_edges_dependency_graph_RETURNSGLOBALS(p->e.c.elsestmt, fun_call, p_return, fun_return, g);
        break;
    }
    }

    recursive_add_edges_dependency_graph_RETURNSGLOBALS(p->next, fun_call, p_return, fun_return, g);

    return;
}


/***************************************************/
static void recursive_add_edges_dependency_graphGLOBALS(bp_stmt_t p, bp_fun_t fun, graph *g) {
    bp_idref_t q;
    bp_ident_t z = NULL, t, tmp;
    bp_fun_t y, fun_p;


    if ((!p) || (p->visited != 0)) return;

    p->visited = 1;

    switch (p->type) {
    case BP_ASSIGN: {
        for (q = (p->e.a.asgnlist); q; q = q->next) {
            z = Is_there_var(bp_globals, q->var_name);
            if (z) {
                if (((q->expr)->token) == BP_CHOOSE) {
                    recursive_add_edges_dependency_graph_EXPRESSIONGLOBALS(q->expr->left, z->varname_replace, g);
                    recursive_add_edges_dependency_graph_EXPRESSIONGLOBALS(q->expr->right, z->varname_replace, g);
                } else recursive_add_edges_dependency_graph_EXPRESSIONGLOBALS(q->expr, z->varname_replace, g);
            }
        }

        break;
    }


    case BP_RETURN: {

        for (fun_p = bp_functions; fun_p; fun_p = fun_p->next)
            recursive_add_edges_dependency_graph_RETURNSGLOBALS(fun_p->stmt, fun_p, p, fun, g);

        break;
    }


    case BP_IF:
    case BP_ELSEIF:
    case BP_WHILE: {   //recursive calls
        recursive_add_edges_dependency_graphGLOBALS(p->next_then, fun, g);
        recursive_add_edges_dependency_graphGLOBALS(p->next_else, fun, g);
        break;
    }

    }

    recursive_add_edges_dependency_graphGLOBALS(p->next_stmt, fun, g);
    return;
}

static void add_edges_dependency_graphGLOBALS(graph *g) {
    bp_fun_t p;

    for (p = bp_functions; p; p = p->next) {
        reset_flag_visited(p->stmt);
        recursive_add_edges_dependency_graphGLOBALS(p->stmt, p, g);
    }
    return;
}


/********************************************************************/

static void build_dependency_graphGLOBALS(graph *g) {
    bp_ident_t q;


    for (q = bp_globals; q; q = q->next)
        addNode(g, q->varname_replace);

    add_edges_dependency_graphGLOBALS(g);
    return;
}


/********************************************************************/
/***           VARIABLE ORDERING for locals                      ****/
/********************************************************************/






/*****************************/
/*    for an expression      */
/*****************************/

static void recursive_add_edges_dependency_graph_EXPRESSIONLOCALS(bp_expr_t p, int a, graph *g) {
    bp_ident_t z, q;


    if (!p) return;

    switch (p->token) {
    case BP_CONST: {
        return;
    }
    case BP_IDENT: {
        z = Is_there_var(bp_all_locals, p->var_name);
        if (z) addEdge(g, a, z->varname_replace);
        return;
    }
    case BP_NOT: {
        recursive_add_edges_dependency_graph_EXPRESSIONLOCALS(p->left, a, g);
        return;
    }

    case BP_NE:
    case BP_EQ:
    case BP_AND:
    case BP_OR:
    case BP_IMP: {
        recursive_add_edges_dependency_graph_EXPRESSIONLOCALS(p->left, a, g);
        recursive_add_edges_dependency_graph_EXPRESSIONLOCALS(p->right, a, g);
        return;
    }
    }

}

/*************************************************/
static void recursive_add_edges_dependency_graph_RETURNSLOCALS(bp_stmt_t p, bp_fun_t fun_call, bp_stmt_t p_return,
        bp_fun_t fun_return, graph *g) {
    bp_ident_t z;
    bp_idref_t assign_call, list_return_expr;


    if (!p) return;
    switch (p->type) {
    case BP_CALL: {
        if ((strcmp(p->e.a.label, fun_return->funname) == 0) && ((p->numlabel) >= 0)) {
            list_return_expr = p_return->e.a.asgnlist;
            for (assign_call = (p->e.a.asgnlist); assign_call; assign_call = assign_call->next) {
                z = Is_there_var(bp_all_locals, assign_call->var_name);

                if (z)
                    recursive_add_edges_dependency_graph_EXPRESSIONLOCALS(list_return_expr->expr,
                            z->varname_replace, g);

                list_return_expr = list_return_expr->next;
            }
        }
        break;
    }

    case BP_IF:
    case BP_ELSEIF:
    case BP_WHILE: {
        recursive_add_edges_dependency_graph_RETURNSLOCALS(p->e.c.thenstmt, fun_call, p_return, fun_return, g);
        recursive_add_edges_dependency_graph_RETURNSLOCALS(p->e.c.elsestmt, fun_call, p_return, fun_return, g);
        break;
    }
    }

    recursive_add_edges_dependency_graph_RETURNSLOCALS(p->next, fun_call, p_return, fun_return, g);

    return;
}


/***************************************************/
static void recursive_add_edges_dependency_graphLOCALS(bp_stmt_t p, bp_fun_t fun, graph *g) {
    bp_idref_t q;
    bp_ident_t z = NULL, t, tmp;
    bp_fun_t y, fun_p;


    if ((!p) || (p->visited != 0)) return;

    p->visited = 1;

    switch (p->type) {
    case BP_ASSIGN: {
        for (q = (p->e.a.asgnlist); q; q = q->next) {
            z = Is_there_var(fun->locals, q->var_name);
            if (z) {
                if (((q->expr)->token) == BP_CHOOSE) {
                    recursive_add_edges_dependency_graph_EXPRESSIONLOCALS(q->expr->left, z->varname_replace, g);
                    recursive_add_edges_dependency_graph_EXPRESSIONLOCALS(q->expr->right, z->varname_replace, g);
                } else recursive_add_edges_dependency_graph_EXPRESSIONLOCALS(q->expr, z->varname_replace, g);
            }
        }
        break;
    }

    case BP_CALL: {

        y = find_function(p->e.a.label);

        z = y->parms;

        for (q = (p->e.a.fmllist); q; q = q->next) {
            tmp = Is_there_var(bp_all_locals, z->varname);
            if (((q->expr)->token) == BP_CHOOSE) {
                recursive_add_edges_dependency_graph_EXPRESSIONLOCALS(q->expr->left, tmp->varname_replace, g);
                recursive_add_edges_dependency_graph_EXPRESSIONLOCALS(q->expr->right, tmp->varname_replace, g);
            } else recursive_add_edges_dependency_graph_EXPRESSIONLOCALS(q->expr, tmp->varname_replace, g);

            z = z->next;
        }

        break;
    }

    case BP_RETURN: {
        for (fun_p = bp_functions; fun_p; fun_p = fun_p->next) {
            recursive_add_edges_dependency_graph_RETURNSLOCALS(fun_p->stmt, fun_p, p, fun, g);
        }
        break;
    }

    case BP_IF:
    case BP_ELSEIF:
    case BP_WHILE: {   //recursive calls
        recursive_add_edges_dependency_graphLOCALS(p->next_then, fun, g);
        recursive_add_edges_dependency_graphLOCALS(p->next_else, fun, g);
        break;
    }
    }

    recursive_add_edges_dependency_graphLOCALS(p->next_stmt, fun, g);
    return;
}

static void add_edges_dependency_graphLOCALS(graph *g) {
    bp_fun_t p;

    for (p = bp_functions; p; p = p->next) {
        reset_flag_visited(p->stmt);
        recursive_add_edges_dependency_graphLOCALS(p->stmt, p, g);
    }

    return;
}

/********************************************************************/

static void build_dependency_graphLOCALS(graph *g) {
    bp_ident_t q;

    for (q = bp_all_locals; q; q = q->next)
        addNode(g, q->varname_replace);

    add_edges_dependency_graphLOCALS(g);
    return;
}


static void print_Variables(int ord) {
    graph *g;
    linkedList *orderedVertexList;
    item *curr, *prev, *first;
    bp_ident_t q;
    bp_fun_t p;
    int i;

    ord = 0;

    printf("class Global {");


    if (bp_globals && (ord != -1)) {

        g = newGraph();

        build_dependency_graphGLOBALS(g);
        orderedVertexList = variableOrdering(g, ord);

        first = curr = orderedVertexList->head;
        while (curr != NULL) {
            if (curr != first) printf(";\n");
            else printf("\n");
            printf("bool v%d", curr->vert->val);

            curr = curr->next;
        }

        printf(";");

    } else printf(" bool fake; ");


    printf("};\n\n");

    printf("class Local {");

    if (bp_all_locals && (ord != -1)) {

        g = newGraph();

        build_dependency_graphLOCALS(g);

        orderedVertexList = variableOrdering(g, ord);

        first = curr = orderedVertexList->head;
        while (curr != NULL) {
            if (curr != first) printf(";\n");
            else printf("\n");
            printf("bool v%d", curr->vert->val);

            curr = curr->next;
        }

        printf(";");

    } else printf(" bool fake; ");

    printf("};\n\n");

}



/********************************************************************/
/********************************************************************/
/***                    ACTIVE GLOBALS FOR EACH MODULE           ****/
/********************************************************************/
/********************************************************************/


/***********************************************************************/
static void discover_called_module_in_module(bp_fun_t fun, char *name) {

    bp_fun_t calledfun;
    int found;

    found = 0;

    for (calledfun = fun->calledmodule; calledfun; calledfun = calledfun->next)
        if (strcmp(calledfun->funname, name) == 0) found = 1;

    if (found == 0) {
        calledfun = malloc(sizeof(struct bp_fun_element));
        calledfun->funname = name;
        calledfun->next = fun->calledmodule;
        fun->calledmodule = calledfun;
    }

    return;
}


static void discover_globals_in_module_in_assign_id_list(bp_idref_t p, bp_fun_t fun) {
    bp_ident_t y;

    for (; p; p = p->next) {
        y = Is_there_var(bp_globals, p->var_name);
        if (y) {
            y = Is_there_var(fun->globals, p->var_name);
            if (!y) {
                bp_decls = fun->globals;
                Insert_var(p->var_name);
                fun->globals = bp_decls;
            }
        }
    }
}


static void discover_globals_in_module_in_expression(bp_expr_t p, bp_fun_t fun) {
    bp_ident_t y, q;

    if (!p) return;

    switch (p->token) {
    case BP_CONST: {
        return;
    }
    case BP_IDENT: {
        y = Is_there_var(bp_globals, p->var_name);
        if (y) {
            y = Is_there_var(fun->globals, p->var_name);
            if (!y) {
                bp_decls = fun->globals;
                Insert_var(p->var_name);
                fun->globals = bp_decls;
            }
        }
        return;
    }
    }

    discover_globals_in_module_in_expression(p->left, fun);
    discover_globals_in_module_in_expression(p->right, fun);
    return;
}


static void discover_globals_in_module_in_decider(bp_expr_t p, bp_fun_t fun) {
    if (p->token != BP_ND)
        discover_globals_in_module_in_expression(p, fun);
}


static void discover_globals_in_module_in_expression_list(bp_idref_t p, bp_fun_t fun) {
    for (; p; p = p->next)
        discover_globals_in_module_in_expression(p->expr, fun);
}


static void discover_globals_in_module_in_statements(bp_stmt_t p, bp_fun_t fun) {
    if ((!p) || (p->visited != 0)) return;

    p->visited = 1;

    switch (p->type) {
    case BP_ASSIGN: {
        discover_globals_in_module_in_assign_id_list(p->e.a.asgnlist, fun);
        discover_globals_in_module_in_expression_list(p->e.a.asgnlist, fun);
        break;
    }
    case BP_ASSERT:
    case BP_CONSTRAIN:
    case BP_ASSUME: {
        discover_globals_in_module_in_decider(p->e.c.expr, fun);
        break;
    }

    case BP_CALL: {
        discover_called_module_in_module(fun, p->e.a.label);
        discover_globals_in_module_in_assign_id_list(p->e.a.asgnlist, fun);
        discover_globals_in_module_in_expression_list(p->e.a.fmllist, fun);
        break;
    }
    case BP_IF:
    case BP_ELSEIF:
    case BP_WHILE: {
        discover_globals_in_module_in_decider(p->e.c.expr, fun);
        discover_globals_in_module_in_statements(p->next_then, fun);
        discover_globals_in_module_in_statements(p->next_else, fun);
        break;
    }
    case BP_RETURN: {
        discover_globals_in_module_in_expression_list(p->e.a.asgnlist, fun);
        break;
    }
    }
    discover_globals_in_module_in_statements(p->next_stmt, fun);
}


// for each module set the field globals with all globals used in the module
//             and set the field calledmodule with all modules called in the module
static void discover_globals_in_module(void) {
    bp_fun_t fun;

    for (fun = bp_functions; fun; fun = fun->next) {
        reset_flag_visited(fun->stmt);
        fun->globals = NULL;
        discover_globals_in_module_in_expression(fun->enforce, fun);
        fun->calledmodule = NULL;
        discover_globals_in_module_in_statements(fun->stmt, fun);
    }
}


//insert module name in fun->calledmodule in case it is not in that list
static int insert_module_in_module_list(bp_fun_t fun, char *name) {

    bp_fun_t funtemp;

    if (find_module(fun->calledmodule, name) == NULL) {
        funtemp = malloc(sizeof(struct bp_fun_element));
        funtemp->funname = name;
        funtemp->next = fun->calledmodule;
        fun->calledmodule = funtemp;
        return 1;
    }
    return 0;
}

static void clousure_calledmodule(void) {
    bp_fun_t calledfun, calledfun2, fun, funtemp;
    int ris;

    ris = 1;

    while (ris) {
        ris = 0;
        for (fun = bp_functions; fun; fun = fun->next) {
            for (calledfun = fun->calledmodule; calledfun; calledfun = calledfun->next) {
                funtemp = find_module(bp_functions, calledfun->funname);
                if (!funtemp) common_error("the function does not exist.\n");
                for (calledfun2 = funtemp->calledmodule; calledfun2; calledfun2 = calledfun2->next)
                    if (insert_module_in_module_list(fun, calledfun2->funname)) ris = 1;
            }
        }
    }

    return;
}

static void clousure_globals(void) {
    bp_fun_t calledfun, fun, funtemp;
    bp_ident_t y, funglobals;

    discover_globals_in_module();

    //add globals in enforse to the module

    for (fun = bp_functions; fun; fun = fun->next) {
        discover_globals_in_module_in_expression(fun->enforce, fun);
    }


    clousure_calledmodule();

    for (fun = bp_functions; fun; fun = fun->next) {
        for (calledfun = fun->calledmodule; calledfun; calledfun = calledfun->next) {
            funtemp = find_module(bp_functions, calledfun->funname);
            for (funglobals = funtemp->globals; funglobals; funglobals = funglobals->next) {
                y = Is_there_var(fun->globals, funglobals->varname);
                if (!y) {
                    bp_decls = fun->globals;
                    Insert_var(funglobals->varname);
                    fun->globals = bp_decls;
                }
            }
        }
    }

}


/****************************************************************************************/

static void recursive_fun_reachabilty(bp_stmt_t p, bp_fun_t fun) {
    bp_fun_t y;

    if ((!p) || (p->visited != 0)) return;
    p->visited = 1;

    switch (p->type) {
    case BP_CALL: {

        y = find_function(p->e.a.label);
        y->reachable = 1;
        recursive_fun_reachabilty(y->stmt, y);

        break;
    }

    case BP_IF:
    case BP_ELSEIF:
    case BP_WHILE: {
        recursive_fun_reachabilty(p->e.c.thenstmt, fun);
        recursive_fun_reachabilty(p->e.c.elsestmt, fun);
        break;
    }
    }

    recursive_fun_reachabilty(p->next, fun);
    return;
}

static bp_fun_t remove_fun_unreachable(bp_fun_t fun) {
    bp_fun_t fun_temp;

    if (fun == NULL) return NULL;

    fun_temp = remove_fun_unreachable(fun->next);
    if (fun->reachable == 1) {
        fun->next = fun_temp;
        return fun;
    } else return fun_temp;
}

static void fun_reachabilty(void) {
    bp_fun_t fun;

    for (fun = bp_functions; fun; fun = fun->next)
        reset_flag_visited(fun->stmt);


    fun = find_function("main");
    fun->reachable = 1;

    recursive_fun_reachabilty(fun->stmt, fun);
    bp_functions = remove_fun_unreachable(bp_functions);

}



/***********************************************************************/


/*''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''*/
/*''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''*/
/*''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''*/
/*''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''*/

static void recursive_set_useful_function_per_single_function(bp_stmt_t p, bp_fun_t fun, int *answ) {
    bp_fun_t fun_call;
    bp_stmt_t temp;


    if (!p) return;

    if (((p->label) != NULL) && (strcmp(p->label, target_label) == 0)) {
        if ((fun->useful) == 0) *answ = 1;
        fun->useful = 1;
    }
    switch (p->type) {
    case BP_IF:
    case BP_ELSEIF: {
        recursive_set_useful_function_per_single_function(p->e.c.thenstmt, fun, answ);
        recursive_set_useful_function_per_single_function(p->e.c.elsestmt, fun, answ);
        break;
    }

    case BP_WHILE: {
        recursive_set_useful_function_per_single_function(p->e.c.thenstmt, fun, answ);
        break;
    }

    case BP_CALL: {
        fun_call = find_module(bp_functions, p->e.a.label);
        if (!fun_call) common_error("function name not found\n");


        if (((fun_call->useful) == 1)// || (fun_call->enforce)
           ) {
            if ((fun->useful) == 0) *answ = 1;
            fun->useful = 1;
        }

        break;
    }

    case BP_RETURN: {
        if ((fun->num_returns) != 0) {
            if ((fun->useful) == 0) *answ = 1;
            fun->useful = 1;
        }
        break;
    }

    default: {
        if ((fun->useful) == 0) *answ = 1;
        fun->useful = 1;
        break;
    }

    }

    recursive_set_useful_function_per_single_function(p->next, fun, answ);
}


/***********************************************************************/
static void recursive_remove_unuseful_calls(bp_stmt_t p) {
    bp_fun_t fun_call;
    bp_stmt_t temp;


    if (!p) return;

    switch (p->type) {
    case BP_IF:
    case BP_ELSEIF: {
        recursive_remove_unuseful_calls(p->e.c.thenstmt);
        recursive_remove_unuseful_calls(p->e.c.elsestmt);
        break;
    }

    case BP_WHILE: {
        recursive_remove_unuseful_calls(p->e.c.thenstmt);
        break;
    }

    case BP_CALL: {
        fun_call = find_module(bp_functions, p->e.a.label);
        if (!fun_call) common_error("function name not found\n");
        if ((fun_call->useful) == 0) {
            if (fun_call->enforce) {
                p->type = BP_ASSUME;
                p->e.c.expr = fun_call->enforce;
            } else p->type = BP_SKIP;
        }
        break;
    }

    }

    recursive_remove_unuseful_calls(p->next);
}


static bp_fun_t remove_unsuful_function_from_list(bp_fun_t fun) {
    if (fun) return (fun);

    if (fun->useful == 1) {
        fun->next = remove_unsuful_function_from_list(fun->next);
        return (fun);
    }

    return (remove_unsuful_function_from_list(fun->next));
}


static void recursive_set_useful_function(void) {
    bp_fun_t fun;
    int answ;

    answ = 1;
    while (answ == 1) {
        answ = 0;
        for (fun = bp_functions; fun; fun = fun->next)
            recursive_set_useful_function_per_single_function(fun->stmt, fun, &answ);
    }

    for (fun = bp_functions; fun; fun = fun->next)
        recursive_remove_unuseful_calls(fun->stmt);


    bp_functions = remove_unsuful_function_from_list(bp_functions);

}

/********************************************************************/

// ####   ####    ###   #   #  #####         ####    ###
// #   #  #   #    #    #   #    #           #   #  #   #
// #   #  #   #    #    ##  #    #           #   #  #
// ####   ####     #    # # #    #           ####   #
// #      # #      #    #  ##    #           #      #
// #      #  #     #    #   #    #           #      #   #
// #      #   #   ###   #   #    #           #       ###

/********************************************************************/
static void print_expression(bp_expr_t p, FILE *debugfile) {
    bp_ident_t y, q;

    if (!p) return;


    switch (p->token) {
    case BP_CHOOSE: {
        fprintf(debugfile, "schoose[");
        print_expression(p->left, debugfile);
        fprintf(debugfile, ",");
        print_expression(p->right, debugfile);
        fprintf(debugfile, "]");
        return;
    }
    case BP_ND: {
        fprintf(debugfile, "*");
        return;
    }
    case BP_CONST: {
        if (p->left)
            fprintf(debugfile, "T");
        else
            fprintf(debugfile, "F");
        return;
    }
    case BP_IDENT: {
        if (p->primed == 1) fprintf(debugfile, "'");
        fprintf(debugfile, "%s", p->var_name);
        return;
    }
    case BP_NOT: {
        fprintf(debugfile, "(!");
        print_expression(p->left, debugfile);
        fprintf(debugfile, ")");
        return;
    }
    case BP_EQ: {
        fprintf(debugfile, "(");
        print_expression(p->left, debugfile);
        fprintf(debugfile, "=");
        print_expression(p->right, debugfile);
        fprintf(debugfile, ")");
        return;
    }
    case BP_NE: {
        fprintf(debugfile, "((");
        print_expression(p->left, debugfile);
        fprintf(debugfile, "!=");
        print_expression(p->right, debugfile);
        fprintf(debugfile, "))");
        return;
    }
    case BP_AND: {
        fprintf(debugfile, "(");
        print_expression(p->left, debugfile);
        fprintf(debugfile, "&");
        print_expression(p->right, debugfile);
        fprintf(debugfile, ")");
        return;
    }
    case BP_OR: {
        fprintf(debugfile, "(");
        print_expression(p->left, debugfile);
        fprintf(debugfile, "|");
        print_expression(p->right, debugfile);
        fprintf(debugfile, ")");
        return;
    }
    case BP_IMP: {
        fprintf(debugfile, "(");
        print_expression(p->left, debugfile);
        fprintf(debugfile, "=>");
        print_expression(p->right, debugfile);
        fprintf(debugfile, ")");
        return;
    }

    }

}


static void print_indent_statement(int indent, FILE *debugfile) {
    int i;

    for (i = 0; i < indent * 3; i++) fprintf(debugfile, " ");
}


/******************************************************************/
static void print_recursive_statement(bp_fun_t fun, bp_stmt_t p, int indent, FILE *debugfile) {
    bp_ident_t y;
    bp_idref_t q, list_return_expr;
    bp_ident_t z = NULL, temp_var;
    int i, tmp;


    if (!p) return;

    print_indent_statement(indent, debugfile);
    if (p->numlabel >= 0) fprintf(debugfile, "/* pc%d */", p->numlabel);
    if (p->label) fprintf(debugfile, "%s: ", p->label);

    switch (p->type) {
    case BP_ASSIGN: {
        for (q = (p->e.a.asgnlist); q; q = q->next) {
            if (q != p->e.a.asgnlist) fprintf(debugfile, ", ");
            fprintf(debugfile, "%s", q->var_name);
        }

        fprintf(debugfile, " := ");

        for (q = (p->e.a.asgnlist); q; q = q->next) {
            if (q != p->e.a.asgnlist) fprintf(debugfile, ", ");
            print_expression(q->expr, debugfile);
        }

        fprintf(debugfile, ";\n");

        break;
    }

    case BP_ASSERT: {
        fprintf(debugfile, "assert(");
        print_expression(p->e.c.expr, debugfile);
        fprintf(debugfile, ");\n");

        break;
    }

    case BP_ASSUME: {
        fprintf(debugfile, "assume(");
        print_expression(p->e.c.expr, debugfile);
        fprintf(debugfile, ");\n");

        break;
    }
    case BP_CONSTRAIN: {
        fprintf(debugfile, "constrain(");
        print_expression(p->e.c.expr, debugfile);
        fprintf(debugfile, ");\n");

        break;
    }

    case BP_SKIP: {
        if (p->next_stmt) {
            fprintf(debugfile, "skip /*next_pc=%d*/;\n", (p->next_stmt)->numlabel);
        } else {
            fprintf(debugfile, "skip /*next_pc=%d*/;\n", pc_return);
        }
        break;
    }

    case BP_GOTO: {
        if (find_label(fun->stmt, p->e.a.label))
            fprintf(debugfile, "goto %s;\n", p->e.a.label);
        else
            fprintf(debugfile, "assume(0);\n");
        break;
    }

    case BP_RETURN: {
        fprintf(debugfile, "return ");

        for (q = (p->e.a.asgnlist); q; q = q->next) {
            if (q != p->e.a.asgnlist) fprintf(debugfile, ", ");
            print_expression(q->expr, debugfile);

        }
        fprintf(debugfile, ";\n");

        break;
    }

    case BP_PRINT: {
        fprintf(debugfile, "print(");

        for (q = (p->e.a.asgnlist); q; q = q->next) {
            if (q != p->e.a.asgnlist) fprintf(debugfile, ", ");
            print_expression(q->expr, debugfile);

        }
        fprintf(debugfile, ");\n");

        break;
    }


    case BP_CALL: {

        if (p->e.a.asgnlist) {
            for (q = (p->e.a.asgnlist); q; q = q->next) {
                if (q != p->e.a.asgnlist) fprintf(debugfile, ", ");
                fprintf(debugfile, "%s", q->var_name);
            }
            fprintf(debugfile, " := ");
        }

        fprintf(debugfile, "%s(", p->e.a.label);

        for (q = (p->e.a.fmllist); q; q = q->next) {
            if (q != p->e.a.fmllist) fprintf(debugfile, ", ");
            print_expression(q->expr, debugfile);
        }

        fprintf(debugfile, ");\n");


        break;
    }


    case BP_IF:
    case BP_ELSEIF: {

        if (p->type == BP_IF) fprintf(debugfile, "if (");
        else fprintf(debugfile, "elsif (");

        print_expression(p->e.c.expr, debugfile);

        fprintf(debugfile, ") then\n");
        print_recursive_statement(fun, p->e.c.thenstmt, indent + 1, debugfile);

        if (p->e.c.elsestmt) {
            if (p->e.c.elsestmt->type != BP_ELSEIF) {
                print_indent_statement(indent, debugfile);
                fprintf(debugfile, "else\n");
                print_recursive_statement(fun, p->e.c.elsestmt, indent + 1, debugfile);
            } else print_recursive_statement(fun, p->e.c.elsestmt, indent, debugfile);
        }
        if (p->type == BP_IF) {
            print_indent_statement(indent, debugfile);
            fprintf(debugfile, "fi\n");
        }
        break;
    }

    case BP_WHILE: {
        fprintf(debugfile, "while (");
        print_expression(p->e.c.expr, debugfile);
        fprintf(debugfile, ") do\n");
        print_recursive_statement(fun, p->e.c.thenstmt, indent + 1, debugfile);
        fprintf(debugfile, "od\n");
        break;
    }

    }

    print_recursive_statement(fun, p->next, indent, debugfile);
    return;
}

static void
print_debug_program(void) {

    int totallocals, processedlocals, max_global_var;

    FILE *debugfile;
    bp_fun_t p, fun;
    bp_ident_t q;
    int i, j, max, mi, mj, mk;
    bp_ident_t z, zprime, z3;
    bp_stmt_t target_stmt;

    //print the declaration of global variables

    debugfile = fopen("debug_program_withpc.bp", "w");

    if (bp_globals) {
        fprintf(debugfile, "decl ");
        for (z = bp_globals; z; z = z->next) {
            if (z != bp_globals) fprintf(debugfile, ", ");
            fprintf(debugfile, "%s", z->varname);
        }
        fprintf(debugfile, "; // global variables\n\n\n");
    }


    //print the functions

    for (fun = bp_functions; fun; fun = fun->next) {
        switch (fun->num_returns) {
        case 0: {
            fprintf(debugfile, "void ");
            break;
        }
        case 1: {
            fprintf(debugfile, "bool ");
            break;
        }
        default: {
            fprintf(debugfile, "bool<%d> ", fun->num_returns);
            break;
        }
        }

        fprintf(debugfile, "%s (", fun->funname);

        // Function parameter
        for (z = fun->parms; z; z = z->next) {
            if (z != fun->parms) fprintf(debugfile, ", ");
            fprintf(debugfile, "%s", z->varname);
        }

        fprintf(debugfile, ")\n");
        fprintf(debugfile, "begin\n");

        // Locals of functions
        if (fun->locals) {
            fprintf(debugfile, "decl ");
            for (z = fun->locals; z; z = z->next) {
                if (z != fun->locals) fprintf(debugfile, ", ");
                fprintf(debugfile, "%s", z->varname);
            }
            fprintf(debugfile, ";\n\n");
        }

        if (fun->enforce) {
            fprintf(debugfile, "enforce ");
            print_expression(fun->enforce, debugfile);
            fprintf(debugfile, ";\n");

        }

        print_recursive_statement(fun, fun->stmt, 1, debugfile);

        fprintf(debugfile, "end\n\n\n");

    }

    fclose(debugfile);
}


/******************************************************************/
/*                      PRINT ENFORCE FORMULA                     */
/******************************************************************/

static void print_enforce(void) {
    bp_fun_t fun;

    printf("bool enforce(\n");
    printf(" Module  m,\n");
    printf(" Local   L,\n");
    printf(" Global  G \n");
    printf(")          \n");
    printf(" m <  L,   \n");
    printf(" L   <  G  \n");
    printf("(false    \n");

    for (fun = bp_functions; fun; fun = fun->next) {
        printf("  | ( m=%d ", fun->funname_replace);
        if (fun->enforce) {
            printf(" & ( ");
            print_Mucke_expression(fun->enforce, "", 0);
            printf(" )");
        }
        printf(" )\n");
    }
    printf("\n);\n\n");
    return;
}

/////////////////////////////////////////////////////////////////////////////////////////
static int
IsGlobalVariableName(char *varname) {
    bp_ident_t v;
    if (bp_globals) {
        for (v = bp_globals; v; v = v->next) {
            if (strcmp(varname, v->varname) == 0) {
                return 1;
            }
        }
        return 0;
    }
    else {
        return 0;
    }
}


static int
IsGlobalExpr(bp_expr_t p) {
    if (!p) return 0;

    switch (p->token) {
    case BP_CHOOSE: {
        return (IsGlobalExpr(p->left) || IsGlobalExpr(p->right));
    }
    case BP_ND:
    case BP_CONST: {
        return 0;
    }
    case BP_IDENT: {
        return IsGlobalVariableName(p->var_name);
    }
    case BP_NOT: {
        return IsGlobalExpr(p->left);
    }
    case BP_EQ:
    case BP_NE:
    case BP_AND:
    case BP_OR:
    case BP_IMP: {
        return (IsGlobalExpr(p->left) || IsGlobalExpr(p->right));
    }
    }
    return 0;
}


int isInAtomicRegion;
int cs_counter;

static void
insert_CS_into_statement_recursive(bp_stmt_t p) {
    int tmp;
    int isAtomicStmt;
    int hasGlobalVar;
    bp_idref_t q;
    bp_fun_t callee;

    if (!p) return;

    switch (p->type) {
    case BP_ASSIGN: {
        // Identify if this is atomic, at := T, T
        tmp = 0;
        isAtomicStmt = 0;
        hasGlobalVar = 0;

        for (q = (p->e.a.asgnlist); q; q = q->next) {
            if (strcmp(q->var_name, "atomic") == 0 && isAtomicStmt == 0) {
                ++isAtomicStmt;
            }
            // else if (strcmp(q->var_name, "at") == 0 && isAtomicStmt == 1)
            // {
            //     ++isAtomicStmt;
            // }
            else {
                if (!hasGlobalVar) {
                    hasGlobalVar = IsGlobalVariableName(q->var_name);
                }
            }
            ++tmp;
        }


        if (tmp == isAtomicStmt && tmp == 1) {
            isAtomicStmt = 1;
        }
        else {
            isAtomicStmt = 0;
        }

        tmp = 0;

        if (isAtomicStmt) {
            if (p->e.a.asgnlist->expr->token == BP_CONST && p->e.a.asgnlist->expr->left) {
                isInAtomicRegion = 1;
            }
            else {
                isInAtomicRegion = 0;
            }
        }
        else if (!hasGlobalVar) {
            for (q = (p->e.a.asgnlist); q; q = q->next) {
                if (!hasGlobalVar) {
                    hasGlobalVar = IsGlobalExpr(q->expr);
                }
            }
        }

        // Set cs_counter
        if (isAtomicStmt) {
            if (isInAtomicRegion) {
                ++cs_counter;
                p->cs = cs_counter;
            }
            else {
                p->cs = -1;
            }
        }
        else if (isInAtomicRegion) {
            p->cs = -1;
        }
        else if (hasGlobalVar) {
            ++cs_counter;
            p->cs = cs_counter;
        }
        else {
            p->cs = -1;
        }

        break;
    }

    case BP_CALL: {
        callee = find_function(p->e.a.label);

        hasGlobalVar = 0;
        if (p->e.a.asgnlist) {
            for (q = (p->e.a.asgnlist); q; q = q->next) {
                if (!hasGlobalVar) {
                    hasGlobalVar = IsGlobalVariableName(q->var_name);
                }
            }
        }

        if (!hasGlobalVar) {
            for (q = (p->e.a.fmllist); q; q = q->next) {
                if (!hasGlobalVar) {
                    hasGlobalVar = IsGlobalExpr(q->expr);
                }
            }
        }

        if (isInAtomicRegion) {
            p->cs = -1;
        }
        else if (hasGlobalVar
                 || strstr(callee->funname, "__VERIFIER_atomic_")) {
            ++cs_counter;
            p->cs = cs_counter;
        }
        else {
            p->cs = -1;
        }
        break;
    }
    case BP_ASSERT:
    case BP_ASSUME:
    case BP_CONSTRAIN: {
        hasGlobalVar = IsGlobalExpr(p->e.c.expr);
        if (isInAtomicRegion) {
            p->cs = -1;
        }
        else if (hasGlobalVar) {
            ++cs_counter;
            p->cs = cs_counter;
        }
        else {
            p->cs = -1;
        }
        break;
    }

    case BP_PRINT:
    case BP_SKIP:
    case BP_GOTO:
    case BP_RETURN: {
        p->cs = -1;
        break;
    }

    case BP_IF:
    case BP_ELSEIF: {
        hasGlobalVar = IsGlobalExpr(p->e.c.expr);
        if (isInAtomicRegion) {
            p->cs = -1;
        }
        else if (hasGlobalVar) {
            ++cs_counter;
            p->cs = cs_counter;
        }
        else {
            p->cs = -1;
        }
        insert_CS_into_statement_recursive(p->e.c.thenstmt);

        if (p->e.c.elsestmt) {
            insert_CS_into_statement_recursive(p->e.c.elsestmt);
        }
        break;
    }

    case BP_WHILE: {
        hasGlobalVar = IsGlobalExpr(p->e.c.expr);
        if (isInAtomicRegion) {
            p->cs = -1;
        }
        else if (hasGlobalVar) {
            ++cs_counter;
            p->cs = cs_counter;
        }
        else {
            p->cs = -1;
        }
        insert_CS_into_statement_recursive(p->e.c.thenstmt);
        break;
    }

    case BP_ATOMIC_BEGIN: {
        break;
    }

    case BP_ATOMIC_END: {
        break;
    }
    }

    insert_CS_into_statement_recursive(p->next);
}


static void
insert_CS_into_module(bp_fun_t f) {
    isInAtomicRegion = 0;    // not in any atomic region
    cs_counter = 0;
    insert_CS_into_statement_recursive(f->stmt);
    f->last_cs = ++cs_counter;
}

/////////////////////////////////////////////////////////////////////////////////////////
static void
print_Mucke_function(bp_fun_t fun) {
    int i;
    printf("mu bool %s(\n", fun->funname);
    // Declaration of variables
    printf("   Module  s_mod,\n");
    printf("   PrCount s_pc,\n");
    printf("   Local   s_CL,\n");
    printf("   Global  s_G,\n");
    printf("   Local   s_ENTRY_CL,\n");
    printf("   Global  s_ENTRY_CG\n");
    printf(")\n");
    // Ordering of variables
    printf("  s_mod   <  s_pc,\n");
    printf("  s_pc    <  s_CL,\n");
    printf("  s_CL   ~+  s_ENTRY_CL,\n");
    printf("  s_CL    <  s_G,\n");
    printf("  s_G    ~+  s_ENTRY_CG\n");
    printf("(false\n");
    printf("|(\n");
    printf("     initPC(s_pc)\n");
    printf("   & s_mod=%d\n", fun->funname_replace);
    // printf("   & CopyLocals(s_mod, s_ENTRY_CL, s_CL)\n");
    printf("   & s_ENTRY_CL = s_CL\n");
    // printf("   & CopyGlobals(s_mod, s_ENTRY_CG, s_G)\n");
    printf("   & s_ENTRY_CG = s_G\n");
    printf(" )\n\n");

    printf("|(\n");
    printf("     s_mod=%d\n", fun->funname_replace);
    printf("   & (exists \n");
    printf("         PrCount  t_pc,\n");
    printf("         Local    t_CL,\n");
    printf("         Global   t_G.\n");
    printf("         (\n");
    printf("              %s(%d, t_pc, t_CL, t_G, s_ENTRY_CL, s_ENTRY_CG)\n", fun->funname, fun->funname_replace);
    printf("            & (\n");
    printf("                (\n");
    printf("                     programInt1nonthread( %d, t_pc, s_pc, t_CL, s_CL, t_G, s_G )\n", fun->funname_replace);
    printf("                   & CopyVariables_ProgramInt( %d, t_pc, t_CL, s_CL, t_G, s_G )\n", fun->funname_replace);
    printf("                )\n");
    printf("                | programInt3nonthread(%d, t_pc, s_pc, t_CL, s_CL, t_G, s_G )\n", fun->funname_replace);
    printf("              )\n");
    printf("         )\n");
    printf("      )\n");
    printf(" )\n\n");

    printf("|(\n");
    printf("     s_mod=%d\n", fun->funname_replace);
    printf("   & (exists \n");
    printf("         PrCount  t_pc.\n");
    printf("         (\n");
    printf("              %s(%d, t_pc, s_CL, s_G, s_ENTRY_CL, s_ENTRY_CG)\n", fun->funname, fun->funname_replace);
    printf("            & programInt2nonthread( %d, t_pc, s_pc, s_CL, s_G)\n", fun->funname_replace);
    printf("         )\n");
    printf("      )\n");
    printf(" )\n\n");

    printf(");\n");
    printf("#size %s;\n", fun->funname);
    printf("\n\n");
}

static void
print_Mucke_functions(void) {
    bp_fun_t p;

    printf("\n\n");
    for (p = bp_functions; p; p = p->next) {
        if (!isThreadFunction(p)) {
            print_Mucke_function(p);
        }
    }

}
/////////////////////////////////////////////////////////////////////////////////////////
static void
print_Mucke_forward_decls(void)
{
    printf("//FORWARD DECLARATIONS\n");
    // // print_Mucke_programInt1();
    // printf("bool programInt1(\n");
    // printf(" Module  cm,\n");
    // printf(" PrCount cp,\n");
    // printf(" PrCount dp,\n");
    // printf(" Local   L,\n");
    // printf(" Local   dL,\n");
    // printf(" Global  G,\n");
    // printf(" Global  dG\n");
    // printf(");\n");
    // // print_Mucke_programInt2();
    // printf("bool programInt2(\n");
    // printf(" Module  cm,\n");
    // printf(" PrCount cp,\n");
    // printf(" PrCount dp,\n");
    // printf(" Local   L, \n");
    // printf(" Global  G  \n");
    // printf(");\n");
    // // print_Mucke_programInt3();
    // printf("bool programInt3(\n");
    // printf(" Module  cm,\n");
    // printf(" PrCount cp,\n");
    // printf(" PrCount dp,\n");
    // printf(" Local   L,\n");
    // printf(" Local   dL,\n");
    // printf(" Global  G,\n");
    // printf(" Global  dG\n");
    // printf(");\n");
    // CopyVariables_ProgramInt();
    // printf("bool CopyVariables_ProgramInt(\n");
    // printf(" Module  m,\n");
    // printf(" PrCount p,\n");
    // printf(" Local   cL,\n");
    // printf(" Local   dL,\n");
    // printf(" Global  cG,\n");
    // printf(" Global  dG\n");
    // printf(");\n");
    // print_Mucke_programCall();
    printf("bool programCall( \n");
    printf(" Module  cm,    \n");                // Caller
    printf(" Module  dm,    \n");                // Callee
    printf(" PrCount cp,     \n");               // Caller PC
    printf(" Local   cL,      \n");              // Caller Local
    printf(" Local   dL,      \n");              // Callee Local (Parameter)
    printf(" Global  cG       \n");
    printf(");\n");
    // print_Mucke_Calling();
    printf("bool Calling(Module m, PrCount p);\n");
    // print_Mucke_Exit();
    printf(" bool Exit( Module cm, PrCount cp );\n");
    // print_SetReturnUS();
    printf("bool SetReturnUS(\n");
    printf(" Module  tm,\n");
    printf(" Module  um,\n");
    printf(" PrCount tp,\n");
    printf(" PrCount up,\n");
    printf(" Local   uL,\n");
    printf(" Local   sL,\n");
    printf(" Global  uG,\n");
    printf(" Global  sG \n");
    printf(");\n");
// print_SetReturnTS();
    printf("bool SetReturnTS( \n");
    printf(" Module  tm,\n");  // Caller
    printf(" Module  um,\n");  // Callee
    printf(" PrCount tp,\n");  // Caller PC
    printf(" PrCount up,\n");  // Callee PC (when returning)
    printf(" Local   tL,\n");  // Local of Caller
    printf(" Local   sL,\n");  // Local of Caller after returning
    printf(" Global  tG,\n");  // Global of Caller
    printf(" Global  sG \n");  // Global of Caller after returning
    printf(");\n");

    // print_Mucke_SkipCall();
    printf("bool SkipCall(\n");
    printf(" Module  cm,\n");    // Current module of caller
    printf(" PrCount cp, \n");   // program counter
    printf(" PrCount dp  \n");   // Next program coutner
    printf(");\n");

    // print_Mucke_functions();
    bp_fun_t p;
    for (p = bp_functions; p; p = p->next) {
        if (!isThreadFunction(p)) {
            printf("mu bool %s(\n", p->funname);
            printf("   Module  s_mod,\n");
            printf("   PrCount s_pc,\n");
            printf("   Local   s_CL,\n");
            printf("   Global  s_G,\n");
            printf("   Local   s_ENTRY_CL,\n");
            printf("   Global  s_ENTRY_CG\n");
            printf(");\n");
        }
    }

}


/////////////////////////////////////////////////////////////////////////////////////////
static void
print_Mucke_program(int ord, char *formulaFilename) {

    int totallocals, processedlocals, max_global_var;

    FILE *formula;
    bp_fun_t p, fun;
    bp_ident_t q;
    int i, j, max, mi, mj, mk;
    int tmp;
    char line[400];    // Increase the length of formula line
    bp_ident_t z, zprime, z3;
    bp_stmt_t target_stmt;

    //recursive_set_useful_function();

    //  fun_reachabilty();

    // set the pointer to the next statement
    set_next_statement();

    eliminate_goto_print_skip();

    // set the pc number
    max = 0;
    for (p = bp_functions; p; p = p->next) {
        i = put_pc_to_statements(p->stmt, -1);
        max = Max(max, i);    // max is the maximum pc
    }
    pc_return = max + 1;
    dead_pc = max + 2;

    printf("// pc_return=%d\n", pc_return);
    printf("// dead_pc=%d\n", dead_pc);

    // print_debug_program();

    // TRUC's modification
    tmp = 0;
    for (p = bp_functions; p; p = p->next) {
        printf("// %s is module %d\n", p->funname, tmp);
        p->funname_replace = tmp; //function thread0  is assigned value 0
        tmp++;
    }
    num_thread = tmp - 1; // does not count init

    printf("enum Module {0..%d};\n\n", tmp);   // Init does not counted as a thread

    n_bit_pc = NumBits(dead_pc);

    printf("class PrCount {\n");
    for (j = 1; j <= n_bit_pc; j++) {
        printf("bool b%d;\n", j);
    }
    printf("};\n\n");

    j = 1;
    for (z = bp_globals; z; z = z->next) {
        (z->varname_replace) = j++;
        printf("// v%d is %s\n", z->varname_replace, z->varname);
    }

    max_global_var = j - 1;

    //some local variables could not be declared
    discover_new_locals();

    bp_all_locals = All_locals();


    clousure_globals();


    // Labeling formula
    for (fun = bp_functions; fun; fun = fun->next) {
        if (strcmp(fun->funname, "init") != 0) {
            insert_CS_into_module(fun);
        }
    }


    //ord is always set to 1
    if (ord == 1) {
        j = 0;
        for (z = bp_all_locals; z; z = z->next) (z->varname_replace) = j++;
        maxindexlocvar = j - 1;
    } else {

        for (z = bp_all_locals; z; z = z->next) (z->varname_replace = -1);

        maxindexlocvar = -1;

        totallocals = 0;
        for (z = bp_all_locals; z; z = z->next) totallocals++;

        processedlocals = 0;

        for (z = bp_all_locals; z; z = z->next) {
            if ((totallocals - processedlocals) + maxindexlocvar + max_global_var < 120) break;
            mi = -1;
            mk = 1;
            while (mk == 1) {
                mi++;
                mk = 0;
                for (p = bp_functions; p; p = p->next) {
                    if (Is_there_var(p->locals, z->varname)) {
                        for (zprime = p->locals; zprime; zprime = zprime->next) {
                            z3 = Is_there_var(bp_all_locals, zprime->varname);
                            if (z3) {
                                if (z3->varname_replace == mi) {
                                    mk = 1;
                                }
                            }
                        }
                    }
                }
            }
            z->varname_replace = mi;
            if (maxindexlocvar < mi) maxindexlocvar = mi;
            processedlocals++;
        }
    }


    for (; z; z = z->next) {
        (z->varname_replace) = ++maxindexlocvar;
    }
    put_name_numb_fun_locals();


    print_Variables(ord);

    print_CopyLocals();

    print_CopyGlobals();


    printf("\n\n");

    //  printf("gaia\n\n\n\n\n\n\n");
    //  fflush(stdout);

    // print initPC
    print_PC_0();

    //  printf("gaia1\n\n\n\n\n\n\n");
    //  fflush(stdout);



    printf("\n\n");

    printf("enum CS {0..%d};", CS - 1);

    printf("\n\n");

    //  printf("enum Thread {0..%d};\n", num_thread-1);

    printf("\n\n");


    printf("class Globals{\n");
    for (i = 0; i <= CS - 1; i++) {
        printf(" Global  g%d;\n", i);
        printf(" Global  h%d;\n", i);
    }
    printf("}\n");
    for (i = 0; i <= CS - 2; i++) {
        printf(" g%d  ~+ h%d,\n", i, i);
        printf(" h%d  ~+  g%d,\n", i, i + 1);
    }

    printf(" g%d  ~+ h%d;\n", CS - 1, CS - 1);

    printf("\n\n");

    printf("bool max_round( CS r ) (r=%d);\n\n", CS - 1);


    //  printf("/******************************************************************************/\n\n");

    //  printf("bool GlobalInit(Global CG)(true);\n\n");


    printf("/******************************************************************************/\n\n");

    printf("bool LessEqualCS(CS r, CS r')\n");
    printf("r ~+ r'\n");
    printf("(   ( r=0 )\n");
    for (i = 0; i < CS - 1; i++) {
        printf("  | ( r=%d ", i);
        for (j = 0; j < i; j++)
            printf("  & r'!=%d", j);
        printf(" )\n");
    }
    printf("  | ( r'=%d )\n", CS - 1);
    printf(");\n\n");

    printf("#size LessEqualCS;\n\n");

    printf("/******************************************************************************/\n\n");

    printf("bool LocalInit(\n");
    printf(" Module   mod, \n");
    printf(" PrCount  pc,  \n");
    printf(" Local    CL   \n");
    printf(")              \n");
    printf(" mod  <  pc,   \n");
    printf(" pc   <  CL    \n");
    printf("( false\n");


    for (i = 0; i < num_thread; i++) {
        //     printf("  | (  mod=%d & pc=0 )\n", i);
        printf("  | (  mod=%d & \n", i);
        print_PC("pc", 0, n_bit_pc);
        printf(" )\n");
    }

    printf(");\n\n");

    printf("#size LocalInit;\n\n");

    printf("/******************************************************************************/\n\n");

    printf("// Increase context-switch counter\n\n");
    printf("bool increaseCS(CS r, CS r')\n");
    printf("r ~+ r'\n");
    printf("( false\n");
    for (i = 0; i < CS - 1; i++)
        printf("  | ( r=%d & r'=%d )\n", i, i + 1);
    printf(");\n\n");

    // printf("#size increaseCS;\n\n");


    printf("/******************************************************************************/\n\n");

    printf("bool copy_g_g(Globals s, Globals t, CS r)\n");
    printf(" r  < s,\n");
    printf(" s  ~+ t\n");
    printf("( true\n");

    for (i = 0; i <= CS - 1; i++) {
        printf("  & ( s.g%d=t.g%d | (false ", i, i);
        for (j = 0; j < i; j++) printf(" | r=%d ", j);
        printf(" ) )\n");
    }
    printf(");\n\n");

    // printf("#size copy_g_g;\n\n");

    printf("/******************************************************************************/\n\n");

    printf("bool copy_h_h(Globals s, Globals t, CS r)\n");
    printf(" r  < s,\n");
    printf(" s  ~+ t\n");
    printf("( true\n");
    for (i = 0; i <= CS - 1; i++) {
        printf("  & ( s.h%d=t.h%d | (false", i, i);
        for (j = 0; j < i; j++) printf(" | r=%d ", j);
        printf(" ) )\n");
    }
    printf(");\n\n");

    // printf("#size copy_h_h;\n\n");

    printf("/******************************************************************************/\n\n");

    printf("bool copy_g_h(Globals s, Globals t, CS r)\n");
    printf(" r  < s,\n");
    printf(" s  ~+ t\n");
    printf("( true\n");
    for (i = 0; i <= CS - 1; i++) {
        printf("  & ( s.g%d=t.h%d | (false", i, i);
        for (j = 0; j < i; j++) printf(" | r=%d ", j);
        printf(" ) )\n");
    }
    printf(");\n\n");

    // printf("#size copy_g_h;\n\n");

    printf("/******************************************************************************/\n\n");

    printf("bool folding( Globals G,  Globals H, CS r )\n");
    printf(" r  < G,\n");
    printf(" G ~+ H\n");
    printf("(\n");
    printf("   true\n");

    for (i = 0; i < CS - 1; i++) {
        printf("   & (H.h%d = G.g%d ", i, i + 1);
        for (j = 0; j <= i; j++) printf(" | r=%d ", j);
        printf(" )\n");
    }
    printf(");\n\n");

    // printf("#size folding;\n\n");

    printf("/******************************************************************************/\n\n");

    /*
    printf("bool copy_g_and_h(Globals s, Globals t)\n");
    printf(" s  ~+ t\n");
    printf("(   true\n");
    for(i=0; i<=CS-1; i++)
      printf("  & s.g%d=t.g%d\n", i, i);

    for(i=0; i<=CS-1; i++)
      printf("  & s.h%d=t.h%d\n", i, i);
    printf(");\n\n");

    printf("#size copy_g_and_h;\n\n");
    */

    //  printf("\n\n");

    //  print_Global_init();

    printf("\n\n");

    // print_Mucke_forward_decls();
    // printf("\n\n");

    print_Mucke_programCall();
    // printf("#size ProgramCall;\n\n");
    printf("\n\n");

    print_Mucke_Calling();
    printf("\n\n");

    print_Mucke_Exit();
    printf("\n\n");

    print_SetReturnUS();
    printf("\n\n");

    print_SetReturnTS();
    printf("\n\n");

    print_Mucke_SkipCall();
    printf("\n\n");

    print_Mucke_programInt1_nonthread();
    printf("\n#size programInt1nonthread;\n");
    printf("\n\n");
    print_Mucke_programInt2_nonthread();
    printf("\n#size programInt2nonthread;\n");
    printf("\n\n");
    print_Mucke_programInt3_nonthread();
    printf("\n#size programInt3nonthread;\n");
    printf("\n\n");

    CopyVariables_ProgramInt();
    printf("\n#size CopyVariables_ProgramInt;\n");
    printf("\n\n");

    print_Mucke_functions();

    print_Mucke_programInt1();
    printf("\n\n");

    printf("\n#size programInt1;\n");
    printf("\n\n");

    print_Mucke_programInt2();
    printf("\n\n");

    printf("\n#size programInt2;\n");
    printf("\n\n");

    print_Mucke_programInt3();
    printf("\n\n");

    printf("\n#size programInt3;\n");
    printf("\n\n");

    print_enforce();
    printf("\n\n");

    // TRUC fix target label
    int found_target_label = 0;
    printf("bool target(Module cm, PrCount cp) (\nfalse\n");
    for (p = bp_functions; p; p = p->next) {
        target_stmt = find_label(p->stmt, target_label);
        if (target_stmt != NULL) {
            found_target_label = 1;
            // Print stuff here
            if (target_stmt->numlabel >= 0) {
                printf("| ( (cm = %d ) & ", p->funname_replace);
                print_PC("cp", target_stmt->numlabel, n_bit_pc);
                printf(")\n");
            } else {
                printf("|( (cm = %d ) & ", p->funname_replace);
                print_PC("cp", dead_pc, n_bit_pc);
                printf(")\n");
            }
        }
    }
    if (!found_target_label) {
        printf("label not found: %s\n", target_label);
        exit(1);
    }
    else {
        printf(");\n\n");
    }

    // find the target in the init module - the label is "endinit"
    for (p = bp_functions; p; p = p->next) {
        target_stmt = find_label(p->stmt, "endinit");
        if (target_stmt != NULL) break;
    }
    if (target_stmt == NULL) {
        printf("label not found: %s\n", "endinit");
        exit(1);
    } else {
        if (target_stmt->numlabel >= 0) {
            printf("bool Target_Init(PrCount cp)\n ");
            print_PC("cp", target_stmt->numlabel, n_bit_pc);
        } else {
            printf("bool Target_Init(PrCount cp)\n ");
            print_PC("cp", dead_pc, n_bit_pc);
        }
    }
    printf(";\n\n");


    //  ##    ##   #  #  ###   ####  #  #  ###           ##   #  #  ###   ###    ##   #  #
    // #  #  #  #  ## #   #    #     #  #   #           #  #  #  #   #     #    #  #  #  #
    // #     #  #  ## #   #    ###    ##    #            #    #  #   #     #    #     ####
    // #     #  #  # ##   #    #      ##    #             #   ####   #     #    #     #  #
    // #  #  #  #  # ##   #    #     #  #   #           #  #  ####   #     #    #  #  #  #
    //  ##    ##   #  #   #    ####  #  #   #            ##   #  #  ###    #     ##   #  #

    printf("// CONTEXTSWITCH SESSION\n");
    print_Mucke_programContextSwitch();
    printf("\n\n");

    // ####   ##   ###   #  #  #  #  #      ##
    // #     #  #  #  #  ####  #  #  #     #  #
    // ###   #  #  #  #  ####  #  #  #     #  #
    // #     #  #  ###   #  #  #  #  #     ####
    // #     #  #  # #   #  #  #  #  #     #  #
    // #      ##   #  #  #  #   ##   ####  #  #

    // print the Winning formula
    printf("// FORMULA SESSION\n");
    if ((formula = fopen(formulaFilename, "r")) == NULL) {
        printf("Cannot open file %s.\n", formulaFilename);
        exit(1);
    }

    while (fgets(line, sizeof line, formula) != NULL)
        fputs(line, stdout);

}


int main(int argc, char **argv) {
    int opt;
    int c;
    char *inputFilename = 0;
    char *formulaFilename = 0;

    static struct option long_options[] = {
        {"input", required_argument,   0, 'i'},
        {"context", required_argument, 0, 'c'},
        {"label", required_argument,   0, 'l'},
        {"formula", required_argument, 0, 'f'},
        {"help", no_argument,          0, 'h'},
        {0, 0,                         0, 0}
    };

    while (1) {
        int option_index = 0;
        c = getopt_long(argc, argv, "i:c:l:f:h", long_options, &option_index);

        if (c == -1) {
            break;
        }
        switch (c) {
        case 'h':
            printf("usage: program -i FILE_NAME -c ROUND -l TARGET_LABEL -f FORMULA\n");
            exit(0);
        case 'i':
            inputFilename = calloc(strlen(optarg) + 1, sizeof(char));
            strncpy(inputFilename, optarg, strlen(optarg));
            break;
        case 'c':
            CS = -1;
            CS = atoi(optarg);
            break;
        case 'l':
            target_label = calloc(strlen(optarg) + 1, sizeof(char));
            strncpy(target_label, optarg, strlen(optarg));
            break;
        case 'f':
            formulaFilename = calloc(strlen(optarg) + 1, sizeof(char));
            strncpy(formulaFilename, optarg, strlen(optarg));
            break;
        default:
            exit(1);
        }
    }

    if (argc <= 1) {
        printf("usage: program -i FILE_NAME -c ROUND -l TARGET_LABEL -f FORMULA\n");
        exit(0);
    }

    opt = 1;

    line = column = 1;
    if (!(yyin = fopen(inputFilename, "r"))) {
        common_error("file %s not found", inputFilename);
    }

    if (CS < 1) {
        common_error("ROUND must be an integer greater than 1\n");
    }

    if (yyparse()) {
        common_error("parse errors encountered");
    }

    fclose(yyin);

    print_Mucke_program(opt, formulaFilename);
}
