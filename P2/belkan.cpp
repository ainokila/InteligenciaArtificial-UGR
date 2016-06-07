#include "belkan.h"
#include "conexion.h"
#include "environment.h"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <utility>
#include <cstring>
#include <string>

using namespace std;



// -----------------------------------------------------------
void PasarVectoraMapaCaracteres(int fila, int columna, char m[200][200], char *v, int brujula){
  m[fila][columna]=v[0];

    switch(brujula){
        case 0: // Orientacion Norte

		m[fila-1][columna]=v[1];
		for (int i=0; i<3; i++){
		  m[fila-2][columna+i-1]=v[2+i];
		}
		for (int j=0; j<5; j++){
		  m[fila-3][columna+j-2]=v[5+j];
		}
	        break;
	case 1: // Orientacion Este
		m[fila][columna+1]=v[1];
		for (int i=0; i<3; i++){
		  m[fila+i-1][columna+2]=v[2+i];
		}
		for (int j=0; j<5; j++){
		  m[fila+j-2][columna+3]=v[5+j];
		}
	        break;
        case 2: // Orientacion Sur
		m[fila+1][columna]=v[1];
		for (int i=0; i<3; i++){
		  m[fila+2][columna+1-i]=v[2+i];
		}
		for (int j=0; j<5; j++){
		  m[fila+3][columna+2-j]=v[5+j];
		}
		                break;
        case 3: // Orientacion Oeste
		m[fila][columna-1]=v[1];
		for (int i=0; i<3; i++){
		  m[fila+1-i][columna-2]=v[2+i];
		}
		for (int j=0; j<5; j++){
		  m[fila+2-j][columna-3]=v[5+j];
		}

                break;
    }

}


// -----------------------------------------------------------
void Agent::Perceive(Environment &env)
{
	env.SenSorStatus(VISTA_, SURFACE_, MENSAJE_, REINICIADO_, EN_USO_, MOCHILLA_, PUNTUACION_, FIN_JUEGO_, SALUD_, false);

}


bool Agent::Perceive_Remote(conexion_client &Cliente, Environment &env)
{
	bool actualizado=false;


	actualizado = env.Perceive_Remote(Cliente);
	if (actualizado)
		env.SenSorStatus(VISTA_, SURFACE_, MENSAJE_, REINICIADO_, EN_USO_, MOCHILLA_, PUNTUACION_, FIN_JUEGO_, SALUD_, true);

    return actualizado;
}


// -----------------------------------------------------------
string ActionStr(Agent::ActionType accion)
{
	switch (accion)
	{
	case Agent::actFORWARD: return "FORWARD";
	case Agent::actTURN_L: return "TURN LEFT";
	case Agent::actTURN_R: return "TURN RIGHT";
	case Agent::actIDLE: return "IDLE";
	case Agent::actPICKUP: return "PICK UP";
	case Agent::actPUTDOWN: return "PUT DOWN";
	case Agent::actPUSH: return "PUSH";
	case Agent::actPOP: return "POP";
	case Agent::actGIVE: return "GIVE";
	case Agent::actTHROW: return "THROW";
	default: return "????";
	}
}

