#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bpstruc.h"
#include "name.h"
#include "common.h"
#include "varOrd.h"

extern FILE *yyin;
extern int yyparse (void);
extern int line;
extern int column;



// #        #    ####   #####  #      #####  ####
// #       # #    #  #  #      #      #      #   #
// #      #   #   #  #  #      #      #      #   #
// #      #   #   ###   ####   #      ####   ####
// #      #####   #  #  #      #      #      # #
// #      #   #   #  #  #      #      #      #  #
// #####  #   #  ####   #####  #####  #####  #   #


bp_ident_t bp_all_locals;
char* target_label;

static bp_stmt_t find_label(bp_stmt_t p, char* label)
{
    bp_stmt_t temp;

    if (!p) return NULL;

    if ((p->label) != NULL)
        if ( strcmp(p->label, label) == 0) return (p);

    switch (p->type)
    {
    case BP_IF:
    case BP_ELSEIF:
    case BP_WHILE:
    {
        temp = find_label(p->e.c.thenstmt, label);
        if (temp != NULL) return temp;

        temp = find_label(p->e.c.elsestmt, label);
        if (temp != NULL) return temp;

        break;
    }
    }

    return (find_label(p->next, label));
}


static void print_cs_expression(bp_expr_t p, FILE * debugfile)
{
    bp_ident_t y, q;

    if (!p) return;


    switch (p->token)
    {
    case BP_CHOOSE:
    {
        fprintf(debugfile, "schoose[");
        print_cs_expression(p->left, debugfile);
        fprintf(debugfile, ",");
        print_cs_expression(p->right, debugfile);
        fprintf(debugfile, "]");
        return;
    }
    case BP_ND:
    {
        fprintf(debugfile, "*");
        return;
    }
    case BP_CONST:
    {
        if (p->left)
            fprintf(debugfile, "T");
        else
            fprintf(debugfile, "F");
        return;
    }
    case BP_IDENT:
    {
        if ( p->primed == 1) fprintf(debugfile, "'");
        fprintf(debugfile, "%s", p->var_name);
        return;
    }
    case BP_NOT:
    {
        fprintf(debugfile, "(!");
        print_cs_expression(p->left, debugfile);
        fprintf(debugfile, ")");
        return;
    }
    case BP_EQ:
    {
        fprintf(debugfile, "(");
        print_cs_expression(p->left, debugfile);
        fprintf(debugfile, "=");
        print_cs_expression(p->right, debugfile);
        fprintf(debugfile, ")");
        return;
    }
    case BP_NE:
    {
        fprintf(debugfile, "((");
        print_cs_expression(p->left, debugfile);
        fprintf(debugfile, "!=");
        print_cs_expression(p->right, debugfile);
        fprintf(debugfile, "))");
        return;
    }
    case BP_AND:
    {
        fprintf(debugfile, "(");
        print_cs_expression(p->left, debugfile);
        fprintf(debugfile, "&");
        print_cs_expression(p->right, debugfile);
        fprintf(debugfile, ")");
        return;
    }
    case BP_OR:
    {
        fprintf(debugfile, "(");
        print_cs_expression(p->left, debugfile);
        fprintf(debugfile, "|");
        print_cs_expression(p->right, debugfile);
        fprintf(debugfile, ")");
        return;
    }
    case BP_IMP:
    {
        fprintf(debugfile, "(");
        print_cs_expression(p->left, debugfile);
        fprintf(debugfile, "=>");
        print_cs_expression(p->right, debugfile);
        fprintf(debugfile, ")");
        return;
    }

    }

}

/******************************************************************/

static void print_cs_indent_statement(int indent, FILE * debugfile)
{
    int i;

    for (i = 0; i < indent * 4; i++)
        fprintf(debugfile, " ");
}


