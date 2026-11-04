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

///Opcion 3
void crearArchivoDeAtaque();
int leerEntero(const string &mensaje);
bool leerSiNo(const string &mensaje);
void pedirDatosOrden(OrdenArchivo &registro);

///Opcion 4
void corregirRegistroDeArchivo();
void mostrarRegistro(const OrdenArchivo &r);

///Opcion 5
void corregirRegistroEnMemoria(Orden **orden);

///Opcion 6
void guardarMemoriaEnArchivo(Orden **orden);

///Opcion 7
void visualizarAtaqueEnHtml(Orden **orden);
const char* claseOrden(const Orden &o);

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
            crearArchivoDeAtaque();
            break;
        case 4:
            corregirRegistroDeArchivo();
            break;
        case 5:
            corregirRegistroEnMemoria(orden);
            break;
        case 6:
            guardarMemoriaEnArchivo(orden);
            break;
        case 7:
            visualizarAtaqueEnHtml(orden);
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

    if (o.despegue)        desc += "DESPEGUE ";
    if (o.aterrizaje)      desc += "ATERRIZAJE ";
    if (o.soltarGranada1)  desc += "GRANADA1 ";
    if (o.soltarGranada2)  desc += "GRANADA2 ";
    if (o.ataqueKamikaze)  desc += "KAMIKAZE ";
    if (o.espera > 0)      desc += "ESPERA=" + to_string(o.espera) + " ";

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

/// ---------------------------------------------------------------------
/// OPCION 3: Crear un archivo de ataque nuevo
/// ---------------------------------------------------------------------

int leerEntero(const string &mensaje)
{
    int valor;
    cout << mensaje;
    cin >> valor;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return valor;
}

bool leerSiNo(const string &mensaje)
{
    char resp;
    do
    {
        cout << mensaje << " (s/n): ";
        cin >> resp;
    } while (resp != 's' && resp != 'S' && resp != 'n' && resp != 'N');

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    return (resp == 's' || resp == 'S');
}

void pedirDatosOrden(OrdenArchivo &registro)
{
    registro.despegue       = leerSiNo("Es orden de despegue?");
    registro.aterrizaje     = leerSiNo("Es orden de aterrizaje?");
    registro.soltarGranada1 = leerSiNo("Suelta granada 1?");
    registro.soltarGranada2 = leerSiNo("Suelta granada 2?");
    registro.ataqueKamikaze = leerSiNo("Es ataque kamikaze?");
    registro.espera         = (unsigned int) leerEntero("Tiempo de espera: ");
    registro.siguientex     = leerEntero("Siguiente posicion X: ");
    registro.siguientey     = leerEntero("Siguiente posicion Y: ");
}

void crearArchivoDeAtaque()
{
    char ruta[260];
    char nombre[100];
    char rutaCompleta[400];

    cout << "=========================================="<<endl;
    cout << "Opcion 3 - Crear archivo de ataque nuevo"<<endl;
    cout << "=========================================="<<endl;
    cout << endl << endl;
    cout << "Ingrese la ruta del archivo: ";
    cin.getline(ruta, sizeof(ruta));
    cout << endl << "Ingrese el nombre del archivo: ";
    cin.getline(nombre, sizeof(nombre));

    snprintf(rutaCompleta, sizeof(rutaCompleta), "%s%s", ruta, nombre);

    FILE *f = fopen(rutaCompleta, "wb");
    if (f == NULL)
    {
        cout << endl << "HUBO UN ERROR AL CREAR EL ARCHIVO." << endl;
        esperarEnter();
        return;
    }

    int cantidad;
    do
    {
        cantidad = leerEntero("\nCuantos registros desea ingresar? ");
        if (cantidad <= 0)
        {
            cout << "Debe ingresar al menos un registro." << endl;
        }
    } while (cantidad <= 0);

    for (int i = 0; i < cantidad; i++)
    {
        OrdenArchivo registro;

        cout << endl << "--- Registro " << (i + 1) << " de " << cantidad << " ---" << endl;

        do
        {
            registro.x = leerEntero("Posicion X (0-199): ");
            registro.y = leerEntero("Posicion Y (0-199): ");

            if (!coordenadaValida(registro.x, registro.y))
            {
                cout << "Coordenadas fuera de rango. Vuelva a ingresarlas." << endl;
            }

        } while (!coordenadaValida(registro.x, registro.y));

        pedirDatosOrden(registro);

        fwrite(&registro, sizeof(OrdenArchivo), 1, f);
    }

    fclose(f);

    cout << endl << "Archivo creado correctamente." << endl;

    esperarEnter();
}

