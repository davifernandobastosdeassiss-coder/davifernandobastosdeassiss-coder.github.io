#ifndef MENU_H
#define MENU_H

#include <tamtypes.h>

//
extern int tela; 
extern int sel_idx;
extern int total_jogos;
extern int user_type; 
extern int downloads_hoje;
extern int key_idx;
extern char username[16];
extern char keyboard[];
extern char *nomes_jogos[];

//
void tela_abertura();
void tela_login();
void redesenhar();

#endif