static void print_cs_recursive_statement( bp_fun_t fun, bp_stmt_t p, int indent, FILE * debugfile)
{
    bp_ident_t y;
    bp_idref_t q, list_return_expr;
    bp_ident_t z = NULL, temp_var;
    int i, tmp;

    if (!p) return;

    if (p->cs > 0)
        fprintf(debugfile, "//CS_%s_%d\n", fun->funname, p->cs);

    print_cs_indent_statement(indent, debugfile);
    if (p->label) fprintf(debugfile, "%s: ", p->label);

    switch (p->type)
    {
    case BP_ASSIGN:
    {
        for ( q = (p->e.a.asgnlist); q; q = q->next )
        {
            if (q != p->e.a.asgnlist) fprintf(debugfile, ", ");
            fprintf(debugfile, "%s", q->var_name);
        }

        fprintf(debugfile, " := ");

        for ( q = (p->e.a.asgnlist); q; q = q->next )
        {
            if (q != p->e.a.asgnlist) fprintf(debugfile, ", ");
            print_cs_expression(q->expr, debugfile);
        }

        fprintf(debugfile, ";\n");

        break;
    }

    case BP_ASSERT:
    {
        fprintf(debugfile, "assert(");
        print_cs_expression(p->e.c.expr, debugfile);
        fprintf(debugfile, ");\n");

        break;
    }

    case BP_ASSUME:
    {
        fprintf(debugfile, "assume(");
        print_cs_expression(p->e.c.expr, debugfile);
        fprintf(debugfile, ");\n");

        break;
    }
    case BP_CONSTRAIN:
    {
        fprintf(debugfile, "constrain(");
        print_cs_expression(p->e.c.expr, debugfile);
        fprintf(debugfile, ");\n");

        break;
    }

    case BP_SKIP:
    {
        fprintf(debugfile, "skip;\n");
        break;
    }

    case BP_GOTO:
    {
        if (find_label(fun->stmt, p->e.a.label))
            fprintf(debugfile, "goto %s;\n", p->e.a.label);
        else
            fprintf(debugfile, "assume(0);\n");
        break;
    }

    case BP_RETURN:
    {
        fprintf(debugfile, "return ");

        for ( q = (p->e.a.asgnlist); q; q = q->next )
        {
            if (q != p->e.a.asgnlist) fprintf(debugfile, ", ");
            print_cs_expression(q->expr, debugfile);

        }
        fprintf(debugfile, ";\n");

        break;
    }

    case BP_PRINT:
    {
        fprintf(debugfile, "print(");

        for ( q = (p->e.a.asgnlist); q; q = q->next )
        {
            if (q != p->e.a.asgnlist) fprintf(debugfile, ", ");
            print_cs_expression(q->expr, debugfile);

        }
        fprintf(debugfile, ");\n");

        break;
    }


    case BP_CALL: {

        if (p->e.a.asgnlist) {
            for ( q = (p->e.a.asgnlist); q; q = q->next )
            {
                if (q != p->e.a.asgnlist) fprintf(debugfile, ", ");
                fprintf(debugfile, "%s", q->var_name);
            }
            fprintf(debugfile, " := ");
        }

        fprintf(debugfile, "%s(", p->e.a.label);

        for ( q = (p->e.a.fmllist); q; q = q->next )
        {
            if (q != p->e.a.fmllist) fprintf(debugfile, ", ");
            print_cs_expression(q->expr, debugfile);
        }

        fprintf(debugfile, ");\n");


        break;
    }


    case BP_IF:
    case BP_ELSEIF:
    {

        if (p->type == BP_IF) fprintf(debugfile, "if (");
        else                  fprintf(debugfile, "elsif (");

        print_cs_expression(p->e.c.expr, debugfile);

        fprintf(debugfile, ") then\n");
        print_cs_recursive_statement(fun, p->e.c.thenstmt, indent + 1, debugfile);

        if (p->e.c.elsestmt) {
            if (p->e.c.elsestmt->type != BP_ELSEIF)
            {   print_cs_indent_statement(indent, debugfile);
                fprintf(debugfile, "else\n");
                print_cs_recursive_statement(fun, p->e.c.elsestmt, indent + 1, debugfile);
            } else print_cs_recursive_statement(fun, p->e.c.elsestmt, indent, debugfile);
        }
        if (p->type == BP_IF) {
            print_cs_indent_statement(indent, debugfile);
            fprintf(debugfile, "fi\n");
        }
        break;
    }

    case BP_WHILE:
    {
        fprintf(debugfile, "while (");
        print_cs_expression(p->e.c.expr, debugfile);
        fprintf(debugfile, ") do\n");
        print_cs_recursive_statement(fun, p->e.c.thenstmt, indent + 1, debugfile);
        fprintf(debugfile, "od\n");
        break;
    }

    }

    print_cs_recursive_statement(fun, p->next, indent, debugfile);
    return;
}