// -----------------------------------------------------------
void Agent::ActualizarInformacion(Environment *env){
	// Actualizar mi informacion interna

	if (REINICIADO_){
 		x_ = 99;
		y_ = 99;
		orientacion_ = 3;
		for (int i=0;i<200;i++)
		    for(int j=0;j<200;j++){

		      mapa_entorno_[i][j]='?';
		      mapa_objetos_[i][j]='?';
			  mapa_pulgarcito[i][j]= -1; //Mapa para la tecnica de PULGARCITO!

		    }

		primerK = false;
		segK = false;
	}

	//COMPROBAMOS SI ESTAMOS EN ALGUN PUNTO K
	

	switch(last_accion_){
	  case 0: //avanzar
	  switch(orientacion_){
	    case 0: // norte
		    y_--;
		    cambiaXY0();
		    break;
	    case 1: // este
		    x_++;
		    cambiaXY1();
		    break;
	    case 2: // sur
		    y_++;
		    cambiaXY2();

		    break;
	    case 3: // este
		    x_--;
		    cambiaXY3();
		    break;
	  }
	  break;
	  case 1: // girar izq
		  orientacion_=(orientacion_+3)%4;
		  break;
	  case 2: // girar dch
		  orientacion_=(orientacion_+1)%4;
		  break;
	}


	calcularOrientacion();
	nEjecucion++;
	if(segK && primerK){
		mapa_solucion_ [y][x] = mapa_entorno_[y_][x_] ;

		//Trasnposicion de posiciones malas a reales dependiendo de la orientacion del mapa 

		switch(orientacion_mapa){
			case 0:
				mapa_solucion_ [y-1][x] = mapa_entorno_[y_-1][x_] ;	
				mapa_solucion_ [y][x-1] = mapa_entorno_[y_][x_-1] ;	
				mapa_solucion_ [y+1][x] = mapa_entorno_[y_+1][x_] ;	
				mapa_solucion_ [y][x+1] = mapa_entorno_[y_][x_+1] ;

				mapa_solucion_ [y-2][x] = mapa_entorno_[y_-2][x_] ;	
				mapa_solucion_ [y][x-2] = mapa_entorno_[y_][x_-2] ;	
				mapa_solucion_ [y+2][x] = mapa_entorno_[y_+2][x_] ;	
				mapa_solucion_ [y][x+2] = mapa_entorno_[y_][x_+2] ;

				break;
			case 1:
				mapa_solucion_ [y][x-1] = mapa_entorno_[y_-1][x_] ;	
				mapa_solucion_ [y+1][x] = mapa_entorno_[y_][x_-1] ;	
				mapa_solucion_ [y][x+1] = mapa_entorno_[y_+1][x_] ;	
				mapa_solucion_ [y-1][x] = mapa_entorno_[y_][x_+1] ;

				mapa_solucion_ [y][x-2] = mapa_entorno_[y_-2][x_] ;	
				mapa_solucion_ [y+2][x] = mapa_entorno_[y_][x_-2] ;	
				mapa_solucion_ [y][x+2] = mapa_entorno_[y_+2][x_] ;	
				mapa_solucion_ [y-2][x] = mapa_entorno_[y_][x_+2] ;

				break;
			case 2:
				mapa_solucion_ [y+1][x] = mapa_entorno_[y_-1][x_] ;	
				mapa_solucion_ [y][x+1] = mapa_entorno_[y_][x_-1] ;	
				mapa_solucion_ [y-1][x] = mapa_entorno_[y_+1][x_] ;	
				mapa_solucion_ [y][x-1] = mapa_entorno_[y_][x_+1] ;

				mapa_solucion_ [y+2][x] = mapa_entorno_[y_-2][x_] ;	
				mapa_solucion_ [y][x+2] = mapa_entorno_[y_][x_-2] ;	
				mapa_solucion_ [y-2][x] = mapa_entorno_[y_+2][x_] ;	
				mapa_solucion_ [y][x-2] = mapa_entorno_[y_][x_+2] ;
				break;
			case 3:
				mapa_solucion_ [y][x+1] = mapa_entorno_[y_-1][x_] ;	
				mapa_solucion_ [y-1][x] = mapa_entorno_[y_][x_-1] ;	
				mapa_solucion_ [y][x-1] = mapa_entorno_[y_+1][x_] ;	
				mapa_solucion_ [y+1][x] = mapa_entorno_[y_][x_+1] ;

				mapa_solucion_ [y][x+2] = mapa_entorno_[y_-2][x_] ;	
				mapa_solucion_ [y-2][x] = mapa_entorno_[y_][x_-2] ;	
				mapa_solucion_ [y][x-2] = mapa_entorno_[y_+2][x_] ;	
				mapa_solucion_ [y+2][x] = mapa_entorno_[y_][x_+2] ;
				break;
		}



	
		//cout << y << " " << x << " " << y_ << " " << x_ << " " << mapa_solucion_ [y][x] << endl << flush ;
	}


	if(nEjecucion == 1800 and orientacion_mapa != -1){
		rellenaMapa();
	}



	// Comprobacion para no salirme del rango del mapa
	bool algo_va_mal=false;

	if (y_<0){
		y_=0;
		algo_va_mal=true;
	} 
	else if (y_>199){
		y_=199;
		algo_va_mal=true;
	}
	if (x_<0){
		x_=0;
		algo_va_mal=true;
	} 
	else if (x_>199){
		x_=199;
		algo_va_mal=true;
	}

	if (algo_va_mal){
		cout << "CUIDADO: NO ESTAS CONTROLANDO BIEN LA UBICACION DE TU AGENTE\n";
	}


	PasarVectoraMapaCaracteres(y_,x_,mapa_entorno_,VISTA_,orientacion_);
	PasarVectoraMapaCaracteres(y_,x_,mapa_objetos_,SURFACE_,orientacion_);

	env->ActualizarMatrizUsuario(mapa_entorno_);

}

	void Agent::cambiaXY0(){
		switch(orientacion_mapa){
		    case 0: //bien puesto
			  	y--;
			    break;
		    case 1: // ->
			 	x--;
			    break;
		    case 2: // \/
			   y++;
			   break;
		    case 3: //<-
				x++;
			    break;
	  }
	}
	void Agent::cambiaXY1(){
		switch(orientacion_mapa){
		    case 0: //bien puesto
			  	x++;
			    break;
		    case 1: // ->
			 	y--;
			    break;
		    case 2: // \/
			   x--;
			   break;
		    case 3: //<-
				y++;
			    break;
	  }
	}
	void Agent::cambiaXY2(){
		switch(orientacion_mapa){
		    case 0: //bien puesto
			  	y++;
			    break;
		    case 1: // ->
			 	x++;
			    break;
		    case 2: // \/
			   y--;
			   break;
		    case 3: //<-
				x--;
			    break;
	  }
	}
	void Agent::cambiaXY3(){

		switch(orientacion_mapa){
		    case 0: //bien puesto
			  	x--;
			    break;
		    case 1: // ->
			 	y++;
			    break;
		    case 2: // \/
			   x++;
			   break;
		    case 3: //<-
				y--;
			    break;
	  }
	}


