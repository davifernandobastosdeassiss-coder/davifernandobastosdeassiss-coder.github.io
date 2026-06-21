import os
import subprocess
import time
import urllib.request

LINKS_JOGOS = [
    "https://download1474.mediafire.com/kv06uuzj6pggTMkSeV1QkvCxulrPUwxxcvcfC0zGM1ApxuonDTtTlI-_PA1m1lYEBl2hnngxotypD15gWmY0eQ0Yw4MYOcAbPdqHx4IYX466FEtKF4CRalY3CVHwhRVtg3wZkztrTS4YWXBkBk6m7-0FtsNg1v3-acfDTTUNhbo-f5o/kft5tt57csr2by6/GTA.S.A.M.B.7z",
    "https://download1531.mediafire.com/x3elrbo3nrggh0P9d9iIhVMdViLfmft1t4chR91E1GI3xyEsteLWP444ZUzUePEeWdoek2j-JEhAmThApHdq16Y5qiKyvOAhWG_ShM2tFmEp02f8B38zPQknP2TDwHzK2xZwUrJYXF-S1Py5zNXs9ABkUYCXRK_gz02Grfn80yxqGKc/8vf0hknnb5q3t5h/PJGAMES.BombaPatch.Workin.12.Abril.2026.7z",
    "https://download1323.mediafire.com/mrc1xojg3npgloLDinN3s8uuWCn0W4pQftQomIPcJMqaCWrX10uW4YajOVW553djrLa2cRMz0xoA3OJMscjx73urzy0VwpoFAgaOVY0Pf-g8xV87-c6mp_qPrVYuQ1uChhZCl2hCQBReRVPiI9G482Z4Y1DEDmUXfWdf5pk_3iaBuQA/07duby4vhvnsi2h/GTA.L.C.S.BR.7z",
    "https://download1501.mediafire.com/yvzylou7kifgRgdywi0By0OaJl3TF1lkjV0DvVsD0JJIPEP5FqYS6qpRo-w2GB4KlmtcGWlVGLbP0P1ZR4130xc_g1w_80vwtkOGuCZ3RZGuS5M3u3UPxXjPw-PwZfrdVMTz6XaADJPykHkV9GrLjTabALGZjFqRN2eWt8JzhspmHzM/jhyxdxn1o8vypf3/Resident+Evil+4+%28LEG%29.7z",
    "https://download2302.mediafire.com/8wew2atjfgjgEvnKdmsAsUotW8afS5EFTClrxYeVs0lYj6uTUkZuGxFh669F6d5-5Bckewzyiz6H1ByjRaXKm8qtJ2azSe5G_4LgZMyvrlTkP8Uur1E7GviakBcVylxalViWSYweVeAKnkIrnxQftsdh97aFBSoZ2j4EnH6R7mX00LU/bxfz8kq16le8jrr/PJGAMES.Black.DUBLADO.I.A.7z",
    "https://download937.mediafire.com/z993m8cqcijgyz0VlN4MkGAy4xC9-7MLRUFFy4vbe7bEV5dRjAttk-9E9bfYC7pa-ozSdIP8Krx_sNFA7eRD1np33WDy-G87WtzIG-11xnpkEH8Frkkka0N2DJ8i98FGkvMfNF_ntw3P4djYuawM6S8JgBEzPNDw9PCPUQWasWLXHfA/7njxh1xi9gmklfu/PJGAMES.Snes.Station.BR.7z",
    "https://download1320.mediafire.com/jyfvs6d4t73gGFyRcnzyGPDqqXIzbbD6eQ_AInFBfVHxg_UhntFBdUnCahXjQVnPZTdF0MceE4M78JbFfBOhK4m4WBuROtZfniXQEF6ocG93hRipEFctlMsUmu5-6PRHGozlRsSQuLuvpBFuLh6eHXNhkykrxtEbqQ0Zi3yjUVEkGxI/619gkwfdpht1kcy/PJGAMES.Super.Mario.64.PS2.7z",
    "https://download1589.mediafire.com/d8vj6go1fwjgD63Jh7JFtlUCIwbkif_xhNc4u6gAIpXrK4e1f252Uhmkaui8HHbNeRf9-BloEfM3bu_kdjCx4Ba7QQUIo32DRIO4Qufq-AHQ590Du9iihNjv8UItB-9HqT2GZDSRPbi6WDq0ImWT6mJydH9uncod94IET52-4QE5dYE/0krli2nexc3k089/PJGAMES.God.Hand.BR.7z",
    "https://download1077.mediafire.com/424uejbzfjtgNMK-al7l74YO8sHg0ecfpTEzX7YU-97lhPdOz3x2AHj3ShIHpK84f-ScxwKTkhOHj-4Ikz2PN85M1FzddYtC01GLfRbEvdlWLM3X-cre-lAJjr5kQjfZ_Xk2KqsmDbEk_QDSiQaLqo0kWomxHcWVmJd34sWs_OUaEcM/zm56y3mg6s3hd1n/PJGAMES.MotorStorm.Arctic.Edge.USA.7z",
    "https://download1351.mediafire.com/76cciwpq5jpgZoP3Vv1Wyq1mQ-QWOzr9pGE04zhoaVUBT5dtpPLmGS5W1BRL38ZJfeII19B5Xi-4hsSZKSioQ5F2Ebci8lGpqAcwNJQCn1YespC6Tt1Au9c3sPgEDAWw4uatcXVyFUY6JKrOcmo1R1CcHx1PJBsdBwmsaFfR9tCEIHM/hmyiuqtttq0aoc2/PJGAMES.BP.Titan.World.Cup2010.7z",
    "https://download937.mediafire.com/z993m8cqcijgyz0VlN4MkGAy4xC9-7MLRUFFy4vbe7bEV5dRjAttk-9E9bfYC7pa-ozSdIP8Krx_sNFA7eRD1np33WDy-G87WtzIG-11xnpkEH8Frkkka0N2DJ8i98FGkvMfNF_ntw3P4djYuawM6S8JgBEzPNDw9PCPUQWasWLXHfA/7njxh1xi9gmklfu/PJGAMES.Snes.Station.BR.7z",
    "https://download1523.mediafire.com/m2rafvryvxsgeCMd5CjUCvau51Tvdx9Wa4pFEYqzRp1vyvWNFWNseR07BFU0A1c4S6Lg6n82dfU21IBPtEBaBgNt4EbTeCS95saAPhNo8S4mgxMROJTUMmtQTJoPzsVWDhmgEjRveyUFrZQ9TWEvmL46rQDYk-Gnm4qACWImNTUbmLE/vzvashd9orw9kxp/PJGAMES.Ghost.Rider.LEGENDADO.BR.7z",
    "https://download856.mediafire.com/tl8mwt7ceskgdLn54zJov8J3Cr7iaNvbm0UQBLwOk4Ap7QAZ9FqtW9QTnLF7cg-oemA10kbYl1-zaAtcVtt-hgOLsJjY-xNGyN4yWGFjX47gXeYL6_moLDgqZ_nzU17AVSt2m0rmjFS4pcLUP9WU1xT0vLxJFb7-wmejbdcbysSXe2I/ppkhpmqf6v68liu/Darkwatch.BR.7z",
    "https://download2295.mediafire.com/gbvf099qy4sgoSX1Qz9xJpz0tRDt13zocRBlgX20CRNh5F33WKchDqJT8Hql_6fVfQM4MAb-Kyg41WMUyzyOgZ0RIYIxz47H519lL4ClpWLW_Fjtd5qsDszr8bLG5XfdQjmKrY3x1a5DIPj7dgSgFp5Q5mQXMgQo-7qjCujW-YIWn6c/b73m9bojtfdikua/PJGAMES.10.In.1.Fighters.Pack.7z",
    "https://download1474.mediafire.com/83f429afs1ngfxsqs4Fs4AEwxNOsPosIzn-KEmUGVea7Ib3u1OMIRMw80eDLdHMhxt2lkOjmkKnKdbZhQ2ervQqVllqtrLk8hbcjJlAsgzN2gh62_STJhNqWz78gCjXRfq99YnfsA1jo1xwZxwyblQSFbpq7B3wE5ld2dUqu-H-IsnI/d2guxawpho84lqd/PJGAMES.187.Ride.or.Die.USA.7z"
]

