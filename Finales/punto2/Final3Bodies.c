#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#define PI 3.14159265358979323846
#define FLOAT double
#define G 39.486
#define T 5000.0
#define R 100.0
#define n_dim 3
#define n_masses 3
#define theta 2*PI/3.0
#define r R*sqrt(3.0)
#define v sqrt((11*G)/(3*r))

FLOAT norm( FLOAT vec1, FLOAT vec2, FLOAT vec3);
FLOAT * allocate( int n );
FLOAT ** allocate2d( int x, int y );
void initPos( FLOAT *xX, FLOAT *xY, FLOAT *xZ );
void initVel( FLOAT *vX, FLOAT *vY, FLOAT *vZ );
void initMass( FLOAT *m );
void updateAcc( FLOAT *xX, FLOAT *xY, FLOAT *xZ, FLOAT *aX, FLOAT *aY, FLOAT *aZ, FLOAT *m);
void rungeKutta( FLOAT *xX, FLOAT *xY, FLOAT *xZ, FLOAT *vX, FLOAT *vY, FLOAT *vZ, FLOAT *m, FLOAT dt);
FLOAT calcEnergy(FLOAT *x, FLOAT *y, FLOAT *z, FLOAT *vx, FLOAT *vy, FLOAT *vz, FLOAT *m);

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
  FLOAT *x;
  FLOAT *y;
  FLOAT *z;
  FLOAT *vX;
  FLOAT *vY;
  FLOAT *vZ;
  FLOAT *m;
  FLOAT actualTime = 0;
  FLOAT dt = atof(argv[1]);
  FLOAT energy = 0;
  // Guarda Memoria
  x = allocate(n_masses);
  y = allocate(n_masses);
  z = allocate(n_masses);
  vX = allocate(n_masses);
  vY = allocate(n_masses);
  vZ = allocate(n_masses);
  m = allocate(n_masses);
  // Inicializa Variables
  initPos(x, y, z);
  initVel(vX, vY, vZ);
  initMass(m);
  // Escribe en el archivo los primeros términos
  fprintf(data1, "%f %f %f %f %f %f %f %f %f %f %f\n",actualTime,x[0],y[0],z[0],x[1],y[1],z[1],x[2],y[2],z[2],energy);
  
  do{

    //------------------------------------------------------------------------
    // 1st second order DE Runge - Kutta (x1,v1)
    //------------------------------------------------------------------------
    
    // RungeKutta 4th order
    rungeKutta(x,y,z,vX,vY,vZ,m,dt);
    // Actualiza el tiempo
    actualTime += dt;
    // Calcula la energía del sistema
    energy = calcEnergy(x,y,z,vX,vY,vZ,m);
    // Escribe en el archivo
    fprintf(data1, "%f %f %f %f %f %f %f %f %f %f %f\n",actualTime,x[0],y[0],z[0],x[1],y[1],z[1],x[2],y[2],z[2],energy);
  
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
 * Inicializa las posiciones
 */
void initPos( FLOAT *xX, FLOAT *xY, FLOAT *xZ ){
  
  xX[0] = R*cos(0);
  xX[1] = R*cos(theta);
  xX[2] = R*cos(2*theta);
  xY[0] = R*sin(0);
  xY[1] = R*sin(theta);
  xY[2] = R*sin(2*theta);
  xZ[0] = 0;
  xZ[1] = 0;
  xZ[2] = 0;

}
/*
 * Inicializa las velocidades
 */
void initVel( FLOAT *vX, FLOAT *vY, FLOAT *vZ ){

  vX[0] = 0;
  vY[0] = 0;
  vZ[0] = v;
  vX[1] = 0;
  vY[1] = 0;
  vZ[1] = v;
  vX[2] = 0;
  vY[2] = 0;
  vZ[2] = v;

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
void updateAcc( FLOAT *xX, FLOAT *xY, FLOAT *xZ, FLOAT *aX, FLOAT *aY, FLOAT *aZ , FLOAT *m){
  int i;
  int j;
  for( i = 0; i < n_masses; i++ ){
    aX[i] = 0;
    aY[i] = 0;
    aZ[i] = 0;
    for( j = 0; j < n_masses; j++ ){
      if( i != j ){
	FLOAT normi = pow(norm((xX[i] - xX[j]),(xY[i] - xY[j]),(xZ[i] - xZ[j])), 3);
	FLOAT grav = (G*m[j])/normi;
	aX[i] += grav*(xX[j] - xX[i]);
	aY[i] += grav*(xY[j] - xY[i]);
	aZ[i] += grav*(xZ[j] - xZ[i]);
      }
    }
  }
}

/*
 * Guarda memoria
 */
FLOAT ** allocate2d( int x, int y ){
  // Guarda memoria para contener punteros
  FLOAT **w = malloc( y*sizeof(FLOAT*) );
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
void rungeKutta( FLOAT *xX, FLOAT *xY, FLOAT *xZ, FLOAT *vX, FLOAT *vY, FLOAT *vZ, FLOAT *m, FLOAT dt){
  // Crea los vectores de aceleracion, posicion y velodidad temporales
  FLOAT **newR;
  FLOAT **newV;
  // Crea el vector k de constantes intermedias del método RungeKutta
  FLOAT **kxX;
  FLOAT **kxY;
  FLOAT **kxZ;
  FLOAT **kvX;
  FLOAT **kvY;
  FLOAT **kvZ;
  
  // Aparta memoria
  newR = allocate2d( n_dim, n_masses );
  newV = allocate2d( n_dim, n_masses );
  kxX = allocate2d( n_masses, 4 );
  kxY = allocate2d( n_masses, 4);
  kxZ = allocate2d( n_masses, 4 );
  kvX = allocate2d( n_masses, 4 );
  kvY = allocate2d( n_masses, 4 );
  kvZ = allocate2d( n_masses, 4 );

  //********************************************************************
  //--------------------------------------------------------------------
  // Cuerpo del RungeKutta
  //--------------------------------------------------------------------
  //********************************************************************

  int i;
  // Actualiza las aceleraciones para el primer paso de RungeKutta
   updateAcc(newR[0],newR[1],newR[2],kvX[0],kvY[0],kvZ[0],m);

  /*
   * Rungekutta 1st step
   */

  for( i = 0; i < n_masses; i++ ){
    
    // RungeKutta 1st step for n_masses
    kxX[0][i] = vX[i];
    kxY[0][i] = vY[i];
    kxZ[0][i] = vZ[i];
    
  }

  // Actualiza las aceleraciones para el segundo paso de RungeKutta
  updateAcc(newR[0],newR[1],newR[2],kvX[1],kvY[1],kvZ[1],m);

  /*
   * Rungekutta 2nd step
   */

  for( i = 0; i < n_masses; i++ ){
    
    // RungeKutta 2nd step for n_masses
    newR[0][i] = xX[i] + kxX[0][i]*dt/2 ;
    newR[1][i] = xY[i] + kxY[0][i]*dt/2 ;
    newR[2][i] = xZ[i] + kxZ[0][i]*dt/2 ;
    newV[0][i] = vX[i] + kvX[0][i]*dt/2 ;
    newV[1][i] = vY[i] + kvY[0][i]*dt/2 ;
    newV[2][i] = vZ[i] + kvZ[0][i]*dt/2 ;
    kxX[1][i] = newV[0][i];
    kxY[1][i] = newV[1][i];
    kxZ[1][i] = newV[2][i];

  }

  // Actualiza las aceleraciones para el tercer paso de RungeKutta
  updateAcc(newR[0],newR[1],newR[2],kvX[2],kvY[2],kvZ[2],m);

  /*
   * Rungekutta 3rd step
   */

  for( i = 0; i < n_masses; i++ ){
    
    // RungeKutta 3rd step for n_masses
    newR[0][i] = xX[i] + kxX[1][i]*dt/2 ;
    newR[1][i] = xY[i] + kxY[1][i]*dt/2 ;
    newR[2][i] = xZ[i] + kxZ[1][i]*dt/2 ;
    newV[0][i] = vX[i] + kvX[1][i]*dt/2 ;
    newV[1][i] = vY[i] + kvY[1][i]*dt/2 ;
    newV[2][i] = vZ[i] + kvZ[1][i]*dt/2 ;
    kxX[2][i] = newV[0][i];
    kxY[2][i] = newV[1][i];
    kxZ[2][i] = newV[2][i];

  }

  // Actualiza las aceleraciones para el cuarto paso de RungeKutta
  updateAcc(newR[0],newR[1],newR[2],kvX[3],kvY[3],kvZ[3],m);

  /*
   * Rungekutta 4th step
   */

  for( i = 0; i < n_masses; i++ ){
    // RungeKutta 4th step for n_masses
    newR[0][i] = xX[i] + kxX[2][i]*dt ;
    newR[1][i] = xY[i] + kxY[2][i]*dt ;
    newR[2][i] = xZ[i] + kxZ[2][i]*dt ;
    newV[0][i] = vX[i] + kvX[2][i]*dt ;
    newV[1][i] = vY[i] + kvY[2][i]*dt ;
    newV[2][i] = vZ[i] + kvZ[2][i]*dt ;
    kxX[3][i] = newV[0][i];
    kxY[3][i] = newV[1][i];
    kxZ[3][i] = newV[2][i];
  }

  //-----------------------------------------------------------------
  //*****************************************************************

  // Promedia los k y actualiza los vectores de entrada en i+1
  for( i = 0; i < n_masses; i++ ){
    // Promedia los ks de n_masses
    // Actualiza los vectores de entrada
    FLOAT kmean = (1/6.0)*(kxX[0][i] + 2*kxX[1][i] + 2*kxX[2][i] + kxX[3][i]);
    xX[i] += kmean*dt;
    kmean = (1/6.0)*(kxY[0][i] + 2*kxY[1][i] + 2*kxY[2][i] + kxY[3][i]);
    xY[i] += kmean*dt;
    kmean = (1/6.0)*(kxZ[0][i] + 2*kxZ[1][i] + 2*kxZ[2][i] + kxZ[3][i]);
    xZ[i] += kmean*dt;
    kmean = (1/6.0)*(kvX[0][i] + 2*kvX[1][i] + 2*kvX[2][i] + kxX[3][i]);
    vX[i] += kmean*dt;
    kmean = (1/6.0)*(kvY[0][i] + 2*kvY[1][i] + 2*kvY[2][i] + kvY[3][i]);
    vY[i] += kmean*dt;
    kmean = (1/6.0)*(kvZ[0][i] + 2*kvZ[1][i] + 2*kvZ[2][i] + kvZ[3][i]);
    vZ[i] += kmean*dt;
  } 
 
}
/*
 * Calcula la energía total del sistema
 */
FLOAT calcEnergy(FLOAT *xX, FLOAT *xY, FLOAT *xZ, FLOAT *vx, FLOAT *vy, FLOAT *vz, FLOAT *m){
  FLOAT ener = 0;
  int i,j;
  for( i = 0; i < n_masses; i++){
    ener += (1/2.0)*m[i]*vx[i]*vx[i];
    ener += (1/2.0)*m[i]*vy[i]*vy[i];
    ener += (1/2.0)*m[i]*vz[i]*vz[i];
    for( j = i+1; j < n_masses; j++){
      FLOAT normi = norm((xX[i] - xX[j]),(xY[i] - xY[j]),(xZ[i] - xZ[j]));
      ener += -G*m[i]*m[j]/normi;
    } 
  }
  return ener;
}



