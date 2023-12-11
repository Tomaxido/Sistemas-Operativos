import threading
import time
import random
from queue import Queue
from datetime import datetime

# Obtener la hora actual del sistema
hora_inicio = datetime.now().time()

fila_DMat = threading.Semaphore(20)
entrar_DMat = threading.Semaphore(0) #10
mutexDMat = threading.Semaphore(1)
cant_DMat = 0

fila_DInf = threading.Semaphore(8)
entrar_DInf = threading.Semaphore(0) #2
mutexDInf = threading.Semaphore(1)
cant_DInf = 0

fila_DFis = threading.Semaphore(15)
entrar_DFis = threading.Semaphore(0) #5
mutexDFis = threading.Semaphore(1)
cant_DFis = 0

fila_DQui = threading.Semaphore(6)
entrar_DQui = threading.Semaphore(0) #3
mutexDQui = threading.Semaphore(1)
cant_DQui = 0

fila_Defi = threading.Semaphore(6)
entrar_Defi = threading.Semaphore(0) #5
mutexDefi = threading.Semaphore(1)
cant_Defi = 0

fila_DMec = threading.Semaphore(9)
entrar_DMec = threading.Semaphore(0) #4
mutexDMec = threading.Semaphore(1)
cant_DMec = 0


fila_DMin = threading.Semaphore(7)
entrar_DMin = threading.Semaphore(0) #2
mutexDMin = threading.Semaphore(1)
cant_DMin = 0

mutex_escrituraPDL_Salida = threading.Semaphore(1)
mutex_compro = threading.Semaphore(1)

total_personas = 500
deptos = ["Departamento_de_Matemáticas", "Departamento_de_Informática", "Departamento_de_Física", "Departamento_de_Química", "DEFIDER", "Departamento_de_Mecánica", "Departamento_de_Minas"]
filas = [fila_DMat, fila_DInf, fila_DFis, fila_DQui, fila_Defi, fila_DMec, fila_DMin]
entrar = [entrar_DMat, entrar_DInf, entrar_DFis, entrar_DQui, entrar_Defi, entrar_DMec, entrar_DMin]
capacidades = [10,2,5,3,5,4,2]
mutexs = [mutexDMat, mutexDInf, mutexDFis, mutexDQui, mutexDefi, mutexDMec, mutexDMin]
cantidades = [cant_DMat, cant_DInf, cant_DFis, cant_DQui, cant_Defi, cant_DMec, cant_DMin]
mutex = [threading.Semaphore(1),threading.Semaphore(1),threading.Semaphore(1),threading.Semaphore(1),threading.Semaphore(1),threading.Semaphore(1),threading.Semaphore(1)]
tiempos_espera = [9,5,7,4,1,4,5]
cola_llegada = Queue()


def crear_txt():
    with open("PdLamparas.txt", 'w') as archivo:
            pass
    with open("Salida.txt", 'w') as archivo:
            pass
    for depto in deptos:
        with open(depto+'.txt', 'w') as archivo:
            pass

def persona(id_persona):
    a_visitar = []
    horas = []
    horas.append(datetime.now().time())
    ns = random.sample(range(len(deptos)), 2)
    a_visitar.append(deptos[ns[0]])
    a_visitar.append(deptos[ns[1]])

    for i in range(2):
        visitando = a_visitar[i]

        filas[ns[i]].acquire()
        horas.append(datetime.now().time())
        mutex[ns[i]].acquire()
        mutexs[ns[i]].acquire()
        cantidades[ns[i]] += 1
        mutexs[ns[i]].release()

        cola_llegada.put(id_persona)
        
        if cantidades[ns[i]] == capacidades[ns[i]]:
            for x in range(capacidades[ns[i]]):
                entrar[ns[i]].release()
            
        else:
            mutex[ns[i]].release()
            entrar[ns[i]].acquire()


        filas[ns[i]].release()

        mutexs[ns[i]].acquire()
        contenido = "Persona" + str(id_persona + 1) + ", " + str(horas[i]) + ", " + str(
            datetime.now().time()) + ", " + str(i + 1) + "\n"
        with open(visitando + ".txt", "a") as archivo:
            archivo.write(contenido)
        mutexs[ns[i]].release()

        ingresar_departamento(ns[i])
        mutexs[ns[i]].acquire()
        cantidades[ns[i]] -= 1
        mutexs[ns[i]].release()

        if cantidades[ns[i]] == 0:
            mutex[ns[i]].release()
        entrar[ns[i]].release()

    result = [horas[0], a_visitar[0], horas[1], a_visitar[1], horas[2]]
    mutex_escrituraPDL_Salida.acquire()

    try:
        with open("PdLamparas.txt", "a") as archivo:
            contenido = "Persona" + str(id_persona + 1) + ", " + str(result[0]) + ", " + result[1] + ", " + str(
                result[2]) + ", " + result[3] + ", " + str(result[4]) + "\n"
            archivo.write(contenido)

        with open("Salida.txt", "a") as archivo:
            contenido = "Persona" + str(id_persona + 1) + ", " + str(datetime.now().time()) + "\n"
            archivo.write(contenido)

    finally:
        mutex_escrituraPDL_Salida.release()
    

def ingresar_departamento(num):
    time.sleep(tiempos_espera[num])

threads_personas = []
crear_txt()
for i in range(total_personas):
    t = threading.Thread(target=persona, args=(i,))
    threads_personas.append(t)

for t in threads_personas:
    t.start()

for t in threads_personas:
    t.join()
    

print('Todas las personas han salido de la universidad.')