#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

/*
 * Define las constantes necesarias
 */

#define PI 3.14159265358979323846
#define FLOAT double
#define manual 'Ejecutar de la forma ./evolve.c N T dataname.dataformat'

// La constante gravitacional en unidades de masa solar, kpc y años
#define G 'some FLOAT value'
// Numero de dimensiones a trabajar
#define n_dim 3

// Factores de conversion
#define s_to_yr 'some FLOAT value'
#define m_to_pc 'some FLOAT value'


FLOAT *** allocate3d( int x, int y, int z );
FLOAT ** allocate2d( int x, int y );
void rungeKutta4( FLOAT ***rmt, FLOAT ***vmt, FLOAT ***rMt, FLOAT ***vMt, FLOAT *M, FLOAT dt, int i , int n_masses, int n_Masses );
void updateAcc( FLOAT **rm, FLOAT **rM, FLOAT **am, FLOAT  **aM, FLOAT *M, int n_masses, int n_Masses );


int main( int argc, char **argv){

  /*
   * Define las variables
   */

  // El puntero que contiene los punteros de posicion para cada masa pequeña en cada instante de tiempo
  FLOAT ***rm;
  // El puntero que contiene los punteros de velocidad para cada masa pequeña en cada instante de tiempo
  FLOAT ***vm;
  // El puntero que contiene los punteros de posicion para cada masa Grande en cada instante de tiempo
  FLOAT ***rM;
  // El puntero que contiene los punteros de velocidad para cada masa Grande en cada instante de tiempo
  FLOAT ***vM;
  // EL archivo donde se guardaron las condiciones iniciales
  FILE *data;
  // El archivo que guarda las posiciones en x
  FILE *xdata;
  // El archivo que guarda las velocidades en x
  FILE *vxdata;
  // El archivo que guarda las posiciones en y
  FILE *ydata;
  // El archivo que guarda las velocidades en y
  FILE *vydata;
  // El archivo que guarda las posiciones en z
  FILE *zdata;
  // El archivo que guarda las velocidades en z
  FILE *vzdata;
  // El numero de masas pequeñas
  int n_masses;
  // El numero de masas grandes
  int n_Masses;
  // El vector de masas grandesy los guarda en la ultima casilla del puntero de ks
  FLOAT *M;
  // El vector de Identidades de masas pequeñas
  FLOAT *idm;
  // El vector de Identidades de masas grandes
  FLOAT *idM;
  // El tiempo total de integracion (en años)
  FLOAT T;
  // El numero de iteraciones que se quiere sobre ese tiempo
  int N;
  // El nombre del archivo
  char *name;
  // El intervalo de tiempo sobre integracion
  FLOAT dt = T/N;

  /*
   * Inicializa las variables que pueden ser inicializadas
   */

  N = atoi( argv[1] );
  T = atof( argv[2] );
  name =  argv[3];
  
  // El archivo de texto de condiciones iniciales y de evolucion en el tiempo
  data = fopen( name, "r" );
  xdata = fopen( "x.data", "w");
  vxdata = fopen( "vx.data", "w");
  ydata = fopen( "y.data", "w");
  vydata = fopen( "vy.data", "w");
  zdata = fopen( "z.data", "w");
  vzdata = fopen( "vz.data", "w");

  //-------------------------------------------------------------------------------------------------------------------
  //*******************************************************************************************************************
  
  /*
   * Lee el archivo y determina el numero de masas pequeñas y el numero de masas grandes (para poder apartar memoria)
   */

  // TODO
  
  //*******************************************************************************************************************
  //-------------------------------------------------------------------------------------------------------------------

  // Por simplicidad, por ahora asumamos n_Masses = n_masses = 1
  n_masses = 1;
  n_Masses = 1;
  
  /*
   * Aparta el espacio en memoria de los punteros
   */
  M = malloc( n_Masses*sizeof(FLOAT) );
  idm = malloc( n_masses*sizeof(FLOAT) );
  idM = malloc( n_Masses*sizeof(FLOAT) );
  rm = allocate3d( n_dim, n_masses, N );
  rM = allocate3d( n_dim, n_Masses, N );
  vm = allocate3d( n_dim, n_Masses, N );
  vM = allocate3d( n_dim, n_Masses, N );

  
  //-------------------------------------------------------------------------------------------------------------------
  //*******************************************************************************************************************

  /*
   * Lee el archivo de nuevo y asigna los valores iniciales de posicion y velocidad, ademas del vector de masas
   */

  // TODO
  
  //*******************************************************************************************************************
  //-------------------------------------------------------------------------------------------------------------------


  /*
   *  Método Runge-Kutta 4to orden sobre el equiespaciado temporal
   */
  
  int i;
  for( i = 0; i < N-1; i++ ){
    rungeKutta4( rm, vm, rM, vM, M, dt, i , n_masses, n_Masses );
  }

  /*
   * Imprime la evolucion de los parametros en los archivos
   * Formato: ID x y z vx vy vz M
   */

  // Mejorar el ID para que sea único (Por ahora será su masa)
  

  for( i = 0; i < n_masses + 1; i++ ){
    // Escribe las posiciones y velocidades de n_masses
    // TODO
  }
  
  for( i = 0; i < n_Masses + 1; i++ ){
    // Escribe las posiciones y velocidades de n_Mmasses
    // TODO
  }
 
  return 0;
}

//**************************************************** 
//---------------------------------------------------
// Metodos
//---------------------------------------------------
//****************************************************

/*
 * Guarda memoria de un puntero (de tamaño y) de punteros (de tamaño x) de FLOATS
 */