void Agent::rellenaMapa(){


	for(int i = 0 ; i<100 ; i++){
		mapa_solucion_[i][0] = 'P';
		mapa_solucion_[i][1] = 'P';
		mapa_solucion_[i][2] = 'P';

		mapa_solucion_[i][97] = 'P';
		mapa_solucion_[i][98] = 'P';
		mapa_solucion_[i][99] = 'P';
	}

	for(int i = 0 ; i<100 ; i++){
		mapa_solucion_[0][i] = 'P';
		mapa_solucion_[1][i] = 'P';
		mapa_solucion_[2][i] = 'P';

		mapa_solucion_[97][i] = 'P';
		mapa_solucion_[98][i] = 'P';
		mapa_solucion_[99][i] = 'P';
	}





}

char colorMayoritario(int fila,int columna){


}

void Agent::pintaMapita(){

		//MODIFICAR CON FUNCION RECURSIVA
	for(int i=4; i<97 ; i++){
		for(int j=4; i<97 ; i++){
			if(mapa_solucion_[i][j] == '?')
				mapa_solucion_[i][j] = mapa_solucion_[i-1][j];
		}
	}

}


void Agent::calcularOrientacion(){

	//SI NO ESTA ORIENTADO INTENTAMOS ORIENTARLO
	if( !primerK || !segK || orientacion_mapa == -1 ){

		if(!primerK && VISTA_[0]=='K'){

			fila_1_mala = y_ ;
			columna_1_mala = x_ ;
			
			//OBTENGO POSICIONES REALES
			string aux = MENSAJE_.substr(MENSAJE_.find(':')+1 , MENSAJE_.find('c') - MENSAJE_.find(':') );
			fila_1_buena = atoi(aux.c_str());

			aux = MENSAJE_.substr(MENSAJE_.rfind(':')+1 , MENSAJE_.find('.') - MENSAJE_.rfind(':') );
			columna_1_buena = atoi(aux.c_str());

			primerK = true;
	
			

		}else if ( !segK && VISTA_[0]=='K'){

			fila_2_mala = y_ ;
			columna_2_mala = x_ ;
			
			//OBTENGO POSICIONES REALES
			string aux = MENSAJE_.substr(MENSAJE_.find(':')+1 , MENSAJE_.find('c') - MENSAJE_.find(':') );
			fila_2_buena = atoi(aux.c_str());

			aux = MENSAJE_.substr(MENSAJE_.rfind(':')+1 , MENSAJE_.find('.') - MENSAJE_.rfind(':') );
			columna_2_buena = atoi(aux.c_str());

			//cout << MENSAJE_ << endl;

			if((fila_1_buena != fila_2_buena) || (columna_1_buena != columna_2_buena and ( fila_2_buena != 0 and columna_2_buena != 0 ) )){

				segK = true;
				//cout << "Encontrado segundo k       " << columna_2_buena << "-" <<  fila_2_buena << endl;
				//cout << "Encontrado segundo(mala) k " << columna_2_mala  << "-" << fila_2_mala << endl << endl;

				y=fila_2_buena;
				x=columna_2_buena;

			}

		}

		if(primerK && segK ){

			//--- Ya podemos calcular la orientacion mapa ---
			// --------------------------TIPOS DE ORIENTACION---------------------
			// Orientacion 0 : /\
			// Orientacion 1 : ->
			// Orientacion 2 : \/
			// Orientacion 3 : <-
			// -------------------------------------------------------------------

			int F1 = (fila_1_mala - fila_2_mala  );
			int C1 = (columna_1_mala - columna_2_mala );
			int F2 = (fila_1_buena - fila_2_buena  );
			int C2 = (columna_1_buena - columna_2_buena );

			//cout << "F1 " << F1 << " C1 " << C1 << endl;
			//cout << "F2 " << F2 << " C2 " << C2 << endl;

			if( (F1 == F2) && (C1  == C2)   ){

				orientacion_mapa = 0;
				cout << "Mapa con orientacion 0 " << endl;

			}else if ( (F1 == - F2) && (C1  == -C2)   ) {

				orientacion_mapa = 2;
				cout << "Mapa con orientacion 2 " << endl;
				//gira90grados();
				//gira90grados();
				//cout << "Giro 2 veces"<< endl;
				//orientacion_mapa = 0;
			}else if ( (F1 == -C2) && (F2  == C1)   ){
				orientacion_mapa = 3;
				//gira90grados();
				//orientacion_mapa = 0;
				cout << "Mapa con orientacion 3 " << endl;
				//cout << "Giro 1 veces"<< endl;

			}else if((F1 == C2) && (F2  == -C1)){
				orientacion_mapa = 1;
				cout << "Mapa con orientacion 1 " << endl;
				//gira90grados();
				//gira90grados();
				//gira90grados();
				//cout << "Giro 3 veces"<< endl;
				//orientacion_mapa = 0;

			}else{

				//cerr << " Error orientando el mapa\n" ;
				primerK = false ;
				segK = false ; 
				orientacion_mapa = -1 ;

			}

		}

	}


}

