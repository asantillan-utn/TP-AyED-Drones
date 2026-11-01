#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <limits>
#include <string>
#define FILAS 200
#define COLUMNAS 200
#define LINEAS_POR_PAGINA 20

using namespace std;

struct Orden
{
    unsigned int espera = 0;
    bool soltarGranada1 = false;
    bool soltarGranada2 = false;
    bool ataqueKamikaze = false;
    bool aterrizaje = false;
    bool despegue = false;
    int siguientex = 0;
    int siguientey = 0;
    bool cargada = false;
};

struct OrdenArchivo
{
    int x = 0;
    int y = 0;
    unsigned int espera = 0;
    bool soltarGranada1 = false;
    bool soltarGranada2 = false;
    bool ataqueKamikaze = false;
    bool aterrizaje = false;
    bool despegue = false;
    int siguientex = 0;
    int siguientey = 0;
};

///Funciones
Orden** iniciarMatrizOrden();
void liberarMatrizOrden(Orden **orden);
void cargarMenuPrincipal(Orden **orden);
void esperarEnter();

///Opcion 1
void cargaArchivoDeAtaque(Orden **orden);
bool esAledana(int x1, int y1, int x2, int y2);
bool coordenadaValida(int x, int y);

///Opcion 2
void mostrarAtaqueCargado(Orden **orden);
string descripcionOrden(const Orden &o);

int main()
{
    Orden **orden = iniciarMatrizOrden();
    cargarMenuPrincipal(orden);
    liberarMatrizOrden(orden);

    return 0;
}

///IMPLEMENTACION DE FUNCIONES
Orden** iniciarMatrizOrden()
{
    Orden **matriz = new Orden*[FILAS];

    for(int i = 0; i < FILAS; i++)
    {
        matriz[i] = new Orden[COLUMNAS];
    }
    return matriz;
}
void liberarMatrizOrden(Orden **orden){

    for( int i=0; i <FILAS; i++){
        delete[] orden[i];
    }

    delete[] orden;
}

void esperarEnter()
{
    cout << "\nPresione ENTER para volver al menu...";
    cin.get();
}

void cargarMenuPrincipal(Orden **orden){
    int op;

    do{
        cout << "TRABAJO PRACTICO - AYED - DRONES"<<endl<<endl;
        cout << "=============================================="<<endl;
        cout << "1. Cargar archivo de ataque en memoria."<<endl;
        cout << "2. Mostrar ataque cargado. "<<endl;
        cout << "3. Crear un archivo de ataque nuevo"<<endl;
        cout << "4. Corregir un registro del archivo.."<<endl;
        cout << "5. Corregir un registro en memoria. "<<endl;
        cout << "6. Guardar memoria en un archivo nuevo."<<endl;
        cout << "7. Visualizar un archivo de ataque en html."<<endl;
        cout << "0. Salir"<<endl;
        cout << "=============================================="<<endl<<endl<<endl;
        cout << "seleccione una opcion: ";
        bool opcionValida = true;
        do{
            cin >> op;
            opcionValida= true;
            if(op < 0 || op > 7){
                cout << "Opcion fuera de rango. Ingrese un numero entre 0 y 7: ";
                opcionValida = false;
            }
        } while(!opcionValida);

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        system("cls");

        switch(op){
        case 1:
            cargaArchivoDeAtaque(orden);
            break;
        case 2:
            mostrarAtaqueCargado(orden);
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
        case 0:
            cout << "Saliendo del programa..."<<endl;
            break;
        }

        if (op != 0)
        {
            system("cls");
        }

    } while(op != 0);
}

/// ---------------------------------------------------------------------
/// OPCION 1: Cargar archivo de ataque en memoria
/// ---------------------------------------------------------------------

bool coordenadaValida(int x, int y)
{
    if (x >= 0 && x < FILAS && y >= 0 && y < COLUMNAS){
        return true;
    }
    else return false;
}

// Verifica si corresponde a la celda siguiente
bool esAledana(int x1, int y1, int x2, int y2)
{
    int dx = abs(x1 - x2);
    int dy = abs(y1 - y2);
    return (dx <= 1 && dy <= 1 && !(dx == 0 && dy == 0));
}

