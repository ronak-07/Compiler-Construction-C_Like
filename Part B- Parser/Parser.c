#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "Parser.h"

#define N 10000
#define no_NT 33
#define no_T 36

char stack[N][50];
char stack_top[N];
int top;
int rule_number;
int curr_trow=0;

char productions[100][500]={
	"",
	"S -> GLOBALS FUNCTIONS { STATEMENTS }",
	"GLOBALS -> DECLARATIONS",
	"FUNCTIONS -> FUNCTION FUNCTIONS",
	"FUNCTIONS -> ''",
	"FUNCTION -> def DATATYPE id ( PARAMETERS ) { STATEMENTS RETURN }",
	"DECLARATIONS -> DEFINITION ASSIGNMENT DECLARATIONS",
	"DECLARATIONS -> ''",
	"DEFINITION -> DATATYPE id",
	"DEFINITION -> ''",
	"ASSIGNMENT -> id = SEQ",
	"ASSIGNMENT -> ''",
	"PARAMETERS -> DEFINITION C PARAMETERS",
	"PARAMETERS -> ''",
	"C -> ,",
	"C -> ''",
	"DATATYPE -> int",
	"DATATYPE -> float",
	"DATATYPE -> char",
	"DATATYPE -> string",
	"RETURN -> return SEQ",
	"STATEMENTS -> STATEMENT STATEMENTS",
	"STATEMENTS -> ''",
	"STATEMENT -> SEQ",
	"STATEMENT -> COND",
	"STATEMENT -> LOOP",
	"STATEMENT -> FUNC_CALL",
	"STATEMENT -> ARRAYDECLARATION",
	"STATEMENT -> ARRAYACCESS",
	"STATEMENT -> ARRAYINITIALIZATION",
	"STATEMENT -> IO",
	"STATEMENT -> DECLARATIONS",
	"SEQ -> ARITH",
	"SEQ -> RELATIONAL",
	"SEQ -> ''",
	"ARITH -> E",
	"E -> T E'",
	"E' -> + T E'",
	"E' -> ''",
	"T -> F T'",
	"T' -> * F T'",
	"T' -> ''",
	"F -> id",
	"FUNC_CALL -> call ( id ( idLIST ) id )",
	"idLIST -> id C idLIST",
	"idLIST -> ''",
	"IO -> INPUT",
	"IO -> OUTPUT",
	"INPUT -> in idLIST",
	"OUTPUT -> out SEQ",
	"COND -> if ( RELATIONAL ) { STATEMENTS } else { STATEMENTS }",
	"LOOP -> for ( id ; RELATIONAL ; id = ARITH ) { STATEMENTS }",
	"ARRAYDECLARATION -> arrayd ( id , id )",
	"ARRAYACCESS -> arraya ( id , id , id )",
	"ARRAYINITIALIZATION -> arrayi ( id , id , ARITH )",
	"RELATIONAL -> ARITH OPERATOR ARITH",
	"OPERATOR -> <=",
	"OPERATOR -> >=",
	"OPERATOR -> <",
	"OPERATOR -> >",
	"OPERATOR -> ==",
	"OPERATOR -> +",
	"OPERATOR -> -",
	"OPERATOR -> *",
	"OPERATOR -> /",
	"OPERATOR -> ''"
};

char non_terminals[100][100] = {
	"",
	"S",
	"GLOBALS",
	"FUNCTIONS",
	"FUNCTION",
	"DECLARATIONS",
	"DEFINITION",
	"ASSIGNMENT",
	"PARAMETERS",
	"C",
	"DATATYPE",
	"RETURN",
	"STATEMENTS",
	"STATEMENT",
	"SEQ",
	"ARITH",
	"E",
	"E'",
	"T",
	"T'",
	"F",
	"FUNC_CALL",
	"idLIST",
	"IO",
	"INPUT",
	"OUTPUT",
	"COND",
	"LOOP",
	"ARRAYDECLARATION",
	"ARRAYACCESS",
	"ARRAYINITIALIZATION",
	"RELATIONAL",
	"OPERATOR"
};


