#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

struct parametros
{
    int ancho;
    int tesoros;
    int minas;
};
struct cazador
{
    int cazadorX;
    int cazadorY;
};
// Variables globales
struct parametros modo;
struct cazador posicion;
int movimientos, tesoros;
// Funciones principales
int seccionMenu(int ultimaPartida, int record);
int calcularRecord(int ultimaPartida, int record, int partidas);
int seleccionarDificultad();
void mostrarMenu(char menu[10][40]);
void mostrarMenuDificultad(char menuDificultad[9][40]);
void llenarMatriz(char matriz[10][modo.ancho]);
void imprimirMatriz(char matriz[10][modo.ancho]);
//  Modos
int modoFacil();
int modoMedio();
int modoDificil();
// Movimientos menu principal
int moverDerechaMenu(char menu[10][40], int puntero);
int moverIzquierdaMenu(char menu[10][40], int puntero);
// Movimientos menu dificultad
int moverArribaMenuDificultad(char menu[9][40], int puntero);
int moverAbajoMenuDificultad(char menu[9][40], int puntero);
// Movimientos cazador
struct cazador moverDerecha(char matriz[10][modo.ancho]);
struct cazador moverIzquierda(char matriz[10][modo.ancho]);
struct cazador moverArriba(char matriz[10][modo.ancho]);
struct cazador moverAbajo(char matriz[10][modo.ancho]);

