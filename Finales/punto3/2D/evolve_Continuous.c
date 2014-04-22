#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

/*
 * Define las constantes necesarias
 */

#define PI 3.14159265358979323846
#define FLOAT double
#define manual 'Ejecutar de la forma ./evolve.c N T filename'

// La constante gravitacional en unidades de masa solar, kpc y miles de millones de años
#define G 4.49956*pow(10,-6)
// Numero de dimensiones a trabajar
#define n_dim 2

// Factores de conversion
#define mmyr_to_s 31556926*pow(10,9) 
#define pc_to_m 3.08567758*pow(10,16)

FLOAT norm( FLOAT vec1, FLOAT vec2);
FLOAT *** allocate3d( int x, int y, int z );
FLOAT ** allocate2d( int x, int y );
void rungeKutta4( FLOAT **rm, FLOAT **vm, FLOAT **rM, FLOAT **vM, FLOAT *M, FLOAT dt, int n_masses, int n_Masses );
void updateAcc( FLOAT **rm, FLOAT **rM, FLOAT **am, FLOAT  **aM, FLOAT *M, int n_masses, int n_Masses );


int main( int argc, char **argv){

  /*
   * Define las variables
   */
  // El puntero que contiene los punteros de posicion para cada masa pequeña en cada instante de tiempo
  FLOAT **rm;
  // El puntero que contiene los punteros de velocidad para cada masa pequeña en cada instante de tiempo
  FLOAT **vm;
  // El puntero que contiene los punteros de posicion para cada masa Grande en cada instante de tiempo
  FLOAT **rM;
  // El puntero que contiene los punteros de velocidad para cada masa Grande en cada instante de tiempo
  FLOAT **vM;
  // EL archivo donde se guardaron las condiciones iniciales
  FILE *data;
  // Los archivos donde se guardara la evolucion
  FILE *data_evolve;
  FILE *data_vel;
  // El archivo donde se guardaran los ids
  FILE *data_id;
  // El numero de masas pequeñas
  int n_masses = 0;
  // El numero de masas grandes
  int n_Masses = 0;
  // El vector de masas grandes
  FLOAT *M;
  // El tiempo total de integracion (en años)
  FLOAT T;
  // El numero de iteraciones que se quiere sobre ese tiempo
  int N;
  // El nombre del archivo
  char *name;
  // Variable de iteracion
  int i;
  /*
   * Inicializa las variables que pueden ser inicializadas
   */
  N = atoi( argv[1] );
  T = atof( argv[2] );
  // El intervalo de tiempo sobre integracion
  FLOAT dt = T/N;
  name =  argv[3];
  
  // El archivo de texto de condiciones iniciales y de evolucion en el tiempo
  data = fopen( name, "r" );
  data_evolve = fopen( "evolve_c.dat", "w" );
  data_id = fopen( "id.dat", "w" );
  data_vel = fopen( "evolve_vel.dat", "w" );

  //-------------------------------------------------------------------------------------------------------------------
  //*******************************************************************************************************************
  
  /*
   * Lee el archivo y determina el numero de masas pequeñas y el numero de masas grandes (para poder apartar memoria)
   */
  FLOAT tmp1;
  int id;
  int test = fscanf(data, "%d %le %le %le %le %le\n", &id, &tmp1, &tmp1, &tmp1, &tmp1, &tmp1);
  if( id < 0 ){

    n_Masses++;
       
  }else{

    n_masses++;

  }
  
  do{
    
    test = fscanf(data, "%d %le %le %le %le %le\n", &id, &tmp1, &tmp1, &tmp1, &tmp1, &tmp1);
    if( id < 0 ){

      n_Masses++;
  
    }else{

      n_masses++;
   
    }

  }while( test!=EOF );
   
  
  // Cierra el archivo y lo vuelve a abrir
  fclose(data);
  fopen(name,"r");
  //*******************************************************************************************************************
  //-------------------------------------------------------------------------------------------------------------------
  
  /*
   * Aparta el espacio en memoria de los punteros
   */

  M = malloc( n_Masses*sizeof(FLOAT) );
  rm = allocate2d( n_masses, n_dim );
  rM = allocate2d( n_Masses, n_dim );
  vm = allocate2d( n_masses, n_dim );
  vM = allocate2d( n_Masses, n_dim );

  
  //-------------------------------------------------------------------------------------------------------------------
  //*******************************************************************************************************************

  /*
   * Lee el archivo de nuevo y asigna los valores iniciales de posicion y velocidad, ademas del vector de masas
   */

  FLOAT *Temp = malloc( 5*sizeof(FLOAT));
  int index = 0;
  int Index = 0;
  test = fscanf(data, "%d %le %le %le %le %le\n", &id, &Temp[0], &Temp[1], &Temp[2], &Temp[3], &Temp[4]);
  if( id < 0 ){
    
    fprintf( data_id, "%d\n", id );
    rM[0][Index] = Temp[0];
    rM[1][Index] = Temp[1];
    vM[0][Index] = Temp[2];
    vM[1][Index] = Temp[3];
    M[Index] = Temp[4];
    Index++;
  
  }else{
    
    fprintf( data_id, "%d\n", id );
    rm[0][index] = Temp[0];
    rm[1][index] = Temp[1];
    vm[0][index] = Temp[2];
    vm[1][index] = Temp[3];
    index++;
    
  }
  
  do{
  
    test = fscanf(data, "%d %le %le %le %le %le\n", &id, &Temp[0], &Temp[1], &Temp[2], &Temp[3], &Temp[4]);
    if( id < 0 ){
      
      fprintf( data_id, "%d\n", id );
      rM[0][Index] = Temp[0];
      rM[1][Index] = Temp[1];
      vM[0][Index] = Temp[2];
      vM[1][Index] = Temp[3];
      M[Index] = Temp[4];
      Index++;
      
    }else{
      
      fprintf( data_id, "%d\n", id );
      rm[0][index] = Temp[0];
      rm[1][index] = Temp[1];
      vm[0][index] = Temp[2];
      vm[1][index] = Temp[3];
      index++;

    }
  }while( test!=EOF );
      
  // Cierra el archivo
  free(Temp);
  fclose(data);

  //*******************************************************************************************************************
  //-------------------------------------------------------------------------------------------------------------------
  
  /*
   * Imprime la evolucion de los parametros en los archivos
   * Formato: x y z vx vy vz
   */
  //printf("M: %d\nm: %d\nM: %le\n", n_Masses, n_masses, M[0]); 
  
  for( i = 0; i < n_Masses; i++ ){
      
    // Escribe las posiciones y velocidades de n_masses
    fprintf( data_evolve, "%le %le ", rM[0][i], rM[1][i] ); 
    fprintf( data_vel, "%le %le ", vM[0][i], vM[1][i] ); 
  }
    
  for( i = 0; i < n_masses - 1; i++ ){
    
    // Escribe las posiciones y velocidades de n_Mmasses
    fprintf( data_evolve, "%le %le ", rm[0][i], rm[1][i] );
    fprintf( data_vel, "%le %le ", vm[0][i], vm[1][i] ); 

  }
  i = n_masses-1;
  fprintf( data_evolve, "%le %le\n", rm[0][i], rm[1][i] ); 
  fprintf( data_vel, "%le %le\n", vm[0][i], vm[1][i] );
      
  /*
   *  Método Runge-Kutta 4to orden sobre el equiespaciado temporal
   */
  int j;
  for( j = 0; j < N; j++ ){
    rungeKutta4( rm, vm, rM, vM, M, dt, n_masses, n_Masses );
      
    /*
     * Imprime la evolucion de los parametros en los archivos
     * Formato: x y z vx vy vz
     */
    
    for( i = 0; i < n_Masses; i++ ){
      
      // Escribe las posiciones y velocidades de n_masses
      fprintf( data_evolve, "%le %le ", rM[0][i], rM[1][i] ); 
      fprintf( data_vel, "%le %le ", vM[0][i], vM[1][i] ); 
    }
    
    for( i = 0; i < n_masses - 1; i++ ){
    
      // Escribe las posiciones y velocidades de n_Mmasses
      fprintf( data_evolve, "%le %le ", rm[0][i], rm[1][i] );
      fprintf( data_vel, "%le %le ", vm[0][i], vm[1][i] ); 

    }
    
    i = n_masses-1;
    fprintf( data_evolve, "%le %le\n", rm[0][i], rm[1][i] ); 
    fprintf( data_vel, "%le %le\n", vm[0][i], vm[1][i] );

  }
  
  fclose(data_evolve);
  fclose(data_vel);
  fclose(data_id);
  free(M);
  free(rm);
  free(rM);
  free(vm);
  free(vM);
  return 0;

}

