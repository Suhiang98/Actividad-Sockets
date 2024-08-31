#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <winsock2.h>

#define PORT 8080
#define MAX 1024

void connectServer(char* mensaje) {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in serverAddr;
    char buffer[MAX];
    int n;

    WSAStartup(MAKEWORD(2,2), &wsa);

    // Crear socket del cliente
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        printf("No se pudo crear el socket. Error: %d\n", WSAGetLastError());
        return;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(PORT);

    if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        printf("Conexión fallida. Error\n");
        return;
    }

    send(sock, mensaje, strlen(mensaje), 0);

    n = recv(sock, buffer, sizeof(buffer) - 1, 0);
    buffer[n] = '\0';
    printf("Respuesta del servidor -> %s\n", buffer);

    closesocket(sock);
    WSACleanup();
}

int main() {
    char mensaje[MAX];
    int opcion;
    int longitud;

    while (1) {
        printf("\nElija una opcion:\n1. Generar Nombre de Usuario\n2. Generar Password\n3. Salir\n");

        if (scanf("%d", &opcion) != 1) {
            printf("Entrada no valida. Por favor, ingrese un número.\n");
            while (getchar() != '\n');
            continue;
        }

        switch(opcion) {
            case 1:
                printf("Ingrese la longitud del nombre de usuario tiene que ser entre este rango indicado (5-15): ");
                if (scanf("%d", &longitud) != 1) {
                    printf("Entrada no válida. Por favor, ingrese un número.\n");
                    while (getchar() != '\n');
                    continue;
                }
                snprintf(mensaje, MAX, "NOMBRE %d", longitud);
                connectServer(mensaje);
                break;
            case 2:
                printf("Ingrese la longitud del password tiene que ser entre este rango indicado(8-50): ");
                if (scanf("%d", &longitud) != 1) {
                    printf("Entrada no válida. Por favor, ingrese un número.\n");
                    while (getchar() != '\n');
                    continue;
                }
                snprintf(mensaje, MAX, "CONTRASENA %d", longitud);
                connectServer(mensaje);
                break;
            case 3:
                printf("Saliendo...\n");
                return 0;
            default:
                printf("Opción no válida. Inténtelo de nuevo.\n");
                break;
        }
    }

    return 0;
}
