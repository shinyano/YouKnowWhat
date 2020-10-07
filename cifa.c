#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

int c,prec;
char token[200]="\0";
int num;
enum symbols{
    Begin=1,End,For,If,Then,Else,Ident,Int,Colon,Plus,Star,Comma,
    LParenthesis,RParenthesis,Assign,Unknown
} symbol;
char out[20][30]={
    "Begin","End","For","If","Then","Else","Ident","Int","Colon","Plus","Star","Comma",
    "LParenthesis","RParenthesis","Assign","Unknown"
};
FILE *fp;

void getChar();
void clearToken();
int isSpace();
int isNewline();
int isTab();
int isLetter();
int isDigit();
int isColon();
int isComma();
int isEqu();
int isPlus();
int isStar();
int isLpar();
int isRpar();
void catToken();
void retract();
int reserver();
int transNum();

int getsym()
{
    clearToken();
    getChar();
    while(isSpace() || isNewline() || isTab()) getChar();
    if(isLetter()){
        while(isLetter() || isDigit()){
            catToken();
            getChar();
        }
        retract();
        int res=reserver();
        if(res==0)
            symbol=Ident;
        else
            symbol=res;
    }
    else if(isDigit()){
        while (isDigit())
        {
            catToken();
            getChar();
        }
        retract();
        num=transNum();
        symbol=Int;
    }
    else if(isColon()){
        getChar();
        if(isEqu())
            symbol=Assign;
        else{
            retract();
            symbol=Colon;
        }
    }
    else if(isPlus()) symbol=Plus;
    else if(isStar()) symbol=Star;
    else if(isComma()) symbol=Comma;
    else if(isLpar()) symbol=LParenthesis;
    else if(isRpar()) symbol=RParenthesis;
    else{
        symbol=Unknown;
        return -1;
    }
    return 0;
}

void printSym(){
    switch (symbol){
    case Begin:
    case End:
    case For:
    case If:
    case Then:
    case Else:
    case Colon:
    case Plus:
    case Star:
    case Comma:
    case LParenthesis:
    case RParenthesis:
    case Assign:
        printf("%s\n",out[symbol-1]);
        break;
    case Ident:
        printf("Ident(%s)\n",token);
        break;
    case Int:
        printf("Int(%d)\n",num);
        break;
    default:
        break;
    }
}

int main(int argc, char **argv)
{
    fp=fopen(argv[1],"r");
    if(fp==NULL){
        printf("error:%s\n",strerror(errno));
        return -1;
    }
    int res;
    while(1){
        res=getsym();
        if(res==-1){
            printf("Unknown\n");
            return 0;
        }
        printSym(symbol);
    }
    fclose(fp);
    return 0;
}


void getChar(){
    prec=c;
    c=fgetc(fp);
    // printf("%c %c\n",c,prec);
}

void retract(){
    ungetc(c,fp);
    c=prec;
    // printf("now c is %c\n",c);
}

void clearToken(){
    memset(token,0,sizeof(token));
}

void catToken(){
    int i=0;
    while(token[i]!='\0') i++;
    token[i]=c; token[i+1]='\0';
}

int strEqu(const char *b){
    int lt=strlen(token), lb=strlen(b),i=0;
    if(lt!=lb){
        return 0;
    }
    for(i=0;i<lt;i++){
        if(token[i]!=b[i]) return 0;
    }
    return 1;
}

int reserver(){
    if(strEqu("BEGIN")) return Begin;
    else if(strEqu("END")) return End;
    else if(strEqu("FOR")) return For;
    else if(strEqu("IF")) return If;
    else if(strEqu("THEN")) return Then;
    else if(strEqu("ELSE")) return Else;
    else return 0;
}

int isSpace(){
    if(c==' ') return 1;
    else return 0;
}

int isNewline(){
    if(c=='\n' || c=='\r') return 1;
    else return 0;
}

int isTab(){
    if(c=='\t') return 1;
    return 0;
}

int isLetter(){
    if(isalpha(c)) return 1;
    else return 0;
}

int isDigit(){
    if(isdigit(c)) return 1;
    else return 0;
}

int transNum(){
    int res=0,i=0;
    for(i=0;token[i]!='\0';i++){
        res=res*10+token[i]-'0';
    }
    return res;
}

int isColon(){
    if(c==':') return 1;
    else return 0;
}

int isEqu(){
    if(c=='=') return 1;
    else return 0;
}

int isPlus(){
    if (c == '+') return 1;
    else return 0;
}

int isStar(){
    if(c=='*') return 1;
    else return 0;
}

int isComma(){
    if(c==',') return 1;
    else return 0;
}

int isLpar(){
    if(c=='(') return 1;
    else return 0;
}
int isRpar(){
    if(c==')') return 1;
    else return 0;
}