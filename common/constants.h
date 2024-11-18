#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <vector>

const int ERROR = 1;
const int SUCCESS = 0;

const int DOS_BYTES = 2;
const int UN_BYTE = 1;

const uint8_t FIRST_GAME_MESSAGE = 0x02;
const uint8_t FIRST_LOBBY_MESSAGE = 0x03;
const uint8_t LOBBY_COMMAND_FAIL = 0x04;
const uint8_t NEW_MATCH_CODE = 0x05;
const uint8_t EXISTING_MATCH_CODE = 0x06; //esto es para conectarme (rarisimo el nombre, de hecho lo odio)
const uint8_t START_MATCH_CODE = 0x07;
const uint8_t LOBBY_STOP_CODE = 0x08;
const uint8_t LIST_MATCH_AVAILABLE = 0x09;
const uint8_t LOBBY_EXIT_CODE = 0x0A;
const uint8_t EXIT_GAME = 0x1B;

const uint8_t MOVE_LEFT = 0x10;
const uint8_t MOVE_RIGHT = 0x11;
const uint8_t MOVE_UP = 0x12;
const uint8_t MOVE_DOWN = 0x13;

const uint8_t STOP_LEFT = 0x14;
const uint8_t STOP_RIGHT = 0x15;
const uint8_t STOP_UP = 0x16;
const uint8_t STOP_DOWN = 0x17;

const uint8_t SHOOT = 0x18;
const uint8_t END_GAME = 0x19;


//flags que indican la direccion en la que mira el patos
static const char LOOKING_RIGHT = 'd';
static const char LOOKING_LEFT = 'a';

//constantes para la gravedad
const int DUCK_JUMP_POWER = 12;
const int DUCK_WALK_POWER = 3;
const int DUCK_FALL_POWER = 2;

//messages from game
const uint8_t MAP_INICIALIZATION = 0x14;

const uint8_t DUCK_POS_UPDATE = 0x15;

const uint8_t BULLET_POS_UPDATE = 0x32;

const uint8_t KILL_DUCK = 0x33;

const uint8_t TAKE_ITEM = 0x34;

const uint8_t ITEM_POSITION = 0x35;

const uint8_t DUCK_PICKUP_ITEM = 0x36;
const uint8_t DUCK_EQUIP_ITEM = 0x37;

const uint8_t DROP_WEAPON = 0X38;

const uint8_t HELMET_BROKEN = 0X77;
const uint8_t ARMOR_BROKEN = 0X78;

const uint8_t SPAWN_PLACE_POSITION = 0X79;

const uint8_t SPAWN_PLACE_ITEM_UPDATE = 0X80;


const int N_SPAWN_PLACES = 4;



//VOY A NECESITAR IDS PARA QUE SEPAN QUE ITEM RENDERERIZAR 
const uint8_t ARMOR_ID = 0x39;
const uint8_t HELMET_ID = 0x40;
const uint8_t WEAPON_1_ID = 0x41;
// ...







static const char QUIT_SERVER[] = "q";

const int MAX_PLAYERS = 6;
const int MIN_PLAYERS = 2;

const int MATRIX_N = 144;
const int MATRIX_M = 200;

const int PLATFORM_SIZE_X = 2;
const int PLATFORM_SIZE_Y = 2;

const int DUCK_SIZE_X = 10;
const int DUCK_SIZE_Y = 14;
const int DUCK_TOTAL_SIZE = 140; //Cantidad total de cuadrados que ocupa el pato 9X13

const int BULLET_SIZE_Y = 1;
const int BULLET_SIZE_X = 2;

const int TILE_SIZE = 4;

const int WINDOW_WIDTH = TILE_SIZE * MATRIX_M;
const int WINDOW_HEIGHT = TILE_SIZE * MATRIX_N;

const std::vector<std::vector<uint8_t>> colors = {
    {255,255,255}, //blanco
    {255,255,102}, //amarillo
    {153,255,153}, //verde
    {153,204,255}, //azul
    {192,192,192}, //gris
    {255,178,102} //naranja
};

const int BUTTON_W = 239;
const int BUTTON_H = 77;

#endif
