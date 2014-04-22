#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define FLOAT float

  /*
    Ecuacion diferencial: dx/dt
  */
FLOAT xprime(FLOAT x, FLOAT y );


  /*
    Ecuacion diferencial: dy/dt
  */
FLOAT yprime(FLOAT x, FLOAT y);


  /*
    Runge-Kutta: cuarto orden
  */
void RK4(FLOAT *vecx, FLOAT *vecy, FLOAT h);


/*----------------------- MAIN-----------------------*/

int main(int argc, char **argv){

  /*
    Definicion de estructuras
  */

  int i = 1;
  int j = 0;
  FLOAT h = atof(argv[3]);
  //printf("%f\n",h);
  int n_points = (int) (1)/h;
  FLOAT* x_t = malloc(sizeof(FLOAT)*30);
  FLOAT* y_t = malloc(sizeof(FLOAT)*30);

  FLOAT x_0 = atof(argv[1]);
  FLOAT y_0 = atof(argv[2]);

  FILE* data;

  x_t[0] = x_0;
  y_t[0] = y_0;

  data = fopen("datass.dat", "w");
    //printf("%d\n", n_points);

  for( i = 0; i < 30; i++){
    x_t[i] = 30-i;
    y_t[i] = y_0;
  }
   
  for(j=0;j<29;j++){
	fprintf(data, "%f %f ", x_t[j], y_t[j]);
      }

      fprintf(data, "%f %f\n", x_t[j], y_t[j]);

  /*
    Resolucion de la ecuacion diferencial:
    4th order Runge-Kutta method
  */

    for(i=0; i<n_points; i++){

      printf("%f\n", h);

      RK4(x_t, y_t, h);

      /* Impresion del archivo */

      for(j=0;j<29;j++){
	fprintf(data, "%f %f ", x_t[j], y_t[j]);
      }

      fprintf(data, "%f %f\n", x_t[j], y_t[j]);

    }
    fclose(data);
    x_0--;

  return 0;

}
/*----------------------------------------------------*/


  /*
    Definicion de las funciones
  */

FLOAT xprime(FLOAT x, FLOAT y){

  FLOAT newX;
  newX = 20*x-(x*y);

  return newX;

  }

FLOAT yprime(FLOAT x, FLOAT y){

  FLOAT newY;
  newY= x*y-30*y;

  return newY;

}

void RK4(FLOAT *vecx, FLOAT *vecy, FLOAT h){

  int i = 0;

  for(i=0; i<30; i++){

    FLOAT x = vecx[i];
    FLOAT y = vecy[i];
    FLOAT k_1_x = yprime(x, y);
    FLOAT k_1_y = xprime(x, y);
    
    // first step
    
    FLOAT x_1 = x + (h/2.0) * k_1_x;
    FLOAT y_1 = y + (h/2.0) * k_1_y;
    FLOAT k_2_x = xprime(x_1, y_1);
    FLOAT k_2_y = yprime(x_1, y_1);
    
    // second step
    FLOAT x_2 = x + (h/2.0) * k_2_x;
    FLOAT y_2 = y + (h/2.0) * k_2_y;
    FLOAT k_3_x = xprime(x_2, y_2);
    FLOAT k_3_y = yprime(x_2, y_2);
        
    // third step
    FLOAT x_3 = x + h * k_3_x;
    FLOAT y_3 = y + h * k_3_y;
    FLOAT k_4_x = xprime(x_3, y_3);
    FLOAT k_4_y = yprime(x_3, y_3);
    
    // fourth step
    FLOAT average_k_x = (1.0/6.0)*(k_1_x + 2.0*k_2_x + 2.0*k_3_x + k_4_x);
    FLOAT average_k_y = (1.0/6.0)*(k_1_y + 2.0*k_2_y + 2.0*k_3_y + k_4_y);
    
    vecx[i] += (h * average_k_x);
    vecy[i] += (h * average_k_y);

  }

}
