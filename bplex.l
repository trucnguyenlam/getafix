D			[0-9]
L			[a-zA-Z_]
H			[a-fA-F0-9]
E			[Ee][+-]?{D}+
FS			(f|F|l|L)
IS			(u|U|l|L)*
RB                      }

%{

#define YY_NO_UNPUT
void count(void);

%}

%option noyywrap


%%

"//"[^\n]+\n            {count (); }

"{"[^ }\n]+"}"          {
                          count();
                          yylval.c = strdup(yytext);
                          return(IDENTIFIER); }

"return"		{ count(); return(RETURN); }
"goto"		        { count(); return(GOTO); }
"skip"		        { count(); return(SKIP); }
"do"			{ count(); return(DO); }
"od"			{ count(); return(OD); }
"else"			{ count(); return(ELSE); }
"elsif"			{ count(); return(ELSIF); }
"if"			{ count(); return(IF); }
"then"			{ count(); return(THEN); }
"fi"			{ count(); return(FI); }
"decl"		        { count(); return(DECL); }
"while"			{ count(); return(WHILE);}
"begin"			{ count(); return(BEG); }
"end"			{ count(); return(END); }
"bool"			{ count(); return(BOOL); }
"void"			{ count(); return(VOID); }
"assert"		{ count(); return(ASSERT); }
"assume"		{ count(); return(ASSUME); }
"print"		        { count(); return(PRINT); }
"enforce"               { count(); return(ENFORCE); }
"constrain"             { count(); return(CONSTRAIN); }
"schoose"		{ count(); return(CHOOSE); }
"atomic_begin"  { count(); return(ATOMIC_BEGIN);}
"atomic_end"  { count(); return(ATOMIC_END);}
"*"			{ count(); return(NONDET); }
"?"		   	{ count(); return(NONDET); }
"_"			{ count(); yylval.c = strdup(yytext);
                          return(IDENTIFIER); }
({D})+                  { count(); yylval.i = atoi(yytext);
                          return(INTEGER); }

{L}({L}|{D})*	        { count(); yylval.c = strdup(yytext);
                          return(IDENTIFIER); }

":="		        { count(); return(ASSIGN); }
"="			{ count(); return(EQ_OP); }
"!="		        { count(); return(NE_OP); }
"=>"		        { count(); return(IMPLIES); }
";"			{ count(); return(';'); }

"'"                     { count(); return('\''); }
","			{ count(); return(','); }
":"			{ count(); return(':'); }
"("			{ count(); return('('); }
")"			{ count(); return(')'); }
"["			{ count(); return('['); }
"]"			{ count(); return(']'); }
"&"			{ count(); return(AND); }
"&&"			{ count(); return(AND); }
"!"			{ count(); return('!'); }
"~"			{ count(); return('~'); }
"^"			{ count(); return('^'); }
"|"			{ count(); return(OR); }
"||"			{ count(); return(OR); }
"<"			{ count(); return('<'); }
">"			{ count(); return('>'); }
[ \t\v\n\f]		{ count(); }
.			{ /* ignore bad characters */ }

%%

int column, line;

void count(void)
{
	int i;

	for (i = 0; yytext[i] != '\0'; i++)
		if (yytext[i] == '\n') {
			column = 0;
                        line++;
		} else if (yytext[i] == '\t')
			column += 8 - (column % 8);
		else
			column++;
}

