#include <stdio.h>

#ifdef _WIN32
#include <windows.h>  // Para Windows
#else
#include <unistd.h>  // Para Linux/Mac
#define RESET "\033[0m"         // Restablece color
#define RED "\033[31m"          // Rojo
#define GREEN "\033[32m"        // Verde
#define YELLOW "\033[33m"       // Amarillo
#define BLUE "\033[34m"         // Azul
#define CYAN "\033[36m"         // Cian
#define WHITE "\033[37m"        // Blanco
#endif

// Definición de constantes
#define TAMANO 10  // Tamaño del tablero
#define VACIO ' '  // Celda vacía
#define GOLPE 'X'  // Golpe exitoso
#define ERRASTE 'O'  // Golpe fallido
#define BARCO 'B'  // Barco

char TableroJugador1[TAMANO][TAMANO];
char TableroJugador2[TAMANO][TAMANO];

// Función para cambiar el color de texto en la consola
void setColor(const char* color) {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (color == "RED") SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
    else if (color == "GREEN") SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    else if (color == "YELLOW") SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
    else if (color == "BLUE") SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
    else if (color == "WHITE") SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    else SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
    printf("%s", color);
#endif
}

// Restablece el color predeterminado
void resetColor() {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
    printf(RESET);
#endif
}

// Función para hacer una pausa entre turnos
void pausaTurno() {
#ifdef _WIN32
    Sleep(2000);  // 2000 milisegundos = 2 segundos
#else
    sleep(2);  // 2 segundos
#endif
}

// Limpia la pantalla
void limpiarConsola() {
    system("cls");
}

// Inicializa el tablero
void inicializarTablero(char tablero[TAMANO][TAMANO]) {
    for (int i = 0; i < TAMANO; i++)
        for (int j = 0; j < TAMANO; j++)
            tablero[i][j] = VACIO;
}

// Imprime el tablero con colores
void imprimirTablero(char tablero[TAMANO][TAMANO], int mostrarBarcos) {
    printf("  ");
    for (int i = 0; i < TAMANO; i++) {
        printf("%d ", i);
    }
    printf("\n");

    for (int i = 0; i < TAMANO; i++) {
        printf("%d ", i);
        for (int j = 0; j < TAMANO; j++) {
            char celda = tablero[i][j];
            if (celda == GOLPE) {
                setColor("RED");
                printf("%c ", celda);
            } else if (celda == ERRASTE) {
                setColor("YELLOW");
                printf("%c ", celda);
            } else if (celda == BARCO && mostrarBarcos) {
                setColor("BLUE");
                printf("%c ", celda);
            } else {
                setColor("WHITE");
                printf("%c ", VACIO);
            }
            resetColor();
        }
        printf("\n");
    }
}

// Función para verificar si un barco puede ser colocado
int puedeColocarBarco(char tablero[TAMANO][TAMANO], int x, int y, int longitud, int direccion) {
    if (direccion == 0) {  // horizontal
        if (y + longitud > TAMANO) return 0;  // Fuera de los límites
        for (int i = 0; i < longitud; i++) {
            if (tablero[x][y + i] == BARCO) return 0;  // Ya hay un barco
        }
    } else {  // vertical
        if (x + longitud > TAMANO) return 0;  // Fuera de los límites
        for (int i = 0; i < longitud; i++) {
            if (tablero[x + i][y] == BARCO) return 0;  // Ya hay un barco
        }
    }
    return 1;  // Puede colocarse
}

// Coloca un barco en el tablero
void colocarBarco(char tablero[TAMANO][TAMANO], int longitud) {
    int x, y, direccion;
    while (1) {
        printf("Colocando un barco de tamano %d. Ingrese la fila y columna (0-9): ", longitud);
        scanf("%d %d", &x, &y);
        printf("Ingrese la direccion (0 = horizontal, 1 = vertical): ");
        scanf("%d", &direccion);

        if (puedeColocarBarco(tablero, x, y, longitud, direccion)) {
            for (int i = 0; i < longitud; i++) {
                int nuevoX = x + (direccion == 1 ? i : 0);
                int nuevoY = y + (direccion == 0 ? i : 0);
                tablero[nuevoX][nuevoY] = BARCO;
            }
            break;
        } else {
            setColor("YELLOW");
            printf("No se puede colocar el barco ahi. Intente de nuevo.\n");
            resetColor();
        }
    }
}

// Función para configurar los barcos del jugador
void configurarBarcosJugador() {
    colocarBarco(TableroJugador1, 5);  // Barco de tamaño 5
    colocarBarco(TableroJugador1, 4);  // Barco de tamaño 4
    colocarBarco(TableroJugador1, 3);  // Barco de tamaño 3
    colocarBarco(TableroJugador1, 2);  // Barco de tamaño 2
}

// Función para configurar los barcos del segundo jugador (humano)
void configurarBarcosJugador2() {
    colocarBarco(TableroJugador2, 5);  // Barco de tamaño 5
    colocarBarco(TableroJugador2, 4);  // Barco de tamaño 4
    colocarBarco(TableroJugador2, 3);  // Barco de tamaño 3
    colocarBarco(TableroJugador2, 2);  // Barco de tamaño 2
}