CRI_7Z = r"C:\Program Files\7-Zip\7z.exe"
if not os.path.exists(CRI_7Z):
    CRI_7Z = r"C:\Program Files (x86)\7-Zip\7z.exe"

def baixar_com_progresso(url, destino):
    try:
        req = urllib.request.Request(url, headers={'User-Agent': 'Mozilla/5.0'})
        with urllib.request.urlopen(req) as resp, open(destino, 'wb') as out:
            tamanho_total = int(resp.info().get('Content-Length', 0))
            tamanho_baixado = 0
            bloco = 1024 * 64
            while True:
                if os.path.exists("comando.txt"):
                    with open("comando.txt", "r") as f:
                        c_chk = f.read().replace('\x00', '').strip()
                    if "CANCELAR" in c_chk or c_chk == "":
                        return False

                data = resp.read(bloco)
                if not data: break
                out.write(data)
                tamanho_baixado += len(data)
                if tamanho_total > 0:
                    try:
                        # Gravação via arquivo temporário anti-trava de rede
                        with open("progresso.tmp", "w") as f:
                            f.write(str(int(tamanho_baixado * 100 / tamanho_total)))
                        os.replace("progresso.tmp", "progresso.txt")
                    except: pass
            return True
    except:
        return False

def limpar_comando_seguro():
    for _ in range(5):
        try:
            if os.path.exists("comando.txt"): os.remove("comando.txt")
            return
        except: time.sleep(0.2)

