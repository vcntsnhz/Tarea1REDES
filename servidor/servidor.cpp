#include <iostream>
#include <vector>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <sstream>
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;

class Juego 
{
private:
    vector<vector<char>> BOARD;
    const int ROWS = 6;
    const int COLUMNS = 7;

public:
    Juego() {
        BOARD = vector<vector<char>>(ROWS, vector<char>(COLUMNS, ' '));
    }

    string BOARDToString() {
        stringstream ss;
        int NUM_FILA = 1;
        for (const auto& FILA : BOARD) {
            ss << NUM_FILA << " ";
            for (char c : FILA) {
                ss << (c == ' ' ? '.' : c) << " ";
            }
            ss << '\n';
            ++NUM_FILA;
        }

        ss << "  ";
        for (int i = 1; i <= COLUMNS; ++i) {
            ss << i << " ";
        }
        ss << '\n';

        return ss.str();
    }

    // Verificar jugadas.
    bool verificarJugada(int COLUMNA) {
        int COLUMNAIndex = COLUMNA - 1;
        return COLUMNAIndex >= 0 && COLUMNAIndex < COLUMNS && BOARD[0][COLUMNAIndex] == ' ';
    }

    bool realizarJugada(int COLUMNA, char PLAYER) {
        int COLUMNAIndex = COLUMNA - 1;
        if (!verificarJugada(COLUMNA)) return false;
        for (int i = ROWS - 1; i >= 0; i--) {
            if (BOARD[i][COLUMNAIndex] == ' ') {
                BOARD[i][COLUMNAIndex] = PLAYER;
                return true;
            }
        }
        return false;
    }

    // Verificación de ganador.
    bool verificarGanador(char PLAYER) {
        // > Verificación horizontal
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLUMNS - 3; j++) {
                if (BOARD[i][j] == PLAYER && BOARD[i][j + 1] == PLAYER &&
                    BOARD[i][j + 2] == PLAYER && BOARD[i][j + 3] == PLAYER) {
                    return true;
                }
            }
        }

        // > Verificar verticalmente.
        for (int j = 0; j < COLUMNS; j++) {
            for (int i = 0; i < ROWS - 3; i++) {
                if (BOARD[i][j] == PLAYER && BOARD[i + 1][j] == PLAYER &&
                    BOARD[i + 2][j] == PLAYER && BOARD[i + 3][j] == PLAYER) {
                    return true;
                }
            }
        }

        // > Verificar en diagonal de izquierda a derecha.
        for (int i = 0; i < ROWS - 3; i++) {
            for (int j = 0; j < COLUMNS - 3; j++) {
                if (BOARD[i][j] == PLAYER && BOARD[i + 1][j + 1] == PLAYER &&
                    BOARD[i + 2][j + 2] == PLAYER && BOARD[i + 3][j + 3] == PLAYER) {
                    return true;
                }
            }
        }

        // > Verificar en diagonal de derecha a izquierda.
        for (int i = 3; i < ROWS; i++) {
            for (int j = 0; j < COLUMNS - 3; j++) {
                if (BOARD[i][j] == PLAYER && BOARD[i - 1][j + 1] == PLAYER &&
                    BOARD[i - 2][j + 2] == PLAYER && BOARD[i - 3][j + 3] == PLAYER) {
                    return true;
                }
            }
        }

        return false;
    }

bool juegaServidor(int& COLUMNA_JUGADA) {
    for (int COLUMNA = 1; COLUMNA <= COLUMNS; COLUMNA++) {
        if (verificarJugada(COLUMNA)) {
            realizarJugada(COLUMNA, 'S');
            if (verificarGanador('S')) {
                COLUMNA_JUGADA = COLUMNA;
                return true;
            }
            bloquearJugada(COLUMNA);
        }
    }

    // El servidor trata bloquear al jugador de ganar en su proxima jugada
    for (int COLUMNA = 1; COLUMNA <= COLUMNS; COLUMNA++) {
        if (verificarJugada(COLUMNA)) {
            realizarJugada(COLUMNA, 'C');
            if (verificarGanador('C')) {
                bloquearJugada(COLUMNA);
                realizarJugada(COLUMNA, 'S');
                COLUMNA_JUGADA = COLUMNA;
                return true;
            }
            bloquearJugada(COLUMNA);
        }
    }
    for (int COLUMNA = 1; COLUMNA <= COLUMNS; COLUMNA++) {
        if (verificarJugada(COLUMNA) && realizarJugada(COLUMNA, 'S')) {
            COLUMNA_JUGADA = COLUMNA;
            return true;
        }
    }

    return false;
}

