# TAREA 1 - COMUNICACIÓN DE DATOS Y REDES: CUATRO EN LINEA CLIENTE-SERVIDOR

## Alumno:

* **Vicente Sanhueza Muñoz**


## Indice

* [Alumno](#alumno)
* [Descripción](#descripción)
* [Pre-requisitos](#pre-requisitos)
* [Compilación y ejecución](#compilación-y-ejecución)

## Descripción

Los objetivos de este repositorio son utilizar diversos mecanismos relacionados con los procesos, tales como la creación y finalización de procesos, así como los mecanismos de comunicación. Se buscara definir e implementar un protocolo para la capa de aplicación y utilizar el protocolo orientado a la conexión TCP en la capa de transporte. 

Para lograr esto, se creo un proyecto el cual tiene como objetivo recrear el juego "Cuatro en línea", en el cual dos jugadores introducen fichas en un tablero vertical con el objetivo de alinear cuatro fichas consecutivas de un mismo color. Por turnos, los jugadores deben introducir una ficha en la columna que prefieran (siempre que no este completa) y esta caera a la posicion mas baja. Gana la partida el primero que consiga alinear cuatro fichas consecutivas de un mismo color en horizontal, vertical o diagonal. 

Se immplementara el juego Cuatro en Lınea mediante el modelo Cliente/Servidor. Para ello, se crearan dos programas de los cuales uno funcionara como el Servidor de juegos (con peticiones concurrentes) y el otro sera el Cliente que se conectara al servidor para jugar.

### Pre-requisitos

_Es importante ejecutar estos comandos antes de compilar los códigos proporcionados en el proyecto para asegurarte de tener todas las dependencias necesarias instaladas._


_Este comando instala el **compilador de C++** en tu sistema._

```bash
   sudo apt-get install g++
   ```

_Este comando instala la **herramienta make** en tu sistema. make es una utilidad que simplifica el proceso de compilación de proyectos al automatizar la ejecución de comandos necesarios para construir un programa a partir de su código fuente._

   ```bash
   sudo apt-get install make
   ```

### Compilación y ejecución

_(1) Ubiquese dentro del directorio "/servidor", luego ejecute el siguiente comando para compilar el programa del servidor._

```bash
   make
   ```

_(2) Luego, ejecute el programa ingresando un puerto, como por ejemplo el puerto "7777"._

```bash
   ./server <puerto>
   ```
_(3) Ubiquese dentro del directorio "/cliente", luego ejecute el siguiente comando para compilar el programa del servidor._

```bash
   make
   ```

_(4) Luego, ejecute el programa ingresando la ip del servidor (esta se puede ver utilizando el comando ifconfig en la terminal), y luego el puerto anteriormente utilizado._

```bash
   ./client <ip> <puerto>
   ```

A continuación se mostrara un ejemplo de la ejecución de los programas utilizando el puerto "7777" para establecer la conexión del cliente al servidor.

![Servidor ejemplo](https://imgur.com/EkzPmI6.png)

![Cliente ejemplo](https://imgur.com/mxmZod7.png)

---
=======
