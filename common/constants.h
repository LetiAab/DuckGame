#ifndef CONSTANTS_H
#define CONSTANTS_H


const int DOS_BYTES = 2;
const int UN_BYTE = 1;

const uint8_t FIRST_LOBBY_MESSAGE = 0x03;
const uint8_t LOBBY_COMMAND_FAIL = 0x04;
const uint8_t NEW_MATCH_CODE = 0x05;
const uint8_t EXISTING_MATCH_CODE = 0x06; //esto es para conectarme (rarisimo el nombre, de hecho lo odio)
const uint8_t START_MATCH_CODE = 0x07;
const uint8_t LOBBY_STOP_CODE = 0x08;
const uint8_t LIST_MATCH_AVAILABLE = 0x09;



static const char QUIT_SERVER[] = "q";


#endif
