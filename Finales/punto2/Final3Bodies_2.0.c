#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#define PI 3.14159265358979323846
#define FLOAT double
#define G 39.486
#define T 100.0
#define RR 100.0
#define n_dim 3
#define n_masses 3
#define theta 2*PI/3.0
#define rr RR*sqrt(3.0)
#define vv sqrt((11*G)/(3*rr))

FLOAT norm( FLOAT vec1, FLOAT vec2, FLOAT vec3);
FLOAT * allocate( int n );
FLOAT ** allocate2d( int x, int y );
FLOAT *** allocate3d( int x, int y, int z );
void initPos( FLOAT **r );
void initVel( FLOAT **v );
void initMass( FLOAT *m );
void updateAcc( FLOAT **r, FLOAT **a, FLOAT *m);
void rungeKutta( FLOAT **r, FLOAT **v, FLOAT *m, FLOAT dt);
FLOAT calcEnergy(FLOAT **r, FLOAT **v, FLOAT *m);

int main(int argc, char **argv){
 
  //------------------------------------------------------------------------
  // Constantes
  //------------------------------------------------------------------------

  // Archivo en formato t x1 y1 z1 ..... x3 y3 z3 energy
  FILE *data1;
  data1 = fopen("3body.data", "w");
    
  //------------------------------------------------------------------------
  // Variables 
  //------------------------------------------------------------------------
   
  // Crea Variables
  FLOAT **r;
  FLOAT **v;
  FLOAT *m;
  FLOAT actualTime = 0;
  FLOAT dt = atof(argv[1]);
  FLOAT energy = 0;
  // Guarda Memoria
  r = allocate2d(n_masses,n_dim);
  v = allocate2d(n_masses,n_dim);
  m = allocate(n_masses);
  // Inicializa Variables
  initPos(r);
  initVel(v);
  initMass(m);
  // Escribe en el archivo los primeros términos
  fprintf(data1, "%f %f %f %f %f %f %f %f %f %f %f\n",actualTime,r[0][0],r[1][0],r[2][0],r[0][1],r[1][1],r[2][1],r[0][2],r[1][2],r[2][2],energy);
  
  do{

    //------------------------------------------------------------------------
    // 1st second order DE Runge - Kutta (x1,v1)
    //------------------------------------------------------------------------
    // RungeKutta 4th order
    rungeKutta(r,v,m,dt);
    // Actualiza el tiempo
    actualTime += dt;
    // Calcula la energía del sistema
    energy = calcEnergy(r,v,m);
    // Escribe en el archivo
    fprintf(data1, "%f %f %f %f %f %f %f %f %f %f %f\n",actualTime,r[0][0],r[1][0],r[2][0],r[0][1],r[1][1],r[2][1],r[0][2],r[1][2],r[2][2],energy);
 
  }while(actualTime < T);
                                                                                      
  return 0;
}

/*
 * La norma de un vector en dos dimensiones
 */
FLOAT norm( FLOAT vec1, FLOAT vec2, FLOAT vec3){
  return sqrt(vec1*vec1 + vec2*vec2 + vec3*vec3);
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
  
  r[0][0] = RR*cos(0);
  r[0][1] = RR*cos(theta);
  r[0][2] = RR*cos(2*theta);
  r[1][0] = RR*sin(0);
  r[1][1] = RR*sin(theta);
  r[1][2] = RR*sin(2*theta);
  r[2][0] = 0;
  r[2][1] = 0;
  r[2][2] = 0;

}

/*
 * Inicializa las velocidades
 */
void initVel( FLOAT **v ){

  v[0][0] = 0;
  v[0][1] = 0;
  v[0][2] = 0;
  v[1][0] = 0;
  v[1][1] = 0;
  v[1][2] = 0;
  v[2][0] = vv;
  v[2][1] = vv;
  v[2][2] = vv;
  
}
/*
 * Inicializa las masas
 */
void initMass( FLOAT *m ){
  
  m[0] = 1;
  m[1] = 2;
  m[2] = 3;

}

/*
 * Actualiza las aceleraciones
 */
void updateAcc( FLOAT **r, FLOAT **a , FLOAT *m ){
  int i;
  int j;
  for( i = 0; i < n_masses; i++ ){
    a[0][i] = 0;
    a[1][i] = 0;
    a[2][i] = 0;
    for( j = 0; j < n_masses; j++ ){
      if( i != j ){
	FLOAT normi = pow(norm((r[0][i] - r[0][j]),(r[1][i] - r[1][j]),(r[2][i] - r[2][j])),3);
	FLOAT grav = (G*m[j])/normi;
	a[0][i] += grav*(r[0][j] - r[0][i]);
	a[1][i] += grav*(r[1][j] - r[1][i]);
	a[2][i] += grav*(r[2][j] - r[2][i]);
      }
    }
  }
}

/*
 * Guarda memoria
 */
FLOAT ** allocate2d( int x, int y ){
  // Guarda memoria para contener punteros
  FLOAT ** w = malloc( y*sizeof(FLOAT *) );
  int i;
  // Guarda memoria para cada puntero contenido en cada casilla
  for( i = 0; i < y ; i++ ){
    w[i] = malloc( x*sizeof(FLOAT) );
  }
  return w;
}

/*
 * Actualiza las posiciones
 */