static void print_cs_program(char * filename)
{
    FILE *debugfile;
    char newfilename[10000];

    bp_fun_t p, fun;
    bp_ident_t q;
    int i, j, max, mi, mj, mk;
    bp_ident_t z, zprime, z3;
    bp_stmt_t target_stmt;

    snprintf(newfilename, sizeof newfilename, "%s.with_cs.bp", filename);

    //print the declaration of global variables
    debugfile = fopen(newfilename, "w");

    if (bp_globals)
    {
        fprintf(debugfile, "decl ");
        for (z = bp_globals; z; z = z->next) {
            if (z != bp_globals)  fprintf(debugfile, ", ");
            fprintf(debugfile, "%s", z->varname);
        }
        fprintf(debugfile, "; // global variables\n\n\n");
    }


    //print the functions

    for (fun = bp_functions; fun; fun = fun->next)
    {
        switch (fun->num_returns) {
        case 0: {fprintf(debugfile, "void "); break;}
        case 1: {fprintf(debugfile, "bool "); break;}
        default: {fprintf(debugfile, "bool<%d> ", fun->num_returns); break;}
        }

        fprintf(debugfile, "%s (", fun->funname);


        for (z = fun->parms; z; z = z->next) {
            if (z != fun->parms)  fprintf(debugfile, ", ");
            fprintf(debugfile, "%s", z->varname);
        }

        fprintf(debugfile, ")\n");
        fprintf(debugfile, "begin\n");


        if (fun->locals) {
            fprintf(debugfile, "decl ");
            for (z = fun->locals; z; z = z->next) {
                if (z != fun->locals)  fprintf(debugfile, ", ");
                fprintf(debugfile, "%s", z->varname);
            }
            fprintf(debugfile, ";\n\n");
        }

        if (fun->enforce)
        {
            fprintf(debugfile, "enforce ");
            print_cs_expression(fun->enforce, debugfile);
            fprintf(debugfile, ";\n");

        }

        fprintf(debugfile, "//CS_%s_0\n", fun->funname);

        print_cs_recursive_statement( fun, fun->stmt, 1 , debugfile);

        fprintf(debugfile, "//CS_%s_%d\n", fun->funname, fun->last_cs);

        fprintf(debugfile, "end\n\n\n");

    }

    fclose(debugfile);
}



/******************************************************************/

static bp_fun_t find_function(char* varname)
{
    bp_fun_t y;

    for (y = bp_functions; y; y = y->next)
        if (strcmp(varname, y->funname) == 0) return (y);
    common_error("error: the name of the called function does not exist: %s\n", varname);
}

/******************************************************************/

static void reset_flag_visited(bp_stmt_t p)
{
    if (!p) return;

    p->visited = 0;

    switch (p->type)
    {
    case BP_IF:
    case BP_ELSEIF:
    case BP_WHILE:
    {
        reset_flag_visited(p->e.c.thenstmt);
        reset_flag_visited(p->e.c.elsestmt);
        break;
    }
    }

    reset_flag_visited(p->next);
}

/********************************************************************/