bool Agent::libreParaPasar(int x){
	
	if(VISTA_[1] != 'B' and VISTA_[1] != 'D' and VISTA_[1] != 'A' and VISTA_[1] != 'M' and VISTA_[1] != 'P' and ( SURFACE_[1] < 'a' or SURFACE_[1] > 'z' ) ){
		return true;
	}

	return false;
}

bool Agent::libreParaPasar(int fila,int columna){
	

	switch(EN_USO_){
		case '0':
			if(mapa_entorno_[fila][columna] != 'B' and mapa_entorno_[fila][columna] != 'D' and mapa_entorno_[fila][columna] != 'M' and mapa_entorno_[fila][columna] != 'P' and ( mapa_objetos_[fila][columna] < 'a' or mapa_objetos_[fila][columna] > 'z' ) ){
					return true;
			}
			break;

		case '2':
			if(mapa_entorno_[fila][columna] != 'B'  and mapa_entorno_[fila][columna] != 'A' and mapa_entorno_[fila][columna] != 'M' and mapa_entorno_[fila][columna] != 'P' and ( mapa_objetos_[fila][columna] < 'a' or mapa_objetos_[fila][columna] > 'z' ) ){
					return true;
			}
			break;

		default:
			if(mapa_entorno_[fila][columna] != 'B' and mapa_entorno_[fila][columna] != 'D' and mapa_entorno_[fila][columna] != 'A' and mapa_entorno_[fila][columna] != 'M' and mapa_entorno_[fila][columna] != 'P' and ( mapa_objetos_[fila][columna] < 'a' or mapa_objetos_[fila][columna] > 'z' ) ){
					return true;
			}
			break;
	}

	return false;
}

