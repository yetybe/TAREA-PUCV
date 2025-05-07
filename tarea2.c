#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
  char id[100];
  char artists[100];
  char album_name[300];
  char track_name[300];
  float tempo;
  char genre[100];
} Song;



// Menú principal
void mostrarMenuPrincipal() {
  limpiarPantalla();
  puts("========================================");
  puts("     Spotifind");
  puts("========================================");

  puts("1) Cargar Canciones");
  puts("2) Buscar por genero");
  puts("3) Buscar por artista");
  puts("4) Buscar por tempo");
  puts("5) Crear Lista de Reproducción");
  puts("6) Agregar Canción a Lista");
  puts("7) Mostrar Canciones de una Lista");
  puts("8) Salir");
}

int is_equal_str(void *key1, void *key2) {
  return strcmp((char *)key1, (char *)key2) == 0;
}

int is_equal_int(void *key1, void *key2) {
  return *(int *)key1 == *(int *)key2; // Compara valores enteros directamente
}

/**
 * Carga las canciones desde un archivo CSV y las almacena en un mapa por id , genero y por artista.
 */
void cargar_canciones(List *everySong, Map *canciones_byid, Map *canciones_bygenres, Map *canciones_byartist) {
  
  char ruta[300];
  printf("Ingresar la ruta de su archivo CSV: ");
  scanf(" %[^\n]",ruta);

  FILE *archivo = fopen(ruta, "r");// Intenta abrir el archivo CSV que contiene datos de películas
  if (archivo == NULL) {
    perror("Error al abrir el archivo"); // Informa si el archivo no puede abrirse
    return;
  }
  char **campos;
  campos = leer_linea_csv(archivo, ','); // Lee los encabezados del CSV

  while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
    Song *cancion = malloc(sizeof(Song));
    strcpy(cancion->id, campos[0]);        // Asigna ID
    strcpy(cancion->artists, campos[1]);     // Asigna título
    strcpy(cancion->album_name, campos[2]); // Asigna director
    strcpy(cancion->track_name, campos[3]);
    cancion->tempo = atof(campos[4]);
    strcpy(cancion->genre, campos[5]);

    list_pushBack(everySong,cancion);
    map_insert(canciones_byid, cancion->id, cancion);

    MapPair *genre_pair = map_search(canciones_bygenres, cancion->genre);
    if (genre_pair == NULL) {
      List *listaGenero = list_create();
      list_pushBack(listaGenero, cancion);
      map_insert(canciones_bygenres, cancion->genre, listaGenero);
    } else {
      List *listaGenero = genre_pair->value;
      list_pushBack(listaGenero, cancion);
    }
    MapPair *artist_pair = map_search(canciones_byartist, cancion->artists);
    if (artist_pair == NULL) {
      List *lista_artista = list_create();
      list_pushBack(lista_artista, cancion);
      map_insert(canciones_byartist, cancion->artists, lista_artista);
    } else {
      List *lista_artista= artist_pair->value;
      list_pushBack(lista_artista, cancion);
    }   
    
  }
  fclose(archivo); 
  printf("Canciones almacenadas :)\n.");

  /*MapPair *pair = map_first(canciones_byid);
  while (pair != NULL) {
    Song *cancion = pair->value;
    printf("ID: %s, Artista: %s, Album: %s, Cancion: %d\n", cancion->id, cancion->artists,
           cancion->album_name, cancion->track_name);
    printf("Tempo: %.2f BPM, Género: %s\n", cancion->tempo, cancion->genre);
    
    pair = map_next(canciones_byid); 
  }*/
}

void buscar_por_genero(Map *canciones_bygenres) {
  char genero[100];
  printf("Ingrese el género de la cancion: ");
  scanf("%s", genero); 
  MapPair *pair = map_search(canciones_bygenres, genero);
  
  if (pair != NULL) {
      List* listaGenero = pair->value;
      void *dato = list_first(listaGenero);
      
      while (dato != NULL) {
        Song* cancion = dato;
        printf("ID: %s, Artista: %s, Album: %s, Cancion: %d\n", cancion->id, cancion->artists,
          cancion->album_name, cancion->track_name);
        dato = list_next(listaGenero);
      }
  } else {
    printf("No se encontraron canciones por ese genero.\n");
  }
}
void buscar_por_artista(Map *canciones_byartist) {
  char artista[100];
  printf("Ingrese el nombre del artista: ");
  scanf("%s", artista); 
  MapPair *pair = map_search(canciones_byartist, artista);
  
  if (pair != NULL) {
      List* lista_artista = pair->value;
      void *dato = list_first(lista_artista);
      
      while (dato != NULL) {
        Song* cancion = dato;
        printf("ID: %s, Artista: %s, Album: %s, Cancion: %d\n", cancion->id, cancion->artists,
          cancion->album_name, cancion->track_name);
        dato = list_next(lista_artista);
      }
  } else {
    printf("No se encontraron canciones por ese genero.\n");
  }
}

void buscar_por_id(Map *canciones_byid) {
  char id[10]; // Buffer para almacenar el ID de la película

  // Solicita al usuario el ID de la película
  printf("Ingrese el id de la película: ");
  scanf("%s", id); // Lee el ID del teclado

  // Busca el par clave-valor en el mapa usando el ID proporcionado
  MapPair *pair = map_search(canciones_byid, id);

  // Si se encontró el par clave-valor, se extrae y muestra la información de la
  // película
  if (pair != NULL) {
    Song *cancion =
        pair->value; // Obtiene el puntero a la estructura de la película
    // Muestra el título y el año de la película
    printf("Título: %s, Año: %d\n", cancion->artists, cancion->id);
  } else {
    // Si no se encuentra la película, informa al usuario
    printf("La película con id %s no existe\n", id);
  }
}



int main() 
{
  
  char opcion; 
  Map *canciones_byid = map_create(is_equal_str);
  Map *canciones_bygenres = map_create(is_equal_str);
  Map *canciones_byartist = map_create(is_equal_str);
  List *everySong = list_create();
  


  do 
  {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion);

    switch (opcion) {
    case '1':
      cargar_canciones(everySong,canciones_byid,canciones_bygenres,canciones_byartist);
      break;
    case '2':
      buscar_por_id(canciones_byid);
      break; 
    case '3':
      buscar_por_artista(canciones_byartist);
      break;
    case '4':
      buscar_por_genero(canciones_bygenres);
      break;
    case '5':
      break;
    case '6':
      break;
    case '7':
      break;
    }
    presioneTeclaParaContinuar();

  } while (opcion != '8');

  return 0;
}

/*gcc TDAS/*.c tarea2.c -Wno-unused-result -o tarea2*/ 

/* codigo para crear ejecutable

./tarea2

codigo para correr el ejecutable

*/