print("Servidor Pstore2 Ativo...")
ultimo_comando = ""

while True:
    if os.path.exists("comando.txt"):
        try:
            with open("comando.txt", "r") as f:
                cmd = f.read().replace('\x00', '').strip()
            
            if cmd.startswith("JOGO_") and cmd != ultimo_comando:
                parts = cmd.split("_")
                if len(parts) > 1 and parts[1].isdigit():
                    idx = int(parts[1])
                    print(f"\nRecebido: Baixar jogo {idx}")
                    ultimo_comando = cmd
                    
                    for f_name in ["pronto.flag", "progresso.txt", "tamanho.txt", "jogo.7z"]:
                        if os.path.exists(f_name):
                            try: os.remove(f_name)
                            except: pass
                    
                    print("Iniciando Download...")
                    if baixar_com_progresso(LINKS_JOGOS[idx], "jogo.7z"):
                        if os.path.exists("jogo.7z") and os.path.getsize("jogo.7z") > 50000:
                            print("Download concluido! Extraindo...")
                            
                            if os.path.exists(CRI_7Z):
                                if os.path.exists("jogo.iso"):
                                    try: os.remove("jogo.iso")
                                    except: pass
                                
                                # ADICIONADO O PARAMETRO OBRIGATÓRIO -bsp1 PARA REDIRECIONAR O PROGRESSO DO 7Z
                                proc = subprocess.Popen([CRI_7Z, 'e', 'jogo.7z', '-o./', '-y', '-bsp1'], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, errors='ignore', bufsize=1)
                                buffer = []
                                while proc.poll() is None:
                                    char = proc.stdout.read(1)
                                    if not char: break
                                    if char == '%':
                                        digits = "".join([c for c in reversed(buffer) if c.isdigit()][::-1])
                                        if digits:
                                            try:
                                                # Gravação segura anti-flicker e anti-lockdown
                                                with open("progresso.tmp", "w") as f:
                                                    f.write(f"E{digits}")
                                                os.replace("progresso.tmp", "progresso.txt")
                                            except: pass
                                        buffer = []
                                    elif char in ['\r', '\n']: buffer = []
                                    else: buffer.append(char)
                                proc.wait()
                                
                                renomeado = False
                                for arquivo in os.listdir("."):
                                    if (arquivo.lower().endswith(".iso") or arquivo.lower().endswith(".bin")) and arquivo != "jogo.iso":
                                        try:
                                            os.rename(arquivo, "jogo.iso")
                                            renomeado = True
                                            break
                                        except: pass
                                
                                if not renomeado and os.path.exists("jogo.iso"):
                                    renomeado = True
                                    
                                if renomeado:
                                    try:
                                        tamanho_real = os.path.getsize("jogo.iso")
                                        with open("tamanho.txt", "w") as f:
                                            f.write(str(tamanho_real))
                                    except: pass
                                    
                                    with open("pronto.flag", "w") as f:
                                        f.write("OK")
                                    print("Jogo pronto!")
                        else:
                            print("[!] Erro: Link invalido ou expirado.")
                    else:
                        print("[!] Cancelado ou Falhou.")
                        if os.path.exists("jogo.7z"):
                            try: os.remove("jogo.7z")
                            except: pass
                    limpar_comando_seguro()
            elif "CANCELAR" in cmd:
                ultimo_comando = ""
                limpar_comando_seguro()
        except Exception as e:
            print(f"Erro: {e}")
            limpar_comando_seguro()
    time.sleep(1)