static bp_ident_t All_locals(void)
{
    bp_fun_t fun;
    bp_ident_t q;
    bp_ident_t bp_decls_temp;


    bp_decls = NULL;
    bp_declcount = 0;

    for (fun = bp_functions; fun; fun = fun->next)
    {
        for (q = fun->locals; q; q = q->next)
            if (!Is_there_var(bp_decls, q->varname))
                Insert_var(q->varname);
        for (q = fun->parms; q; q = q->next)
            if (!Is_there_var(bp_decls, q->varname))
                Insert_var(q->varname);
    }

    bp_all_locals = bp_decls;

    for (fun = bp_functions; fun; fun = fun->next)
    {
        bp_decls = fun->locals;

        for (q = fun->parms; q; q = q->next)
            if (!Is_there_var(bp_decls, q->varname))
                Insert_var(q->varname);

        fun->locals = bp_decls;
    }

    return (bp_all_locals);
}

/********************************************************************/

/********************************************************************/

static void
discover_new_locals_in_assign_id_list(bp_idref_t p, bp_fun_t fun)
{
    bp_ident_t y;

    for ( ; p; p = p->next )
    {
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

static void
discover_new_locals_in_expression(bp_expr_t p, bp_fun_t fun)
{
    bp_ident_t y, q;

    if (!p) return;

    switch (p->token)
    {
    case BP_CONST:
    {
        return;
    }
    case BP_IDENT:
    {
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

    discover_new_locals_in_expression(p->left,  fun);
    discover_new_locals_in_expression(p->right, fun);
    return;
}

static void
discover_new_locals_in_decider(bp_expr_t p, bp_fun_t fun)
{
    if (p->token != BP_ND)
        discover_new_locals_in_expression(p, fun);
}

static void
discover_new_locals_in_expression_list(bp_idref_t p, bp_fun_t fun)
{
    for ( ; p; p = p->next )
        discover_new_locals_in_expression(p->expr, fun);
}

static void
discover_new_locals_in_statements(bp_stmt_t p, bp_fun_t fun)
{
    if ( (!p) || (p->visited != 0) ) return;

    p->visited = 1;

    switch (p->type)
    {
    case BP_ASSIGN:
    {
        discover_new_locals_in_assign_id_list(p->e.a.asgnlist, fun);
        discover_new_locals_in_expression_list(p->e.a.asgnlist, fun);
        break;
    }
    case BP_ASSERT:
    case BP_CONSTRAIN:
    case BP_ASSUME:
    {
        discover_new_locals_in_decider(p->e.c.expr, fun);
        break;
    }

    case BP_CALL:
    {
        discover_new_locals_in_assign_id_list(p->e.a.asgnlist, fun);
        discover_new_locals_in_expression_list(p->e.a.fmllist, fun);
        break;
    }
    case BP_IF:
    case BP_ELSEIF:
    case BP_WHILE:
    {
        discover_new_locals_in_decider(p->e.c.expr, fun);
        discover_new_locals_in_statements(p->next_then, fun);
        discover_new_locals_in_statements(p->next_else, fun);
        break;
    }
    case BP_RETURN:
    {
        discover_new_locals_in_expression_list(p->e.a.asgnlist, fun);
        break;
    }
    }

    discover_new_locals_in_statements(p->next_stmt, fun);
}

static void
discover_new_locals(void)
{
    bp_fun_t p;

    for (p = bp_functions; p; p = p->next)
    {
        discover_new_locals_in_statements(p->stmt, p);
    }

}


/********************************************************************/

static void recursive_set_next_statement(bp_stmt_t start, bp_stmt_t p, bp_stmt_t next_p)
{

    bp_stmt_t temp;

    if (!p) return;

    if (p->next)   p->next_stmt = p->next;
    else           p->next_stmt = next_p;


    switch (p->type)
    {
    case BP_IF:
    case BP_ELSEIF:
    {
        recursive_set_next_statement(start, p->e.c.thenstmt, p->next_stmt);
        recursive_set_next_statement(start, p->e.c.elsestmt, p->next_stmt);
        break;
    }
    case BP_WHILE:
    {
        recursive_set_next_statement(start, p->e.c.thenstmt, p);
        break;
    }
    case BP_GOTO:
    {
        p->next_stmt = find_label(start, p->e.a.label );
        if (p->next_stmt == NULL)  common_error("error: label of goto not found: %s\n", p->e.a.label );

        break;
    }
    }

    recursive_set_next_statement(start, p->next, next_p);
}

static void
set_next_statement(void)
{
    bp_fun_t p;

    for (p = bp_functions; p; p = p->next)
        recursive_set_next_statement(p->stmt, p->stmt, NULL);
}



/********************************************************************/


static bp_stmt_t
next_no_skip_goto_skip(bp_stmt_t p, bp_fun_t fun)
{
    int i;
    bp_stmt_t z;


    if ( (!p) || (p->visited != 0) ) return NULL;

    if ((p->label) != NULL) {
        if (strcmp(p->label, target_label) == 0) return p;
        if (strcmp(p->label, "endinit") == 0) return p;
    }
    p->visited = 1;

    switch (p->type)
    {
    case BP_ASSUME:
    {
        if ( (((p->e.c.expr)->token) == BP_CONST) && ((p->e.c.expr)->left)) {
            z = next_no_skip_goto_skip(p->next, fun);

            p->visited = 0;
            return (z);
        }

        break;
    }

    case BP_GOTO:
    {
        z = next_no_skip_goto_skip(find_label(fun->stmt, p->e.a.label), fun);

        p->visited = 0;
        return (z);
    }
    case BP_PRINT:
    case BP_SKIP:
    {
        z = next_no_skip_goto_skip(p->next_stmt, fun);

        p->visited = 0;
        return (z);
    }
    }
    p->visited = 0;
    return (p);
}

static void recursive_eliminate_goto_print_skip(bp_stmt_t p, bp_fun_t fun)
{
    int tmp;
    bp_stmt_t z;

    if (!p) return;

    switch (p->type)
    {
    case BP_IF:
    case BP_ELSEIF:
    case BP_WHILE:
    {
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

static void eliminate_goto_print_skip(void)
{
    bp_fun_t p;

    for (p = bp_functions; p; p = p->next) {

        reset_flag_visited(p->stmt);
        recursive_eliminate_goto_print_skip(p->stmt, p);

        //remove the first statement in case it is a skip, goto ...

        if (((p->stmt->label) == NULL) || (strcmp(p->stmt->label, target_label) != 0))
            switch ((p->stmt)->type)
            {
            case BP_ASSUME:
            {
                if ( (((p->stmt->e.c.expr)->token) == BP_CONST) && ((p->stmt->e.c.expr)->left))
                    p->stmt = p->stmt->next_stmt;
                break;
            }
            //         case BP_GOTO:
            case BP_PRINT:
            case BP_SKIP:
            {
                p->stmt = (p->stmt)->next_stmt;
                break;
            }
            }
    }
}


/********************************************************************/
static void
put_name_numb_fun_locals(void)
{
    bp_fun_t p;
    bp_ident_t z, t;

    for (p = bp_functions; p; p = p->next) {
        for (z = p->locals; z; z = z->next) {
            t = Is_there_var(bp_all_locals, z->varname );
            if (t == NULL)  common_error("error: local variable not found %s\n", z->varname);

            z->varname_replace = t->varname_replace;
        }
    }
}


/********************************************************************/

static void clousure_globals(void);


static int
IsGlobalVariableName(char * varname)
{
    bp_ident_t v;
    if (bp_globals)
    {
        for (v = bp_globals; v; v=v->next)
        {
            if (strcmp(varname, v->varname) == 0)
            {
                return 1;
            }
        }
        return 0;
    }
    else
    {
        return 0;
    }
}


static int
IsGlobalExpr(bp_expr_t p)
{
    if (!p) return 0;

    switch (p->token)
    {
    case BP_CHOOSE:
    {
        return (IsGlobalExpr(p->left) || IsGlobalExpr(p->right));
    }
    case BP_ND:
    case BP_CONST:
    {
        return 0;
    }
    case BP_IDENT:
    {
        return IsGlobalVariableName(p->var_name);
    }
    case BP_NOT:
    {
        return IsGlobalExpr(p->left);
    }
    case BP_EQ:
    case BP_NE:
    case BP_AND:
    case BP_OR:
    case BP_IMP:
    {
        return (IsGlobalExpr(p->left) || IsGlobalExpr(p->right));
    }
    }
    return 0;
}


int isInAtomicRegion;
int cs_counter;

static void
insert_CS_into_statement_recursive(bp_stmt_t p)
{
    int tmp;
    int isAtomicStmt;
    int hasGlobalVar;
    bp_idref_t q;

    if (!p) return;

    switch (p->type)
    {
    case BP_ASSIGN:
    {
        // Identify if this is atomic, at := T, T
        tmp = 0;
        isAtomicStmt = 0;
        hasGlobalVar = 0;

        for ( q = (p->e.a.asgnlist); q; q = q->next )
        {
            if (strcmp(q->var_name, "atomic") == 0 && isAtomicStmt == 0)
            {
                ++isAtomicStmt;
            }
            else if (strcmp(q->var_name, "at") == 0 && isAtomicStmt == 1)
            {
                ++isAtomicStmt;
            }
            else
            {
                if (!hasGlobalVar)
                {
                    hasGlobalVar = IsGlobalVariableName(q->var_name);
                }
            }
            ++tmp;
        }


        if (tmp == isAtomicStmt && tmp == 2)
        {
            isAtomicStmt = 1;
        }
        else
        {
            isAtomicStmt = 0;
        }

        tmp = 0;

        if (isAtomicStmt)
        {
            if (p->e.a.asgnlist->expr->token == BP_CONST && p->e.a.asgnlist->expr->left)
            {
                isInAtomicRegion = 1;
            }
            else
            {
                isInAtomicRegion = 0;
            }
        }
        else if (!hasGlobalVar)
        {
            for ( q = (p->e.a.asgnlist); q; q = q->next )
            {
                if (!hasGlobalVar)
                {
                    hasGlobalVar = IsGlobalExpr(q->expr);
                }
            }
        }

        // Set cs_counter
        if (isAtomicStmt)
        {
            if (isInAtomicRegion)
            {
                ++cs_counter;
                p->cs = cs_counter;
            }
            else
            {
                p->cs = -1;
            }
        }
        else if (isInAtomicRegion)
        {
            p->cs = -1;
        }
        else if (hasGlobalVar)
        {
            ++cs_counter;
            p->cs = cs_counter;
        }
        else
        {
            p->cs = -1;
        }

        break;
    }

    case BP_CALL:
    {
        hasGlobalVar = 0;
        if (p->e.a.asgnlist)
        {
            for ( q = (p->e.a.asgnlist); q; q = q->next )
            {
                if (!hasGlobalVar)
                {
                    hasGlobalVar = IsGlobalVariableName(q->var_name);
                }
            }
        }

        if (!hasGlobalVar)
        {
            for ( q = (p->e.a.fmllist); q; q = q->next )
            {
                if (!hasGlobalVar)
                {
                    hasGlobalVar = IsGlobalExpr(q->expr);
                }
            }
        }

        if (isInAtomicRegion)
        {
            p->cs = -1;
        }
        else if (hasGlobalVar)
        {
            ++cs_counter;
            p->cs = cs_counter;
        }
        else
        {
            p->cs = -1;
        }
        break;
    }
    case BP_ASSERT:
    case BP_ASSUME:
    case BP_CONSTRAIN:
    {
        hasGlobalVar = IsGlobalExpr(p->e.c.expr);
        if (isInAtomicRegion)
        {
            p->cs = -1;
        }
        else if (hasGlobalVar)
        {
            ++cs_counter;
            p->cs = cs_counter;
        }
        else
        {
            p->cs = -1;
        }
        break;
    }

    case BP_PRINT:
    case BP_SKIP:
    case BP_GOTO:
    case BP_RETURN:
    {
        p->cs = -1;
        break;
    }

    case BP_IF:
    case BP_ELSEIF:
    {
        hasGlobalVar = IsGlobalExpr(p->e.c.expr);
        if (isInAtomicRegion)
        {
            p->cs = -1;
        }
        else if (hasGlobalVar)
        {
            ++cs_counter;
            p->cs = cs_counter;
        }
        else
        {
            p->cs = -1;
        }
        insert_CS_into_statement_recursive(p->e.c.thenstmt);

        if (p->e.c.elsestmt) {
            insert_CS_into_statement_recursive(p->e.c.elsestmt);
        }
        break;
    }

    case BP_WHILE:
    {
        hasGlobalVar = IsGlobalExpr(p->e.c.expr);
        if (isInAtomicRegion)
        {
            p->cs = -1;
        }
        else if (hasGlobalVar)
        {
            ++cs_counter;
            p->cs = cs_counter;
        }
        else
        {
            p->cs = -1;
        }
        insert_CS_into_statement_recursive(p->e.c.thenstmt);
        break;
    }

    case BP_ATOMIC_BEGIN:
    {
        break;
    }

    case BP_ATOMIC_END:
    {
        break;
    }
    }

    insert_CS_into_statement_recursive(p->next);
}


static void
insert_CS_into_module(bp_fun_t f)
{
    isInAtomicRegion = 0;    // not in any atomic region
    cs_counter = 0;
    insert_CS_into_statement_recursive(f->stmt);
    f->last_cs = ++cs_counter;
}

void
label_program(char * filename)
{
    bp_fun_t fun;
    bp_ident_t q;
    int i, j, max, mi, mj, mk;
    int tmp;
    bp_ident_t z, zprime, z3;
    bp_stmt_t target_stmt;

    // Preprocess
    set_next_statement();
    eliminate_goto_print_skip();

    discover_new_locals();
    bp_all_locals = All_locals();

    clousure_globals();

    for (fun = bp_functions; fun; fun = fun->next)
    {
        if (strcmp(fun->funname, "init") != 0)
        {
            insert_CS_into_module(fun);
        }
    }

    print_cs_program(filename);
}



/********************************************************************/
/********************************************************************/
/***                    ACTIVE GLOBALS FOR EACH MODULE           ****/
/********************************************************************/
/********************************************************************/


/***********************************************************************/

static void discover_called_module_in_module(bp_fun_t fun, char* name)
{

    bp_fun_t calledfun;
    int found;

    found = 0;

    for (calledfun = fun->calledmodule; calledfun; calledfun = calledfun->next)
        if (strcmp(calledfun->funname, name) == 0) found = 1;

    if (found == 0)
    {
        calledfun = malloc(sizeof(struct bp_fun_element));
        calledfun->funname = name;
        calledfun->next = fun->calledmodule;
        fun->calledmodule = calledfun;
    }

    return;
}



static void discover_globals_in_module_in_assign_id_list(bp_idref_t p, bp_fun_t fun)
{
    bp_ident_t y;

    for ( ; p; p = p->next )
    {
        y = Is_there_var(bp_globals, p->var_name);
        if (y) {
            y = Is_there_var(fun->globals, p->var_name);
            if (!y)
            {
                bp_decls = fun->globals;
                Insert_var(p->var_name);
                fun->globals = bp_decls;
            }
        }
    }
}



static void discover_globals_in_module_in_expression(bp_expr_t p, bp_fun_t fun)
{
    bp_ident_t y, q;

    if (!p) return;

    switch (p->token)
    {
    case BP_CONST:
    {
        return;
    }
    case BP_IDENT:
    {
        y = Is_there_var(bp_globals, p->var_name);
        if (y) {
            y = Is_there_var(fun->globals, p->var_name);
            if (!y)
            {
                bp_decls = fun->globals;
                Insert_var(p->var_name);
                fun->globals = bp_decls;
            }
        }
        return;
    }
    }

    discover_globals_in_module_in_expression(p->left,  fun);
    discover_globals_in_module_in_expression(p->right, fun);
    return;
}



static void discover_globals_in_module_in_decider(bp_expr_t p, bp_fun_t fun)
{
    if (p->token != BP_ND)
        discover_globals_in_module_in_expression(p, fun);
}



static void discover_globals_in_module_in_expression_list(bp_idref_t p, bp_fun_t fun)
{
    for ( ; p; p = p->next )
        discover_globals_in_module_in_expression(p->expr, fun);
}



static void discover_globals_in_module_in_statements(bp_stmt_t p, bp_fun_t fun)
{
    if ( (!p) || (p->visited != 0) ) return;

    p->visited = 1;

    switch (p->type)
    {
    case BP_ASSIGN:
    {
        discover_globals_in_module_in_assign_id_list(p->e.a.asgnlist, fun);
        discover_globals_in_module_in_expression_list(p->e.a.asgnlist, fun);
        break;
    }
    case BP_ASSERT:
    case BP_CONSTRAIN:
    case BP_ASSUME:
    {
        discover_globals_in_module_in_decider(p->e.c.expr, fun);
        break;
    }

    case BP_CALL:
    {
        discover_called_module_in_module(fun, p->e.a.label);
        discover_globals_in_module_in_assign_id_list(p->e.a.asgnlist, fun);
        discover_globals_in_module_in_expression_list(p->e.a.fmllist, fun);
        break;
    }
    case BP_IF:
    case BP_ELSEIF:
    case BP_WHILE:
    {
        discover_globals_in_module_in_decider(p->e.c.expr, fun);
        discover_globals_in_module_in_statements(p->next_then, fun);
        discover_globals_in_module_in_statements(p->next_else, fun);
        break;
    }
    case BP_RETURN:
    {
        discover_globals_in_module_in_expression_list(p->e.a.asgnlist, fun);
        break;
    }
    }
    discover_globals_in_module_in_statements(p->next_stmt, fun);
}



// for each module set the field globals with all globals used in the module
//             and set the field calledmodule with all modules called in the module
static void discover_globals_in_module(void)
{
    bp_fun_t fun;

    for (fun = bp_functions; fun; fun = fun->next)
    {
        reset_flag_visited(fun->stmt);
        fun->globals = NULL;
        discover_globals_in_module_in_expression(fun->enforce, fun);
        fun->calledmodule = NULL;
        discover_globals_in_module_in_statements(fun->stmt, fun);
    }
}

static bp_fun_t find_module(bp_fun_t funlist, char* name)
{
    for (; funlist; funlist = funlist->next)
        if (strcmp(funlist->funname, name) == 0) return funlist;

    return NULL;
}


//insert module name in fun->calledmodule in case it is not in that list
static int insert_module_in_module_list(bp_fun_t fun, char * name)
{

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

static void
clousure_calledmodule(void)
{
    bp_fun_t calledfun, calledfun2, fun, funtemp;
    int ris;

    ris = 1;

    while (ris) {
        ris = 0;
        for (fun = bp_functions; fun; fun = fun->next)
        {
            for (calledfun = fun->calledmodule; calledfun; calledfun = calledfun->next)
            {
                funtemp = find_module(bp_functions, calledfun->funname);
                if (!funtemp) common_error("the function does not exist.\n");
                for (calledfun2 = funtemp->calledmodule; calledfun2; calledfun2 = calledfun2->next)
                    if (insert_module_in_module_list(fun, calledfun2->funname)) ris = 1;
            }
        }
    }

    return;
}


static void
clousure_globals(void)
{
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


int main(int argc, char **argv )
{
    if ( argc != 3 )
        common_error("usage: ./a.out FILE_NAME TARGET_LABEL \n");

    line = column = 1;
    if (!(yyin = fopen(argv[1], "r")))
    {
        common_error("file %s not found", argv[1]);
    }
    target_label = argv[2];

    if (yyparse())
    {
        common_error("parse errors encountered");
    }

    fclose(yyin);

    label_program(argv[1]);
}

