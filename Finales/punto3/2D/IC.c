#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

/*
 * Define las constantes necesarias
 */

#define PI 3.14159265358979323846
#define FLOAT double
#define manual 'Ejecutar de la forma ./IC.c x y vx vy N M R seed filename'

// La constante gravitacional en unidades de masa solar, kpc y miles de millones de años
#define G 4.49956*pow(10,-6)
// Numero de dimensiones a trabajar
#define n_dim 2

// Factores de conversion
#define conv 1.022690323


FLOAT ** allocate( int x, int y );
FLOAT *gen_Orto_Unit(FLOAT rx, FLOAT ry);
void gen_random( FLOAT R, FLOAT **r, FLOAT n_masses );
void gen_velocities( FLOAT M, FLOAT **r, FLOAT **v, FLOAT rx, FLOAT ry, int n_masses );

int main( int argc, char **argv){


  /*
   * Define las variables
   */

  // El puntero que contiene los punteros de posicion para cada masa (siendo el primer valor, la posicion de la estrella)
  FLOAT **r;
  // El puntero que contiene los punteros de velocidad para cada masa (siendo el primer valor, la velocidad de la estrella)
  FLOAT **v;
  // EL archivo donde se guardaran las condiciones iniciales
  FILE *data;
  // El numero de masas
  int n_masses;
  // El radio de la galaxia
  FLOAT R;
  // La masa de la galaxia
  FLOAT M;
  // La semilla de aleatoriedad
  int seed;


  /*
   * Aparta memoria para los punteros
   */
  n_masses = atoi( argv[5] );
  // Guarda memoria para las masas, y la estrella
  r = allocate( n_masses, n_dim );
  v = allocate( n_masses, n_dim );
  
  /*

   * Inicializa las variables que pueden ser inicializadas
   */
  
  // Obtiene las posiciones de los valores de entrada (MODIFICABLE SEGUN EL NUMERO DE DIMENSIONES DEL PROBLEMA)
  
  FLOAT rx = atof( argv[1] );
  FLOAT ry = atof( argv[2] );
  FLOAT vx = atof( argv[3] );
  FLOAT vy = atof( argv[4] );
  M = atof( argv[6] );
  R = atof( argv[7] );
  seed = atoi( argv[8] );
  char *filename = argv[9];
  // Le asigna la semilla a la sucesion de randoms
  srand48(seed);
  // El archivo de texto
  data = fopen( filename, "w" );

  // Inicializa los punteros todos con el mismo valor ( El valor de la estrella ) ( Transformada de Posiciones-Velocidades relativas )
  // Asi se puede reducir el problema a su analogo con estrella estática centrada en 0
  int i;
  for( i = 0; i < n_masses; i++ ){
    // Modificar si cambia de dimension
    r[0][i] = rx;
    r[1][i] = ry;
    v[0][i] = (vx*conv);
    v[1][i] = (vy*conv);
  }

  /*
   * Inicializa las posiciones de las masas aleatoriamente
   */
  
  // Modifica el vector posicion asignando a cada vectore
  gen_random( R, r, n_masses );
  /*
   * Asigna a cada posicion su velocidad (modificando el puntero v) necesaria para orbitar circularmente en la galaxia
   */
  
  gen_velocities( M, r, v, rx, ry, n_masses );
  /*
   * Imprime las condiciones iniciales en el archivo IC.data
   * Formato: ID x y z vx vy vz Mviaviaviaviaviavia
   */

  fprintf( data, "%d %le %le %le %le %le\n" , -seed*(10000000) , rx, ry, vx*(conv), vy*(conv), M );

  for( i = 0; i < n_masses; i++ ){
    fprintf( data, "%d %le %le %le %le %le\n" , seed*(10000000) + (i+1) , r[0][i], r[1][i], v[0][i], v[1][i],  0.0 );
  }
 
  return 0;
}

//**************************************************** 
//---------------------------------------------------
// Metodos
//---------------------------------------------------
//****************************************************



/*
 * Guarda memoria de un puntero (de tamaño y) de punteros (de tamaño x)
 */

FLOAT ** allocate( int x, int y ){
  // Guarda memoria para contener punteros
  FLOAT **r = malloc( y*sizeof(FLOAT*) );
  int i;
  // Guarda memoria para cada puntero contenido en cada casilla
  for( i = 0; i < y ; i++){
    r[i] = malloc( x*sizeof(FLOAT) );
  }
  return r;
}

/*
 * Genera n_masses vectores aleatorios de posicion ( en una bola de radio R centrada en r[0] ) y actualiza r para guardarlo
 */

void gen_random( FLOAT R, FLOAT **r, FLOAT n_masses ){
  
  FLOAT rx;
  FLOAT ry;
  FLOAT normi;

  int i;
  for( i = 0; i < n_masses; i++ ){
    // Genera un vector aleatorio en un cubo de lado 2 centrado en 0
    rx = 1 - 2*drand48();
    ry = 1 - 2*drand48();
    // Normaliza el vector( genera vectores aleatorios en la superficie de una esfera de radio 1 )
    // Esto genera un vector de direcciones aleatorias, de norma 1
    // Luego le asigna al vector una norma aleatoria entre 0.01pc y R
    normi = (3*pow(10,0) + (R-3*pow(10,0))*drand48())/sqrt( rx*rx + ry*ry );
    rx *= normi;
    ry *= normi;
    // Agrega el vector aleatorio al vector inicial
    r[0][i] += rx;
    r[1][i] += ry;
  }
}

/*
 * Genera un vector perpendicular a un vector no nulo dado
 */
FLOAT *gen_Orto_Unit(FLOAT rx, FLOAT ry){
  FLOAT *temp = malloc(3*sizeof(FLOAT));
  // si x e y son no-nulas, encuentra un vector perpendicular
  temp[0] = -ry/sqrt(rx*rx + ry*ry);
  temp[1] = rx/sqrt(rx*rx + ry*ry);
  
  return temp;
}

/*
 * Genera las velocidades a partir de los vectores posicion
 */

void gen_velocities( FLOAT M, FLOAT **r, FLOAT **v, FLOAT rx, FLOAT ry, int n_masses ){

  int i;
  for( i = 0; i < n_masses; i++ ){
    
    // Le asigna la direccion a la velocidad 
    FLOAT *v0 = gen_Orto_Unit(r[0][i]-rx, r[1][i]-ry);
    // Saca la norma de la velocidad necesaria para orbitar circularmente
    FLOAT normi = sqrt(G*M/sqrt(((r[0][i]-rx)*(r[0][i]-rx)) + ((r[1][i]-ry)*(r[1][i]-ry)) ));
    v[0][i] += normi*v0[0];
    v[1][i] += normi*v0[1];

  }
}

