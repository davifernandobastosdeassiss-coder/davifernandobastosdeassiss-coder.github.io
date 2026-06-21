#include <tamtypes.h>
#include <kernel.h>
#include <sifrpc.h>
#include <loadfile.h>
#include <debug.h>
#include <libpad.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
#include "menu.h"
#include "usb.h"

static char padBuf[256] __attribute__((aligned(64)));
struct padButtonStatus buttons;
u32 new_pad = 0, old_pad = 0;
int refresh_counter = 0;

int main() {
    init_scr(); 
    tela_abertura();
    
    SifInitRpc(0); 
    SifLoadModule("rom0:SIO2MAN", 0, NULL); 
    SifLoadModule("rom0:PADMAN", 0, NULL);
    SifLoadModule("host:usbd.irx", 0, NULL);
    
    if (SifLoadModule("host:usbhdfsd.irx", 0, NULL) < 0) {
        SifLoadModule("host:usb_mass.irx", 0, NULL);
    }
    
    padInit(0); 
    padPortOpen(0, 0, padBuf);

    tela_login();

    while(1) {
        if(padRead(0, 0, &buttons) != 0) {
            u32 pads = 0xffff ^ buttons.btns;
            new_pad = pads & ~old_pad;
            old_pad = pads;
        }

        if (tela == -1) { // LOGIN
            if (new_pad & PAD_RIGHT) { if (key_idx < 35) key_idx++; tela_login(); }
            if (new_pad & PAD_LEFT) { if (key_idx > 0) key_idx--; tela_login(); }
            if (new_pad & PAD_DOWN) { if (key_idx + 9 < 36) key_idx += 9; tela_login(); }
            if (new_pad & PAD_UP) { if (key_idx - 9 >= 0) key_idx -= 9; tela_login(); }
            
            if (new_pad & PAD_CROSS) {
                if (strlen(username) < 15) {
                    int len = strlen(username);
                    username[len] = keyboard[key_idx];
                    username[len+1] = '\0';
                }
                tela_login();
            }
            if (new_pad & PAD_SQUARE) {
                int len = strlen(username);
                if (len > 0) username[len-1] = '\0';
                tela_login();
            }
            if (new_pad & PAD_TRIANGLE) {
                if (strcmp(username, "ADMINDAVI") == 0) user_type = 1;
                else user_type = 0;
                tela = 0; 
                checar_status();
                redesenhar();
            }
        }
        else if (tela == 0) { // LOJA
            if (new_pad & PAD_R1) { tela = 3; redesenhar(); }
            if (new_pad & PAD_RIGHT) { tela = 2; checar_status(); redesenhar(); }
            if (new_pad & PAD_UP) { if(sel_idx > 0) sel_idx--; redesenhar(); }
            if (new_pad & PAD_DOWN) { if(sel_idx < total_jogos - 1) sel_idx++; redesenhar(); }
            if (new_pad & PAD_CROSS) {
                if (user_type == 1 || downloads_hoje < 10) {
                    int f = fioOpen("host:progresso.txt", O_WRONLY | O_CREAT | O_TRUNC); if(f>=0) fioClose(f);
                    f = fioOpen("host:tamanho.txt", O_WRONLY | O_CREAT | O_TRUNC); if(f>=0) fioClose(f);
                    f = fioOpen("host:comando.txt", O_WRONLY | O_CREAT | O_TRUNC);
                    if(f >= 0) {
                        char cmd[20];
                        sprintf(cmd, "JOGO_%d", sel_idx);
                        fioWrite(f, cmd, 7);
                        fioClose(f);
                    }
                    
                    if (user_type != 1) {
                        downloads_hoje++;
                    }
                    
                    tela = 1; 
                    arquivo_extraido = 0; 
                    redesenhar();
                } else {
                    scr_printf("\n LIMITE DIARIO ATINGIDO (10/10)!");
                }
            }
        }
        else if (tela == 1) { // DOWNLOAD
            if (new_pad & PAD_RIGHT) { tela = 2; checar_status(); redesenhar(); }
            refresh_counter++;
            if (refresh_counter > 3000000) { checar_status(); redesenhar(); refresh_counter = 0; }
            if (new_pad & PAD_TRIANGLE) {
                int f = fioOpen("host:comando.txt", O_WRONLY | O_CREAT | O_TRUNC);
                if(f >= 0) { fioWrite(f, "CANCELAR", 8); fioClose(f); }
                tela = 0; 
                redesenhar();
            }
        }
        else if (tela == 2) { // GERENCIADOR
            if (new_pad & PAD_LEFT) { tela = 0; redesenhar(); }
            if (new_pad & PAD_TRIANGLE) { checar_status(); redesenhar(); }
            if ((new_pad & PAD_CROSS) && arquivo_extraido && pendrive_ok) {
                int res = copiar_arquivo();
                if (res == 1) { arquivo_extraido = 0; scr_printf("\n >>> COPIA FINALIZADA! <<<\n"); }
                else if (res == 2) { scr_printf("\n >>> COPIA CANCELADA! <<<\n"); }
                scr_printf(" Pressione [ESQUERDA] para voltar.\n");
                while(!(new_pad & PAD_LEFT)) {
                    padRead(0, 0, &buttons);
                    u32 pads = 0xffff ^ buttons.btns;
                    new_pad = pads & ~old_pad;
                    old_pad = pads;
                }
                tela = 0;
                redesenhar();
            }
        }
        else if (tela == 3) { // SAIR
            if (new_pad & PAD_CROSS) { 
                tela = -1; 
                username[0] = '\0'; 
                key_idx = 0; 
                tela_login(); 
            }
            if (new_pad & PAD_TRIANGLE) { 
                tela = 0; 
                redesenhar(); 
            }
        }
    }
    return 0;
}