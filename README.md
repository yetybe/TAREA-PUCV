Spotifind
Descripción: 
Es una herramienta de consola en la cual tendrá una base de datos de canciones, permitiendo que el usuario las busque según género, artista y tempo. Otra cualidad de spotifind es que nos permite almacenar canciones en listas de reproducciones que el usuario desee.

Programa:
Spotifind se ha realizado en lenguaje C y se puede ejecutar en Visual Studio Code con extensión C/C++. Para trabajar con sistema se deberán seguir los siguientes pasos:

Requisitos previos:
•   Haber descargado Visual Studio Code.
•   Tener el compilador C/C++, haber descargado el MinGW para Windows o el entorno WSL.
•   Tener la extensión C/C++ en Visual Studio Code.

Compilar y ejecutar:
•   Se debe descargar esta carpeta y se descomprime el archivo .zip.
•   En Visual Studio Code se va a, file y se escoge open folder, luego selecciona la carpeta descomprimida.
•   Se debe abrir el archivo tarea1.c!!! para visualizar código. 
•   Se abre su terminal, escoger Git Bash y se debe compilar el programa con siguiente comando:
                  gcc TDAS/*.c tarea2.c -Wno-unused-result -o tarea2
•   Ya compilado, para ejecutar el código se debe escribir en terminal:
                  ./tarea2
Para tener acceso a la base de datos de las canciones, cuando se pide ingresar la ruta se debe escribir:
                 data/song_dataset_.csv

Funciones del código explicados y sus ejemplos de como interactuar con la aplicación:
El programa comienza preguntando que opción desea realizar:
•	========================================
•	Spotifind
•	========================================
•	Cargar Canciones
•	Buscar por genero
•	Buscar por artista
•	Buscar por tempo
•	Crear Lista de Reproduccion
•	Agregar Cancion a Lista
•	Mostrar Canciones de una Lista
•	Salir
•	Ingrese su opcion:

1.Cargar Canciones: Se le pide al usuario que ingrese el archivo CVS y una vez ingresado, cargan todas las canciones.
•	Ingrese su opcion: 1
•	Ingresar la ruta de su archivo CSV: data/song_dataset_.csv
•	Canciones cargadas con exito. Total: ----
•	Presione una tecla para continuar...

2.Buscar por Género: Si la usuaria quiere buscar por género, se le pide que ingrese el que desee y se cargaran todas las canciones del género con su información correspondiente. En caso de que no exista, se mostrará un mensaje de error.
•	Ingrese su opción: 2
•	Ingrese el género de la canción:

3.Buscar por Artista: Si la usuaria quiere buscar por artista, se le pide que ingrese el que desee y se cargaran todas las canciones del artista con su información correspondiente. En caso de que no exista el artista en la base de datos, se mostrará un mensaje de error.
•	Ingrese su opcion: 3
•	Ingrese el nombre del artista:

4.Buscar por Tempo: Si la usuaria quiere buscar por tempo, se le pide que ingrese la velocidad que desee, en este caso sería lentas, moderadas o rápidas. Una vez seleccionado el tempo, se cargarán todas las canciones del tempo con su información correspondiente. En caso de que no exista, se mostrará un mensaje de error.
•	Ingrese su opcion: 4
•	Ingrese velocidad deseada (Lenta / Moderada / Rapida):

5.Crear Lista de Reproducción: Se le pide a la usaría ingresar el nombre de la lista de reproducción, para agregar canciones que desee.
•	Ingrese su opcion: 5
•	Ingrese el nombre de la lista: --
•	Lista '--' creada exitosamente.
•	Presione una tecla para continuar...

6.Agregar Canción a Lista: Se le pide a la usaría ingresar el ID de la canción y el nombre de lista, para luego agregar las canciones a la lista. En caso de que no exista ID o el nombre de la lista, se indicara con un mensaje.
•	Ingrese su opcion: 6
•	Ingresar ID cancion: ---
•	Ingrese nombre de la lista: hola
•	Se agrego cancion a la lista '--' :).
•	Presione una tecla para continuar...

7.Mostrar Canciones de una Lista: La usaría ingresa el nombre de la lista y se mostraran todas las canciones dentro de esta.
•	Ingrese su opcion: 7
•	Ingrese nombre lista: --
•	Canciones de la lista '--':.

Salir: Finaliza la ejecución de la aplicación.

Funcionalidades
Funcionando correctamente:
Cargar archivo CVS: Carga exitosamente la ruta de las canciones 
Buscar por género, artista y tempo: Las 3 funciones verifica que exista el género, artista o tempo antes de imprimir las canciones correspondientes al ítem seleccionado. 
Crear listas de reproducción personalizadas: Se ingresa el nombre de la lista y se comprueba correctamente que no exista lista de reproducción con mismo nombre antes de crearla.
Agregar canción a una lista  (agregar_canciones): Se ingresar ID canción y nombre lista correctamente, verificando que existan en ambos casos, si cumple esto, se agregan las canciones mostrando mensaje.
Mostrar canciones dentro de una lista (imprimir_lista_canciones): Se muestran las canciones correctamente en caso de que exista lista.

Problemas conocidos:
Si la usaria no lee las instrucciones en el README no va a saber que ruta debe ingresar para cargas canciones, por lo que, en vez de pedir que ingrese una ruta se podría agregar la base de datos directamente en el programa.
Una vez cerrada la aplicación no se guardan los datos, como las listas de reproducción, por lo que, faltaría un sistema de guardado.

A mejorar:
Eliminar las listas de reproducciones si la usaría desea.
Mejorar interfaz del sistema, que el apartado grafico sea distinto, ya que solo alguien que sabe programar sabrá como interactuar.

Contribuciones:
Benjamín Vargas:
•	Implementar la base de datos al programa.
•	Diseñar todas las funciones del código.
•	Verificar posibles errores de compilación.
•	Se encargo de comentar el código.
•	Autoevaluación: 3/3 (Aporte excelente).
•	Coevaluación : 3/3 (Aporte excelente)

Francisca Zamora:
•	Diseñar todas las funciones del código.
•	Modificar la salida e interfaz del programa.
•	Verificar posibles errores de compilación.
•	Diseño y redacción del README.
•	Autoevaluación: 3/3 (Aporte excelente).
•	Coevaluación : 3/3 (Aporte excelente)
