#include <QCoreApplication>
#include "assemble.h"
using namespace std;
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    FILE *assp,*machp;
    int countOflabels=countOfSymbolsFirstRead(assp);
    symbolTable* symT=(symbolTable*)malloc(sizeof (symbolTable)*countOflabels);
    int n=fillSymTab(symT,assp);
    instruction* instructionsArr=(instruction*)malloc(n*sizeof(instruction));
    firstReadForinstructions(assp,instructionsArr,symT,countOflabels);
    secondRead(machp,instructionsArr,n,symT,countOflabels);
    exit(0);
    return a.exec();
}
int charArray2int(char* arr)
{
    int s=1;
    int i=-1;
    int res=0;
    if(arr[0]=='-')
    {
        s=-1;
        i=0;
    }
    while(arr[++i]!='\0')
    {
        res=(res*10)+(arr[i]-'0');
    }
    return res*s;
}
int hex2int( char* hex)
{
    int result=0;
    while ((*hex)!='\0')
    {
        if (('0'<=(*hex))&&((*hex)<='9'))
            result = result*16 + (*hex) -'0';
        else if (('a'<=(*hex))&&((*hex)<='f'))
            result = result*16 + (*hex) -'a'+10;
        else if (('A'<=(*hex))&&((*hex)<='F'))
            result = result*16 + (*hex) -'A'+10;
        hex++;
    }
    return(result);
}
void int2hex16(char *lower,int a){
    sprintf(lower,"%X",a);
    if(a <0x10){
    lower[4]='\0';
    lower[3]=lower[0];
    lower[2]='0';
    lower[1]='0';
    lower[0]='0';
    }
    else if(a <0x100){
    lower[4]='\0';
    lower[3]=lower[1];
    lower[2]=lower[0];
    lower[1]='0';
    lower[0]='0';
    }
    else if(a <0x1000){
    lower[4]='\0';
    lower[3]=lower[2];
    lower[2]=lower[1];
    lower[1]=lower[0];
    lower[0]='0';
    }
}

