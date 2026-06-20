#include <debug.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fileio.h>
#include "menu.h"
#include "usb.h"

// Inicialização real das variáveis declaradas no menu.h
int tela = -1; 
int sel_idx = 0;
int total_jogos = 15;
int user_type = 0; 
int downloads_hoje = 0;
int key_idx = 0;
char username[16] = "";

char keyboard[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

char *nomes_jogos[] = {
    "GTA Brasil", "Bomba Patch 2026", "Gta Liberty", "Resident Evil 4", "Black", 
    "Killzone", "Super Mario 64--- EM Breve", "God Hand BR", "MotorStorm", "TitanWorld", 
    "Super Mario World--- EM Breve", "Ghost Rider", "DarkWatch", "10 in 1 Fighters", "Rider or Die"
};

void tela_abertura() {
    int i; 
    scr_clear();
    scr_printf("\n\n\n\n\n");
    scr_printf("  _____   _____   _______ ____  _____  ______  ___ \n");
    scr_printf(" |  __ \\ / ____||__   __/ __ \\|  __ \\|  ____||__ \\ \n");
    scr_printf(" | |__) | (___     | | | |  | | |__) | |__     ) |\n");
    scr_printf(" |  ___/ \\___ \\    | | | |  | |  _  /|  __|   / / \n");
    scr_printf(" | |     ____) |   | | | |__| | | \\ \\| |____ / /_ \n");
    scr_printf(" |_|    |_____/    |_|  \\____/|_|  \\_\\______|____|\n\n");
    scr_printf("            CARREGANDO PSTORE2... \n");
    for(i = 0; i < 400000000; i++) { asm("nop"); }
    scr_clear();
}

void tela_login() {
    int i; 
    scr_clear();
    scr_printf("=================================================\n");
    scr_printf("                LOGIN PSTORE2 \n");
    scr_printf("=================================================\n\n");
    scr_printf(" Usuario: %s\n\n", username);
    
    for(i = 0; i < 36; i++) {
        if (i == key_idx) scr_printf("[%c] ", keyboard[i]);
        else scr_printf(" %c  ", keyboard[i]);
        if ((i + 1) % 9 == 0) scr_printf("\n");
    }
    
    scr_printf("\n [X] Adicionar | [TRIANGULO] Entrar | [QUADRADO] Apagar\n");
}

void redesenhar() {
    int i, fd, b;
    char p[6];
    int ext_pct;
    int dl_pct;
    
    static int ult_tela = -1;
    static int ult_pct = -1;
    static int ult_tipo = -1; 
    static int ult_pronto = -1;
    
    if (tela != ult_tela) {
        ult_tela = tela;
        ult_pct = -1;
        ult_tipo = -1;
        ult_pronto = -1;
    }
    
    if (tela == 1) {
        int pct_atual = -1;
        int tipo_atual = -1;
        
        fd = fioOpen("host:progresso.txt", O_RDONLY);
        if (fd >= 0) {
            memset(p, 0, sizeof(p));
            fioRead(fd, p, 5);
            fioClose(fd);
            
            if (p[0] == 'E') {
                tipo_atual = 1; 
                pct_atual = atoi(&p[1]);
            } else {
                tipo_atual = 0; 
                pct_atual = atoi(p);
            }
        }
        
        if (pct_atual == ult_pct && tipo_atual == ult_tipo && arquivo_extraido == ult_pronto) {
            return; 
        }
        
        ult_pct = pct_atual;
        ult_tipo = tipo_atual;
        ult_pronto = arquivo_extraido;
    }
    
    scr_clear();
    scr_printf("=================================================\n");
    scr_printf("        Seja Bem-Vindo a PSTORE 2 v0.1 Alpha \n");
    scr_printf("=================================================\n\n");

    if (tela == 0) {
        scr_printf(" [LOJA] - PRESSIONE SETA PARA A [DIREITA] PARA ABRIR O GERENCIADOR\n");
        scr_printf(" [R1] PARA SAIR DA LOJA\n\n");
        scr_printf(" Use CIMA/BAIXO para selecionar:\n\n");
        for(i = 0; i < total_jogos; i++) {
            if(i == sel_idx) scr_printf(" -> %s\n", nomes_jogos[i]);
            else scr_printf("    %s\n", nomes_jogos[i]);
        }
        scr_printf("\n [X] Fazer Download (%d/10)\n", downloads_hoje);
    } 
    else if (tela == 1) {
        scr_printf(" [DOWNLOAD] - PRESSIONE SETA PARA A [DIREITA] PARA ABRIR O GERENCIADOR\n\n");
        scr_printf(" Processando: %s\n\n", nomes_jogos[sel_idx]);
        
        if (arquivo_extraido) {
            scr_printf(" >>> EXTRACAO CONCLUIDA WITH SUCESSO! <<<\n");
            scr_printf(" PRESSIONE SETA PARA A [DIREITA] PARA IR PARA O GERENCIADOR E COPIAR.\n");
        } else {
            fd = fioOpen("host:progresso.txt", O_RDONLY);
            if (fd >= 0) {
                memset(p, 0, sizeof(p));
                fioRead(fd, p, 5);
                fioClose(fd);
                
                if (p[0] == 'E') {
                    ext_pct = atoi(&p[1]);
                    scr_printf(" >>> EXTRAINDO...POR FAVOR AGUARDE: %d%% <<<\n\n", ext_pct);
                    scr_printf(" Progresso: [");
                    for (b = 0; b < 20; b++) {
                        if (b < (ext_pct / 5)) scr_printf("=");
                        else scr_printf(" ");
                    }
                    scr_printf("]\n");
                } else {
                    dl_pct = atoi(p);
                    scr_printf(" Download em Progresso.. Por Favor Aguarde: %d%%\n\n", dl_pct);
                    scr_printf(" Progresso: [");
                    for (b = 0; b < 20; b++) {
                        if (b < (dl_pct / 5)) scr_printf("=");
                        else scr_printf(" ");
                    }
                    scr_printf("]\n");
                }
            } else {
                scr_printf(" Processando, NAO DESLIGUE O PS2...\n");
            }
        }
        scr_printf("\n\n [TRIANGULO] Para Cancelar Download\n");
    }
    else if (tela == 2) {
        scr_printf(" [GERENCIADOR] - PRESSIONE SETA PARA A [ESQUERDA] PARA VOLTAR\n\n");
        
        scr_printf(" Status do USB (mass:) : [%s]\n", pendrive_ok ? "CONECTADO" : "DESCONECTADO");
        scr_printf(" Status do Jogo (host:): [%s]\n\n", arquivo_extraido ? "AGUARDANDO" : "AGUARDANDO/VAZIO");
        
        if (arquivo_extraido && pendrive_ok) {
            scr_printf(" >>> TUDO PRONTO! Pressione [X] para Copiar a ISO Pro Pendrive/Hdd <<<\n");
        } else if (!pendrive_ok) {
            scr_printf(" Aviso: Conecte seu Pendrive/Hdd para habilitar a copia.\n");
        } else if (!arquivo_extraido) {
            scr_printf(" Aviso: Faca um download na Loja e aguarde a extracao.\n");
        }
        scr_printf("\n [TRIANGULO] Atualizar Status do USB manualmente\n");
    }
    else if (tela == 3) {
        scr_printf("\n\n\n=================================================\n");
        scr_printf("        DESEJA SAIR MESMO DA LOJA?                \n");
        scr_printf("=================================================\n\n");
        scr_printf(" [X] SIM (Voltar ao Login)                       \n");
        scr_printf(" [TRIANGULO] NAO (Continuar na Loja)             \n");
    }
}