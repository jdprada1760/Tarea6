#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#define PI 3.14159265358979323846
#define FLOAT double
#define G 'Some FLOAT value'
#define T pow(10,9)
#define R 100
#define n_masses pow(10,4)
#define n_dim 2

FLOAT norm( FLOAT vec1, FLOAT vec2 );
FLOAT dotP( FLOAT vec11, FLOAT vec12, FLOAT vec21, FLOAT vec22);
FLOAT * crossP( FLOAT vec1, FLOAT vec2);
FLOAT * allocate( int n );
FLOAT ** allocate2d( int x, int y );
void initPos( FLOAT **r );
void initVel( FLOAT **v );
void initMass( FLOAT *m );
void updateAcc( FLOAT **r, FLOAT **a , FLOAT *m, FLOAT n_Masses);
void rungeKutta( FLOAT **r, FLOAT **v, FLOAT **a, FLOAT *m, FLOAT dt);
FLOAT calcEnergy(FLOAT **r, FLOAT **v, FLOAT *m);
FLOAT *** allocate3d( int x, int y, int z );

int main(int argc, char **argv){
 
  //------------------------------------------------------------------------
  // Constantes
  //------------------------------------------------------------------------

  // Archivo en formato t x1 y1 z1 ............... xn yn zn 
  FILE *data;
  FILE *energydat;
  energydat = fopen("energy.data", "w");
  
  //------------------------------------------------------------------------
  // Variables 
  //------------------------------------------------------------------------
  
  /*
   * Define las variables
   */

  // El puntero que contiene los punteros de posicion para cada masa pequeña 
  FLOAT **r;
  // El puntero que contiene los punteros de velocidad para cada masa pequeña 
  FLOAT **v;
  // El puntero que contiene la aceleracion de cada estrella
  FLOAT **a;
  // El puntero que contiene las masasa de cada estrella
  FLOAT *m;
  FLOAT actualTime = 0;
  FLOAT dt = atof(argv[1]);
  FLOAT energy;
  // Guarda Memoria
  r = allocate2d( n_dim, n_masses );
  v = allocate2d( n_dim, n_masses );
  m = allocate(n_masses);
  // Inicializa Variables
  initPos(r);
  initVel(v);
  initMass(m);
  updateAcc(r,a,m, n_masses);
  
  //************************************************
  // Escribe en el archivo las condiciones iniciales
  //************************************************
  // TODO
  fprintf(energydat, "%f %f\n",actualTime,energy);
  
  do{

    //------------------------------------------------------------------------
    // 1st second order DE Runge - Kutta (x1,v1)
    //------------------------------------------------------------------------
    
    // Actualiza la aceleración (función de segunda derivada)
    updateAcc(r,a,m, n_masses);
    // RungeKutta 4th order
    rungeKutta(r,v,a,m,dt);
    // Actualiza el tiempo
    actualTime += dt;
    // Calcula la energía del sistema
    energy = calcEnergy(r,v,m);

    //************************************************
    // Escribe en el archivo la evolucion
    //************************************************
    // TODO
    fprintf(energydat, "%f %f\n",actualTime,energy);

  }while(actualTime < T);
                                                                                          
  return 0;
}


/*
 * La norma de un vector en dos dimensiones
 */
FLOAT norm( FLOAT vec1, FLOAT vec2 ){
  return sqrt(vec1*vec1 + vec2*vec2);
}
/*
 * El producto punto de dos vectores
 */
FLOAT dotP( FLOAT vec11, FLOAT vec12, FLOAT vec21, FLOAT vec22){
  return vec11*vec21 + vec21*vec22;
}
/*
 * El producto cruz de un vector 2D en xy con el vector k
 */
FLOAT * crossP( FLOAT vec1, FLOAT vec2){
  FLOAT *vectemp = malloc(2*sizeof(FLOAT));
  FLOAT norma = norm(vec1,vec2);
  vectemp[0] = vec2/norma;
  vectemp[1] = -vec1/norma;
  return vectemp;
}
/*
 * Guarda memoria
 */
