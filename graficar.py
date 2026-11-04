
import numpy as np
import matplotlib.pyplot as plt
import os

# Obtener la carpeta donde se encuentra este programa
carpeta = os.path.dirname(os.path.abspath(__file__))

# Ruta del archivo de resultados
ruta = os.path.join(carpeta, "output", "resultados.txt")

# Leer encabezado y datos
with open(ruta, "r") as archivo:
    nombres = archivo.readline().split()

datos = np.loadtxt(ruta, skiprows=1)

# Primera columna: x
x = datos[:, 0]

# Graficar todas las soluciones
for i in range(1, datos.shape[1]):
    plt.plot(
        x,
        datos[:, i],
        marker="o",
        label=nombres[i]
    )

# Configuracion del grafico
plt.title("Comparacion de soluciones")
plt.xlabel("x")
plt.ylabel("y(x)")
plt.grid(True)
plt.legend()
plt.tight_layout()

# Guardar grafico en la carpeta output
salida = os.path.join(carpeta, "output", "comparacion.png")

plt.savefig(salida, dpi=300)

# Mostrar grafico
plt.show()