/// ---------------------------------------------------------------------
/// OPCION 4: Corregir un registro del archivo
/// ---------------------------------------------------------------------

void mostrarRegistro(const OrdenArchivo &r)
{
    cout << "Posicion:            (" << r.x << "," << r.y << ")" << endl;
    cout << "Despegue:            " << (r.despegue ? "si" : "no") << endl;
    cout << "Aterrizaje:          " << (r.aterrizaje ? "si" : "no") << endl;
    cout << "Granada 1:           " << (r.soltarGranada1 ? "si" : "no") << endl;
    cout << "Granada 2:           " << (r.soltarGranada2 ? "si" : "no") << endl;
    cout << "Ataque kamikaze:     " << (r.ataqueKamikaze ? "si" : "no") << endl;
    cout << "Espera:              " << r.espera << endl;
    cout << "Siguiente posicion:  (" << r.siguientex << "," << r.siguientey << ")" << endl;
}

void corregirRegistroDeArchivo()
{
    char ruta[260];
    char nombre[100];
    char rutaCompleta[400];

    cout << "=========================================="<<endl;
    cout << "Opcion 4 - Corregir un registro del archivo"<<endl;
    cout << "=========================================="<<endl;
    cout << endl << endl;
    cout << "Ingrese la ruta del archivo: ";
    cin.getline(ruta, sizeof(ruta));
    cout << endl << "Ingrese el nombre del archivo: ";
    cin.getline(nombre, sizeof(nombre));

    snprintf(rutaCompleta, sizeof(rutaCompleta), "%s%s", ruta, nombre);

    FILE *f = fopen(rutaCompleta, "r+b");
    if (f == NULL)
    {
        cout << endl << "HUBO UN ERROR AL ABRIR EL ARCHIVO." << endl;
        esperarEnter();
        return;
    }

    int x = leerEntero("\nPosicion X del registro a corregir: ");
    int y = leerEntero("Posicion Y del registro a corregir: ");

    OrdenArchivo registro;
    bool encontrado = false;

    while (!encontrado && fread(&registro, sizeof(OrdenArchivo), 1, f) == 1)
    {
        if (registro.x == x && registro.y == y)
        {
            encontrado = true;
        }
    }

    if (!encontrado)
    {
        cout << endl << "No existe un registro en esa posicion." << endl;
    }
    else
    {
        cout << endl << "Registro actual:" << endl << endl;
        mostrarRegistro(registro);

        cout << endl << "Ingrese los nuevos datos:" << endl;
        pedirDatosOrden(registro);

        fseek(f, -(long) sizeof(OrdenArchivo), SEEK_CUR);
        fwrite(&registro, sizeof(OrdenArchivo), 1, f);

        cout << endl << "Registro corregido correctamente." << endl;
    }

    fclose(f);

    esperarEnter();
}

/// ---------------------------------------------------------------------
/// OPCION 5: Corregir un registro en memoria
/// ---------------------------------------------------------------------