char terminals[100][100] = {
	"",
	"{","}","''","def","id","(",")","=",",","int","float","char","string","return","+","*","call","in","out","if","else","for",";","arrayd","arraya","arrayi","<=",">=","<",">","==","-","/","$"
};

int transition_table[100][100] = {
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,67,67,1,67,67,67,67,67,67,1,1,1,1,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,66},{0,67,67,2,66,67,67,67,67,67,2,2,2,2,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67},{0,66,67,4,3,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67},{0,67,67,66,5,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67},{0,67,67,7,66,66,67,67,67,67,6,6,6,6,67,67,67,66,66,66,66,67,66,67,66,66,66,67,67,67,67,67,67,67,67},{0,67,67,9,67,66,67,67,67,66,8,8,8,8,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67},{0,67,67,11,67,10,67,67,67,67,66,66,66,66,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67},{0,67,67,13,67,67,67,66,67,67,12,12,12,12,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67},{0,67,67,15,67,66,67,67,67,14,66,66,66,66,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67},{0,67,67,67,67,66,67,67,67,67,16,17,18,19,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67},{0,67,66,67,67,67,67,67,67,67,67,67,67,67,20,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67},{0,67,66,22,67,21,67,67,67,67,21,21,21,21,66,67,67,21,21,21,21,67,21,67,21,21,21,67,67,67,67,67,67,67,67},{0,67,67,31,67,23,67,67,67,67,31,31,31,31,67,67,67,26,30,30,24,67,25,67,27,28,29,67,67,67,67,67,67,67,67},{0,67,66,34,67,33,67,67,67,67,66,66,66,66,67,67,67,66,66,66,66,67,66,67,66,66,66,67,67,67,67,67,67,67,67},{0,67,66,66,67,35,67,66,67,67,66,66,66,66,67,66,66,66,66,66,66,67,66,66,66,66,66,66,66,66,66,66,66,66,67},{0,67,66,66,67,36,67,66,67,67,66,66,66,66,67,66,66,66,66,66,66,67,66,66,66,66,66,66,66,66,66,66,66,66,67},{0,67,66,38,67,66,67,66,67,67,66,66,66,66,67,37,66,66,66,66,66,67,66,66,66,66,66,66,66,66,66,66,66,66,67},{0,67,67,66,67,39,67,67,67,67,67,67,67,67,67,66,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67},{0,67,67,41,67,67,67,67,67,67,67,67,67,67,67,66,40,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67},{0,67,67,66,67,42,67,67,67,67,67,67,67,67,67,67,66,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67,67},{0,67,67,66,67,66,67,67,67,67,66,66,66,66,67,67,67,43,66,66,66,67,66,67,66,66,66,67,67,67,67,67,67,67,67},{0,67,67,45,67,44,67,66,67,67,66,66,66,66,67,67,67,66,66,66,66,67,66,67,66,66,66,67,67,67,67,67,67,67,67},{0,67,67,66,67,66,67,67,67,67,66,66,66,66,67,67,67,66,46,47,66,67,66,67,66,66,66,67,67,67,67,67,67,67,67},{0,67,67,66,67,66,67,67,67,67,66,66,66,66,67,67,67,66,48,66,66,67,66,67,66,66,66,67,67,67,67,67,67,67,67},{0,67,67,66,67,66,67,67,67,67,66,66,66,66,67,67,67,66,66,49,66,67,66,67,66,66,66,67,67,67,67,67,67,67,67},{0,67,67,66,67,66,67,67,67,67,66,66,66,66,67,67,67,66,66,66,50,67,66,67,66,66,66,67,67,67,67,67,67,67,67},{0,67,67,66,67,66,67,67,67,67,66,66,66,66,67,67,67,66,66,66,66,67,51,67,66,66,66,67,67,67,67,67,67,67,67},{0,67,67,66,67,66,67,67,67,67,66,66,66,66,67,67,67,66,66,66,66,67,66,67,52,66,66,67,67,67,67,67,67,67,67},{0,67,67,66,67,66,67,67,67,67,66,66,66,66,67,67,67,66,66,66,66,67,66,67,66,53,66,67,67,67,67,67,67,67,67},{0,67,67,66,67,66,67,67,67,67,66,66,66,66,67,67,67,66,66,66,66,67,66,67,66,66,54,67,67,67,67,67,67,67,67},{0,67,66,66,67,55,67,66,67,67,66,66,66,66,67,67,67,66,66,66,66,67,66,66,66,66,66,67,67,67,67,67,67,67,67},{0,67,67,65,67,66,67,67,67,67,67,67,67,67,67,61,63,67,67,67,67,67,67,67,67,67,67,56,57,58,59,60,62,64,67}
};


