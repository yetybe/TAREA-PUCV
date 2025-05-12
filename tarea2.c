#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Definición de la estructura Song que almacena la información de una canción
typedef struct 
{
  char id[100];
  char artists[100];
  char album_name[300];
  char track_name[300];
  float tempo;
  char genre[100];
} Song;

// Función para mostrar el menú principal de la aplicación
void mostrarMenuPrincipal() 
{
  limpiarPantalla();
  puts("========================================");
  puts("     Spotifind");
  puts("========================================");
  puts("1) Cargar Canciones");
  puts("2) Buscar por genero");
  puts("3) Buscar por artista");
  puts("4) Buscar por tempo");
  puts("5) Crear Lista de Reproduccion");
  puts("6) Agregar Cancion a Lista");
  puts("7) Mostrar Canciones de una Lista");
  puts("8) Salir");
}

// Función para comparar cadenas (usada como criterio de igualdad en los mapas)
int is_equal_str(void *key1, void *key2) {
  return strcmp((char *)key1, (char *)key2) == 0;
}

// Función para comparar enteros (no utilizada en este código, pero definida)
int is_equal_int(void *key1, void *key2) {
  return *(int *)key1 == *(int *)key2;
}

// Función que carga las canciones desde un archivo CSV y las organiza en múltiples estructuras
void cargar_canciones(List *everySong, Map *canciones_byid, Map *canciones_bygenres, Map *canciones_byartist) 
{
    char ruta[300];
    printf("Ingresar la ruta de su archivo CSV: ");
    scanf(" %[^\n]", ruta); // Se pide al usuario la ruta del archivo

    FILE *archivo = fopen(ruta, "r"); // Se abre el archivo
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return;
    }

    char **campos;
    // Lectura de cada línea del archivo CSV
    while ((campos = leer_linea_csv(archivo, ',')) != NULL) 
    {
        // Se verifica que existan los campos necesarios
        if (campos[0] == NULL || campos[2] == NULL || campos[3] == NULL || 
            campos[4] == NULL || campos[18] == NULL || campos[20] == NULL) {
            free(campos);
            continue;
        }

        // Se crea estructura Song
        Song *cancion = malloc(sizeof(Song));
        // Se copian los campos desde el archivo CSV hacia la estructura Song 
        strcpy(cancion->id, campos[0]);
        strcpy(cancion->artists, campos[2]);
        strcpy(cancion->album_name, campos[3]);
        strcpy(cancion->track_name, campos[4]);
        cancion->tempo = atof(campos[18]);
        strcpy(cancion->genre, campos[20]);

        // Se agrega la canción a la lista de todas las canciones y al mapa que tiene por "key" el ID de la cancion
        list_pushBack(everySong, cancion);
        map_insert(canciones_byid, cancion->id, cancion);

        // Se agrega la canción al mapa que tie por "key" el género de la cancion
        MapPair *genre_pair = map_search(canciones_bygenres, cancion->genre);
        if (genre_pair == NULL) 
        {
            //Si no se encuentra el genre_pair dentro del mapa , se crea una nueva lista para ese genero en particular
            List *listaGenero = list_create();
            list_pushBack(listaGenero, cancion);
            map_insert(canciones_bygenres, cancion->genre, listaGenero);
        } else 
        {
            //Si el genre_pair ya existe dentro del mapa , tan solo se agrega la cancion a dicho genero
            List *listaGenero = genre_pair->value;
            list_pushBack(listaGenero, cancion);
        }

        // Se agrega la canción al mapa que tiene por "key" el artista de la cancion
        MapPair *artist_pair = map_search(canciones_byartist, cancion->artists);
        if (artist_pair == NULL) 
        {
            //Si no se encuentra el artist_pair dentro del mapa , se crea una nueva lista para ese artista en particular
            List *lista_artista = list_create();
            list_pushBack(lista_artista, cancion);
            map_insert(canciones_byartist, cancion->artists, lista_artista);
        } else 
        { 
            //Si el artist_pair ya existe dentro del mapa , tan solo se agrega la cancion a dicho artista
            List *lista_artista = artist_pair->value;
            list_pushBack(lista_artista, cancion);
        }

        free(campos); // Liberación de memoria de campos leídos
    }

    // Se cierra el archivo CSV y se notifica al usuario que las canciones fueron cargadas exitosamente

    fclose(archivo);
    printf("Canciones cargadas con exito. Total: %d\n", list_size(everySong));
}


void hacerMinuscula(char *texto){ //hacemos minuscula el texto que nos entreguen, en este caso la prioridad
  for(int i = 0; texto[i]; i++){  //para no tener problemas a futuro
    texto[i] = tolower(texto[i]);
  }
}