void corregirRegistroEnMemoria(Orden **orden)
{
    cout << "=========================================="<<endl;
    cout << "Opcion 5 - Corregir un registro en memoria"<<endl;
    cout << "=========================================="<<endl;
    cout << endl << endl;

    int x = leerEntero("Posicion X del registro a corregir: ");
    int y = leerEntero("Posicion Y del registro a corregir: ");

    if (!coordenadaValida(x, y))
    {
        cout << endl << "Las coordenadas estan fuera de la tabla (0-199)." << endl;
        esperarEnter();
        return;
    }

    if (!orden[x][y].cargada)
    {
        cout << endl << "No existe un registro en esa posicion." << endl;
        esperarEnter();
        return;
    }

    OrdenArchivo registro;
    registro.x = x;
    registro.y = y;
    registro.espera = orden[x][y].espera;
    registro.soltarGranada1 = orden[x][y].soltarGranada1;
    registro.soltarGranada2 = orden[x][y].soltarGranada2;
    registro.ataqueKamikaze = orden[x][y].ataqueKamikaze;
    registro.aterrizaje = orden[x][y].aterrizaje;
    registro.despegue = orden[x][y].despegue;
    registro.siguientex = orden[x][y].siguientex;
    registro.siguientey = orden[x][y].siguientey;

    cout << endl << "Registro actual:" << endl << endl;
    mostrarRegistro(registro);

    cout << endl << "Ingrese los nuevos datos:" << endl;
    pedirDatosOrden(registro);

    orden[x][y].espera = registro.espera;
    orden[x][y].soltarGranada1 = registro.soltarGranada1;
    orden[x][y].soltarGranada2 = registro.soltarGranada2;
    orden[x][y].ataqueKamikaze = registro.ataqueKamikaze;
    orden[x][y].aterrizaje = registro.aterrizaje;
    orden[x][y].despegue = registro.despegue;
    orden[x][y].siguientex = registro.siguientex;
    orden[x][y].siguientey = registro.siguientey;

    cout << endl << "Registro corregido correctamente." << endl;

    esperarEnter();
}

/// ---------------------------------------------------------------------
/// OPCION 6: Guardar memoria en un archivo nuevo
/// ---------------------------------------------------------------------

void guardarMemoriaEnArchivo(Orden **orden)
{
    char ruta[260];
    char nombre[100];
    char rutaCompleta[400];

    cout << "=========================================="<<endl;
    cout << "Opcion 6 - Guardar memoria en un archivo nuevo"<<endl;
    cout << "=========================================="<<endl;
    cout << endl << endl;

    int cantidad = 0;
    for (int i = 0; i < FILAS; i++)
    {
        for (int j = 0; j < COLUMNAS; j++)
        {
            if (orden[i][j].cargada) cantidad++;
        }
    }

    if (cantidad == 0)
    {
        cout << "No hay ningun ataque cargado en memoria." << endl;
        esperarEnter();
        return;
    }

    cout << "Ingrese la ruta del archivo: ";
    cin.getline(ruta, sizeof(ruta));
    cout << endl << "Ingrese el nombre del archivo: ";
    cin.getline(nombre, sizeof(nombre));

    snprintf(rutaCompleta, sizeof(rutaCompleta), "%s%s", ruta, nombre);

    FILE *f = fopen(rutaCompleta, "wb");
    if (f == NULL)
    {
        cout << endl << "HUBO UN ERROR AL CREAR EL ARCHIVO." << endl;
        esperarEnter();
        return;
    }

    for (int i = 0; i < FILAS; i++)
    {
        for (int j = 0; j < COLUMNAS; j++)
        {
            if (!orden[i][j].cargada) continue;

            OrdenArchivo registro;
            registro.x = i;
            registro.y = j;
            registro.espera = orden[i][j].espera;
            registro.soltarGranada1 = orden[i][j].soltarGranada1;
            registro.soltarGranada2 = orden[i][j].soltarGranada2;
            registro.ataqueKamikaze = orden[i][j].ataqueKamikaze;
            registro.aterrizaje = orden[i][j].aterrizaje;
            registro.despegue = orden[i][j].despegue;
            registro.siguientex = orden[i][j].siguientex;
            registro.siguientey = orden[i][j].siguientey;

            fwrite(&registro, sizeof(OrdenArchivo), 1, f);
        }
    }

    fclose(f);

    cout << endl << "Archivo guardado correctamente." << endl;

    esperarEnter();
}

/// ---------------------------------------------------------------------
/// OPCION 7: Visualizar el ataque en memoria en un archivo html
/// ---------------------------------------------------------------------

const char* claseOrden(const Orden &o)
{
    if (o.despegue) return "despegue";
    if (o.aterrizaje) return "aterrizaje";
    if (o.soltarGranada1) return "granada1";
    if (o.soltarGranada2) return "granada2";
    if (o.ataqueKamikaze) return "kamikaze";
    if (o.espera > 0) return "espera";
    return "mover";
}