//**************************************************** 
//---------------------------------------------------
// Metodos
//---------------------------------------------------
//****************************************************

/*
 * Calcula el producto punto de 2 vectores 3d
 */

FLOAT norm( FLOAT vec1, FLOAT vec2){
  return sqrt(vec1*vec1 + vec2*vec2);
}

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
  for( i = 0; i < n_Masses; i++ ){
    aM[0][i] = 0;
    aM[1][i] = 0;
    for( j = 0; j < n_Masses; j++ ){
      if( i != j ){
	// Actualizar las aceleraciones de cada Masa grande
	FLOAT normi = pow(norm((rM[0][i] - rM[0][j]),(rM[1][i] - rM[1][j])),3);
	FLOAT grav = (G*M[j])/normi;
	aM[0][i] += grav*(rM[0][j] - rM[0][i]);
	aM[1][i] += grav*(rM[1][j] - rM[1][i]);
      }
    }
  }

  for( i = 0; i < n_masses; i++ ){
    am[0][i] = 0;
    am[1][i] = 0;
    for( j = 0; j < n_Masses; j++ ){
      // Actualizar las aceleraciones de cada Masa pequeña
      FLOAT normi = pow(norm((rm[0][i] - rM[0][j]),(rm[1][i] - rM[1][j])),3) + pow(10,-1);
      FLOAT grav = (G*M[j])/normi;
      am[0][i] += grav*(rM[0][j] - rm[0][i]);
      am[1][i] += grav*(rM[1][j] - rm[1][i]);
      //printf("%le %le ",am[0][i], am[1][i]);
    }
  }
}

