#include <fileio.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <debug.h>
#include <libpad.h>
#include "usb.h"
#include "menu.h"

int pendrive_ok = 0;
int arquivo_extraido = 0;

// Puxa o controle do main.c para aceitar cancelamentos no meio da cópia
extern struct padButtonStatus buttons;

int copiar_arquivo() {
    int f1, f2, n;
    static char buffer[65536];
    long long tamanho_total = 0;   
    long long bytes_copiados = 0;  
    int ultima_pct = -1;
    int b;
    int tf;
    char size_str[32];
    char pasta_destino[256] = "mass:"; 
    char caminho_completo_destino[300];
    
    int cf = fioOpen("host:config.txt", O_RDONLY);
    if (cf >= 0) {
        char cfg_buf[512];
        memset(cfg_buf, 0, sizeof(cfg_buf));
        int lidos = fioRead(cf, cfg_buf, sizeof(cfg_buf) - 1);
        fioClose(cf);
        
        if (lidos > 0) {
            char *pos = strstr(cfg_buf, "PASTA_DESTINO=");
            if (pos) {
                pos += 14; 
                int idx_c = 0;
                while (*pos && *pos != '\r' && *pos != '\n' && idx_c < 250) {
                    pasta_destino[idx_c++] = *pos++;
                }
                pasta_destino[idx_c] = '\0';
            }
        }
    }

    int len = strlen(pasta_destino);
    if (len > 0 && (pasta_destino[len - 1] == '/' || pasta_destino[len - 1] == ':')) {
        sprintf(caminho_completo_destino, "%sjogo.iso", pasta_destino);
    } else {
        sprintf(caminho_completo_destino, "%s/jogo.iso", pasta_destino);
    }
    
    f1 = fioOpen("host:jogo.iso", O_RDONLY);
    if (f1 < 0) {
        scr_printf("\n ERRO: O arquivo 'jogo.iso' nao existe no servidor!\n");
        return 0;
    }
    
    tf = fioOpen("host:tamanho.txt", O_RDONLY);
    if (tf >= 0) {
        memset(size_str, 0, sizeof(size_str));
        fioRead(tf, size_str, sizeof(size_str) - 1);
        
        char *ptr = size_str;
        tamanho_total = 0;
        while (*ptr >= '0' && *ptr <= '9') {
            tamanho_total = tamanho_total * 10 + (*ptr - '0');
            ptr++;
        }
        fioClose(tf);
    }
    
    if (tamanho_total <= 0) {
        tamanho_total = fioLseek(f1, 0, SEEK_END);
        fioLseek(f1, 0, SEEK_SET);
    }
    
    f2 = fioOpen(caminho_completo_destino, O_WRONLY | O_CREAT | O_TRUNC);
    if (f2 < 0) {
        scr_printf("\n ERRO: Nao foi possivel gravar no local: %s\n", caminho_completo_destino);
        scr_printf(" Verifique se a pasta configurada realmente existe no seu USB!\n");
        fioClose(f1);
        return 0;
    }
    
    while ((n = fioRead(f1, buffer, sizeof(buffer))) > 0) {
        fioWrite(f2, buffer, n);
        bytes_copiados += n;
        
        if (tamanho_total > 0) {
            int pct = (int)(bytes_copiados * 100 / tamanho_total);
            if (pct != ultima_pct) {
                ultima_pct = pct;
                scr_clear();
                scr_printf("=================================================\n");
                scr_printf("        Seja Bem-Vindo a PSTORE 2 v0.1 Alpha \n");
                scr_printf("=================================================\n\n");
                scr_printf(" >>> COPIANDO PARA O PENDRIVE/HDD... <<<\n");
                scr_printf(" Destino: %s\n", caminho_completo_destino);
                scr_printf(" >>> NAO REMOVA O PENDRIVE, NAO DESLIGUE O PS2, OU DESCONECTE O WIFI! <<<\n\n");
                scr_printf(" Jogo: %s\n\n", nomes_jogos[sel_idx]);
                
                scr_printf(" Progresso: [");
                for (b = 0; b < 20; b++) {
                    if (b < (pct / 5)) scr_printf("=");
                    else scr_printf(" ");
                }
                scr_printf("] %d%%\n\n", pct);
                scr_printf(" [TRIANGULO] CANCELAR COPIA E APAGAR ARQUIVO INCOMPLETO\n");
            }
        }
        
        if (padRead(0, 0, &buttons) != 0) {
            u32 pads = 0xffff ^ buttons.btns;
            if (pads & PAD_TRIANGLE) {
                fioClose(f1);
                fioClose(f2);
                fioRemove(caminho_completo_destino); 
                return 2; 
            }
        }
    }
    
    fioClose(f1); 
    fioClose(f2);
    return 1;
}

void checar_status() {
    int dir = fioDopen("mass:");
    if (dir >= 0) {
        pendrive_ok = 1;
        fioDclose(dir);
    } else {
        pendrive_ok = 0;
    }

    int flag = fioOpen("host:pronto.flag", O_RDONLY);
    if (flag >= 0) {
        arquivo_extraido = 1;
        fioClose(flag);
    } else {
        arquivo_extraido = 0;
    }
}