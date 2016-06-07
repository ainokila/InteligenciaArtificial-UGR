#ifndef PLAYER_H
#define PLAYER_H

#include "environment.h"

class Player{
    public:
      	Player(int jug);
     	Environment::ActionType Think();
     	void Perceive(const Environment &env);


	  	double Valoracion(const Environment &estado, int jugador);

	  	//Metodos propios de la practica -> 
	  	
	 	double alfabeta(Environment entorno , int profundidad ,int &accion, double alpha , double beta );
		double comprueba(const Environment &estado, int jugador);
		double valorConfiguracion(const Environment &estado ,int fila, int columna);

    private:
      int jugador_;
      Environment actual_;
};
#endif
