#ifndef USB_H
#define USB_H

// Compartilha o status dos dispositivos com o menu e o main
extern int pendrive_ok;
extern int arquivo_extraido;

// Declaração das funções de IO
int copiar_arquivo();
void checar_status();

#endif