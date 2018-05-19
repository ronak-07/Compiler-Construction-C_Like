#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#define numKeywords 20
#define numOperators 18
#define numDelimiters 8
#define MaxTokenSize 100

#define keyRange 1
#define opRange 50
#define delRange 100
char keyWords[100][40] = {"int","float","break","long","char","for","if","switch","else","while","goto","continue","return","auto","double","boolean","string","default","do","case"};

char operators[100][4] = { "+", "-", "*", "/", "%", "!", "?", ":", ">", "<", ":=", "==", ">=", "<=", "!=", "&&", "||", "="};
char delimiters[100][4] = { "{", "}", "(", ")", "[", "]", ";", ","};

char buffer[100];

int token_no = 1;
int line_no = 0;
int flag = 0;

// Function to check for operator(return 1) or delimiter(return 2) for tokenization
int checker_od(char a){   
    int i;
    for(i=0;i<numOperators;i++)
	{
        if(a == operators[i][0])
            return 1;
    }
    for(i=0;i<numDelimiters;i++)
	{	
        if(a == delimiters[i][0])
            return 2;
    }
    return 0;
}

//lexer dfa

int dfa(char tok[100]){
    int i;

    //invalid token
    if(strlen(tok) == 0)
        return 0;

    //matching with keywords
    for(i=0;i<numKeywords;i++)
	{
        if(strcmp(tok,keyWords[i]) == 0)
		{
            printf("Token %d, keyword %s, line number %d\n",keyRange+i,tok,line_no);
            return 0;
        }
    }

    //matching with operators
    for(i=0;i<numOperators;i++)
	{
        if(strcmp(tok,operators[i]) == 0)
		{
            printf("Token %d, operator %s, line number %d\n",opRange+i,tok,line_no);
            return 0;
        }
    }

    //matching with delimiter
    for(i=0;i<numDelimiters;i++)
	{
        if(strcmp(tok,delimiters[i]) == 0)
        {
            printf("Token %d, delimiter %s, line number %d\n",delRange+i,tok,line_no);
            return 0;
        }
    }

    //DFA for others
    int state = 1;
    i = 0;
    char t;
    // State 10 represents accepted state or final state
    while(tok[i] != '\0' && state != 10)
	{
        t = tok[i++];
        switch(state)
		{
            case 1:
                if(isalpha(t))
                    state = 2;
                else if(t == '+' || t == '-')
                    state = 3;
                else if(isdigit(t) && t != '0')
                    state = 4;
                else if(t == '0')
                    state = 5;
                else if(t == '"')
                    state = 6;
                else
                    state = 10;
                break;
            case 2:
                if(isalpha(t) || isdigit(t) || t == '_')
                    state = 2;
                else
                    state = 10;
                break;
            case 3:
                if(isdigit(t) && t != '0')
                    state = 4;
                else if(t == '0')
                    state = 11;
                else
                    state = 10;
                break;
            case 4:
                if(isdigit(t))
                    state = 4;
                else if(t == '.')
                    state = 7;
                else 
                    state = 10;
                break;
            case 5:
                if(t == '.')
                    state = 7;
                else
                    state = 10;
                break;
            case 6:
                if(t != '"')
                    state = 6;
                else
                    state = 9;
                break;
            case 7:
                if(isdigit(t))
                    state = 8;
                else 
                    state = 10;
                break;
            case 8:
                if(isdigit(t))
                    state = 8;
                else
                    state = 10;
                break;
            case 9:
                state = 10;
                break;
            case 10:
                printf("invalid token\n");
                return 10;
            case 11:
                if(t == '.')
                    state = 7;
                else
                    state = 10;
        }
    }

    char type[10];
    
    switch(state){
        case 2:
        		// Identifier
            printf("Token %d, id %s, line number %d\n",150,tok,line_no);
            break;
        case 4:
        		//Integers from 1-9
            printf("Token %d, integer %s, line number %d\n",151,tok,line_no);
            break;
        case 5:
        	    // Specifically for integer =0
            printf("Token %d, integer %s, line number %d\n",152,tok,line_no);
            break;
        case 8:
        	// Float
            printf("Token %d, float%s, line number %d\n",153,tok,line_no);
            break;
        case 9:
        	//empty string
            printf("Token %d, string %s, line number %d\n",154,tok,line_no);
            break;
        default:
            printf("Token %d, invalid %s, line number %d.......\n",404,tok,line_no);
            break;
    }
    return state;
}

