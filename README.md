# DuckGame

# Como ejecutar el script

Te paras en DuckGame y ejecutas

sudo ./install.sh




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

Para el modo un jugador:
Usar las teclas WASD para el movimiento
- Se salta con la tecla W y si se vuelve a tocar se aletea.
- La tecla S es para tirarse al suelo.
- La tecla E para agarrar un arma.
- La tecla F para disparar y equipar un casco/armadura que se haya agarrado antes.
- La tecla Q para descartar un arma.


# Status hasta la fecha

## Cosas implementadas
- Conexion al lobby y soporte de multiples partidas. 
- Protocolo general de comunicación
- Creación genérica del mapa
- Spawn points
- Disparo con armas
- Se implementaron todas las armas menos la granada y la banana
- Movimientos: derecha, izquierda, salto y aleteo

## Cosas no implementadas (todavía)

- Armas: Granada y Banana
- Cajas
- Diseñador de mapa
- Mayor variedad de mapas
- Movimiento: tirarse al piso (letra S)
- Apuntar y disparar para arriba.
- Conexion del lobby con el protocolo y el servidor (por ahora se ingresa por consola y se ve la renderizacion del lobby donde se inicia al tocar boton start).
- Rondas
- Condicion de victoria y fin de la partida. (por ahora se termina una partida cuando todos los patos mueren, y esta se elimina del lobby).
- Modo dos jugadores
- Camara

## Cosas que se pueden mejorar

- El retroceso en las armas no se muestra cuando el pato está quieto, recién se muestra el cambio cuando el pato se mueve
- Para el rebote del laser en el Laser Rifle, cuando el laser choca se pierden los movimientos que podía hacer el laser en ese turno
- En la Ak47 la dispersión no es incremental en el disparo cuando se deja apretado, sino siempre. Tiene un if para que no se descontrole, pero debería mejorarse
- La implementacion de los Spawn Places puede simplificarse, dandole la responsabilidad a esa clase de almacenar los Items, en lugar de a Game.
- La implementacion de los mensajes que recibe el cliente, estos pueden ser una clase similar a Command.
- El orden en que se realiza el broadcast de los distintos mensajes.
- Actualmente contamos con un sleep comun, debemos modificarlo para tener un constant rate loop.

# Supuestos

- Cuando termina la partida los jugadores no vuelven al lobby, se termina el juego.
- Cantidad de jugadores: minimo 2 y maximo 6. Por ahora se puede iniciar una partida con un unico jugador, pra probar cosas.
- Cualquiera que este dentro de la partida puede iniciarla
- Un jugador puede crear varias partidas, pero solo unirse a una
- Spawn places:
  * se muestran en el mapa, pero se pueden atravesar
  * se encarga de generar un casco/armadura/arma aleatoria
  * decide en qué tiempo mínimo + un tiempo aleatorio aparece el arma
- Cajas:
  * se muestran en el mapa, pero se pueden atravesar
  * se ponen al principio en cada ronda en lugares al azar libres (1 por pato)
  * tienen cascos/armaduras/armas y granadas que EXPLOTAN

# Fuente

Se utilizo como diseño inicial el ejemplo de CMAKE otorgado por la Catedra Veiga.
