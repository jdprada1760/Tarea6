#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

/*
 * Define las constantes necesarias
 */

#define PI 3.14159265358979323846
#define FLOAT double
#define manual 'Ejecutar de la forma ./IC.c x y z vx vy vz N M R seed'

// La constante gravitacional en unidades de masa solar, kpc y años
#define G 'some FLOAT value'
// Numero de dimensiones a trabajar
#define n_dim 3

// Factores de conversion
#define s_to_yr 'some FLOAT value'
#define m_to_pc 'some FLOAT value'


FLOAT ** allocate( int x, int y );
void gen_random( FLOAT R, FLOAT **r, FLOAT n_masses );
void gen_velocities( FLOAT M, FLOAT **r, FLOAT **v, int n_masses );

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
  n_masses = atoi( argv[7] );
  // Guarda memoria para las masas, y la estrella
  r = allocate( n_dim, n_masses+1 );
  v = allocate( n_dim, n_masses+1 );
  
  /*
   * Inicializa las variables que pueden ser inicializadas
   */
  
  // Obtiene las posiciones de los valores de entrada (MODIFICABLE SEGUN EL NUMERO DE DIMENSIONES DEL PROBLEMA)
  
  FLOAT rx = atof( argv[1] );
  FLOAT ry = atof( argv[2] );
  FLOAT rz = atof( argv[3] );
  FLOAT vx = atof( argv[4] );
  FLOAT vy = atof( argv[5] );
  FLOAT vz = atof( argv[6] );
  M = atof( argv[8] );
  R = atof( argv[9] );
  seed = atoi( argv[10] );
  // Le asigna la semilla a la sucesion de randoms
  srand48(seed);

  // El archivo de texto
  data = fopen( "IC.data", "w" );

  // Inicializa los punteros todos con el mismo valor ( El valor de la estrella ) ( Transformada de Posiciones-Velocidades relativas )
  // Asi se puede reducir el problema a su analogo con estrella estática centrada en 0
  int i;
  for( i = 0; i < n_masses + 1; i++ ){
    // Modificar si cambia de dimension
    r[i][0] = rx;
    r[i][1] = ry;
    r[i][2] = rz;
    v[i][0] = vx;
    v[i][1] = vy;
    v[i][2] = vz;
  }

  /*
   * Inicializa las posiciones de las masas aleatoriamente
   */
  
  // Modifica el vector posicion asignando a cada vectore
  gen_random( R, r, n_masses + 1 );
  
  /*
   * Asigna a cada posicion su velocidad (modificando el puntero v) necesaria para orbitar circularmente en la galaxia
   */
  
  gen_velocities( M, r, v, n_masses + 1 );

  /*
   * Imprime las condiciones iniciales en el archivo IC.data
   * Formato: ID x y z vx vy vz M
   */

  // Mejorar el ID para que sea único (Por ahora será su masa)
  fprintf( data, "%f %f %f %f %f %f %f %f\n" , -M , r[0][0], r[0][1], r[0][2], v[0][0], v[0][1], v[0][2], M );

  for( i = 1; i < n_masses + 1; i++ ){
    // Para las masas, su ID será su posición en el puntero, dividido ( por el numero de masas mas pi )
    fprintf( data, "%f %f %f %f %f %f %f %f\n" , i/(n_masses + PI) , r[i][0], r[i][1], r[i][2], v[i][0], v[i][1], v[i][2], 0.0 );
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
  FLOAT rz;
  FLOAT normi;

  int i;
  for( i = 1; i < n_masses; i++ ){
    // Genera un vector aleatorio en un cubo de lado 2 centrado en 0
    rx = 1 - 2*drand48();
    ry = 1 - 2*drand48();
    rz = 1 - 2*drand48();
    // Normaliza el vector( genera vectores aleatorios en la superficie de una esfera de radio 1 )
    // Esto genera un vector de direcciones aleatorias, de norma 1
    // Luego le asigna al vector una norma aleatoria entre 0 y R
    normi = R*drand48()/sqrt( rx*rx + ry*ry + rz*rz );
    rx *= normi;
    ry *= normi;
    rz *= normi;
    // Agrega el vector aleatorio al vector inicial
    r[i][0] += rx;
    r[i][1] += ry;
    r[i][2] += rz;
  }
}

/*
 * Genera las velocidades a partir de los vectores posicion
 */

void gen_velocities( FLOAT M, FLOAT **r, FLOAT **v, int n_masses ){

  int i;
  for( i = 1; i < n_masses; i++ ){
    
    FLOAT normi = sqrt( r[i][0]*r[i][0] + r[i][1]*r[i][1] + r[i][2]*r[i][2] );
    FLOAT normv;
    FLOAT vx;
    FLOAT vy;
    FLOAT vz;

    // Saca la norma de la velocidad necesaria para orbitar circularmente
    
    // TODO
    
    // Le asigna la direccion a la velocidad 
    
    // TODO
  }
}

