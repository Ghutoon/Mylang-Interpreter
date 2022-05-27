%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <stdarg.h>
	#include "mylang.h"
	#include <stdbool.h>
	
	/*prototypes*/
	nodeType *opr(int oper, int nops, ...);
	nodeType *id(int i, bool which); //bool = =true -> integer symbol table entry else float sym table entry
	nodeType *con(int value, double dvalue, bool which); //true int false float
	void freeNode(nodeType *p);
	exReturn ex(nodeType *p);
	int yylex(void);

	void yyerror(char *s);
	int symInt[1400];
	double symFloat[1200];
%}

%union {
	int ivalue;
	double fvalue;
	int sIndex;
	nodeType *nPtr;
};

%token <ivalue> INTEGER
%token <fvalue>	FLOATIE
%token <sIndex>	SIVAR FPVAR
%token WHILE IF DISPLAY
%nonassoc IFX
%nonassoc ELSE

%left GE LE EQ NE '>' '<'
%left '+' '-'
%left '*' '/' '%'
%nonassoc  UMINUS
%right '^'

%type <nPtr> stmt iex fex stmt_list expr

%%

program:
	function		{exit(0);}
	;

function:
	function stmt 	{ex($2).inti; freeNode($2);}
	| //null
	;

stmt:	
			';'									{$$ = opr(';', 2, NULL, NULL);}
		| 	expr ';' 							{ $$ = $1; }
		| 	DISPLAY expr ';' 					{ $$ = opr(DISPLAY, 1, $2); }
		| 	SIVAR '=' expr ';' 					{ $$ = opr('=', 2, id($1, true), $3); }
		| 	FPVAR '=' expr ';' 					{ $$ = opr('=', 2, id($1, false), $3); }
		| 	WHILE '(' expr ')' stmt 			{ $$ = opr(WHILE, 2, $3, $5); }
		| 	IF '(' expr ')' stmt %prec IFX 		{ $$ = opr(IF, 2, $3, $5); } //%prec tells yacc to use the precedence of IFX for this piece of code
		| 	IF '(' expr ')' stmt ELSE stmt 		{ $$ = opr(IF, 3, $3, $5, $7); }
		| 	'{' stmt_list '}' 					{ $$ = $2; }
		;

stmt_list:	
			stmt 					{$$=$1;}
		|	stmt_list stmt 			{$$ = opr(';', 2, $1, $2);}
		;


expr: 
			iex						{$$ = $1;}
		|	fex						{$$ = $1;}
		|	UMINUS expr				{$$ = opr(UMINUS, 1, $2);}
		|	expr '+' expr 			{ $$ = opr('+', 2, $1, $3); }
		|	expr '-' expr 			{ $$ = opr('-', 2, $1, $3); }
		|	expr '*' expr 			{ $$ = opr('*', 2, $1, $3); }
		|	expr '/' expr 			{ $$ = opr('/', 2, $1, $3); }
		|	expr '%' expr 			{ $$ = opr('%', 2, $1, $3); }
		|	expr '^' expr 			{ $$ = opr('^', 2, $1, $3); }
		|	expr '<' expr 			{ $$ = opr('<', 2, $1, $3); }
		|	expr '>' expr 			{ $$ = opr('>', 2, $1, $3); }
		|	expr GE expr 			{ $$ = opr(GE, 2, $1, $3); }
		|	expr LE expr 			{ $$ = opr(LE, 2, $1, $3); }
		|	expr NE expr 			{ $$ = opr(NE, 2, $1, $3); }
		|	expr EQ expr 			{ $$ = opr(EQ, 2, $1, $3); }
		|	'(' expr ')' 			{ $$ = $2; }
		;

iex:
			INTEGER					{$$ = con($1, 0.0, true);}
		|	SIVAR					{$$ = id($1, true);}
		;		

fex:
			FLOATIE					{$$ = con(0, $1, false);}
		|	FPVAR					{$$ = id($1, false);}
		;

%%

#define SIZEOF_NODETYPE ((char *)&p->con - (char *)p)

nodeType *con(int value, double dvalue, bool which)
{
	nodeType *p;
	/* allocate node */
	if ((p = malloc(sizeof(nodeType))) == NULL)
		yyerror("out of memory");
	
	/* copy information */
	p->type = typeCon;
	p->con.iORf = which;
	if(which)
		p->con.ival = value; //or maybe p->con.ival
	else
		p->con.fval = dvalue;
	
	return p;
}

nodeType *id(int index, bool which)
{
	nodeType *p;

	/* allocate node */
	if ((p = malloc(sizeof(nodeType))) == NULL)
		yyerror("out of memory");
	
	/* copy information */
	p->type = typeId;
	p->id.iORf = which;
	if(which)
		p->id.iIndex = index;
	else
		p->id.fIndex = index;
	
	return p;
}

nodeType *opr(int oper, int nops, ...)
{
	va_list ap;
	nodeType *p;
	int i;
	/* allocate node, extending op array */
	

	if ((p = malloc(sizeof(nodeType) + (nops-1) * sizeof(nodeType *))) == NULL)
		yyerror("out of memory");
	
	/* copy information */
	p->type = typeOpr;
	p->opr.oper = oper;
	p->opr.nops = nops;
	
	va_start(ap, nops);
	for (i = 0; i < nops; i++)
		p->opr.op[i] = va_arg(ap, nodeType*);

	va_end(ap);
	return p;
}



void freeNode(nodeType *p)
{
	int i;
	if (!p)
		return;
	if(p-> type == typeOpr)
		for (i = 0; i < p->opr.nops; i++)
			freeNode(p->opr.op[i]);
	free(p);
}
void yyerror(char *s)
{
	fprintf(stdout, "%s\n", s);
}

int main(void)
{
	return yyparse();
}