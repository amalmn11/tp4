#include "fichier_entete.h"
#include<stdio.h>
#include<stdlib.h>


int main()
{
  FILE * fichier1=fopen("source.txt","r");
  FILE * fichier2=fopen("file_Token.txt","w");
  analyseur_Lexical(fichier1,fichier2);
//   printf(" %d",Next_Token(fichier1));
//   printf(" %d",Next_Token(fichier1));
//   printf(" %d",Next_Token(fichier1));
//   printf(" %d",Next_Token(fichier1));
//     printf(" %d",Next_Token(fichier1));
//   printf(" %d",Next_Token(fichier1));
//   printf(" %d",Next_Token(fichier1));
//   printf(" %d",Next_Token(fichier1));
//    printf(" %d",Next_Token(fichier1));    printf(" %d",Next_Token(fichier1));
//    printf(" %d",Next_Token(fichier1));    printf(" %d",Next_Token(fichier1));


  fclose(fichier1);
  fclose(fichier2);
  return 0;
}