void bloquearJugada(int COLUMNA) {
    int COLUMNAIndex = COLUMNA - 1;
    for (int i = 0; i < ROWS; i++) {
        if (BOARD[i][COLUMNAIndex] != ' ') {
            BOARD[i][COLUMNAIndex] = ' ';
            break;
        }
    }
}

};

class Server {
private:
    int serverSocket;
    struct sockaddr_in serverAddress;

public:
    Server(int puerto) {
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket < 0) {
            cerr << "[!] Ocurrio un error en la creación del socket.\n";
            exit(1);
        }

        memset(&serverAddress, 0, sizeof(serverAddress));
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
        serverAddress.sin_port = htons(puerto);
        if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
            cerr << "[!] Ocurrio un error en bind().\n";
            exit(1);
        }
        if (listen(serverSocket, 10) < 0) {
            cerr << "[!] Ocurrio un error en listen().\n";
            exit(1);
        }
    }

    ~Server() 
    {
        close(serverSocket);
    }
    
    void aceptarClientes() 
    {
        cout << ">>> Esperando conexiones ...\n";
        while (true) {
            int* clientSocket = new int;
            *clientSocket = accept(serverSocket, nullptr, nullptr);
            if (*clientSocket < 0) {
                cerr << "[!] Ocurrio un error en accept().\n";
                delete clientSocket;
                continue;
            }
            struct sockaddr_in addr;
            socklen_t addr_len = sizeof(addr);
            getpeername(*clientSocket, (struct sockaddr*)&addr, &addr_len);
            char ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &addr.sin_addr, ip, INET_ADDRSTRLEN);
            int port = ntohs(addr.sin_port);
            cout << ">> Se ha iniciado una nueva partida [" << ip << ":" << port << "]" << endl;
            pthread_t thread_id;
            pthread_create(&thread_id, nullptr, &Server::jugar, clientSocket);
            pthread_detach(thread_id);
        }
    }

    static void* jugar(void* arg) {
        int clientSocket = *((int*)arg);
        Juego juego;
        char buffer[1024];
        struct sockaddr_in addr;
        socklen_t addr_len = sizeof(addr);
        getpeername(clientSocket, (struct sockaddr*)&addr, &addr_len);

        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &addr.sin_addr, ip, INET_ADDRSTRLEN);
        int port = ntohs(addr.sin_port);

        while (true) {
            memset(buffer, 0, 1024);
            int n_bytes = recv(clientSocket, buffer, 1024, 0);
            if (n_bytes <= 0) break;

            int COLUMNA = atoi(buffer);
            stringstream ss;
            if (!juego.realizarJugada(COLUMNA, 'C')) {
                ss << "[!] Ese movimiento no es válido, por favor realiza otro.\n";
            } else {
                ss << "CONECTA-CUATRO [" << ip << ":" << port << "]: El cliente jugó la columna " << COLUMNA << ".\n";
                ss << juego.BOARDToString();
                cout << "CONECTA-CUATRO [" << ip << ":" << port << "]: El cliente jugó la columna " << COLUMNA << ".\n";

                if (juego.verificarGanador('C')) {
                    ss << "\n >>> El cliente ha ganado la partida.\n";
                    cout << "CONECTA-CUATRO [" << ip << ":" << port << "]: El cliente ha ganado la partida... Fin del juego.\n";
                    send(clientSocket, ss.str().c_str(), ss.str().length(), 0);
                    break;
                }

                int COLUMNA_JUGADA;
                if (juego.juegaServidor(COLUMNA_JUGADA)) {
                    ss << "CONECTA-CUATRO [" << ip << ":" << port << "]: El servidor jugó la columna " << COLUMNA_JUGADA << ".\n";
                    ss << juego.BOARDToString();
                    cout << "CONECTA-CUATRO [" << ip << ":" << port << "]: El servidor jugó la columna " << COLUMNA_JUGADA << ".\n";

                    if (juego.verificarGanador('S')) {
                        ss << "\n >>> El servidor ha ganado la partida.\n";
                        cout << "CONECTA-CUATRO [" << ip << ":" << port << "]: El servidor ha ganado la partida... Fin del juego.\n";
                        send(clientSocket, ss.str().c_str(), ss.str().length(), 0);
                        break;
                    }
                }
            }
            send(clientSocket, ss.str().c_str(), ss.str().length(), 0);
        }

        close(clientSocket);
        delete (int*)arg;
        return nullptr;
    }
};

int main(int argc, char **argv) {
    if (argc != 2) {
        cerr << "Uso: " << argv[0] << " <puerto>\n";
        return 1;
    }

    int puerto = atoi(argv[1]);
    Server servidor(puerto);
    servidor.aceptarClientes();

    return 0;
}