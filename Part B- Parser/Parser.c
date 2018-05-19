#define numKeywords 27
#define numOperators 18
#define numDelimiters 8
#define MaxTokenSize 100

#define keyRange 1
#define opRange 50
#define delRange 100
int  trow = 0; 
char tokens[1000][100];
char keyWords[100][40] = {"int","float","break","long","char","for","if","switch","else","while","goto","continue","return","auto","double","boolean","string","default","do","case","in","out","def","call","arrayd","arraya","arrayi"};

char operators[100][4] = { "+", "-", "*", "/", "%", "!", "?", ":", ">", "<", ":=", "==", ">=", "<=", "!=", "&&", "||", "="};
char delimiters[100][4] = { "{", "}", "(", ")", "[", "]", ";", ","};

char buffer[100];

int token_no = 1;
int line_no = 0;
int flag = 0;

// function to check for operator or delimiter for tokenization

int checker_od(char a){   
    int i;
    for(i=0;i<numOperators;i++){
        if(a == operators[i][0])
            return 1;
    }
    for(i=0;i<numDelimiters;i++){
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
    for(i=0;i<numKeywords;i++){
        if(strcmp(tok,keyWords[i]) == 0){
            strcpy(tokens[trow++], tok);
            return 0;
        }
    }

    //matching with operators
    for(i=0;i<numOperators;i++){
        if(strcmp(tok,operators[i]) == 0){
            strcpy(tokens[trow++], tok);
            return 0;
        }
    }

    //matching with delimiter
    for(i=0;i<numDelimiters;i++){
        if(strcmp(tok,delimiters[i]) == 0)
        {
            strcpy(tokens[trow++], tok);
            return 0;
        }
    }

    //dfa
    int state = 1;
    i = 0;
    char t;
    while(tok[i] != '\0' && state != 10){
        t = tok[i++];

        switch(state){
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
            strcpy(tokens[trow++], "id");
            break;
        case 4:
            strcpy(tokens[trow++], "id");
            break;
        case 5:
            strcpy(tokens[trow++], "id");
            break;
        case 8:
            strcpy(tokens[trow++], "float");
            break;
        case 9:
            strcpy(tokens[trow++], "string");
            break;
        default:
            strcpy(tokens[trow++], "invalid_token");
            break;
    }
    return state;
}


int tokenizer(char tok[100]){
    
    int i=0,curr,k=0,flag1 = 0;
    char buffer[100],c;
    while(tok[i] != '\0'){   
        curr = checker_od(tok[i]);

        if(flag1 == 1){
            if(tok[i] == '=' || tok[i] == '&' || tok[i] == '|'){
                buffer[k++] = tok[i];
                flag1 = 2;
            }
            buffer[k] = '\0';
            dfa(buffer);
            k=0;
            if(flag1 != 2){
                buffer[k++] = tok[i];
                if(curr == 2){
                    buffer[k] = '\0';
                    dfa(buffer);
                    k=0;
                }
            }
            flag1 = 0;
            i++;
            continue;
        }

        if(curr == 0)
            buffer[k++] = tok[i];

        else if(curr == 1){
            flag1 = 1;
            buffer[k] = '\0';
            dfa(buffer);
            k=0;
            buffer[k++] = tok[i];
        }

        else if(curr == 2){
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

    if(k!=0){
        buffer[k] = '\0';
        dfa(buffer);
    }
}


int scanner(char line[100]){
    
    int k=0,x , i;
    for(i=0;i<100;i++)
        buffer[i] = 0;
    for(i=0;i<strlen(line);i++){

        if(flag == 0 && line[i] == '/' && i+1 < strlen(line) && line[i+1] == '/'){
            flag = 2;
            i+=2;
        }

        if(flag == 2 && line[i] == '\n'){
            flag = 0;
            i+=1;
        }

        if(flag == 0 && line[i] == '/' && i+1 < strlen(line) && line[i+1] == '*'){
            flag = 1;
            i+=2;
        }

        if(flag == 1 && line[i] == '*' && i+1 < strlen(line) && line[i+1] == '/'){
            flag = 0;
            i+=2;
        }

        else if(flag == 0){
            if((line[i] == ' ' || line[i] == '\n' || line[i] == '\0')&& k!=0){
                buffer[k] = '\0';
                tokenizer(buffer);
                k=0;
            }

            else{
                if(line[i] != ' ' || line[i] != '\n' || line[i] != '\0')
                    buffer[k++] = line[i];
            }
        }
   }
}

int driver(char filename[])
{
    FILE *file = fopen ( filename, "r" );

    if ( file != NULL ){
        char line [100]; 
        while ( fgets ( line, sizeof line, file ) != NULL ) {
            ++line_no;
            scanner(line);

        }
        fclose ( file );
   }

   else{
      printf("unable to open file\n");
   }
   return 0;
}