int fillSymTab(struct symbolTable *symT,FILE *inputFile){
   int lineNo=0;
   size_t lineSize=72;
   char *line;
   int i=0;
   char *token;
   line=(char *)malloc(72);
   fopen_s(&inputFile,"test.txt","r");
   while(fgets(line, lineSize,inputFile) !=NULL){
       if((line[0] == ' ') || (line[0] == '\t'));
      else{
       token=strtok(line,"\t, ");
       strcpy(symT[i].symbol,token);
       symT[i].value=lineNo;
       token=strtok(NULL,"\t");
       if(i!=0)
       {
          for(int j=0;j<i;j++)
           if(strcmp(symT[i].symbol,symT[j].symbol)==0)
           {
               printf("wrong. a lebel defined more than 1 time\n");
               exit(1);
           }
       }
       i++;
      }
      lineNo++;
   }
   rewind(inputFile);
   free(line);
   fclose(inputFile);
   return lineNo;
}
 int countOfSymbolsFirstRead(FILE* inputFile)
 {
     size_t lineSize=72;
     char *line;
     int i=0;
     line=(char *)malloc(72);
     fopen_s(&inputFile,"test.txt","r");
     while(fgets(line, lineSize,inputFile) !=NULL)
     {
        if((line[0] != ' ') && (line[0] != '\t'))
            i++;
     }
     rewind(inputFile);
     free(line);
     fclose(inputFile);
     return i;
 }
 void firstReadForinstructions(FILE* inputFile,instruction* instructionsArr,symbolTable* symT,int size)
 {
     size_t lineSize=72;
     int j=-1;
     char *line;
     int i=0;
     char *token;
     line=(char *)malloc(72);
     fopen_s(&inputFile,"test.txt","r");
     while(fgets(line, lineSize,inputFile) !=NULL)
     {
         j++;
         line=strtok(line,"\n");
         token=strtok(line,"\t, ");
         if((line[0] == ' ') || (line[0] == '\t'))
         {

              strcpy(instructionsArr[j].mnemonic,token);
         }
         else
         {
             token = strtok(nullptr,"\t");
             token = strtok(token," ");
             strcpy(instructionsArr[j].mnemonic,token);
         }

         token = strtok(nullptr," ");
         if(strcmp(instructionsArr[j].mnemonic,"add")==0 ||strcmp(instructionsArr[j].mnemonic,"sub")==0 ||
                 strcmp(instructionsArr[j].mnemonic,"slt")==0 ||strcmp(instructionsArr[j].mnemonic,"or")==0||strcmp(instructionsArr[j].mnemonic,"nand")==0)
         {
                instructionsArr[j].instType=RTYPE;
               token=strtok(token,",");
               instructionsArr[j].rd=charArray2int(token);
               token=strtok(nullptr,",");
               instructionsArr[j].rs=charArray2int(token);
               token=strtok(nullptr,",");
         //       token=strtok(token,"\n");
               token=strtok(token," ");
               instructionsArr[j].rt=charArray2int(token);
               if(strcmp(instructionsArr[j].mnemonic,"add")==0)
                   instructionsArr[j].opCode=0;
               else if(strcmp(instructionsArr[j].mnemonic,"sub")==0)
                       instructionsArr[j].opCode=1;
               else if(strcmp(instructionsArr[j].mnemonic,"slt")==0)
                       instructionsArr[j].opCode=2;
               else if(strcmp(instructionsArr[j].mnemonic,"or")==0)
                       instructionsArr[j].opCode=3;
               else if(strcmp(instructionsArr[j].mnemonic,"nand")==0)
                       instructionsArr[j].opCode=4;
               instructionsArr[j].imm=0;
         }
         else if(strcmp(instructionsArr[j].mnemonic,"addi")==0 || strcmp(instructionsArr[j].mnemonic,"slti")==0
                 || strcmp(instructionsArr[j].mnemonic,"ori")==0)
         {
             instructionsArr[j].instType=ITYPE;
             token=strtok(token,",");
             instructionsArr[j].rt=charArray2int(token);
             token=strtok(nullptr,",");
             instructionsArr[j].rs=charArray2int(token);
             token=strtok(nullptr,",");
         //    token=strtok(token,"\n");
             token=strtok(token,"#, ");
             instructionsArr[j].imm=charArray2int(token);
             if(strcmp(instructionsArr[j].mnemonic,"addi")==0)
                 instructionsArr[j].opCode=5;
             else if(strcmp(instructionsArr[j].mnemonic,"slti")==0)
                 instructionsArr[j].opCode=6;
             else if(strcmp(instructionsArr[j].mnemonic,"ori")==0)
                 instructionsArr[j].opCode=7;
         }
         else if(strcmp(instructionsArr[j].mnemonic,"lui")==0)
         {
             instructionsArr[j].instType=ITYPE;
             token=strtok(token,",");
             instructionsArr[j].rt=charArray2int(token);
             token=strtok(nullptr,",");
         //    token=strtok(token,"\n");
             token=strtok(token,"#, ");
             instructionsArr[j].imm=charArray2int(token);
             instructionsArr[j].rs=0;
             instructionsArr[j].opCode=8;
         }
         else if(strcmp(instructionsArr[j].mnemonic,"lw")==0 || strcmp(instructionsArr[j].mnemonic,"sw")==0
                 || strcmp(instructionsArr[j].mnemonic,"beq")==0)
         {
             instructionsArr[j].instType=ITYPE;
             token=strtok(token,",");
             instructionsArr[j].rt=charArray2int(token);
             token=strtok(nullptr,",");
             instructionsArr[j].rs=charArray2int(token);
             token=strtok(nullptr,",");
          //   token=strtok(token,"\n");
             token=strtok(token,"#, ");
             if((token[0]>='0' && token[0]<='9')||token[0]=='-')
                 instructionsArr[j].imm=charArray2int(token);
             else
             {
                 instructionsArr[j].imm=findLabelAddress(symT,size,token);
                if(strcmp(instructionsArr[j].mnemonic,"beq")==0)
                    instructionsArr[j].imm=findLabelAddress(symT,size,token)-(j+1);
             }

             if(strcmp(instructionsArr[j].mnemonic,"lw")==0)
                 instructionsArr[j].opCode=9;
             if(strcmp(instructionsArr[j].mnemonic,"sw")==0)
                 instructionsArr[j].opCode=10;
             if(strcmp(instructionsArr[j].mnemonic,"beq")==0)
                 instructionsArr[j].opCode=11;
         }
         else if(strcmp(instructionsArr[j].mnemonic,"jalr")==0)
         {
             instructionsArr[j].instType=ITYPE;
             token=strtok(token,",");
             instructionsArr[j].rt=charArray2int(token);
             token=strtok(nullptr,",");
         //    token=strtok(token,"\n");
             token=strtok(token,"#, ");
             instructionsArr[j].rs=charArray2int(token);
             instructionsArr[j].imm=0;
             instructionsArr[j].opCode=12;
         }
          else if(strcmp(instructionsArr[j].mnemonic,"j")==0)
         {
           instructionsArr[j].instType=JTYPE;
         //  token=strtok(token,"\n");
           token=strtok(token,"#, ");
           if((token[0]>='0' && token[0]<='9')|| token[0]=='-')
               instructionsArr[j].imm=charArray2int(token);
           else
               instructionsArr[j].imm=findLabelAddress(symT,size,token);
           instructionsArr[j].opCode=13;
         }
         else if(strcmp(instructionsArr[j].mnemonic,"halt")==0)
         {
          //   token=strtok(token,"\n");
             token=strtok(token,"#, ");
             instructionsArr[j].instType=JTYPE;
             instructionsArr[j].opCode=14;
         }
         else if(strcmp(instructionsArr[j].mnemonic,".fill")==0)
         {
             instructionsArr[j].instType=-1;
           //  token=strtok(token,"\n");
             token=strtok(token,"#, ");
             instructionsArr[j].imm=0;
             if((token[0]>='0' && token[0]<='9')||token[0]=='-')
                 instructionsArr[j].fill=charArray2int(token);
             else
                 instructionsArr[j].fill=findLabelAddress(symT,size,token);
         }
         else if(strcmp(instructionsArr[j].mnemonic,".space")==0)
         {
             instructionsArr[j].instType=-2;
              instructionsArr[j].imm=0;
          //   token=strtok(token,"\n");
             token=strtok(token,"#, ");
             if((token[0]>='0' && token[0]<='9')||token[0]=='-')
                 instructionsArr[j].space=charArray2int(token);
             else
                 instructionsArr[j].space=findLabelAddress(symT,size,token);
         }

         else {
             printf("wrong opcode for instruction %i   -       %s       -\n",j+1,instructionsArr[j].mnemonic);
             exit(1);
         }
         if(instructionsArr[j].imm>65535 || instructionsArr[j].imm<-32767)
         {
             printf("ofsset in instruction %i is more than 16 bits   -          %i          -",j+1,instructionsArr[j].imm);
             exit(1);
         }


     }
     rewind(inputFile);
     free(line);
     fclose(inputFile);
 }
 int findLabelAddress(symbolTable* symT,int size,char* labelName)
 {
     for(int i=0;i<size;i++)
         if(strcmp(symT[i].symbol,labelName)==0)
             return symT[i].value;
     printf("wrong name for label - %s\n",labelName);
     exit(1);
 }
 void secondRead(FILE* outputFile,instruction* instructionsArr,int size,symbolTable* symT,int countOfSymbols)
 {
     int pc=0;
     char hexCode[9];
     char temp[5];
    fopen_s(&outputFile,"outPut.txt","w");
    for(int j=0;j<size;j++)
    {

        if(instructionsArr[j].instType==-1)
        {
            fprintf(outputFile,"%i\n",instructionsArr[j].fill);
            continue;
        }
        if(instructionsArr[j].instType==-2)
        {
          //  fprintf(outputFile,"%i\n",0);
            int2hex16(temp,instructionsArr[j].space);
            for(int i=5;i<=7;i++)
                hexCode[i]=temp[i-5];
            hexCode[4]='0';
            hexCode[3]='0';
            hexCode[2]='0';
            sprintf(temp,"%X",15);
            hexCode[1]=temp[0];
            hexCode[0]='0';
            fprintf(outputFile,"%i\n",hex2int(hexCode));
            continue;
        }
        else if(instructionsArr[j].instType==RTYPE)
        {
            for(int i=5;i<=7;i++)
                hexCode[i]='0';
            sprintf(temp,"%X",instructionsArr[j].rd);
            hexCode[4]=temp[0];
            sprintf(temp,"%X",instructionsArr[j].rt);
            hexCode[3]=temp[0];
            sprintf(temp,"%X",instructionsArr[j].rs);
            hexCode[2]=temp[0];
            sprintf(temp,"%X",instructionsArr[j].opCode);
            hexCode[1]=temp[0];
            hexCode[0]='0';

        }
        else if(instructionsArr[j].instType==ITYPE)
        {
           int2hex16(temp,instructionsArr[j].imm);
           for(int i=4;i<=7;i++)
               hexCode[i]=temp[i-4];
           sprintf(temp,"%X",instructionsArr[j].rt);
           hexCode[3]=temp[0];
           sprintf(temp,"%X",instructionsArr[j].rs);
           hexCode[2]=temp[0];
           sprintf(temp,"%X",instructionsArr[j].opCode);
           hexCode[1]=temp[0];
           hexCode[0]='0';

        }
        else if(instructionsArr[j].instType=JTYPE)
        {
            if(strcmp(instructionsArr[j].mnemonic,"j")==0)
            {
                int2hex16(temp,instructionsArr[j].imm);
                for(int i=4;i<=7;i++)
                    hexCode[i]=temp[i-4];
                hexCode[3]='0';
                hexCode[2]='0';
                sprintf(temp,"%X",instructionsArr[j].opCode);
                hexCode[1]=temp[0];
                hexCode[0]='0';
            }
            else if(strcmp(instructionsArr[j].mnemonic,"halt")==0)
            {
                for(int i=4;i<=7;i++)
                    hexCode[i]='0';
                hexCode[3]='0';
                hexCode[2]='0';
                sprintf(temp,"%X",instructionsArr[j].opCode);
                hexCode[1]=temp[0];
                hexCode[0]='0';
            }
        }
        fprintf(outputFile,"%i\n",hex2int(hexCode));
    }
 }

