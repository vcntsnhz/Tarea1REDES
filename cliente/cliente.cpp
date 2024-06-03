#include <iostream>
#include <unistd.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;

class Cliente 
{
private:
    int socketCliente;
    struct sockaddr_in direccionServidor;

public:
    Cliente(const char* ipServidor, int puertoServidor) 
    {
        socketCliente = socket(AF_INET, SOCK_STREAM, 0);
        if (socketCliente < 0) {
            cerr << "Error al crear el socket del cliente." << endl;
            exit(1);
        }

        direccionServidor.sin_family = AF_INET;
        direccionServidor.sin_port = htons(puertoServidor);

        if (inet_pton(AF_INET, ipServidor, &direccionServidor.sin_addr) <= 0) {
            cerr << "La dirección IP no es válida o no es soportada." << endl;
            exit(1);
        }

        if (connect(socketCliente, (struct sockaddr *)&direccionServidor, sizeof(direccionServidor)) < 0) {
            cerr << "Error al conectar con el servidor." << endl;
            exit(1);
        }
    }

    ~Cliente() {
        close(socketCliente);
    }

    void iniciarJuego() 
    {
        cout << "Se establecio conexión exitosamente al servidor.\n\n";
        string entradaUsuario;
        char bufferRespuesta[1024] = {0};

        while (true) {
            cout << ">>> Ingrese el número de la columna que desea jugar (solo números entre 1-7): ";
            getline(cin, entradaUsuario);

            send(socketCliente, entradaUsuario.c_str(), entradaUsuario.size(), 0);
            int bytesLeidos = read(socketCliente, bufferRespuesta, 1024);

            if (bytesLeidos > 0) {
                cout << "Server: " << bufferRespuesta << endl;
                if (strstr(bufferRespuesta, "es el ganador de la partida.")) {
                    break;
                }
            } else {
                cerr << "[!] Error de lectura o servidor desconectado." << endl;
                break;
            }
        }
    }
};

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cerr << "Uso: " << argv[0] << " <direccion_ip> <puerto>" << endl;
        return 1;
    }

    Cliente cliente(argv[1], atoi(argv[2]));
    cliente.iniciarJuego();

    return 0;
}