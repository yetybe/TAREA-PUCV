#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct 
{
  char id[100];
  char artists[100];
  char album_name[300];
  char track_name[300];
  float tempo;
  char genre[100];
} Song;


// Menú principal
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

int is_equal_str(void *key1, void *key2) {
  return strcmp((char *)key1, (char *)key2) == 0;
}

int is_equal_int(void *key1, void *key2) {
  return *(int *)key1 == *(int *)key2;
}


void cargar_canciones(List *everySong, Map *canciones_byid, Map *canciones_bygenres, Map *canciones_byartist) 
{
    char ruta[300];
    printf("Ingresar la ruta de su archivo CSV: ");
    scanf(" %[^\n]", ruta);

    FILE *archivo = fopen(ruta, "r");
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        return;
    }

    char **campos;
    while ((campos = leer_linea_csv(archivo, ',')) != NULL) 
    {
        // Verificar que tenemos suficientes campos
        if (campos[0] == NULL || campos[2] == NULL || campos[3] == NULL || 
            campos[4] == NULL || campos[18] == NULL || campos[20] == NULL) {
            free(campos);
            continue;
        }

        // Copiar datos 
        Song *cancion = malloc(sizeof(Song));
        strcpy(cancion->id, campos[0]);
        strcpy(cancion->artists, campos[2]);
        strcpy(cancion->album_name, campos[3]);
        strcpy(cancion->track_name, campos[4]);
        cancion->tempo = atof(campos[18]);
        strcpy(cancion->genre, campos[20]);
        

        // Agregar a las estructuras de datos
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

        free(campos);  // Liberar memoria de los campos
    }

    fclose(archivo);
    printf("Canciones cargadas con exito. Total: %d\n", list_size(everySong));
}

void buscar_por_genero(Map *canciones_bygenres) {
  char genero[100];
  printf("Ingrese el genero de la cancion: ");
  scanf(" %[^\n]", genero);

  MapPair *pair = map_search(canciones_bygenres, genero);

  if (pair != NULL) {
    List *listaGenero = pair->value;
    void *dato = list_first(listaGenero);

    while (dato != NULL) 
    {
      Song *cancion = dato;
      printf("ID: %s, Artista: %s, Album: %s, Cancion: %s, Tempo: %.2f\n", cancion->id, cancion->artists,
             cancion->album_name, cancion->track_name, cancion->tempo);
      dato = list_next(listaGenero);
    }
  } else {
    printf("No se encontraron canciones por ese genero.\n");
  }
}

void buscar_por_artista(Map *canciones_byartist) {
  char artista[100];
  printf("Ingrese el nombre del artista: ");
  scanf(" %[^\n]", artista);

  MapPair *pair = map_search(canciones_byartist, artista);

  if (pair != NULL) {
    List *lista_artista = pair->value;
    void *dato = list_first(lista_artista);

    while (dato != NULL) {
      Song *cancion = dato;
      printf("ID: %s, Artista: %s, Album: %s, Cancion: %s, Tempo: %.2f\n", cancion->id, cancion->artists,
             cancion->album_name, cancion->track_name, cancion->tempo);
      dato = list_next(lista_artista);
    }
  } else {
    printf("No se encontraron canciones por ese artista.\n");
  }
}

void buscar_por_tempo(List *songs)
{
  char velocidad[100];
  printf("Ingrese velocidad deseada (Lenta / Moderada / Rapida) : ");
  scanf("%s", velocidad);
  if (strcmp(velocidad, "Lenta") != 0 &&
    strcmp(velocidad, "Moderada") != 0 &&
    strcmp(velocidad, "Rapida") != 0) 
  {
    printf("Velocidad no valida. Use 'Lenta', 'Moderada' o 'Rapida'.\n");
    return;
  }

  Song *aux = list_first(songs);
  int encontrada = 0;

  while (aux != NULL) 
  {
    if ((strcmp(velocidad, "Lenta") == 0 && aux->tempo < 80) ||
        (strcmp(velocidad, "Moderada") == 0 && aux->tempo >= 80 && aux->tempo <= 120) ||
        (strcmp(velocidad, "Rapida") == 0 && aux->tempo > 120)) {
      printf("ID: %s, Artista: %s, Album: %s, Cancion: %s, Tempo: %.2f\n",
             aux->id, aux->artists, aux->album_name, aux->track_name, aux->tempo);
      encontrada = 1;
    }
    aux = list_next(songs);
  }

  if (!encontrada) 
  {
    printf("No se encontraron canciones con esa velocidad.\n");
  }
}

void crear_listaRepro(Map *lista_reproduccion) 
{
  char listaRepro[100];
  printf("Ingrese el nombre de la lista: ");
  scanf(" %[^\n]", listaRepro);

  if (map_search(lista_reproduccion, listaRepro)) {
    printf("Intente de nuevo, ya existe una lista con ese nombre.\n");
    return;
  }

  List *newList = list_create();

  // Copiar el nombre dinámicamente para usarlo como clave segura en el mapa
  char *clave = malloc(strlen(listaRepro) + 1);
  if (clave == NULL) {
    printf("Error al asignar memoria.\n");
    return;
  }
  strcpy(clave, listaRepro);

  map_insert(lista_reproduccion, clave, newList);
  printf("Lista '%s' creada exitosamente.\n", listaRepro);
}

void agregar_canciones(Map *canciones_byid, Map *lista_reproduccion) {
  char id[100];
  char nameList[100];
  printf("Ingresar ID cancion: ");
  scanf(" %[^\n]", id);
  MapPair *parCancion = map_search(canciones_byid, id);
  if (!parCancion) {
    printf("No hay cancion con ese ID.\n");
    return;
  }

  printf("Ingrese nombre de la lista: ");
  scanf(" %[^\n]", nameList);
  MapPair *parLista = map_search(lista_reproduccion, nameList);
  if (!parLista) {
    printf("No existe la lista '%s'.\n", nameList);
    return;
  }
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
  list_pushBack(lista, comprobar);
  printf("Se añadio cancion a la lista '%s' :).\n", nameList);
}

void imprimir_lista_canciones(Map *lista_reproduccion) {
  char nameList[100];
  printf("Ingrese nombre lista: ");
  scanf(" %[^\n]", nameList);
  MapPair *parLista = map_search(lista_reproduccion, nameList);
  if (!parLista) {
    printf("No existe, intente de nuevo.\n");
    return;
  }
  List *listaNueva = parLista->value;
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

int main() 
{
  char opcion;
  Map *canciones_byid = map_create(is_equal_str);
  Map *canciones_bygenres = map_create(is_equal_str);
  Map *canciones_byartist = map_create(is_equal_str);
  Map *listasDeReproduccion = map_create(is_equal_str);
  List *everySong = list_create();

  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opcion: ");
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

    presioneTeclaParaContinuar();
  } while (opcion != '8');

  return 0;
}



/*   gcc TDAS/*.c tarea2.c -Wno-unused-result -o tarea2
*/ 


/*./tarea2

data/song_dataset_.csv

Shiritsu Ebisu Chugaku

codigo para correr el ejecutable

*/