FLOAT * allocate( int n ){
  FLOAT * s;
  s = malloc( n*sizeof( FLOAT ) );
  return s;
}

/*
 * Guarda memoria
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
 * Guarda memoria
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
 * Inicializa las posiciones
 */
void initPos( FLOAT **r ){
  // Inicializa las posiciones en random
  // TODO
}
/*
 * Inicializa las velocidades
 */
void initVel( FLOAT **v ){
  // Inicializa las velocidades en 0
  // TODO
}

/*
 * Inicializa las masas
 */
void initMass( FLOAT *m ){
  // Inicializa todas las masas en 1
  // TODO
}

/*
 * Actualiza las aceleraciones
 */
void updateAcc( FLOAT **r, FLOAT **a , FLOAT *m, FLOAT n_Masses){
  int i;
  int j;
  for( i = 0; i < n_Masses; i++ ){
    // Preliminares
    for( j = 0; j < n_Masses; j++ ){
      // Si no son la misma masa, calcula la atraccion gravitacional
      if( i != j ){
	// Calcula la atraccion gravitacional con la ley modificada y la suma a la acumulacion del punto anterior
	// TODO 
      }
    }
  }
}

/*
 * Actualiza las posiciones
 */
void rungeKutta( FLOAT **r, FLOAT **v, FLOAT **acc, FLOAT *m, FLOAT dt){
  // Crea los vectores de aceleracion, posicion y velodidad temporales
  FLOAT **a;
  FLOAT **newR;
  FLOAT **newV;
  // Crea el vector k de constantes intermedias del método RungeKutta
  FLOAT ***kx;
  FLOAT ***kv;
  // Aparta memoria
  a = allocate2d( n_dim, n_masses );
  newR = allocate2d( n_dim, n_masses );
  newV = allocate2d( n_dim, n_masses );
  kx = allocate3d( n_dim, n_masses, 5 );
  kv = allocate3d( n_dim, n_masses, 5 );

  //********************************************************************
  //--------------------------------------------------------------------
  // Cuerpo del RungeKutta
  //--------------------------------------------------------------------
  //********************************************************************

  int j;
  // Actualiza las aceleraciones para el primer paso de RungeKutta
  updateAcc( r, a, m, n_masses );

  /*
   * Rungekutta 1st step
   */

  for( j = 0; j < n_masses; j++ ){
    // RungeKutta 1st step for n_masses
    // TODO
  }

  // Actualiza las aceleraciones para el segundo paso de RungeKutta
  updateAcc( newR, a, m, n_masses );

  /*
   * Rungekutta 2nd step
   */

  for( j = 0; j < n_masses; j++ ){
    // RungeKutta 2nd step for n_masses
    // TODO
  }
  
  // Actualiza las aceleraciones para el tercer paso de RungeKutta
  updateAcc( newR, a, m, n_masses );

  /*
   * Rungekutta 3rd step
   */

  for( j = 0; j < n_masses; j++ ){
    // RungeKutta 3rd step for n_masses
    // TODO
  }
  
  // Actualiza las aceleraciones para el cuarto paso de RungeKutta
  updateAcc( newR, a, m, n_masses );

  /*
   * Rungekutta 4th step
   */

  for( j = 0; j < n_masses; j++ ){
    // RungeKutta 4th step for n_masses
    // TODO
  }

  //-----------------------------------------------------------------
  //*****************************************************************

  // Promedia los k y actualiza los vectores de entrada en i+1
  for( j = 0; j < n_masses; j++ ){
    // Promedia los ks de n_masses
    // Actualiza los vectores de entrada
    // TODO
  } 
}
/*
 * Calcula la energía total del sistema
 */
FLOAT calcEnergy(FLOAT **r, FLOAT **v, FLOAT *m){
  // Calcula la energía potencial y cinetica
  // TODO
}