// Función para buscar canciones por género y mostrarlas
void buscar_por_genero(Map *canciones_bygenres) 
{

  // Se solicita al usuario que ingrese el género musical 
  char genero[100];
  printf("Ingrese el genero de la cancion: ");
  scanf(" %[^\n]", genero);

  // Se convierte el string "genero" a minúsculas para estandarizar la entrada del usuario
  hacerMinuscula(genero);

  //Se busca si existe el genero ingresado dentro del mapa "canciones_bygenres"
  MapPair *pair = map_search(canciones_bygenres, genero);

  // Se verifica si se encontró un par (clave-valor) para el género buscado
  if (pair != NULL) 
  {

    // Se obtiene la lista de canciones asociadas al género encontrado
    List *listaGenero = pair->value;

    //Se obtiene el primer dato de la lista , para poder recorrerla
    void *dato = list_first(listaGenero);

    // Se recorre la lista de canciones y se inmprimen las canciones de ese genero
    while (dato != NULL) 
    {
      Song *cancion = dato;
      printf("ID: %s, Artista: %s, Album: %s, Cancion: %s, Tempo: %.2f\n", cancion->id, cancion->artists,
             cancion->album_name, cancion->track_name, cancion->tempo);
      dato = list_next(listaGenero);
    }
  } 
  // Si no se encontró el género, se muestra un mensaje informativo
  else 
  {
    printf("No se encontraron canciones por ese genero.\n");
  }
}

// Función para buscar canciones por nombre del artista
void buscar_por_artista(Map *canciones_byartist) 
{
  // Se solicita al usuario el nombre del artista a busca
  char artista[100];
  printf("Ingrese el nombre del artista: ");
  scanf(" %[^\n]", artista);

  // Se busca en el mapa si existe el aritsta ingresado 
  MapPair *pair = map_search(canciones_byartist, artista);

  if (pair != NULL) 
  {
    // Si se encuentra el artista, se obtiene la lista de canciones
    List *lista_artista = pair->value;

    //Obtenemos el primer dato de la lista para poder recorrerla
    void *dato = list_first(lista_artista);

    // Se recorre la lista de canciones del artista
    while (dato != NULL) {
      Song *cancion = dato;
      // Se imprime la información de la canción
      printf("ID: %s, Artista: %s, Album: %s, Cancion: %s, Tempo: %.2f\n", cancion->id, cancion->artists,
             cancion->album_name, cancion->track_name, cancion->tempo);
      dato = list_next(lista_artista);
    }
    
  }
  // Si no se encuentra el artista, se informa al usuario 
  else {
    printf("No se encontraron canciones por ese artista.\n");
  }
}



// Función para buscar canciones por velocidad (tempo)
void buscar_por_tempo(List *songs)
{
  // Se solicita al usuario que ingrese la velocidad deseada
  char velocidad[100];
  printf("Ingrese velocidad deseada (Lenta / Moderada / Rapida) : ");
  scanf("%s", velocidad);

  // Validación del valor ingresado por el usuario
  if (strcmp(velocidad, "Lenta") != 0 &&
      strcmp(velocidad, "Moderada") != 0 &&
      strcmp(velocidad, "Rapida") != 0) 
  {
    // Si el valor no es válido, se muestra un mensaje de error y se termina la función
    printf("Velocidad no valida. Use 'Lenta', 'Moderada' o 'Rapida'.\n");
    return;
  }

  // Se obtiene la primera canción de la lista "songs"
  Song *aux = list_first(songs);
  int encontrada = 0; // flag para verificar si se encontraron canciones

  // Se recorre la lista de canciones y se filtran según el tempo
  while (aux != NULL) 
  {
    // Se verifica si la canción coincide con la velocidad solicitada
    if ((strcmp(velocidad, "Lenta") == 0 && aux->tempo < 80) ||
        (strcmp(velocidad, "Moderada") == 0 && aux->tempo >= 80 && aux->tempo <= 120) ||
        (strcmp(velocidad, "Rapida") == 0 && aux->tempo > 120)) 
    {
      // Si cumple con la condición, se imprime la información de la canción
      printf("ID: %s, Artista: %s, Album: %s, Cancion: %s, Tempo: %.2f\n",
             aux->id, aux->artists, aux->album_name, aux->track_name, aux->tempo);
      encontrada = 1; // Se marca que al menos una canción fue encontrada
    }
    // Se avanza a la siguiente canción en la lista
    aux = list_next(songs);
  }

  // Si no se encontró ninguna canción que coincida con la velocidad, se notifica al usuario
  if (!encontrada) 
  {
    printf("No se encontraron canciones con esa velocidad.\n");
  }
}