void cargaArchivoDeAtaque(Orden **orden)
{
    char ruta[260];
    char nombre[100];
    char rutaCompleta[400];

    cout << "=========================================="<<endl;
    cout << "Opcion 1 - Carga de archivo de ataque"<<endl;
    cout << "=========================================="<<endl;
    cout << endl << endl;
    cout << "Ingrese la ruta del archivo: ";
    cin.getline(ruta, sizeof(ruta));
    cout << endl << "Ingrese el nombre del archivo: ";
    cin.getline(nombre, sizeof(nombre));

    snprintf(rutaCompleta, sizeof(rutaCompleta), "%s%s", ruta, nombre);

    FILE *f = fopen(rutaCompleta, "rb");
    if (f == NULL)
    {
        cout << endl << "HUBO UN ERROR AL ABRIR EL ARCHIVO." << endl;
        esperarEnter();
        return;
    }
    const int MAX_REGISTROS = FILAS * COLUMNAS;
    OrdenArchivo *registros = new OrdenArchivo[MAX_REGISTROS];
    int cantidad = 0;

    OrdenArchivo temp;
    while (cantidad < MAX_REGISTROS && fread(&temp, sizeof(OrdenArchivo), 1, f) == 1)
    {
        registros[cantidad] = temp;
        cantidad++;
    }
    fclose(f);

    if (cantidad == 0)
    {
        cout << endl << "EL ARCHIVO ESTA VACIO. NO HAY NADA PARA CARGAR." << endl;
        delete[] registros;
        esperarEnter();
        return;
    }

    // 2) Validaciones
    static bool ocupada[FILAS][COLUMNAS];
    for (int i = 0; i < FILAS; i++)
    {
        for (int j = 0; j < COLUMNAS; j++) ocupada[i][j] = false;
    }

    bool archivoValido = true;
    string error = "";

    int cantDespegues = 0;
    int cantFinales = 0; // aterrizaje o ataqueKamikaze
    int xDespegue = -1, yDespegue = -1;
    int xFinal = -1, yFinal = -1;

    for (int i = 0; i < cantidad && archivoValido; i++)
    {
        OrdenArchivo &r = registros[i];

        if (!coordenadaValida(r.x, r.y))
        {
            archivoValido = false;
            error = "Hay un registro con coordenadas fuera de la tabla (0-199).";
            break;
        }

        if (ocupada[r.x][r.y] == true)
        {
            archivoValido = false;
            error = "Hay coordenadas repetidas en el archivo (no se puede pasar dos veces por el mismo casillero).";
            break;
        }
        ocupada[r.x][r.y] = true;

        if (r.despegue && (r.soltarGranada1 || r.soltarGranada2 || r.ataqueKamikaze))
        {
            archivoValido = false;
            error = "La instruccion de despegue no puede estar acompanada de ningun ataque.";
            break;
        }

        if (r.despegue)
        {
            cantDespegues++;
            xDespegue = r.x;
            yDespegue = r.y;
        }

        if (r.aterrizaje || r.ataqueKamikaze)
        {
            cantFinales++;
            xFinal = r.x;
            yFinal = r.y;
        }
    }

    if (archivoValido && cantDespegues != 1)
    {
        archivoValido = false;
        error = "El archivo debe tener una unica instruccion de despegue.";
    }

    if (archivoValido && cantFinales != 1)
    {
        archivoValido = false;
        error = "Debe existir una unica instruccion de aterrizaje o de ataque kamikaze.";
    }

    //3) Validar que la ruta sea continua
    if (archivoValido)
    {
        static bool visitada[FILAS][COLUMNAS];
        for (int i = 0; i < FILAS; i++)
        {
            for (int j = 0; j < COLUMNAS; j++) visitada[i][j] = false;
        }

        int x = xDespegue, y = yDespegue;
        bool llego = false;

        while (archivoValido && !llego)
        {
            if (!ocupada[x][y])
            {
                archivoValido = false;
                error = "La ruta tiene huecos: pasa por una celda sin orden cargada.";
                break;
            }
            if (visitada[x][y])
            {
                archivoValido = false;
                error = "La ruta forma un bucle (vuelve a pasar por una celda ya visitada).";
                break;
            }
            visitada[x][y] = true;

            if (x == xFinal && y == yFinal)
            {
                llego = true;
                break;
            }

            OrdenArchivo *actual = nullptr;
            for (int i = 0; i < cantidad; i++)
            {
                if (registros[i].x == x && registros[i].y == y)
                {
                    actual = &registros[i];
                    break;
                }
            }

            int nx = actual->siguientex;
            int ny = actual->siguientey;

            if (!coordenadaValida(nx, ny))
            {
                archivoValido = false;
                error = "La ruta sale de los limites de la tabla.";
                break;
            }

            if (!esAledana(x, y, nx, ny))
            {
                archivoValido = false;
                error = "La ruta tiene un salto entre celdas que no son aledanas.";
                break;
            }

            x = nx;
            y = ny;
        }
    }

    //4) Resultado
    if (!archivoValido)
    {
        cout << endl << "ERROR: " << error << endl;
        cout << "No se cargo el archivo en memoria." << endl;
    }
    else
    {
        // Setea registros a la matriz definitiva.
        for (int i = 0; i < FILAS; i++)
        {
            for (int j = 0; j < COLUMNAS; j++)
            {
                orden[i][j] = Orden();
            }
        }

        for (int i = 0; i < cantidad; i++)
        {
            OrdenArchivo &r = registros[i];
            Orden &o = orden[r.x][r.y];

            o.espera = r.espera;
            o.soltarGranada1 = r.soltarGranada1;
            o.soltarGranada2 = r.soltarGranada2;
            o.ataqueKamikaze = r.ataqueKamikaze;
            o.aterrizaje = r.aterrizaje;
            o.despegue = r.despegue;
            o.siguientex = r.siguientex;
            o.siguientey = r.siguientey;
            o.cargada = true;
        }

        cout << endl << "Archivo cargado correctamente." << endl;
    }

    delete[] registros;

    esperarEnter();
}

