#include <iostream>
#include <cstdlib>
#include <vector>
#include <queue>
#include "player.h"
#include "environment.h"

#define PROF 8

using namespace std;

const double masinf=9999999999.0, menosinf=-9999999999.0;

// Constructor
Player::Player(int jug){
    jugador_=jug;
}

// Actualiza el estado del juego para el jugador
void Player::Perceive(const Environment & env){
    actual_=env;
}

double Puntuacion(int jugador, const Environment &estado){
    double suma=0;

    for (int i=0; i<7; i++)
      for (int j=0; j<7; j++){
         if (estado.See_Casilla(i,j)==jugador){
            if (j<3)
               suma += j;
            else
               suma += (6-j);
         }
      }

    return suma;
}


// Funcion de valoracion para testear Poda Alfabeta
double ValoracionTest(const Environment &estado, int jugador){
    int ganador = estado.RevisarTablero();

    if (ganador==jugador)
       return 99999999.0; // Gana el jugador que pide la valoracion
    else if (ganador!=0)
            return -99999999.0; // Pierde el jugador que pide la valoracion
    else if (estado.Get_Casillas_Libres()==0)
            return 0;  // Hay un empate global y se ha rellenado completamente el tablero
    else
          return Puntuacion(jugador,estado);
}

// ------------------- Los tres metodos anteriores no se pueden modificar

double Player::valorConfiguracion(const Environment &estado ,int fila, int columna){

    if (estado.See_Casilla(fila,columna)==0)
        return 0.0;

    //Comprueba 4 en la fila 
    if (columna<4 && estado.See_Casilla(fila,columna) ){
      if ((estado.See_Casilla(fila,columna))   == (estado.See_Casilla(fila,columna+1)) and
          (estado.See_Casilla(fila,columna+1)) == (estado.See_Casilla(fila,columna+2)) and
          (estado.See_Casilla(fila,columna+2)) == (estado.See_Casilla(fila,columna+3))){
         

          if(estado.See_Casilla(fila,columna) == jugador_){
              return -999999.0;
          }else{
              return 999999.0;
          }
          
      }
    }

    //Comprueba 4 en la columna
        if (fila<4){
      if ((estado.See_Casilla(fila,columna))   == (estado.See_Casilla(fila+1,columna)) and
          (estado.See_Casilla(fila+1,columna)) == (estado.See_Casilla(fila+2,columna)) and
          (estado.See_Casilla(fila+2,columna)) == (estado.See_Casilla(fila+3,columna))){
          
          if(estado.See_Casilla(fila,columna) == jugador_){
              return -999999.0;
          }else{
              return 999999.0;
          };
      }
    }

    //Comprueba 4 en una diagonal
    if (fila<4 and columna<4){
      if ((estado.See_Casilla(fila,columna))     == (estado.See_Casilla(fila+1,columna+1)) and
          (estado.See_Casilla(fila+1,columna+1)) == (estado.See_Casilla(fila+2,columna+2)) and
          (estado.See_Casilla(fila+2,columna+2)) == (estado.See_Casilla(fila+3,columna+3))){

          if(estado.See_Casilla(fila,columna) == jugador_){
              return -999999.0;
          }else{
              return 999999.0;
          };
      }
    }

    
    //Comprueba 4 en otra diagonal
    if (fila>2 and columna<4){
      if ((estado.See_Casilla(fila,columna))   == (estado.See_Casilla(fila-1,columna+1)) and
          (estado.See_Casilla(fila-1,columna+1)) == (estado.See_Casilla(fila-2,columna+2)) and
          (estado.See_Casilla(fila-2,columna+2)) == (estado.See_Casilla(fila-3,columna+3))){

          if(estado.See_Casilla(fila,columna) == jugador_){
              return -999999.0;
          }else{
              return 999999.0;
          };
      }
    }

	//Comprueba 3 en la fila 
    if (columna<5 && estado.See_Casilla(fila,columna) ){
      if ((estado.See_Casilla(fila,columna))   == (estado.See_Casilla(fila,columna+1)) and
          (estado.See_Casilla(fila,columna+1)) == (estado.See_Casilla(fila,columna+2))){

          if(estado.See_Casilla(fila,columna) == jugador_){
              return -99.0;
          }else{
              return 99.0;
          }
          
      }
    }
    
	//Comprueba 3 en la columna
    if (fila<5){
      if ((estado.See_Casilla(fila,columna))   == (estado.See_Casilla(fila+1,columna)) and
          (estado.See_Casilla(fila+1,columna)) == (estado.See_Casilla(fila+2,columna))){
          
          if(estado.See_Casilla(fila,columna) == jugador_){
              return -99.0;
          }else{
              return 99.0;
          };
      }
    }
    
    //Comprueba 3 en la diagonal
    if (fila<5 and columna<5){
      if ((estado.See_Casilla(fila,columna))     == (estado.See_Casilla(fila+1,columna+1)) and
          (estado.See_Casilla(fila+1,columna+1)) == (estado.See_Casilla(fila+2,columna+2))){

          if(estado.See_Casilla(fila,columna) == jugador_){
              return -99.0;
          }else{
              return 99.0;
          };
      }
    }

    //Comprueba 3 en la diagonal
    if (fila>1 and columna<5){
      if ((estado.See_Casilla(fila,columna))   == (estado.See_Casilla(fila-1,columna+1)) and
          (estado.See_Casilla(fila-1,columna+1)) == (estado.See_Casilla(fila-2,columna+2))){

          if(estado.See_Casilla(fila,columna) == jugador_){
              return -99.0;
          }else{
              return 99.0;
          };
      }
    }


	//Comprueba 2 en la fila 
    if (columna<6 && estado.See_Casilla(fila,columna) ){
      if ((estado.See_Casilla(fila,columna))   == (estado.See_Casilla(fila,columna+1))){

          if(estado.See_Casilla(fila,columna) == jugador_){
              return -9.0;
          }else{
              return 9.0;
          }
          
      }
    }
    //Comprueba 2 en la columna
    if (fila<6){
      if ((estado.See_Casilla(fila,columna))   == (estado.See_Casilla(fila+1,columna))){
          
          if(estado.See_Casilla(fila,columna) == jugador_){
              return -9.0;
          }else{
              return 9.0;
          };
      }
    }
    //Comprueba 2 en diagonal
    if (fila<6 and columna<6){
      if ((estado.See_Casilla(fila,columna))     == (estado.See_Casilla(fila+1,columna+1))){

          if(estado.See_Casilla(fila,columna) == jugador_){
              return -9.0;
          }else{
              return 9.0;
          };
      }
    }

    //Comprueba 2 en diagonal
    if (fila>0 and columna<6){
      if ((estado.See_Casilla(fila,columna))   == (estado.See_Casilla(fila-1,columna+1))){

          if(estado.See_Casilla(fila,columna) == jugador_){
              return -9.0;
          }else{
              return 9.0;
          };
      }
    }

    return 0.0;

}