FLOAT ** allocate2d( int x, int y ){
  // Guarda memoria para contener punteros
  FLOAT **r = malloc( y*sizeof(FLOAT*) );
  int i;
  // Guarda memoria para cada puntero contenido en cada casilla
  for( i = 0; i < y ; i++ ){
    r[i] = malloc( x*sizeof(FLOAT) );
  }
  return r;
}

/*
 * Guarda memoria de un puntero (de tamaño y) de punteros (de tamaño x) de punteros (de tamaño z) de FLOATS
 */

FLOAT *** allocate3d( int x, int y, int z ){
  // Guarda memoria para contener punteros
  FLOAT ***r = malloc( z*sizeof(FLOAT**) );
  int i;
  int j;
  // Guarda memoria para cada puntero contenido en cada casilla
  for( i = 0; i < z ; i++){
    r[i] = malloc( y*sizeof(FLOAT*) );
    for( j = 0; j < y; j++ ){
      r[i][j] = malloc( x*sizeof(FLOAT) );
    }
  }
  return r;
}

/*
 * Actualiza los punteros de aceleracion que entran por marámetro en función de las variables de posicion que dan
 */

void updateAcc( FLOAT **rm, FLOAT **rM, FLOAT **am, FLOAT  **aM, FLOAT *M, int n_masses, int n_Masses ){
  int i;
  int j;
  int k;
  for( j = 0; j < n_Masses; j++ ){
     for( i = 0; i < n_Masses; i++ ){
       if( i != j ){
	 // Actualizar las aceleraciones de cada Masa grande
	 // TODO
       }
     }
  }

  for( j = 0; j < n_masses; j++ ){
    for( i = 0; i < n_Masses; i++ ){
      // Actualizar las aceleraciones de cada Masa pequeña
      // TODO
     }
  }
}

/*
 * Método de Runge-Kutta 4to orden aplicado
 */

void rungeKutta4( FLOAT ***rmt, FLOAT ***vmt, FLOAT ***rMt, FLOAT ***vMt, FLOAT *M, FLOAT dt, int i , int n_masses, int n_Masses ){
  // Crea los vectores de aceleracion, posicion y velodidad temporales
  FLOAT **am;
  FLOAT **aM;
  FLOAT **newRm;
  FLOAT **newRM;
  FLOAT **newVm;
  FLOAT **newVM;
  // Crea el vector k de constantes intermedias del método RungeKutta
  FLOAT ***kxm;
  FLOAT ***kxM;
  FLOAT ***kvm;
  FLOAT ***kvM;
  // Aparta memoria
  am = allocate2d( n_dim, n_masses );
  aM = allocate2d( n_dim, n_Masses );
  newRm = allocate2d( n_dim, n_masses );
  newRM = allocate2d( n_dim, n_Masses );
  newVm = allocate2d( n_dim, n_masses );
  newVM = allocate2d( n_dim, n_Masses );
  kxm = allocate3d( n_dim, n_masses, 5 );
  kxM = allocate3d( n_dim, n_Masses, 5 );
  kvm = allocate3d( n_dim, n_masses, 5 );
  kvM = allocate3d( n_dim, n_Masses, 5 );

  //********************************************************************
  //--------------------------------------------------------------------
  // Cuerpo del RungeKutta
  //--------------------------------------------------------------------
  //********************************************************************

  int j;
  // Actualiza las aceleraciones para el primer paso de RungeKutta
  updateAcc( rmt[i], rMt[i], am, aM, M, n_masses, n_Masses );

  /*
   * Rungekutta 1st step
   */

  for( j = 0; j < n_Masses; j++ ){
    // RungeKutta 1st step for n_Masses
    // TODO
  }

  for( j = 0; j < n_masses; j++ ){
    // RungeKutta 1st step for n_masses
    // TODO
  }

  // Actualiza las aceleraciones para el segundo paso de RungeKutta
  updateAcc( newRm, newRM, am, aM, M, n_masses, n_Masses );

  /*
   * Rungekutta 2nd step
   */

  for( j = 0; j < n_Masses; j++ ){
    // RungeKutta 2nd step for n_Masses
    // TODO
  }

  for( j = 0; j < n_masses; j++ ){
    // RungeKutta 2nd step for n_masses
    // TODO
  }
  
  // Actualiza las aceleraciones para el tercer paso de RungeKutta
  updateAcc( newRm, newRM, am, aM, M, n_masses, n_Masses );

  /*
   * Rungekutta 3rd step
   */

  for( j = 0; j < n_Masses; j++ ){
    // RungeKutta 3rd step for n_Masses
    // TODO
  }

  for( j = 0; j < n_masses; j++ ){
    // RungeKutta 3rd step for n_masses
    // TODO
  }
  
  // Actualiza las aceleraciones para el cuarto paso de RungeKutta
  updateAcc( newRm, newRM, am, aM, M, n_masses, n_Masses );

  /*
   * Rungekutta 4th step
   */

  for( j = 0; j < n_Masses; j++ ){
    // RungeKutta 4th step for n_Masses
    // TODO
  }

  for( j = 0; j < n_masses; j++ ){
    // RungeKutta 4th step for n_masses
    // TODO
  }

  //-----------------------------------------------------------------
  //*****************************************************************

  // Promedia los k y actualiza los vectores de entrada en i+1
  
  for( j = 0; j < n_Masses; j++ ){
    // Promedia los ks de n_Masses
    // Actualiza los vectores de entrada
    // TODO
  }

  for( j = 0; j < n_masses; j++ ){
    // Promedia los ks de n_masses
    // Actualiza los vectores de entrada
    // TODO
  }

}
