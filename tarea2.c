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
  return *(int *)key1 == *(int *)key2;
}

/**
 * Carga las canciones desde un archivo CSV y las almacena en un mapa por id, genero y artista.
 */
void cargar_canciones(List *everySong, Map *canciones_byid, Map *canciones_bygenres, Map *canciones_byartist) 
{
  char ruta[300];
  printf("Ingresar la ruta de su archivo CSV: ");
  scanf(" %[^\n]",ruta);

  FILE *archivo = fopen(ruta, "r");
  if (archivo == NULL) {
    perror("Error al abrir el archivo");
    return;
  }

  char **campos = leer_linea_csv(archivo, ',');

  while ((campos = leer_linea_csv(archivo, ',')) != NULL) {
    Song *cancion = malloc(sizeof(Song));
    strcpy(cancion->id, campos[0]);
    strcpy(cancion->artists, campos[2]);
    strcpy(cancion->album_name, campos[3]);
    strcpy(cancion->track_name, campos[4]);
    cancion->tempo = atof(campos[18]);
    strcpy(cancion->genre, campos[20]);

    list_pushBack(everySong, cancion);
    map_insert(canciones_byid, cancion->id, cancion);

    // Mapa por género
    MapPair *genre_pair = map_search(canciones_bygenres, cancion->genre);
    if (genre_pair == NULL) {
      List *listaGenero = list_create();
      list_pushBack(listaGenero, cancion);
      map_insert(canciones_bygenres, cancion->genre, listaGenero);
    } else {
      List *listaGenero = genre_pair->value;
      list_pushBack(listaGenero, cancion);
    }

    // Mapa por artista
    MapPair *artist_pair = map_search(canciones_byartist, cancion->artists);
    if (artist_pair == NULL) {
      List *lista_artista = list_create();
      list_pushBack(lista_artista, cancion);
      map_insert(canciones_byartist, cancion->artists, lista_artista);
    } else {
      List *lista_artista = artist_pair->value;
      list_pushBack(lista_artista, cancion);
    }
  }

  fclose(archivo);
  printf("Canciones cargadas con éxito.\n");
}

void buscar_por_genero(Map *canciones_bygenres) {
  char genero[100];
  printf("Ingrese el género de la canción: ");
  scanf("%s", genero);

  MapPair *pair = map_search(canciones_bygenres, genero);

  if (pair != NULL) {
    List *listaGenero = pair->value;
    void *dato = list_first(listaGenero);

    while (dato != NULL) {
      Song *cancion = dato;
      printf("ID: %s, Artista: %s, Album: %s, Canción: %s\n", cancion->id, cancion->artists,
             cancion->album_name, cancion->track_name);
      dato = list_next(listaGenero);
    }
  } else {
    printf("No se encontraron canciones por ese género.\n");
  }
}

void buscar_por_artista(Map *canciones_byartist) {
  char artista[100];
  printf("Ingrese el nombre del artista: ");
  scanf("%s", artista);

  MapPair *pair = map_search(canciones_byartist, artista);

  if (pair != NULL) {
    List *lista_artista = pair->value;
    void *dato = list_first(lista_artista);

    while (dato != NULL) {
      Song *cancion = dato;
      printf("ID: %s, Artista: %s, Album: %s, Canción: %s\n", cancion->id, cancion->artists,
             cancion->album_name, cancion->track_name);
      dato = list_next(lista_artista);
    }
  } else {
    printf("No se encontraron canciones por ese artista.\n");
  }
}

void buscar_por_id(Map *canciones_byid) {
  char id[100];
  printf("Ingrese el id de la canción: ");
  scanf("%s", id);

  MapPair *pair = map_search(canciones_byid, id);

  if (pair != NULL) {
    Song *cancion = pair->value;
    printf("ID: %s, Artista: %s, Album: %s, Canción: %s\n", cancion->id, cancion->artists,
           cancion->album_name, cancion->track_name);
  } else {
    printf("La canción con id %s no existe.\n", id);
  }
}

int main() {
  char opcion;
  Map *canciones_byid = map_create(is_equal_str);
  Map *canciones_bygenres = map_create(is_equal_str);
  Map *canciones_byartist = map_create(is_equal_str);
  List *everySong = list_create();

  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion);

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
        buscar_por_id(canciones_byid);
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


/*   gcc TDAS/*.c tarea2.c -Wno-unused-result -o tarea2
*/ 

/* codigo para crear ejecutable

./tarea2

codigo para correr el ejecutable

*/