void visualizarAtaqueEnHtml(Orden **orden)
{
    char ruta[260];
    char nombre[100];
    char rutaCompleta[400];

    cout << "=========================================="<<endl;
    cout << "Opcion 7 - Visualizar el ataque en html"<<endl;
    cout << "=========================================="<<endl;
    cout << endl << endl;

    int cantidad = 0;
    for (int i = 0; i < FILAS; i++)
    {
        for (int j = 0; j < COLUMNAS; j++)
        {
            if (orden[i][j].cargada) cantidad++;
        }
    }

    if (cantidad == 0)
    {
        cout << "No hay ningun ataque cargado en memoria." << endl;
        esperarEnter();
        return;
    }

    cout << "Ingrese la ruta del archivo: ";
    cin.getline(ruta, sizeof(ruta));
    cout << endl << "Ingrese el nombre del archivo (con extension .html): ";
    cin.getline(nombre, sizeof(nombre));

    snprintf(rutaCompleta, sizeof(rutaCompleta), "%s%s", ruta, nombre);

    FILE *f = fopen(rutaCompleta, "w");
    if (f == NULL)
    {
        cout << endl << "HUBO UN ERROR AL CREAR EL ARCHIVO." << endl;
        esperarEnter();
        return;
    }

    fprintf(f, "<!DOCTYPE html>\n");
    fprintf(f, "<html lang=\"es\">\n");
    fprintf(f, "<head>\n");
    fprintf(f, "<meta charset=\"UTF-8\">\n");
    fprintf(f, "<title>Vista previa - archivo de ataque</title>\n");
    fprintf(f, "<style>\n");
    fprintf(f, "body{margin:0;padding:24px;background:#0a1512;color:#d9f2e4;font-family:monospace;}\n");
    fprintf(f, "h1{margin:0 0 6px;font-size:22px;}\n");
    fprintf(f, "h2{margin:24px 0 10px;color:#6f9c85;font-size:12px;font-weight:normal;}\n");
    fprintf(f, "p{margin:0 0 16px;color:#6f9c85;font-size:13px;}\n");
    fprintf(f, ".leyenda{display:flex;flex-wrap:wrap;gap:16px;margin-bottom:16px;font-size:12px;}\n");
    fprintf(f, ".leyenda div{display:flex;align-items:center;gap:6px;}\n");
    fprintf(f, ".contenedor{overflow:auto;max-height:75vh;border:1px solid #1d3a2f;}\n");
    fprintf(f, "table{border-collapse:collapse;}\n");
    fprintf(f, ".grilla td,.grilla th{width:24px;min-width:24px;height:24px;padding:0;text-align:center;font-size:10px;border:1px solid #16302a;}\n");
    fprintf(f, ".grilla th{position:sticky;background:#0f1e19;color:#6f9c85;font-weight:normal;}\n");
    fprintf(f, ".grilla thead th{top:0;z-index:1;}\n");
    fprintf(f, ".grilla tbody th{left:0;}\n");
    fprintf(f, ".grilla thead th:first-child{left:0;z-index:2;}\n");
    fprintf(f, ".marca{display:inline-block;width:24px;height:24px;line-height:24px;text-align:center;font-size:10px;}\n");
    fprintf(f, ".despegue{background:#ffb300;color:#0a1512;}\n");
    fprintf(f, ".aterrizaje{background:#4dd0e1;color:#0a1512;}\n");
    fprintf(f, ".granada1{background:#ff6a4d;color:#0a1512;}\n");
    fprintf(f, ".granada2{background:#ff9a4d;color:#0a1512;}\n");
    fprintf(f, ".kamikaze{background:#ff3b3b;color:#0a1512;}\n");
    fprintf(f, ".espera{background:#5a4a14;color:#ffb300;}\n");
    fprintf(f, ".mover{background:#1d3a2f;color:#ffb300;}\n");
    fprintf(f, ".despegue::before{content:'D';}\n");
    fprintf(f, ".aterrizaje::before{content:'A';}\n");
    fprintf(f, ".granada1::before{content:'1';}\n");
    fprintf(f, ".granada2::before{content:'2';}\n");
    fprintf(f, ".kamikaze::before{content:'K';}\n");
    fprintf(f, ".espera::before{content:'E';}\n");
    fprintf(f, ".mover::before{content:'*';}\n");
    fprintf(f, ".registros{font-size:12px;}\n");
    fprintf(f, ".registros th,.registros td{padding:5px 10px;text-align:left;white-space:nowrap;border-bottom:1px solid #16302a;}\n");
    fprintf(f, ".registros th{color:#d9f2e4;font-weight:normal;}\n");
    fprintf(f, ".registros td{color:#6f9c85;}\n");
    fprintf(f, "</style>\n");
    fprintf(f, "</head>\n");
    fprintf(f, "<body>\n");

    fprintf(f, "<h1>Vista previa del archivo de ataque</h1>\n");
    fprintf(f, "<p>Grilla de %d x %d con el ataque cargado en memoria (x en horizontal, y en vertical). ", FILAS, COLUMNAS);
    fprintf(f, "Pase el mouse sobre una celda para ver su orden completa. Registros cargados: %d.</p>\n", cantidad);

    fprintf(f, "<div class=\"leyenda\">\n");
    fprintf(f, "<div><span class=\"marca despegue\"></span>Despegue</div>\n");
    fprintf(f, "<div><span class=\"marca aterrizaje\"></span>Aterrizaje</div>\n");
    fprintf(f, "<div><span class=\"marca granada1\"></span>Granada 1</div>\n");
    fprintf(f, "<div><span class=\"marca granada2\"></span>Granada 2</div>\n");
    fprintf(f, "<div><span class=\"marca kamikaze\"></span>Ataque kamikaze</div>\n");
    fprintf(f, "<div><span class=\"marca espera\"></span>Espera</div>\n");
    fprintf(f, "<div><span class=\"marca mover\"></span>Movimiento</div>\n");
    fprintf(f, "</div>\n");

    fprintf(f, "<div class=\"contenedor\">\n");
    fprintf(f, "<table class=\"grilla\">\n");
    fprintf(f, "<thead><tr><th></th>");
    for (int x = 0; x < FILAS; x++)
    {
        fprintf(f, "<th>%d</th>", x);
    }
    fprintf(f, "</tr></thead>\n");
    fprintf(f, "<tbody>\n");

    for (int y = 0; y < COLUMNAS; y++)
    {
        fprintf(f, "<tr><th>%d</th>", y);

        for (int x = 0; x < FILAS; x++)
        {
            if (orden[x][y].cargada)
            {
                fprintf(f, "<td class=\"%s\" title=\"(%d,%d) %s\"></td>", claseOrden(orden[x][y]), x, y, descripcionOrden(orden[x][y]).c_str());
            }
            else
            {
                fprintf(f, "<td></td>");
            }
        }

        fprintf(f, "</tr>\n");
    }

    fprintf(f, "</tbody>\n");
    fprintf(f, "</table>\n");
    fprintf(f, "</div>\n");

    fprintf(f, "<h2>Registros en memoria</h2>\n");
    fprintf(f, "<table class=\"registros\">\n");
    fprintf(f, "<thead><tr><th>x</th><th>y</th><th>espera</th><th>granada1</th><th>granada2</th><th>kamikaze</th><th>aterrizaje</th><th>despegue</th><th>siguiente</th></tr></thead>\n");
    fprintf(f, "<tbody>\n");

    for (int x = 0; x < FILAS; x++)
    {
        for (int y = 0; y < COLUMNAS; y++)
        {
            if (!orden[x][y].cargada) continue;

            const Orden &o = orden[x][y];

            fprintf(f, "<tr><td>%d</td><td>%d</td><td>%u</td>", x, y, o.espera);
            fprintf(f, "<td>%s</td><td>%s</td><td>%s</td>", o.soltarGranada1 ? "true" : "false", o.soltarGranada2 ? "true" : "false", o.ataqueKamikaze ? "true" : "false");
            fprintf(f, "<td>%s</td><td>%s</td>", o.aterrizaje ? "true" : "false", o.despegue ? "true" : "false");

            if (o.aterrizaje || o.ataqueKamikaze)
            {
                fprintf(f, "<td>-</td></tr>\n");
            }
            else
            {
                fprintf(f, "<td>%d,%d</td></tr>\n", o.siguientex, o.siguientey);
            }
        }
    }

    fprintf(f, "</tbody>\n");
    fprintf(f, "</table>\n");
    fprintf(f, "</body>\n");
    fprintf(f, "</html>\n");

    fclose(f);

    cout << endl << "Archivo html generado correctamente." << endl;

    esperarEnter();
}
