#!/bin/bash

# Hacer que el script falle si ocurre un error
set -e

# Instalar las dependencias necesarias
echo "Instalando dependencias necesarias..."

sudo apt-get update 
sudo apt-get install -y libjpeg-dev libpng-dev libfreetype-dev libopusfile-dev libflac-dev libxmp-dev libfluidsynth-dev libwavpack-dev cmake libmodplug-dev libsdl2-dev

# SDL_IMAGE
SDL_IMAGE_DIR="libs/SDL_image-release-2.6.3"
if [ -d "$SDL_IMAGE_DIR" ]; then
  echo "Entrando a la carpeta $SDL_IMAGE_DIR..."
  cd "$SDL_IMAGE_DIR"
else
  echo "Error: La carpeta $SDL_IMAGE_DIR no existe."
  exit 1
fi

echo "Creando directorio build..."
mkdir -p build
cd build

echo "Ejecutando cmake..."
cmake ..

echo "Compilando el proyecto..."
make -j4

echo "Instalando SDL_image..."
sudo make install

# SDL_MIXER
SDL_MIXER_DIR="libs/SDL_mixer-release-2.6.3"
if [ -d "$SDL_MIXER_DIR" ]; then
  echo "Entrando a la carpeta $SDL_MIXER_DIR..."
  cd "$SDL_MIXER_DIR"
else
  echo "Error: La carpeta $SDL_MIXER_DIR no existe."
  exit 1
fi

echo "Creando directorio build..."
mkdir -p build
cd build

echo "Ejecutando cmake..."
cmake ..

echo "Compilando el proyecto..."
make -j4

echo "Instalando SDL_mixer..."
sudo make install

#SDL TTF

SDL_TTF_DIR="libs/SDL_ttf-release-2.20.2"
if [ -d "$SDL_TTF_DIR" ]; then
  echo "Entrando a la carpeta $SDL_TTF_DIR..."
  cd "$SDL_TTF_DIR"
else
  echo "Error: La carpeta $SDL_TTF_DIR no existe."
  exit 1
fi

echo "Creando directorio build..."
mkdir -p build
cd build

echo "Ejecutando cmake..."
cmake ..

echo "Compilando el proyecto..."
make -j4

echo "Instalando SDL_mixer..."
sudo make install




echo "Instalación completada con éxito."
