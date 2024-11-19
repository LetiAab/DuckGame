# DuckGame

# Librería SDL
Para descargar las depencias de la librería SDL2, primero se deben ejecutar los siguientes comandos :
```
sudo apt-get install libjpeg-dev libpng-dev libfreetype-dev libopusfile-dev libflac-dev libxmp-dev libfluidsynth-dev libwavpack-dev cmake libmodplug-dev libsdl2-dev
```
Luego se deben compilar e instalar las librerías `SDL2_image`, `SDL2_mixer` y `SDL2_ttf`. Para ello se deben ejecutar los siguientes comandos:
```
cd nombre_de_la carpeta_descomprimida
mkdir build
cd build
cmake ..
make -j4
sudo make install
```

# Como ejecutar el programa

1. Compilacion: En la consola escribe make

```
make
```
2. Levantar cliente y servidor: Dentro de la carpeta build ejecuta

Para levantar el server
```
 ./taller_server 8080
```

Para levantar el cliente
```
 ./taller_client localhost 8080
```

# Como jugar

Para el modo un jugador, usar las teclas wasd para el movimiento, la letra e para agarrar un arma, la letra f para disparar y la letra q para descartar un arma.


# Ejemplo de CMAKE para el TP Final

**Importante:** el primer commit de este repositorio tiene el setup
básico para el TP Final que incluye la instalación de la lib
`libSDL2pp` (el wrapper de C++).

El resto de los commits son a modo de ejemplo de como se pueden
agregar mas código fuente al proyecto.

Este ejemplo **no** incluye instalarse la librería `SDL2` ni tampoco
instala otras librerías que puedan ser necesarias ni tampoco un
instalador (aunque **si** incluye Google Tests)

**Se deben agregar las librerias necesarias y el instalador.**

También el ejemplo usa una estructura de carpetas muy simple:

```
client/
server/
editor/
common/
```

Bien se puede mejorar (cambiando el cmakefile) agregando mas
sub-carpetas.

Asi tambien **deben** ser cambiados los *targets* del cmake (`taller_client`,
`taller_server`, ...) por nombres mas acordes al TP que se este
haciendo.

Tambien, por default solo se compila una version *debug* sin
optimizar. Si se quiere compilar binarios optimizados
(lo que cmake llama *release*) se puede, solo hay modificar
cmake.

Aprender del ejemplo para saber como extenderlo!

**Importante:** este repositorio **no** incluye pre-commits hooks,
ni scripts adicionales (como correr valgrind).

**Eso esta a cargo de los estudiantes,** tal como fue mostrado
en los tps individuales, recaps y hands-on.


# Status hasta la fecha

## Cosas implementadas

- Protocolo general de comunicación
- Creación genérica del mapa
- Spawn points
- Disparo con armas
- Se implementaron todas las armas menos la granada y la banana

## Cosas no implementadas (todavía)

- Armas: Granada y Banana
- Cajas
- Diseñador de mapa
- Mayor variedad de mapas

## Cosas que se pueden mejorar

- El retroceso en las armas no se muestra cuando el pato está quieto, recién se muestra el cambio cuando el pato se mueve
- Para el rebote del laser en el Laser Rifle, cuando el laser choca se pierden los movimientos que podía hacer el laser en ese turno
- En la Ak47 la dispersión no es incremental en el disparo cuando se deja apretado, sino siempre. Tiene un if para que no se descontrole, pero debería mejorarse