double Player::comprueba(const Environment &estado, int jugador){

	/*
	double suma = 0;

	for(int i = 0 ; i < 7 ; i++){
		suma = compruebaDosColumna(estado, jugador,i) + compruebaDosFila(estado, jugador,i) + compruebaTresColumna(estado, jugador,i) +compruebaTresFila(estado, jugador,i);
	}
	return suma + sumaContiguasVert(estado , jugador) + sumaContiguasHor(estado , jugador) + sumaContiguasHor(estado,jugador);*/

int ganador = estado.RevisarTablero();

    if (ganador==jugador)
       return 99999999.0; // Gana el jugador que pide la valoracion
    else if (ganador!=0)
            return -99999999.0; // Pierde el jugador que pide la valoracion
    else if (estado.Get_Casillas_Libres()==0)
            return 0;  // Hay un empate global y se ha rellenado completamente el tablero
    else{

		double suma = valorConfiguracion(estado,0,0);
		int ganador;

		for (int i=0; i<7; i++)
	      for (int j=0;j<7; j++){

	      	if(jugador == jugador_){
	          if (estado.See_Casilla(i,j)!=0 && suma > valorConfiguracion(estado,i,j) )
	            suma = valorConfiguracion(estado,i,j);
	      	}else{
	      		if (estado.See_Casilla(i,j)!=0 && suma < valorConfiguracion(estado,i,j) )
	             suma = valorConfiguracion(estado,i,j);
	      	}

	      }

    	return suma;
    }


}


double Player::alfabeta(Environment entorno , int profundidad ,int &accion, double alpha , double beta ){

	//cout << " Accion -> " << accion << endl;
	Environment nuevo ; 
	double mejorValor;
	int auxiliar = -1;

	if(profundidad == 0 || entorno.JuegoTerminado()) {

		if(entorno.JuegoTerminado()){
			return ValoracionTest(entorno,jugador_);
		}
		return comprueba(entorno,jugador_);

	}

	if(entorno.JugadorActivo() == jugador_ ){

		nuevo = entorno.GenerateNextMove(auxiliar);
		mejorValor = alpha;

		while(auxiliar<8){
			int otra = auxiliar;

			double valorHijo = alfabeta(nuevo , profundidad -1 ,otra, mejorValor , beta);
			
			//cout << "Mejor valor Max " << mejorValor << " y valor hijo " << valorHijo << " a prof. " << profundidad << endl; 

			if(valorHijo > mejorValor && auxiliar != -1 && profundidad == PROF){
       		 cout << "Mov . " << auxiliar << " con  " << valorHijo << " puntos \n";
				accion = auxiliar;

			}

			mejorValor = max( mejorValor , valorHijo);

			if (beta <= mejorValor) {
                break;
            }
			nuevo = entorno.GenerateNextMove(auxiliar);
			

		}

	}else{

		nuevo = entorno.GenerateNextMove(auxiliar);
		mejorValor = beta;

		while(auxiliar<8){
			int otra = auxiliar;

			double valorHijo = alfabeta(nuevo , profundidad -1 ,otra, alpha , mejorValor);
			
			//cout << "Mejor valor Min " << mejorValor << "y valor hijo " << valorHijo << " a prof. " << profundidad << endl;

			if(valorHijo < mejorValor && auxiliar != -1 && profundidad == PROF){
        		cout << "Mov . " << auxiliar << " con  " << valorHijo << " puntos \n";
				accion = auxiliar;
			}

			mejorValor = min( mejorValor , valorHijo);
            if (mejorValor <= alpha) {
                break;
            }
			nuevo = entorno.GenerateNextMove(auxiliar);

		}

	}

	return mejorValor;





}

