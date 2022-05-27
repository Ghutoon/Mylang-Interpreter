#include <stdbool.h>

typedef enum {typeCon, typeId, typeOpr} nodeEnum;
/*constants*/
typedef struct {
	union{
		int ival;
		double fval;
	};
	bool iORf; //true = integer constant, false = floating point constant
} conNodeType; // <--------------- *con function refers to this


/*identifiers*/
typedef struct {
	union{
		int iIndex;
		int fIndex;
	};
	bool iORf; // true = int, false = float
} idNodeType; //<---------- *id function refers to this


/*operators*/
typedef struct{
	int oper;	//operand
	int nops;	//number of operands
	struct nodeTypeTag *op[1];	/*operands extended at runtime*/ //a list of pointers of type p pointing to an expression
	// in expr -> if 
} oprNodeType; //<----------- *opr function refers to this

/**lets take the example of 
 * SIVAR '=' expr ';'
 * opr('=', 2, id($1, true), $3) is called
 * opr is getting:
 * 		oper as '='
 * 		nops as 2 (SIVAR and expr)
 * 		and id($1, true) , $3 are pointers to these two things
 * 		id($1, true) is of type p with p->type = typeCon and p->con.ival = $1, p->con.iORf = true;
 * 		$3 is of type p
**/

//return struct for ex function in mylangInterpreter.c to return int/double and a boolean variable
typedef struct{
	union{
		int inti;
		double floatie;
	};
	bool iORf;
} exReturn;



typedef struct nodeTypeTag{
	nodeEnum type;
	
	union{
		conNodeType con;
		idNodeType id;
		oprNodeType opr;
	};
} nodeType; // 
//nodeEnum type <------- type of token
//union <---- any one of the following
//either a constant
// variables id in the symbol table 
//or the type of operator it is (while, if, else, =, + etc)

extern int symInt[1400];
extern double symFloat[1200];


//HOW TO RUN
/*
bison -d mylang.y && flex mylang.l mylang.tab.h && gcc mylangInterpreter.c lex.yy.c mylang.tab.c

a.exe < printPrimes.txt
a.exe <printEven.txt
a.exe < calculateSI.txt
a.exe <fizzbuzz.txt
*/

/**
 * you can comment using # <insert text here>
 * single line comments only
**/