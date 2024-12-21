#ifndef FICHIER_ENTETE_H_INCLUDED
#define FICHIER_ENTETE_H_INCLUDED
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include <string.h>
//enumeration des tokens 
typedef enum
{
    IF,ELSE,THEN,LET,GRET,EQUAL,
    LET_EQUAL,GRET_EQUAL,DIFF,
    ID,NB
}TOKEN;
/*les variable gobales du programme*/
int state=0;//defini l'etat courant dant l'automate
int start=0;//difini dans quel automates on se trouve
int lexical_value;
int line=1;//pour le comptage de lignes dans le fichier
int cmp=0;
/*Les protypes des fonctions utilisees*/
int Fail();
TOKEN Next_Token(FILE *fichier);
char Next_char(FILE *fichier);
void Retract(FILE *fichier,int nb_pas);
void Token_to_string(TOKEN tkn,char string[20]);
void analyseur_Lexical(FILE *fichier ,FILE *UL);
void recover();
/*/
/*
Nom_Fonction:
Entrée:
Sortie:
Description:
*/
char Next_char(FILE *fichier) 
{
   if(!fichier)
    {
        printf("\nImpossible de lire dans le fichier \n");
        exit(-1);
    }
    return((char)fgetc(fichier));
}
/*
Nom_Fonction:
Entrée:
Sortie:
Description:
*/
void Retract(FILE *fichier,int nb_pas)
{
   fseek(fichier,(long)(-1*nb_pas),SEEK_CUR);
}
/*
Nom_Fonction:
Entrée:
Sortie:
Description:
*/
int  Fail()
{
  switch (start)
  {
  case 0: start=3;
    break;
  case 3: start=15;
      break;
  //case 9:start=15;
      break;    
 case 15: start=22;
    break;
  case 22: start=26;
      break;
  case 26: //recover();
      break;    
  default: //Erreur/
    break;
  }
  return((int)start);
}
/*
Nom_Fonction:
Entrée:
Sortie:
Description:
*/
void Token_to_string(TOKEN tkn,char string[20])
{
    switch (tkn)
    {
    case IF:strcpy(string,"IF");        
        break;
    case ELSE:strcpy(string,"ELSE");        
        break;    
    case THEN:strcpy(string,"THEN");        
        break;
    case LET:strcpy(string,"LET");        
        break; 
    case GRET:strcpy(string,"GRET");        
        break;     
    case LET_EQUAL:strcpy(string,"LET_EQUAL");        
        break; 
    case GRET_EQUAL:strcpy(string,"GRET_EQUAL"); 
         break; 
    case NB:strcpy(string,"NB");        
        break;        
    case ID:strcpy(string,"ID");        
        break; 
    case DIFF:strcpy(string,"DIFF");        
        break;
    case EQUAL:strcpy(string,"EQUAL");        
        break;                  
    default:
        break;
    }
}
/*
Nom_Fonction:
Entrée:
Sortie:
Description:
*/
TOKEN Next_Token(FILE *fichier)
{
   char car; state =0;
  while (!feof(fichier))
  {
	
     car=Next_char(fichier);
     if(car==EOF) return -1;
     cmp++;
    switch (state)
   {
    case 0:
         while(isspace(car)||(car=='\n'))
         {
           state=0;
           if(car=='\n')
           {
             line++;
             return 15;
           }
           
           car=Next_char(fichier);
         }
         start=state;
         if(car=='s') state=1;
         else
         {
            Retract(fichier,1);
            state=Fail();
         }
         break;
    case 1:if(car=='i') state =2;
           else
           {
            Retract(fichier,2);
            state=Fail();
           }
           break;
    case 2: if(!isalnum(car)) 
             {
                if(car!=EOF)Retract(fichier,1);
                return((TOKEN)IF);
             }
             else 
             {
                Retract(fichier,1);
                state=11;
             } 
             break;
    case 3: start=state;
      if(car=='a') state=4;
      else
      {
         Retract(fichier,1);
        state=Fail();
      }break;
    case 4:
      if(car=='l') state=5;
      else
      {
        Retract(fichier,2);
        state=Fail();
      }
      break;
    case 5: 
    if(car=='o') state=6;
      else
      {
        Retract(fichier,3);
        state=Fail();
      }                         
      break;
    case 6: 
    if(car=='r') state=7;
      else
      {
        Retract(fichier,4);
        state=Fail();
      }                         
      break;
    case 7: 
    if(car=='s') state=8;
    else
      {
        Retract(fichier,5);
        state=Fail();
      }                         
      break; 
    case 8: 
     if(!isalnum(car)) 
      {
        
        if(car!=EOF)Retract(fichier,1);
        return((TOKEN)THEN);
      }
      Retract(fichier,6);
      state=Fail();
      break;
    case 11:
        if(car=='n') state=12;
        else
        {  
          Retract(fichier,3);
          state=Fail();
        }  break; 
    case 12:
        if(car=='o') state=13;
        else
        {
          Retract(fichier,4);
          state=Fail();
        }  break;
    case 13:
        if(car=='n') state=14;
        else
        {
          Retract(fichier,5);
          state=Fail();
        }  break;
    case 14:
        if(!isalnum(car))
        {
            if(car!=EOF)Retract(fichier,1);
            return((TOKEN)ELSE);
        } 
        Retract(fichier,6);
        state=Fail();
         break;
    case 15:start=state;
        if(car=='<') state=17;
        else if(car=='=') return((TOKEN)EQUAL);
        else if(car=='>') state=16;
        else
        {
             if(car!=EOF)Retract(fichier,1);
            state=Fail();
        }
        break;
    case 16:
         if(car=='=') return((TOKEN)GRET_EQUAL);
         else
         {
            if(car!=EOF)Retract(fichier,1);
            return((TOKEN)GRET);
         }
          break;
    case 17:
         if(car=='=') return((TOKEN)LET_EQUAL);   
         else if (car=='>')return((TOKEN)DIFF);             
         else
         {
            if(car!=EOF)Retract(fichier,1);
            return((TOKEN)LET);
         }break;
    case 22:start=state;
   
        if(isalpha(car)) 
        {  
            if(!isalnum((car=Next_char(fichier))))
            {   //printf("+");
                if(car!=EOF)Retract(fichier,1);
                return((TOKEN)ID);
            }
            state=23;
            if(car!=EOF)Retract(fichier,1);
        }
        else
        {
            if(car!=EOF)Retract(fichier,1);
            state=Fail();
        }break;
    case 23:
           if(isalpha(car))
           {
            if(!isalnum((car=Next_char(fichier))))
            {
                if(car!=EOF)Retract(fichier,1);
                return((TOKEN)ID);
            }
            state=24;
           if(car!=EOF) Retract(fichier,1);
           }
           else if(isdigit(car))
           {
            if(!isalnum((car=Next_char(fichier))))
            {
                if(car!=EOF)Retract(fichier,1);
                return((TOKEN)ID);
            }
            state=25;
            if(car!=EOF)Retract(fichier,1);
           } 
                
          break;
    case 24:
        if(isalpha(car))
           {
            if(!isalnum((car=Next_char(fichier))))
            {
                if(car!=EOF)Retract(fichier,1);
                return((TOKEN)ID);
            }
            state=24;
            if(car!=EOF)Retract(fichier,1);
           }
           else if(isdigit(car))
           {
            if(!isalnum((car=Next_char(fichier))))
            { //printf("+");
                if(car!=EOF)Retract(fichier,1);
                return((TOKEN)ID);
            }
            state=25;
            if(car!=EOF)Retract(fichier,1);
           } 
          break;
    case 25:
           if(isalpha(car))
           {
            if(!isalnum((car=Next_char(fichier))))
            {
               if(car!=EOF) Retract(fichier,1);
                return((TOKEN)ID);
            }
            state=24;
           if(car!=EOF) Retract(fichier,1);
           }
           else if(isdigit(car))
           {
            if(!isalnum((car=Next_char(fichier))))
            {
                if(car!=EOF)Retract(fichier,1);
                return((TOKEN)ID);
            }
            state=25;
            if(car!=EOF)Retract(fichier,1);
           }     
          break;    
    case 26:start=state;
         
          if(isdigit(car)) 
          {
             car=Next_char(fichier);
            if(!isdigit(car)&&(car!='.')&&(car!='E'))
            {
               if(car!=EOF) Retract(fichier,1);
                return((TOKEN)NB);
            }
           if(car!=EOF) Retract(fichier,1);
            state=27;
          }
          else 
          {
            printf("\nLexical error with charactere %c  in line %d \n",car,line);
            state=0;
            if(Next_char(fichier)==EOF) return -1;
            Retract(fichier,1);
          }
          break;
    case 27:         
          if(isdigit(car)) 
          {
             car=Next_char(fichier);
            if(!isdigit(car)&&(car!='.')&&(car!='E'))
            {
               if(car!=EOF) Retract(fichier,1);
                return((TOKEN)NB);
            }
            if(car!=EOF) Retract(fichier,1);
            state=27;
          }
          else
          if(car=='E')state=30;
          else if(car=='.') state=28;
          else 
          {
            if(car!=EOF) Retract(fichier,1);
            return((TOKEN)NB);
          }
          break;
    case 28:
         if(isdigit(car)) state=29; 
         else 
         {
            if(car!=EOF) Retract(fichier,1);
            printf("\nLexical error with charactere %c  in line %d \n",fgetc(fichier),line);
            return((TOKEN)NB);
         }
         break;
    case 29:
        if(isdigit(car))
        {
            car=Next_char(fichier);
            if(!isdigit(car)&&(car!='E'))
            {
                if(car!=EOF)Retract(fichier,1);
                return((TOKEN)NB);
            }
            if(car!=EOF) Retract(fichier,1);
            state=29;
            
        }
        else if (car=='E')state=30;
        else 
        {
            if(car!=EOF) Retract(fichier,1);
            return((TOKEN)NB);
        }
        break;
    case 30:
         if(car=='+') state=32;
         else if (car=='-')state=31;
         else if (isdigit(car))state=33;
         else 
         {
           Retract(fichier,1);
           //printf("\nLexical error with character %c in line %d\n",car ,line);
            return((TOKEN)NB);
         }
         break;
    case 31:
    case 32:
       if(isdigit(car)) state=33;
       else 
         {
             if(car!=EOF) Retract(fichier,2);
             else Retract(fichier,1);
            //printf("\nchiffre omis apres %c  in line %d \n",Next_char(fichier),line);
            Retract(fichier,2);
            return((TOKEN)NB);
         }
       break;
    
    case 33:
         if(isdigit(car))
         {
            if(!isdigit((car=Next_char(fichier))))
            {
                if(car!=EOF)Retract(fichier,1);
                return((TOKEN)NB);
            }
            if(car!=EOF)Retract(fichier,1);
            state=33;
         }
         if(car!=EOF)Retract(fichier,1); 
         return((TOKEN)NB);     
        break;                                                       
   default:
    break;

  }
  }
  
  return((TOKEN)-1);
}
void analyseur_Lexical(FILE *fichier ,FILE *UL)
{
    char string[20];
    TOKEN tkn;
    if(!fichier) return;
    while (!feof(fichier))
    {
        tkn=Next_Token(fichier);
        if(tkn==15)fputc('\n',UL);
        else
        {
            Token_to_string(tkn,string);
            fputs(string,UL);
             fputc(' ',UL);
        }
        
    }
    

}

#endif