// Funcion heuristica (ESTA ES LA QUE TENEIS QUE MODIFICAR)
double Player::Valoracion(const Environment &estado, int jugador){
	return Puntuacion(jugador, estado);
}





// Esta funcion no se puede usar en la version entregable
// Aparece aqui solo para ILUSTRAR el comportamiento del juego
// ESTO NO IMPLEMENTA NI MINIMAX, NI PODA ALFABETA
void JuegoAleatorio(bool aplicables[], int opciones[], int &j){
    j=0;
    for (int i=0; i<8; i++){
        if (aplicables[i]){
           opciones[j]=i;
           j++;
        }
    }
}






// Invoca el siguiente movimiento del jugador
Environment::ActionType Player::Think(){
    const int PROFUNDIDAD_MINIMAX = 6;  // Umbral maximo de profundidad para el metodo MiniMax
    const int PROFUNDIDAD_ALFABETA = 8; // Umbral maximo de profundidad para la poda Alfa_Beta

    Environment::ActionType accion; // acción que se va a devolver

    bool aplicables[8]; // Vector bool usado para obtener las acciones que son aplicables en el estado actual. La interpretacion es
                        // aplicables[0]==true si PUT1 es aplicable
                        // aplicables[1]==true si PUT2 es aplicable
                        // aplicables[2]==true si PUT3 es aplicable
                        // aplicables[3]==true si PUT4 es aplicable
                        // aplicables[4]==true si PUT5 es aplicable
                        // aplicables[5]==true si PUT6 es aplicable
                        // aplicables[6]==true si PUT7 es aplicable
                        // aplicables[7]==true si BOOM es aplicable



    double valor; // Almacena el valor con el que se etiqueta el estado tras el proceso de busqueda.
    double alpha = -10000000000000.0, beta= 10000000000000.0; // Cotas de la poda AlfaBeta

    int n_act; //Acciones posibles en el estado actual


    n_act = actual_.possible_actions(aplicables); // Obtengo las acciones aplicables al estado actual en "aplicables"
    int opciones[10];

    // Muestra por la consola las acciones aplicable para el jugador activo
    //actual_.PintaTablero();
    cout << " Acciones aplicables ";
    (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
    for (int t=0; t<8; t++)
      if (aplicables[t])
         cout << " " << actual_.ActionStr( static_cast< Environment::ActionType > (t)  );
    cout << endl;


    //--------------------- COMENTAR Desde aqui 
/*
    cout << "\n\t";
    int n_opciones=0;
    JuegoAleatorio(aplicables, opciones, n_opciones);

    if (n_act==0){
      (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
      cout << " No puede realizar ninguna accion!!!\n";
      //accion = Environment::actIDLE;
    }
    else if (n_act==1){
           (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
            cout << " Solo se puede realizar la accion "
                 << actual_.ActionStr( static_cast< Environment::ActionType > (opciones[0])  ) << endl;
            accion = static_cast< Environment::ActionType > (opciones[0]);

         }
         else { // Hay que elegir entre varias posibles acciones
            int aleatorio = rand()%n_opciones;
            cout << " -> " << actual_.ActionStr( static_cast< Environment::ActionType > (opciones[aleatorio])  ) << endl;
            accion = static_cast< Environment::ActionType > (opciones[aleatorio]);
         }
*/
    //--------------------- COMENTAR Hasta aqui




    //--------------------- AQUI EMPIEZA LA PARTE A REALIZAR POR EL ALUMNO ------------------------------------------------


    // Opcion: Poda AlfaBeta
    // NOTA: La parametrizacion es solo orientativa
//alfabeta(Environment entorno , int profundidad ,int &accion, double alpha , double beta )
	int accionAuxiliar = -1;
    valor = alfabeta(actual_,PROF, accionAuxiliar, menosinf, masinf);
    //cout << "Valor MiniMax: " << valor << "  Accion: " << actual_.ActionStr(accion) << endl;

	cout << accionAuxiliar;

	switch (accionAuxiliar)
	{
	case 0: return Environment::PUT1;
	case 1: return Environment::PUT2;
	case 2: return Environment::PUT3;
	case 3: return Environment::PUT4;
	case 4: return Environment::PUT5;
	case 5: return Environment::PUT6;
	case 6: return Environment::PUT7;
	case 7: return Environment::BOOM;

	}

    return accion;
}