void push(char const* str)
{
	if(top == N)
	{
		printf("Stack Overflow!\n");
		exit(1);
	}
	if(!strcmp("''", str))
		return;
	strcpy(stack[top], str);
	top++;	
} 

void pop(){
	top--;
	if(top == -1)
	{
		printf("Stack Underflow!\n");
		exit(1);
	}
}

int check_nt(char const* a){
	int i=0;
	for(i=0;i<no_NT;i++){
		if(strcmp(non_terminals[i], a)==0){
			return 1;
		}
	}
	return 0;
}

int find_terminal(char const* a){
	int i=0;
	while(i<no_T){
		if(strcmp(terminals[i++], a)==0)
			return --i;
	}
}

int find_non_terminal(char const* a){
	int i=0;
	while(i<no_NT){
		if(strcmp(non_terminals[i++], a)==0)
			return --i;
	}
}

int shift_move(int production_number)
{
	printf("%d\n", production_number);
	pop();

	if(production_number == 0){
		printf("Invalid transition!\n");
		return -1;
	}
	
	// Operator is null
	if(production_number == 66)
		printf("\nSYNC ERROR\n");
	
	// No Rule defined
	if(production_number == 67)
		printf("\nPOP ERROR\n");

	char production[500];

	strcpy(production, productions[production_number]);

	char* token = strtok (production," \n\t");

	char temp_stack[100][50];
	int temp_top=0;
	int flag=0;

	while(token!=NULL)
	{
		if(strcmp("->", token)==0)
			flag = 1;
		if(flag)
			strcpy(temp_stack[temp_top++], token);
		token = strtok(NULL, " \n\t");
	}

	while(temp_top>0)
	{
		if(strcmp("->", temp_stack[temp_top-1])!=0)
			push(temp_stack[temp_top-1]);
		temp_top--;
	}
	return 0;
}

int print_contents(){
	int i;
	printf("\n\n\nINPUTS:\n");
	for(i=curr_trow;i<trow;i++)
		printf("%s ", tokens[i]);

	printf("\n");

	printf("STACK:\n");
	for(i=top-1;i>=0;i--)
		printf("%s ", stack[i]);

	printf("\n");

	return 0;
}

int main(){
	char filename[100];
    scanf("%s",filename);
	
	//Driver Function has been defined in Parser.h
    driver(filename);
    
    // Push $ and S
    push("$");
    push("S");
    
    // Tokens defined in Parser.h
    strcpy(tokens[trow++], "$");
    
    
    while(curr_trow < trow)
	{
    	print_contents();

    	strcpy(stack_top,stack[top-1]);
    	int is_nt = check_nt(stack_top);
		// Top of Stack is Non Terminal
    	if(is_nt)
		{
    		int ter_no = find_terminal(tokens[curr_trow]);
    		int non_ter_no = find_non_terminal(stack_top);

    		rule_number = transition_table[non_ter_no][ter_no];

    		printf("PRODUCTION:\n");
    		printf("%s\n",productions[rule_number] );

    		shift_move(rule_number);
    	}
    	// Top of Stack is Terminal
    	else
		{
			// Terminal - terminal match , therefore pop()
    		if(strcmp(tokens[curr_trow],stack_top)==0)
			{
    			printf("\nMATCH!\n");
    			pop();
    		}
    		
    		// Mismatch. Error.
    		else
			{	
    			printf("\nMISMATCH... SYNC ERROR!\n!\n");
    			pop();
    		}
    		curr_trow++;
    	}
    }
	return 0;
}