void rungeKutta( FLOAT **r, FLOAT **v, FLOAT *m, FLOAT dt){
  // Crea los vectores de aceleracion, posicion y velodidad temporales
  FLOAT **newR;
  // Crea el vector k de constantes intermedias del método RungeKutta
  FLOAT ***kr;
  FLOAT ***kv;
  
  // Aparta memoria
  newR = allocate2d( n_masses, n_dim );
  kr = allocate3d( n_masses, n_dim, 4 );
  kv = allocate3d( n_masses, n_dim, 4 );

  //********************************************************************
  //--------------------------------------------------------------------
  // Cuerpo del RungeKutta
  //--------------------------------------------------------------------
  //********************************************************************

  int i;

  /*
   * Rungekutta 1st step
   */

  for( i = 0; i < n_masses; i++ ){
    
    // RungeKutta 1st step for n_masses
    kr[0][0][i] = v[0][i];
    kr[0][1][i] = v[1][i];
    kr[0][2][i] = v[2][i];
   
  }

  // Actualiza las aceleraciones para el primer paso de RungeKutta
  updateAcc(r,kv[0],m);

  /*
   * Rungekutta 2nd step
   */

  for( i = 0; i < n_masses; i++ ){
    
    // RungeKutta 2nd step for n_masses
    newR[0][i] = r[0][i] + kr[0][0][i]*dt/2 ;
    newR[1][i] = r[1][i] + kr[0][1][i]*dt/2 ;
    newR[2][i] = r[2][i] + kr[0][2][i]*dt/2 ;
    kr[1][0][i]  = v[0][i] + kv[0][0][i]*dt/2 ;
    kr[1][1][i]  = v[1][i] + kv[0][1][i]*dt/2 ;
    kr[1][2][i]  = v[2][i] + kv[0][2][i]*dt/2 ;
    
  }

  // Actualiza las aceleraciones para el segundo paso de RungeKutta
  updateAcc(newR,kv[1],m);

  /*
   * Rungekutta 3rd step
   */

  for( i = 0; i < n_masses; i++ ){
    
    // RungeKutta 3rd step for n_masses
    newR[0][i] = r[0][i] + kr[1][0][i]*dt/2 ;
    newR[1][i] = r[1][i] + kr[1][1][i]*dt/2 ;
    newR[2][i] = r[2][i] + kr[1][2][i]*dt/2 ;
    kr[2][0][i]  = v[0][i] + kv[1][0][i]*dt/2 ;
    kr[2][1][i]  = v[1][i] + kv[1][1][i]*dt/2 ;
    kr[2][2][i]  = v[2][i] + kv[1][2][i]*dt/2 ;

  }

  // Actualiza las aceleraciones para el tercer paso de RungeKutta
  updateAcc(newR,kv[2],m);

  /*
   * Rungekutta 4th step
   */

  for( i = 0; i < n_masses; i++ ){
    // RungeKutta 4th step for n_masses
    newR[0][i] = r[0][i] + kr[2][0][i]*dt ;
    newR[1][i] = r[1][i] + kr[2][1][i]*dt ;
    newR[2][i] = r[2][i] + kr[2][2][i]*dt ;
    kr[3][0][i]  = v[0][i] + kv[2][0][i]*dt ;
    kr[3][1][i]  = v[1][i] + kv[2][1][i]*dt ;
    kr[3][2][i]  = v[2][i] + kv[2][2][i]*dt ;
  }

  // Actualiza las aceleraciones para el cuarto paso de RungeKutta
  updateAcc(newR,kv[3],m);

  //-----------------------------------------------------------------
  //*****************************************************************

  // Promedia los k y actualiza los vectores de entrada en i+1
  for( i = 0; i < n_masses; i++ ){
    // Promedia los ks de n_masses
    // Actualiza los vectores de entrada
    FLOAT kmeanx = (1/6.0)*(kr[0][0][i] + 2*kr[1][0][i] + 2*kr[2][0][i] + kr[3][0][i]);
    r[0][i] = r[0][i] + kmeanx*dt;
    FLOAT kmeany = (1/6.0)*(kr[0][1][i] + 2*kr[1][1][i] + 2*kr[2][1][i] + kr[3][1][i]);
    r[1][i] = r[1][i] + kmeany*dt;
    FLOAT kmeanz = (1/6.0)*(kr[0][2][i] + 2*kr[1][2][i] + 2*kr[2][2][i] + kr[3][2][i]);
    r[2][i] = r[2][i] + kmeanz*dt;
    FLOAT kmeanvx = (1/6.0)*(kv[0][0][i] + 2*kv[1][0][i] + 2*kv[2][0][i] + kv[3][0][i]);
    v[0][i] = v[0][i] + kmeanvx*dt;
    FLOAT kmeanvy = (1/6.0)*(kv[0][1][i] + 2*kv[1][1][i] + 2*kv[2][1][i] + kv[3][1][i]);
    v[1][i] = v[1][i] + kmeanvy*dt;
    FLOAT kmeanvz = (1/6.0)*(kv[0][2][i] + 2*kv[1][2][i] + 2*kv[2][2][i] + kv[3][2][i]);
    v[2][i] = v[2][i] + kmeanvz*dt;
  } 
 
}
/*
 * Calcula la energía total del sistema
 */
FLOAT calcEnergy(FLOAT **r, FLOAT **v, FLOAT *m){
  FLOAT ener = 0;
  int i,j;
  for( i = 0; i < n_masses; i++){
    ener += (1/2.0)*m[i]*v[0][i]*v[0][i];
    ener += (1/2.0)*m[i]*v[1][i]*v[1][i];
    ener += (1/2.0)*m[i]*v[2][i]*v[2][i];
    for( j = i+1; j < n_masses; j++){
      FLOAT normi = norm((r[0][i] - r[0][j]),(r[1][i] - r[1][j]),(r[2][i] - r[2][j]));
      ener += -G*m[i]*m[j]/normi;
    } 
  }
  return ener;
}