int main()
{
    int dificultad = 0, opcionMenu = 0, ultimaPartida = 0, record = 0, partidas = 0;

    while (opcionMenu != 2)
    {
        opcionMenu = seccionMenu(ultimaPartida, record);
        if (opcionMenu == 1) // Jugar
        {
            dificultad = seleccionarDificultad();

            if (dificultad == 1)
                ultimaPartida = modoFacil();

            if (dificultad == 2)
                ultimaPartida = modoMedio();

            if (dificultad == 3)
                ultimaPartida = modoDificil();
        }
        
        record = calcularRecord(ultimaPartida, record, partidas);
        partidas++;

        if (opcionMenu == 2) // Salir
            break;
    }
    printf("Gracias por jugar :D");
    return 0;
}
// Funciones principales
int seccionMenu(int ultimaPartida, int record)
{
    int opcion, puntero = 7; // Posicion: 7 [Jugar], 25 [Salir]
    char tecla, strUltimaPartida[10], strRecord[10];
    char menu[10][40] = {
        "*--------------------------------------*",
        "|          CAZADOR DE TESOROS          |",
        "|--------------------------------------|",
        "|      > Jugar             Salir       |",
        "|--------------------------------------|",
        "|               REGISTRO               |",
        "|--------------------------------------|",
        "| Movimentos ultima partida:           |",
        "| Menor cantidad de movimientos:       |",
        "*--------------------------------------*",
    };                                              // Falta agregar record de movimientos y movimientos ultima partida
    sprintf(strUltimaPartida, "%d", ultimaPartida); // Convierte int a cadena de caracteres
    sprintf(strRecord, "%d", record);

    sprintf(menu[7] + 29, "%s", strUltimaPartida);
    sprintf(menu[8] + 33, "%s", strRecord);

    mostrarMenu(menu);
    do
    {
        tecla = toupper(getch());
        fflush(stdin);

        if (tecla == 'A' && puntero == 25)
            puntero = moverIzquierdaMenu(menu, puntero);

        if (tecla == 'D' && puntero == 7)
            puntero = moverDerechaMenu(menu, puntero);

    } while (tecla != 13);

    if (puntero == 7)
        opcion = 1;
    if (puntero == 25)
        opcion = 2;

    return opcion;
}
int seleccionarDificultad()
{
    int opcion, puntero = 5; // 5, 6, 7
    char tecla;
    char menuDificultad[9][40] = {"*--------------------------------------*",
                                  "|          CAZADOR DE TESOROS          |",
                                  "|--------------------------------------|",
                                  "|        SELECCION DE DIFICULTAD       |",
                                  "|--------------------------------------|",
                                  "|             > FACIL                  |",
                                  "|               MEDIO                  |",
                                  "|               DIFICIL                |",
                                  "*--------------------------------------*"};
    mostrarMenuDificultad(menuDificultad);
    do
    {
        tecla = toupper(getch());
        fflush(stdin);

        if (tecla == 'W' && puntero > 5)
            puntero = moverArribaMenuDificultad(menuDificultad, puntero);

        if (tecla == 'S' && puntero < 7)
            puntero = moverAbajoMenuDificultad(menuDificultad, puntero);

    } while (tecla != 13);
    if (puntero == 5)
        opcion = 1;
    if (puntero == 6)
        opcion = 2;
    if (puntero == 7)
        opcion = 3;

    return opcion;
}
int calcularRecord(int ultimaPartida, int record, int partidas)
{
    if (partidas == 0 || record > ultimaPartida)
        return ultimaPartida;
    else
        return record;
}
void mostrarMenu(char menu[10][40])
{
    int X, Y;
    system("cls");
    for (Y = 0; Y < 10; Y++)
    {
        for (X = 0; X < 40; X++)
            printf("%c", menu[Y][X]);
        printf("\n");
    }
}
void mostrarMenuDificultad(char menuDificultad[9][40])
{
    int X, Y;
    system("cls");
    for (Y = 0; Y < 9; Y++)
    {
        for (X = 0; X < 40; X++)
            printf("%c", menuDificultad[Y][X]);
        printf("\n");
    }
}
void llenarMatriz(char matriz[10][modo.ancho])
{
    srand(time(NULL));
    // Llenado con espacios
    int X, Y, i;
    for (Y = 0; Y < 10; Y++)
    {
        for (X = 0; X < modo.ancho; X++)
            matriz[Y][X] = ' ';
    }

    // Posicion del cazador
    matriz[0][0] = '*';

    // Llenado con tesoros
    for (i = 0; i < modo.tesoros; i++)
    {
        do
        {
            X = rand() % modo.ancho;
            Y = rand() % 10;
        } while (matriz[Y][X] != ' ');
        matriz[Y][X] = 'T';
    }

    // Llenado con minas
    for (i = 0; i < modo.minas; i++)
    {
        do
        {
            X = rand() % modo.ancho;
            Y = rand() % 10;
        } while (matriz[Y][X] != ' ');
        matriz[Y][X] = 'M';
    }
}
void imprimirMatriz(char matriz[10][modo.ancho])
{
    int X, Y;
    printf("Movimientos: %d\t   Tesoros: %d\n", movimientos, tesoros);
    for (Y = 0; Y < 10; Y++)
    {
        for (X = 0; X < modo.ancho; X++)
            printf("[%c]", matriz[Y][X]);
        printf("\n");
    }
}
// Modos
int modoFacil()
{
    system("cls");
    modo.ancho = 10;
    modo.tesoros = 5;
    modo.minas = 10;
    posicion.cazadorX = 0;
    posicion.cazadorY = 0;
    movimientos = 0;
    tesoros = modo.tesoros;

    char matriz[10][modo.ancho];
    char tecla;

    llenarMatriz(matriz);
    system("cls");
    imprimirMatriz(matriz);
    do
    {
        do
        {
            tecla = toupper(getch());
            fflush(stdin);
        } while ((tecla == 'W' && posicion.cazadorY == 0) ||
                 (tecla == 'A' && posicion.cazadorX == 0) ||
                 (tecla == 'S' && posicion.cazadorY == 9) ||
                 (tecla == 'D' && posicion.cazadorX == 9));
        switch (tecla)
        {
        case 'W':
            posicion = moverArriba(matriz);
            break;
        case 'A':
            posicion = moverIzquierda(matriz);
            break;
        case 'S':
            posicion = moverAbajo(matriz);
            break;
        case 'D':
            posicion = moverDerecha(matriz);
            break;
        }
        system("cls");
        imprimirMatriz(matriz);
    } while (tesoros != 0);
    return movimientos;
}
int modoMedio()
{
    system("cls");
    modo.ancho = 15;
    modo.tesoros = 10;
    modo.minas = 20;
    posicion.cazadorX = 0;
    posicion.cazadorY = 0;
    movimientos = 0;
    tesoros = modo.tesoros;

    char matriz[10][modo.ancho];
    char tecla;

    llenarMatriz(matriz);
    system("cls");
    imprimirMatriz(matriz);
    do
    {
        do
        {
            tecla = toupper(getch());
            fflush(stdin);
        } while ((tecla == 'W' && posicion.cazadorY == 0) ||
                 (tecla == 'A' && posicion.cazadorX == 0) ||
                 (tecla == 'S' && posicion.cazadorY == 9) ||
                 (tecla == 'D' && posicion.cazadorX == 14));
        switch (tecla)
        {
        case 'W':
            posicion = moverArriba(matriz);
            break;
        case 'A':
            posicion = moverIzquierda(matriz);
            break;
        case 'S':
            posicion = moverAbajo(matriz);
            break;
        case 'D':
            posicion = moverDerecha(matriz);
            break;
        }
        system("cls");
        imprimirMatriz(matriz);
    } while (tesoros != 0);
    return movimientos;
}
int modoDificil()
{
    system("cls");
    modo.ancho = 15;
    modo.tesoros = 20;
    modo.minas = 50;
    posicion.cazadorX = 0;
    posicion.cazadorY = 0;
    movimientos = 0;
    tesoros = modo.tesoros;

    char matriz[10][modo.ancho];
    char tecla;

    llenarMatriz(matriz);
    system("cls");
    imprimirMatriz(matriz);
    do
    {
        do
        {
            tecla = toupper(getch());
            fflush(stdin);
        } while ((tecla == 'W' && posicion.cazadorY == 0) ||
                 (tecla == 'A' && posicion.cazadorX == 0) ||
                 (tecla == 'S' && posicion.cazadorY == 9) ||
                 (tecla == 'D' && posicion.cazadorX == 14));
        switch (tecla)
        {
        case 'W':
            posicion = moverArriba(matriz);
            break;
        case 'A':
            posicion = moverIzquierda(matriz);
            break;
        case 'S':
            posicion = moverAbajo(matriz);
            break;
        case 'D':
            posicion = moverDerecha(matriz);
            break;
        }
        system("cls");
        imprimirMatriz(matriz);
    } while (tesoros != 0);
    return movimientos;
}
// Movimientos del menu de principal
int moverDerechaMenu(char menu[10][40], int puntero)
{
    menu[3][puntero] = ' ';
    puntero += 18;
    menu[3][puntero] = '>';
    mostrarMenu(menu);
    return puntero;
}
int moverIzquierdaMenu(char menu[10][40], int puntero)
{
    menu[3][puntero] = ' ';
    puntero -= 18;
    menu[3][puntero] = '>';
    mostrarMenu(menu);
    return puntero;
}
// Movimientos del menu de dificultad
int moverArribaMenuDificultad(char menuDificultad[9][40], int puntero)
{
    menuDificultad[puntero][14] = ' ';
    puntero--;
    menuDificultad[puntero][14] = '>';
    mostrarMenuDificultad(menuDificultad);
    return puntero;
}
int moverAbajoMenuDificultad(char menuDificultad[9][40], int puntero)
{
    menuDificultad[puntero][14] = ' ';
    puntero++;
    menuDificultad[puntero][14] = '>';
    mostrarMenuDificultad(menuDificultad);
    return puntero;
}
// Movimientos del cazador
struct cazador moverArriba(char matriz[10][modo.ancho])
{
    movimientos++;
    matriz[posicion.cazadorY][posicion.cazadorX] = ' ';
    if (matriz[posicion.cazadorY - 1][posicion.cazadorX] == 'M')
    {
        matriz[posicion.cazadorY - 1][posicion.cazadorX] = ' ';
        posicion.cazadorX = 0;
        posicion.cazadorY = 0;
        matriz[0][0] = '*';
        return posicion;
    }
    if (matriz[posicion.cazadorY - 1][posicion.cazadorX] == 'T')
        tesoros--;
    matriz[posicion.cazadorY - 1][posicion.cazadorX] = '*';
    posicion.cazadorY--;
    return posicion;
}
struct cazador moverIzquierda(char matriz[10][modo.ancho])
{
    movimientos++;
    matriz[posicion.cazadorY][posicion.cazadorX] = ' ';
    if (matriz[posicion.cazadorY][posicion.cazadorX - 1] == 'M')
    {
        matriz[posicion.cazadorY][posicion.cazadorX - 1] = ' ';
        posicion.cazadorX = 0;
        posicion.cazadorY = 0;
        matriz[0][0] = '*';
        return posicion;
    }
    if (matriz[posicion.cazadorY][posicion.cazadorX - 1] == 'T')
        tesoros--;
    matriz[posicion.cazadorY][posicion.cazadorX - 1] = '*';
    posicion.cazadorX--;
    return posicion;
}
struct cazador moverAbajo(char matriz[10][modo.ancho])
{
    movimientos++;
    matriz[posicion.cazadorY][posicion.cazadorX] = ' ';
    if (matriz[posicion.cazadorY + 1][posicion.cazadorX] == 'M')
    {
        matriz[posicion.cazadorY + 1][posicion.cazadorX] = ' ';
        posicion.cazadorX = 0;
        posicion.cazadorY = 0;
        matriz[0][0] = '*';
        return posicion;
    }
    if (matriz[posicion.cazadorY + 1][posicion.cazadorX] == 'T')
        tesoros--;
    matriz[posicion.cazadorY + 1][posicion.cazadorX] = '*';
    posicion.cazadorY++;
    return posicion;
}
struct cazador moverDerecha(char matriz[10][modo.ancho])
{
    movimientos++;
    matriz[posicion.cazadorY][posicion.cazadorX] = ' ';
    if (matriz[posicion.cazadorY][posicion.cazadorX + 1] == 'M')
    {
        matriz[posicion.cazadorY][posicion.cazadorX + 1] = ' ';
        posicion.cazadorX = 0;
        posicion.cazadorY = 0;
        matriz[0][0] = '*';
        return posicion;
    }
    if (matriz[posicion.cazadorY][posicion.cazadorX + 1] == 'T')
        tesoros--;
    matriz[posicion.cazadorY][posicion.cazadorX + 1] = '*';
    posicion.cazadorX++;
    return posicion;
}
