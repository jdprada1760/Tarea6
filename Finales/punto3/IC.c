#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

/*
 * Define las constantes necesarias
 */

#define PI 3.14159265358979323846
#define FLOAT double
#define manual 'Ejecutar de la forma ./IC.c x y z vx vy vz N M R seed filename'

// La constante gravitacional en unidades de masa solar, kpc y miles de millones de años
#define G 4.49956*pow(10,-6)
// Numero de dimensiones a trabajar
#define n_dim 3

// Factores de conversion
#define mmyr_to_s 31556926*pow(10,9) 
#define pc_to_m 3.08567758*pow(10,16)


FLOAT ** allocate( int x, int y );
FLOAT *gen_Orto_Unit(FLOAT rx, FLOAT ry, FLOAT rz);
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
  r = allocate( n_masses, n_dim );
  v = allocate( n_masses, n_dim );
  
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
  char *filename = argv[11];
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
    r[2][i] = rz;
    v[0][i] = vx*(mmyr_to_s/pc_to_m);
    v[1][i] = vy*(mmyr_to_s/pc_to_m);
    v[2][i] = vz*(mmyr_to_s/pc_to_m);
  }

  /*
   * Inicializa las posiciones de las masas aleatoriamente
   */
  
  // Modifica el vector posicion asignando a cada vectore
  gen_random( R, r, n_masses );
  /*
   * Asigna a cada posicion su velocidad (modificando el puntero v) necesaria para orbitar circularmente en la galaxia
   */
  
  gen_velocities( M, r, v, n_masses );
  /*
   * Imprime las condiciones iniciales en el archivo IC.data
   * Formato: ID x y z vx vy vz M
   */

  fprintf( data, "%i %f %f %f %f %f %f %f\n" , -seed*(10000000) , rx, ry, rz, vx, vy, vz, M );

  for( i = 0; i < n_masses; i++ ){
    fprintf( data, "%i %f %f %f %f %f %f %f\n" , seed*(10000000) + (i+1) , r[0][i], r[1][i], r[2][i], v[0][i], v[1][i], v[2][i], 0.0 );
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
  for( i = 0; i < n_masses; i++ ){
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
    r[0][i] += rx;
    r[1][i] += ry;
    r[2][i] += rz;
  }
}

/*
 * Genera un vector perpendicular a un vector no nulo dado
 */
FLOAT *gen_Orto_Unit(FLOAT rx, FLOAT ry, FLOAT rz){
  FLOAT *temp = malloc(3*sizeof(FLOAT));
  // si x e y son no-nulas, encuentra un vector perpendicular
  if( ((rx!=0) && (ry!=0)) ){
  
    temp[0] = -ry/sqrt(rx*rx + ry*ry);
    temp[1] = rx/sqrt(rx*rx + ry*ry);
    temp[2] = 0;
    
  }else{
    
    temp[0] = 1;
    temp[1] = 0;
    temp[2] = 0;
  
  }
  
  return temp;
}

/*
 * Genera las velocidades a partir de los vectores posicion
 */

void gen_velocities( FLOAT M, FLOAT **r, FLOAT **v, int n_masses ){

  int i;
  for( i = 0; i < n_masses; i++ ){
    
    // Le asigna la direccion a la velocidad 
    FLOAT *v0 = gen_Orto_Unit(r[0][i], r[1][i], r[2][i]);
    // Saca la norma de la velocidad necesaria para orbitar circularmente
    FLOAT normi = sqrt(G*M/sqrt(r[0][i]*r[0][i] + r[1][i]*r[1][i] + r[2][i]*r[2][i]));
    v[0][i] += normi*v0[0];
    v[1][i] += normi*v0[1];
    v[2][i] += normi*v0[2];

  }
}