void Agent::buscaK(){

	// ACCION PARA IR A UN PUNTO K 
	if ( (VISTA_[1] == 'K' or VISTA_[2] == 'K' or VISTA_[3] == 'K' or VISTA_[4] == 'K' )  and movimientos.empty() and orientacion_mapa == -1) {
		
		if( VISTA_[1] == 'K' ){
			movimientos.push(actFORWARD);

		}else if(VISTA_[2] == 'K'){

			if(libreParaPasar(1) and libreParaPasar(3)){

						movimientos.push(actFORWARD);
						movimientos.push(actFORWARD);
						movimientos.push(actTURN_L);
						movimientos.push(actFORWARD);
			}
			
		}else if( VISTA_[3] == 'K' ){

			if(libreParaPasar(1)){

						movimientos.push(actFORWARD);
						movimientos.push(actFORWARD);

			}

		}else if(VISTA_[4] == 'K'){

			if(libreParaPasar(1) and libreParaPasar(3)){

						movimientos.push(actFORWARD);
						movimientos.push(actFORWARD);
						movimientos.push(actTURN_R);
						movimientos.push(actFORWARD);
			}

		}



	}



}

	int Agent::delante(int fila , int columna){
			// y fila x columna

			switch(orientacion_){
			    case 0: // norte
			    	if(!libreParaPasar(fila-1,columna)){
			    		return 25000;
			    	}
				   	return mapa_pulgarcito[fila-1][columna];
				    break;
			    case 1: // este
			    	if(!libreParaPasar(fila,columna+1)){
			    		return 25000;
			    	}
				    return mapa_pulgarcito[fila][columna+1];
				    break;
			    case 2: // sur
			    	if(!libreParaPasar(fila+1,columna)){
			    		return 25000;
			    	}
				   	return mapa_pulgarcito[fila+1][columna];
				    break;
			    case 3: // este
			    	if(!libreParaPasar(fila,columna-1)){
			    		return 25000;
			    	}
			    	return mapa_pulgarcito[fila][columna-1];
				    
				    break;
	 		 }
	}
	int Agent::izquierda(int fila , int columna){
			// y fila x columna

			switch(orientacion_){
			    case 0: // norte
			    	if(!libreParaPasar(fila,columna-1)){
			    		return 25000;
			    	}
				    return mapa_pulgarcito[fila][columna-1];
				    break;
			    case 1: // este
			    	if(!libreParaPasar(fila-1,columna)){
			    		return 25000;
			    	}
				    return mapa_pulgarcito[fila-1][columna];
				    break;
			    case 2: // sur
			    	if(!libreParaPasar(fila,columna+1)){
			    		return 25000;
			    	}
			    	return mapa_pulgarcito[fila][columna+1];				    
				    break;
			    case 3: // este
			    	if(!libreParaPasar(fila+1,columna)){
			    		return 25000;
			    	}
			    	return mapa_pulgarcito[fila+1][columna];
				    break;
	 		 }

	}
	int Agent::derecha(int fila , int columna ){

			switch(orientacion_){
			    case 0: // norte
			    	if(!libreParaPasar(fila,columna+1)){
			    		return 25000;
			    	}
				   	return mapa_pulgarcito[fila][columna+1];
				    break;
			    case 1: // este
			    	if(!libreParaPasar(fila+1,columna)){
			    		return 25000;
			    	}
				   	return mapa_pulgarcito[fila+1][columna];
				    break;
			    case 2: // sur
			    	if(!libreParaPasar(fila,columna-1)){
			    		return 25000;
			    	}
				    return mapa_pulgarcito[fila][columna-1];
				    break;
			    case 3: // este
			    	if(!libreParaPasar(fila-1,columna)){
			    		return 25000;
			    	}
				    return mapa_pulgarcito[fila-1][columna];
				    break;
	 		 }
	}

//METODO DE PULGARCITO - VALIDADO 
Agent::ActionType Agent::siguienteMovimiento(int fila , int columna ){


	Agent::ActionType accion ;

	if((delante(fila,columna) < derecha(fila,columna)) and (delante(fila,columna) < izquierda(fila,columna))){
			accion = actFORWARD;

			

	}else if ( (derecha(fila,columna) < delante(fila,columna)) and (derecha(fila,columna) < izquierda(fila,columna) ) ){
			accion = actTURN_R;

	}else if ((izquierda(fila,columna) < derecha(fila,columna)) and (izquierda(fila,columna) < delante(fila,columna) ) ){
			accion = actTURN_L;

	}else if (delante(fila,columna)==-1) {
		accion = actFORWARD;


	}else if(izquierda(fila,columna)==-1){
		accion = actTURN_L;
	}else if(derecha(fila,columna)==-1){
		accion = actTURN_R;
	}else{
		if(rand()%2==0){	
			accion = actTURN_L ;
		}else{
			accion = actTURN_R ;
		}
	}

	return accion;

}

// -----------------------------------------------------------
Agent::ActionType Agent::Think()
{
	Agent::ActionType accion ;

	contadorPulgarcito++;
	mapa_pulgarcito[y_][x_] = contadorPulgarcito;



	accion = siguienteMovimiento(y_,x_);


	buscaK();

	if( !movimientos.empty() ){
		accion = movimientos.front();
		movimientos.pop();

	}


	if((SURFACE_[0] == '2' || SURFACE_[0] == '0') && EN_USO_=='-'){
		accion = actPICKUP;
		SURFACE_[0]='?';
	}

	// recuerdo la ultima accion realizada
	last_accion_ = accion;

	return accion;

}