// Función para crear una nueva lista de reproducción
void crear_listaRepro(Map *lista_reproduccion) 
{
  char listaRepro[100];

  // Solicita al usuario que ingrese el nombre de la nueva lista
  printf("Ingrese el nombre de la lista: ");
  scanf(" %[^\n]", listaRepro);

  // Verifica si ya existe una lista con ese nombre en el mapa
  if (map_search(lista_reproduccion, listaRepro)) {
    printf("Intente de nuevo, ya existe una lista con ese nombre.\n");
    return;
  }

  // Crea una nueva lista vacía
  List *newList = list_create();

  // Reserva memoria para copiar el nombre de la lista (clave del mapa)
  char *clave = malloc(strlen(listaRepro) + 1); 
  if (clave == NULL) {
    // Verifica si la reserva de memoria fue exitosa
    printf("Error al asignar memoria.\n");
    return;
  }

  // Copia el nombre ingresado a la clave
  strcpy(clave, listaRepro);

  // Inserta la nueva lista en el mapa usando la clave
  map_insert(lista_reproduccion, clave, newList);

  // Informa al usuario que la lista fue creada con éxito
  printf("Lista '%s' creada exitosamente.\n", listaRepro);
}


// Función para agregar una canción existente a una lista de reproducción
void agregar_canciones(Map *canciones_byid, Map *lista_reproduccion) 
{
  //El usuario ingresa el ID y el nombre de la lista de la reproduccion a la que agregara la cancion
  char id[100];
  char nameList[100];
  printf("Ingresar ID cancion: ");
  scanf(" %[^\n]", id);

  // Busca la canción por ID y verifica si ese ID existe
  MapPair *parCancion = map_search(canciones_byid, id);
  if (!parCancion) {
    printf("No hay cancion con ese ID.\n");
    return;
  }

  printf("Ingrese nombre de la lista: ");
  scanf(" %[^\n]", nameList);

  // Busca la lista por nombre y verifica que la lista exista
  MapPair *parLista = map_search(lista_reproduccion, nameList);
  if (!parLista) {
    printf("No existe la lista '%s'.\n", nameList);
    return;
  }

  // Evita agregar canciones repetidas a la misma lista
  List *lista = parLista->value;
  Song *comprobar = parCancion->value;
  Song *current = list_first(lista);
  while (current != NULL) {
    if (strcmp(current->id, comprobar->id) == 0) {
      printf("La cancion ya existe.\n");
      return;
    }
    current = list_next(lista);
  }

  // Agrega la canción a la lista y notifica al usuario con un mensaje
  list_pushBack(lista, comprobar);
  printf("Se agrego cancion a la lista '%s' :).\n", nameList);
}

// Función para mostrar las canciones que contiene una lista de reproducción
void imprimir_lista_canciones(Map *lista_reproduccion) 
{
  // El usuario ingresa el nombre de la lista de reproduccion
  char nameList[100];
  printf("Ingrese nombre lista: ");
  scanf(" %[^\n]", nameList);

  // Busca la lista y verifica si existe o no
  MapPair *parLista = map_search(lista_reproduccion, nameList);
  if (!parLista) {
    printf("No existe, intente de nuevo.\n");
    return;
  }

  // Muestra todas las canciones de la lista
  List *listaNueva = parLista->value;
  // Obtenemos el primer dato de la lista para poder recorrerla
  void *name = list_first(listaNueva);
  printf("Canciones de la lista '%s':.\n", nameList);
  while (name != NULL) {
    Song *cancion = name;
    printf("ID: %s, Artista: %s, Album: %s, Cancion: %s, Genero: %s Tempo %.2f\n", 
           cancion->id, cancion->artists, cancion->album_name, 
           cancion->track_name, cancion->genre, cancion->tempo);
    name = list_next(listaNueva);
  }
}

// Función principal donde se inicializan estructuras y se maneja el menú
int main() 
{
  char opcion;
  // Se crean los mapas y listas necesarias para el programa
  Map *canciones_byid = map_create(is_equal_str);
  Map *canciones_bygenres = map_create(is_equal_str);
  Map *canciones_byartist = map_create(is_equal_str);
  Map *listasDeReproduccion = map_create(is_equal_str);
  List *everySong = list_create();

  // Bucle del menú principal
  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opcion: ");
    scanf(" %c", &opcion);

    // Se ejecuta la acción correspondiente a la opción seleccionada
    switch (opcion) {
      case '1':
        cargar_canciones(everySong, canciones_byid, canciones_bygenres, canciones_byartist);
        break;
      case '2':
        buscar_por_genero(canciones_bygenres);
        break;
      case '3':
        buscar_por_artista(canciones_byartist);
        break;
      case '4':
        buscar_por_tempo(everySong);
        break;
      case '5':
        crear_listaRepro(listasDeReproduccion);
        break;
      case '6':
        agregar_canciones(canciones_byid, listasDeReproduccion);
        break;
      case '7':
        imprimir_lista_canciones(listasDeReproduccion);
        break;
    }

    presioneTeclaParaContinuar(); // Consume el '\n' del buffer de entrada y  espera a que el usuario presione una tecla
  } while (opcion != '8'); // Sale si elige opción 8

  return 0;// Se termina el programa :)
}



/*   gcc TDAS/*.c tarea2.c -Wno-unused-result -o tarea2
*/ 


/*./tarea2

data/song_dataset_.csv

Shiritsu Ebisu Chugaku

codigo para correr el ejecutable

*/



