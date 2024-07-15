#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// Función para mostrar el mensaje de bienvenida
void bienvenida() {
    printf("Bienvenido al juego de Blackjack!\n");
    printf("Tienes un presupuesto de $1000 para apostar.\n");
    printf("La apuesta mínima es de $50.\n");
}

// Función para obtener la apuesta del usuario
int obtener_apuesta(int presupuesto) {
    int apuesta;
    char buffer[256]; // Buffer para almacenar la entrada del usuario

    while (1) {
        printf("Ingrese su apuesta (Presupuesto: $%d): ", presupuesto);
        if (!fgets(buffer, sizeof(buffer), stdin)) { // Lee la entrada del usuario
            printf("Error al leer la entrada.\n");
            continue;
        }

        // Intenta convertir la entrada a un entero
        if (sscanf(buffer, "%d", &apuesta) != 1) {
            printf("Entrada no válida. Por favor ingrese un número.\n");
            continue;
        }

        if (apuesta < 50) {
            printf("Es erroneo, La apuesta mínima es de $50.\n");
        } else if (apuesta > presupuesto) {
            printf("No puedes apostar más de lo que tienes en tu presupuesto.\n");
        } else {
            return apuesta;
        }
    }
}

// Función para repartir cartas aleatorias
void repartir_cartas(int cartas[], int cantidad) {
    for (int i = 0; i < cantidad; i++) {
        int carta = rand() % 13 + 1; // Genera una carta aleatoria entre 1 y 13
        cartas[i] = carta;
    }
}

// Función para calcular el valor total de las cartas
int calcular_valor(int cartas[], int cantidad) {
    int valor = 0;
    for (int i = 0; i < cantidad; i++) {
        valor += cartas[i];
    }
    return valor;
}

// Función para mostrar las cartas de un jugador
void mostrar_cartas(char *jugador, int cartas[], int cantidad) {
    printf("%s tiene las cartas: ", jugador);
    for (int i = 0; i < cantidad; i++) {
        if (cartas[i] == 11) {
            printf("J ");
        } else if (cartas[i] == 12) {
            printf("Q ");
        } else if (cartas[i] == 13) {
            printf("K ");
        } else {
            printf("%d ", cartas[i]);
        }
    }
    printf("con un valor total de %d\n", calcular_valor(cartas, cantidad));
}

// Función para determinar el ganador
char* determinar_ganador(int valor_jugador, int valor_programa) {
    if (valor_jugador > 21) {
        return "programa"; // El jugador pierde si su valor supera 21
    } else if (valor_programa > 21 || valor_jugador > valor_programa) {
        return "jugador"; // El jugador gana si el programa se pasa o tiene un valor menor
    } else {
        return "programa"; // El programa gana en otros casos
    }
}

int main() {
    int presupuesto = 1000;
    bienvenida(); // Muestra el mensaje de bienvenida
    srand(time(NULL)); // Inicializa el generador de números aleatorios
    
    while (presupuesto >= 50) {
        int apuesta = obtener_apuesta(presupuesto); // Obtiene la apuesta del jugador
        
        int cartas_jugador[5];
        int cartas_programa[2];
        
        repartir_cartas(cartas_jugador, 2); // Reparte 2 cartas al jugador
        repartir_cartas(cartas_programa, 2); // Reparte 2 cartas al programa
        
        int cantidad_jugador = 2;
        int valor_jugador = calcular_valor(cartas_jugador, cantidad_jugador);
        int valor_programa = calcular_valor(cartas_programa, 2);
        
        mostrar_cartas("El jugador", cartas_jugador, cantidad_jugador); // Muestra las cartas del jugador
        
        // Permite al jugador pedir hasta 3 cartas adicionales
        while (cantidad_jugador < 5 && valor_jugador <= 21) {
            char otra_carta;
            printf("¿Deseas otra carta? (s/n): ");
            scanf(" %c", &otra_carta);
            if (otra_carta == 's') {
                repartir_cartas(&cartas_jugador[cantidad_jugador], 1); // Reparte una carta adicional
                cantidad_jugador++;
                valor_jugador = calcular_valor(cartas_jugador, cantidad_jugador);
                mostrar_cartas("El jugador", cartas_jugador, cantidad_jugador); // Muestra las cartas actualizadas del jugador
                if (valor_jugador > 21) {
                    break; // El jugador pierde si su valor supera 21
                }
            } else {
                break;
            }
        }
        
        printf("El programa está revelando sus cartas en 30 segundos...\n");
        sleep(30); // Retraso de 30 segundos
        
        mostrar_cartas("El programa", cartas_programa, 2); // Muestra las cartas del programa
        
        char* ganador = determinar_ganador(valor_jugador, valor_programa); // Determina el ganador
        
        // Actualiza el presupuesto según el resultado
        if (ganador == "jugador") {
            printf("¡Felicidades! Has ganado.\n");
            presupuesto += apuesta;
        } else {
            printf("Lo siento, has perdido.\n");
            presupuesto -= apuesta;
        }
        
        if (presupuesto < 50) {
            printf("No tienes suficiente presupuesto para seguir jugando.\n");
            break; // Termina el juego si el presupuesto es menor a 50
        }
        
        char continuar;
        printf("¿Quieres seguir jugando? (s/n): ");
        scanf(" %c", &continuar);
        if (continuar != 's') {
            break; // Termina el juego si el jugador no quiere seguir jugando
        }
    }
    
    printf("Te quedaste con un presupuesto de $%d. ¡Gracias por jugar!\n", presupuesto);
    return 0;
}
