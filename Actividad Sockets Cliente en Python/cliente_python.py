import socket

def conectar_al_servidor(mensaje):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.connect(('127.0.0.1', 8080))

        sock.sendall(mensaje.encode())

        data = sock.recv(1024)

    print(f"Respuesta del servidor: {data.decode()}")

def main():
    while True:
        # Mostrar el menú de opciones
        print("\nElija una opción:")
        print("1. Generar Nombre de Usuario")
        print("2. Generar Contraseña")
        print("3. Salir")

        opcion = input("Ingrese su opción: ")

        if opcion == '1':
            longitud = input("Ingrese la longitud del nombre de usuario (5-15): ")
            if longitud.isdigit() and 5 <= int(longitud) <= 15:
                mensaje = f"NOMBRE {longitud}"
                conectar_al_servidor(mensaje)
            else:
                print("Longitud no válida. Debe ser entre 5 y 15.")
        elif opcion == '2':

            longitud = input("Ingrese la longitud de la contraseña (8-50): ")
            if longitud.isdigit() and 8 <= int(longitud) <= 50:
                mensaje = f"CONTRASENA {longitud}"
                conectar_al_servidor(mensaje)
            else:
                print("Longitud no válida. Debe ser entre 8 y 50.")
        elif opcion == '3':
            print("Saliendo...")
            break
        else:
            print("Opción no válida. Inténtelo de nuevo.")

if __name__ == "__main__":
    main()