//Input: words. For Example: "ronak"	"yash"	"kartik"	"for"	"while"
int tokenizer(char tok[100])
{    
    int i=0,curr,k=0,flag1 = 0;
    char buffer[100],c;
    while(tok[i] != '\0')
	{   
		//Check for operator and delimiter
        curr = checker_od(tok[i]);
        
        // Operator has been read
        if(flag1 == 1)
		{
			//Boolean op.
            if(tok[i] == '=' || tok[i] == '&' || tok[i] == '|')
			{
                buffer[k++] = tok[i];
                flag1 = 2;
            }
            buffer[k] = '\0';
            dfa(buffer);
            k=0;
            if(flag1 != 2)
			{
                buffer[k++] = tok[i];
            }
            flag1 = 0;
            i++;
            continue;
        }
        // Neither of Op or Delimiter
        if(curr == 0)
            buffer[k++] = tok[i];
		
		// tok[i] is an operator
        else if(curr == 1)
		{
            flag1 = 1;
            buffer[k] = '\0';
            dfa(buffer);
            k=0;
            buffer[k++] = tok[i];
        }
		// tok[i] is delimiter
        else if(curr == 2)
		{
            buffer[k] = '\0';
            dfa(buffer);
            k=0;
            buffer[k++] = tok[i];
            buffer[k] = '\0';
            dfa(buffer);
            k=0;
        }
        i++;
    }
    //tok ends but buffer still isnt empty like identifiers
    if(k!=0) 
	{
        buffer[k] = '\0';
        dfa(buffer);
    }
}


int scanner(char line[100]){
    
    int k=0,x , i;
    for(i=0;i<100;i++)
        buffer[i] = 0;
    for(i=0;i<strlen(line);i++)
	{
		// Case for Double Slash
        if(flag == 0 && line[i] == '/' && i+1 < strlen(line) && line[i+1] == '/')
		{
            flag = 2;
            i+=2;
        }
		// Traversing in double slash comment
        if(flag == 2 && line[i] == '\n')
		{	
            flag = 0;
            i+=1;
        }
		// Multi Line Comment Start
        if(flag == 0 && line[i] == '/' && i+1 < strlen(line) && line[i+1] == '*'){
            flag = 1;
            i+=2;
        }
		// Multi Line Comment End
        if(flag == 1 && line[i] == '*' && i+1 < strlen(line) && line[i+1] == '/'){
            flag = 0;
            i+=2;
        }
		// Regular Lines: Not in comments
        else if(flag == 0)
		{
			// K iterates over buffer. Case for space, new line or end of file
            if((line[i] == ' ' || line[i] == '\n' || line[i] == '\0') && k!=0)
			{
                buffer[k] = '\0';
                tokenizer(buffer);
                k=0;
            }
            else
			{
                if(line[i] != ' ' || line[i] != '\n' || line[i] != '\0')
                    buffer[k++] = line[i];
            }
        }
   }
}

int main ( void )
{    
    char filename[300];
    scanf("%s",filename);
    FILE *file = fopen ( filename, "r" );

    if ( file != NULL )
	{
        char line [100]; 
        while ( fgets ( line, sizeof line, file ) != NULL ) 
		{
			++line_no;
            scanner(line);

        }
        fclose ( file );
   	}

   else
   {
      printf("unable to open file\n");
   }
   return 0;
}
