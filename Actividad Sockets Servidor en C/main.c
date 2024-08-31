#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <winsock2.h>

#define PORT 8080
#define MAX 1024


int esVocal(char c) {
    c = tolower(c);
    return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
}


void generateUser(char* nombre, int longitud) {
    const char* vocales = "aeiou";
    const char* consonantes = "bcdfghjklmnpqrstvwxyz";

    int usarVocal = rand() % 2;

    for (int i = 0; i < longitud; i++) {
        if (usarVocal) {
            nombre[i] = vocales[rand() % strlen(vocales)];
        } else {
            nombre[i] = consonantes[rand() % strlen(consonantes)];
        }
        usarVocal = !usarVocal;
    }
    nombre[longitud] = '\0';
}


void generatePassword(char* contrasena, int longitud) {
    const char* caracteres = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

    for (int i = 0; i < longitud; i++) {
        contrasena[i] = caracteres[rand() % strlen(caracteres)];
    }
    contrasena[longitud] = '\0';
}

void manejarCliente(SOCKET clientSocket) {
    char buffer[MAX];
    int n;

    n = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    buffer[n] = '\0';

    char comando[MAX];
    int longitud;
    sscanf(buffer, "%s %d", comando, &longitud);

    if (strncmp("NOMBRE", comando, 6) == 0) {
        printf("Se recibio del cliente una solicitud para generar un nombre de usuario de longitud %d.\n", longitud);
        if (longitud >= 5 && longitud <= 15) {
            char nombreUsuario[longitud + 1];
            generateUser(nombreUsuario, longitud);
            send(clientSocket, nombreUsuario, strlen(nombreUsuario), 0);
        } else {
            char mensaje[] = "Error: La longitud del nombre de usuario debe ser entre 5 y 15.";
            send(clientSocket, mensaje, strlen(mensaje), 0);
        }
    } else if (strncmp("CONTRASENA", comando, 10) == 0) {
        printf("Se recibio del cliente una solicitud para generar un password de longitud %d.\n", longitud);
        if (longitud >= 8 && longitud <= 50) {
            char contrasena[longitud + 1];
            generatePassword(contrasena, longitud);
            send(clientSocket, contrasena, strlen(contrasena), 0);
        } else {
            char mensaje[] = "Error: La longitud de la contraseña debe ser entre 8 y 50.";
            send(clientSocket, mensaje, strlen(mensaje), 0);
        }
    } else {
        char mensaje[] = "Comando no reconocido.";
        send(clientSocket, mensaje, strlen(mensaje), 0);
    }

    closesocket(clientSocket);
}

int main() {
    WSADATA wsa;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int addrLen = sizeof(clientAddr);

    srand(time(0));

    printf("Inicializando Winsock...\n");
    WSAStartup(MAKEWORD(2,2), &wsa);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        printf("No se pudo crear el socket. Error: %d\n", WSAGetLastError());
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        printf("Error en bind. Error: %d\n", WSAGetLastError());
        return 1;
    }

    listen(serverSocket, 3);
    printf("Esperando conexiones...\n");

    while ((clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen)) != INVALID_SOCKET) {
        manejarCliente(clientSocket);
    }

    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