// Función para guardar los puntos en un archivo 
void guardarPuntajes(int golpesJugador1, int golpesJugador2) { 
    FILE* archivo = fopen("puntajes.txt", "a"); // Abrir el archivo en modo añadir 
    if (archivo == NULL) { 
        setColor("RED"); // Cambiar el color del texto a rojo 
        printf("Error al abrir el archivo de puntajes.\n");
        resetColor(); // Restablecer el color del texto al predeterminado 
        return; // Salir de la función si no se pudo abrir el archivo 
    }  
    fprintf(archivo, "Jugador 1: %d puntos | Jugador 2: %d puntos\n", golpesJugador1, golpesJugador2); // Escribir los puntajes en el archivo
    fclose(archivo); // Cerrar el archivo 
}

int main() {

    printf("    Bienvenido a Batalla Naval    \n\n"); 
    printf("Reglas del juego:\n"); 
    printf("1. El juego se juega en un tablero de 10x10.\n"); 
    printf("2. Ambos jugadores (tu y la computadora) colocaran sus barcos en el tablero.\n"); 
    printf("3. Hay barcos de diferentes tamanos (2, 3, 4 y 5 casillas).\n"); 
    printf("4. El objetivo es hundir todos los barcos del oponente antes de que hundan los tuyos.\n"); 
    printf("5. Los tiros exitosos se marcan con una 'X' y los fallidos con una 'O'.\n\n");

    // Inicialización de los tableros
    inicializarTablero(TableroJugador1);
    inicializarTablero(TableroJugador2);

    setColor("CYAN");
    resetColor();

    configurarBarcosJugador();
    limpiarConsola();
    configurarBarcosJugador2();
    
    int golpesJugador1 = 0;
    int golpesJugador2 = 0;
    int turno = 0;
    limpiarConsola();

    while (golpesJugador1 < 14 && golpesJugador2 < 14) {
        // Jugar contra otro jugador
        if (turno % 2 == 0) {
            // Turno del jugador 1
            printf("\nTablero del Jugador 1:\n"); 
            imprimirTablero(TableroJugador1, 1);//Muestra por pantalla tablero del jugador 1
            printf("\nTablero del Jugador 2:\n"); 
            imprimirTablero(TableroJugador2, 0);//Muestra por pantalla tablero del jugador 2

            int adivinaX, adivinaY;
            printf("Es tu turno. Ingresa coordenadas (fila columna): ");
            scanf("%d %d", &adivinaX, &adivinaY);

            if (adivinaX < 0 || adivinaX >= TAMANO || adivinaY < 0 || adivinaY >= TAMANO) {
                setColor("YELLOW");
                printf("Coordenadas fuera de rango. Intenta de nuevo.\n");
                resetColor();
                continue;
            }

            if (TableroJugador2[adivinaX][adivinaY] == BARCO) {
                setColor("GREEN");
                printf("¡Tocado!\n");
                resetColor();
                TableroJugador2[adivinaX][adivinaY] = GOLPE;
                golpesJugador1++;
            } else if (TableroJugador2[adivinaX][adivinaY] == VACIO) {
                setColor("YELLOW");
                printf("¡Agua!\n");
                resetColor();
                TableroJugador2[adivinaX][adivinaY] = ERRASTE;
            }
        } else {
            // Turno del jugador 2
            printf("\nTablero del Jugador 1:\n");
            imprimirTablero(TableroJugador1, 0);
            printf("\nTablero del Jugador 2:\n");
            imprimirTablero(TableroJugador2, 1);

            int adivinaX, adivinaY;
            printf("Es tu turno. Ingresa coordenadas (fila columna): ");
            scanf("%d %d", &adivinaX, &adivinaY);

            if (adivinaX < 0 || adivinaX >= TAMANO || adivinaY < 0 || adivinaY >= TAMANO) {
                setColor("YELLOW");
                printf("Coordenadas fuera de rango. Intenta de nuevo.\n");
                resetColor();
                continue;
            }

            if (TableroJugador1[adivinaX][adivinaY] == BARCO) {
                setColor("GREEN");
                printf("¡Tocado!\n");
                resetColor();
                TableroJugador1[adivinaX][adivinaY] = GOLPE;
                golpesJugador2++;
            } else if (TableroJugador1[adivinaX][adivinaY] == VACIO) {
                setColor("YELLOW");
                printf("¡Agua!\n");
                resetColor();       
                TableroJugador1[adivinaX][adivinaY] = ERRASTE;
            }
        }

        // Pausa de 2 segundos antes del siguiente turno
        pausaTurno();
        limpiarConsola();
        turno++;
    }

    if (golpesJugador1 >= 14) {
        setColor("GREEN");
        printf("¡JUGADOR 1 ES EL VENCEDOR!\n");
    } else {
        setColor("RED");
        printf("¡JUGADOR 2 ES EL VENCEDOR!\n");
    }
    resetColor();

    guardarPuntajes(golpesJugador1,golpesJugador2);

    system("pause"); // Pausar la consola para permitir ver el mensaje final antes de salir

    return 0;
}