/// ---------------------------------------------------------------------
/// OPCION 2: Mostrar ataque cargado
/// ---------------------------------------------------------------------

string descripcionOrden(const Orden &o)
{
    string desc = "";

    if (o.despegue) desc += "DESPEGUE ";
    if (o.aterrizaje) desc += "ATERRIZAJE ";
    if (o.soltarGranada1) desc += "GRANADA1 ";
    if (o.soltarGranada2) desc += "GRANADA2 ";
    if (o.ataqueKamikaze) desc += "KAMIKAZE ";
    if (o.espera > 0) desc += "ESPERA=" + to_string(o.espera) + " ";

    desc += "-> (" + to_string(o.siguientex) + "," + to_string(o.siguientey) + ")";

    return desc;
}

void mostrarAtaqueCargado(Orden **orden)
{
    int cantidad = 0;
    for (int i = 0; i < FILAS; i++)
    {
        for (int j = 0; j < COLUMNAS; j++)
        {
            if (orden[i][j].cargada) cantidad++;
        }
    }

    cout << "=========================================="<<endl;
    cout << "Opcion 2 - Ataque cargado en memoria"<<endl;
    cout << "=========================================="<<endl<<endl;

    if (cantidad == 0)
    {
        cout << "No hay ningun ataque cargado en memoria." << endl;
        esperarEnter();
        return;
    }
    int mostrados = 0;
    int enEstaPagina = 0;

    cout << "  X    Y    ORDEN" << endl;
    cout << "----------------------------------------------------" << endl;

    for (int i = 0; i < FILAS; i++)
    {
        for (int j = 0; j < COLUMNAS; j++)
        {
            if (!orden[i][j].cargada) continue;

            cout << i << "    " << j << "    " << descripcionOrden(orden[i][j]) << endl;
            mostrados++;
            enEstaPagina++;

            if (enEstaPagina == LINEAS_POR_PAGINA && mostrados < cantidad)
            {
                cout << endl << "-- Presione ENTER para ver mas --";
                cin.get();
                system("cls");
                cout << "  X    Y    ORDEN" << endl;
                cout << "----------------------------------------------------" << endl;
                enEstaPagina = 0;
            }
        }
    }

    esperarEnter();
}