/*
 * Método de Runge-Kutta 4to orden aplicado
 */

void rungeKutta4( FLOAT **rm, FLOAT **vm, FLOAT **rM, FLOAT **vM, FLOAT *M, FLOAT dt, int n_masses, int n_Masses ){
  // Crea los vectores de aceleracion, posicion y velodidad temporales
  FLOAT **newRm;
  FLOAT **newRM;
  // Crea el vector k de constantes intermedias del método RungeKutta
  FLOAT ***krm;
  FLOAT ***krM;
  FLOAT ***kvm;
  FLOAT ***kvM;
  // Aparta memoria
  newRm = allocate2d( n_masses, n_dim );
  newRM = allocate2d( n_Masses, n_dim );
  krm = allocate3d( n_masses, n_dim, 4 );
  krM = allocate3d( n_Masses, n_dim, 4 );
  kvm = allocate3d( n_masses, n_dim, 4 );
  kvM = allocate3d( n_Masses, n_dim, 4 );

  //********************************************************************
  //--------------------------------------------------------------------
  // Cuerpo del RungeKutta
  //--------------------------------------------------------------------
  //********************************************************************

  int i;

  /*
   * Rungekutta 1st step
   */

  for( i = 0; i < n_Masses; i++ ){

    // RungeKutta 1st step for n_Masses
    krM[0][0][i] = vM[0][i];
    krM[0][1][i] = vM[1][i];

  }

  for( i = 0; i < n_masses; i++ ){
   
    // RungeKutta 1st step for n_masses
    krm[0][0][i] = vm[0][i];
    krm[0][1][i] = vm[1][i];

  }
  
  // Actualiza las aceleraciones para el primer paso de RungeKutta
  updateAcc( rm, rM, kvm[0], kvM[0], M, n_masses, n_Masses );

  /*
   * Rungekutta 2nd step
   */

  for( i = 0; i < n_Masses; i++ ){
    // RungeKutta 2nd step for n_Masses
    
    newRM[0][i] = rM[0][i] + krM[0][0][i]*dt/2 ;
    newRM[1][i] = rM[1][i] + krM[0][1][i]*dt/2 ;
    krM[1][0][i]  = vM[0][i] + kvM[0][0][i]*dt/2 ;
    krM[1][1][i]  = vM[1][i] + kvM[0][1][i]*dt/2 ;

  }

  for( i = 0; i < n_masses; i++ ){
    
    // RungeKutta 2nd step for n_masses
    newRm[0][i] = rm[0][i] + krm[0][0][i]*dt/2 ;
    newRm[1][i] = rm[1][i] + krm[0][1][i]*dt/2 ;
    krm[1][0][i]  = vm[0][i] + kvm[0][0][i]*dt/2 ;
    krm[1][1][i]  = vm[1][i] + kvm[0][1][i]*dt/2 ;
  
  }

  // Actualiza las aceleraciones para el segundo paso de RungeKutta
  updateAcc( newRm, newRM, kvm[1], kvM[1], M, n_masses, n_Masses );
 
  /*
   * Rungekutta 3rd step
   */

  for( i = 0; i < n_Masses; i++ ){
    
    // RungeKutta 3rd step for n_Masses
    newRM[0][i] = rM[0][i] + krM[1][0][i]*dt/2 ;
    newRM[1][i] = rM[1][i] + krM[1][1][i]*dt/2 ;
    krM[2][0][i]  = vM[0][i] + kvM[1][0][i]*dt/2 ;
    krM[2][1][i]  = vM[1][i] + kvM[1][1][i]*dt/2 ;
 
  }

  for( i = 0; i < n_masses; i++ ){
    
    // RungeKutta 3rd step for n_masses
    newRm[0][i] = rm[0][i] + krm[1][0][i]*dt/2 ;
    newRm[1][i] = rm[1][i] + krm[1][1][i]*dt/2 ;
    krm[2][0][i]  = vm[0][i] + kvm[1][0][i]*dt/2 ;
    krm[2][1][i]  = vm[1][i] + kvm[1][1][i]*dt/2 ;

  }
 
  // Actualiza las aceleraciones para el segundo paso de RungeKutta
  updateAcc( newRm, newRM, kvm[2], kvM[2], M, n_masses, n_Masses );
 
  /*
   * Rungekutta 4th step
   */

  for( i = 0; i < n_Masses; i++ ){
   
    // RungeKutta 4th step for n_Masses
    newRM[0][i] = rM[0][i] + krM[2][0][i]*dt ;
    newRM[1][i] = rM[1][i] + krM[2][1][i]*dt ;
    krM[3][0][i]  = vM[0][i] + kvM[2][0][i]*dt ;
    krM[3][1][i]  = vM[1][i] + kvM[2][1][i]*dt ;

  }

  for( i = 0; i < n_masses; i++ ){
   
    // RungeKutta 4th step for n_masses
    newRm[0][i] = rm[0][i] + krm[2][0][i]*dt ;
    newRm[1][i] = rm[1][i] + krm[2][1][i]*dt ;
    krm[3][0][i]  = vm[0][i] + kvm[2][0][i]*dt ;
    krm[3][1][i]  = vm[1][i] + kvm[2][1][i]*dt ;
 
  }

  // Actualiza las aceleraciones para el segundo paso de RungeKutta
  updateAcc( newRm, newRM, kvm[3], kvM[3], M, n_masses, n_Masses );
 
  //-----------------------------------------------------------------
  //*****************************************************************

  // Promedia los k y actualiza los vectores de entrada en i+1
  
  for( i = 0; i < n_Masses; i++ ){
  
    // Promedia los ks de n_Masses
    // Actualiza los vectores de entrada
    FLOAT kmean = (1/6.0)*(krM[0][0][i] + 2*krM[1][0][i] + 2*krM[2][0][i] + krM[3][0][i]);
    rM[0][i] = rM[0][i] + kmean*dt;
    kmean = (1/6.0)*(krM[0][1][i] + 2*krM[1][1][i] + 2*krM[2][1][i] + krM[3][1][i]);
    rM[1][i] = rM[1][i] + kmean*dt;
    kmean = (1/6.0)*(kvM[0][0][i] + 2*kvM[1][0][i] + 2*kvM[2][0][i] + kvM[3][0][i]);
    vM[0][i] = vM[0][i] + kmean*dt;
    kmean = (1/6.0)*(kvM[0][1][i] + 2*kvM[1][1][i] + 2*kvM[2][1][i] + kvM[3][1][i]);
    vM[1][i] = vM[1][i] + kmean*dt;
  
  }

  for( i = 0; i < n_masses; i++ ){

    // Promedia los ks de n_masses
    // Actualiza los vectores de entrada
    FLOAT kmean = (1/6.0)*(krm[0][0][i] + 2*krm[1][0][i] + 2*krm[2][0][i] + krm[3][0][i]);
    rm[0][i] = rm[0][i] + kmean*dt;
    kmean = (1/6.0)*(krm[0][1][i] + 2*krm[1][1][i] + 2*krm[2][1][i] + krm[3][1][i]);
    rm[1][i] = rm[1][i] + kmean*dt;
    kmean = (1/6.0)*(kvm[0][0][i] + 2*kvm[1][0][i] + 2*kvm[2][0][i] + kvm[3][0][i]);
    vm[0][i] = vm[0][i] + kmean*dt;
    kmean = (1/6.0)*(kvm[0][1][i] + 2*kvm[1][1][i] + 2*kvm[2][1][i] + kvm[3][1][i]);
    vm[1][i] = vm[1][i] + kmean*dt;

  }

  // Libera memoria
  int q,w;
  for(q = 0; q < 4; q++){
    for( w = 0; w < 2; w++){
      free(krm[q][w]);
      free(kvm[q][w]);
    }
    free(krm[q]);
    free(kvm[q]);
  }

  for( w = 0; w < 2; w++){
    free(newRM[w]); 
  }
  
  for( w = 0; w < 2; w++){
    free(newRm[w]); 
  }

  for(q = 0; q < 4; q++){
    for( w = 0; w < 2; w++){
      free(krM[q][w]);
      free(kvM[q][w]);
    }
    free(krM[q]);
    free(kvM[q]);
  }
 

  free(krm);
  free(krM);
  free(kvm);
  free(kvM);
}
