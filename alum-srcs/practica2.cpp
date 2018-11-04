// *********************************************************************
// **
// ** Informática Gráfica, curso 2018-19
// ** Práctica 2  (implementación)
// **
// *********************************************************************

#include "aux.hpp"
#include "tuplasg.hpp"   // Tupla3f
#include "practicas.hpp"
#include "practica2.hpp"
#include "Objeto3D.hpp"
#include "MallaPLY.hpp"
#include "MallaRevol.hpp"

using namespace std ;

// objeto activo: malla ply (0), malla revol (1)
// Cilindro (2), Cono (3), Esfera (4)
static unsigned objetoActivo2 = 0 ;
static constexpr int numObjetos2 = 5 ;

static Objeto3D * objetos2[numObjetos2] = { nullptr, nullptr, nullptr,
                                            nullptr, nullptr };

// ---------------------------------------------------------------------
// Función para implementar en la práctica 1 para inicialización.
// Se llama una vez al inicio, cuando ya se ha creado la ventana e
// incializado OpenGL.

void P2_Inicializar(  )
{
   cout << "Creando objetos de la práctica 2 .... " << flush ;
   objetos2[0] = new MallaPLY("../plys/ant.ply");
   objetos2[1] = new MallaRevol("../plys/peon.ply", 30, 1, 1);
   objetos2[2] = new Cilindro(10, 30, true, true, 1, 1);
   objetos2[3] = new Cono(10, 30, true, true, 1, 1);
   objetos2[4] = new Esfera(50, 50, true, false, 1);
   cout << "hecho." << endl << flush ;
}

// ---------------------------------------------------------------------
// Función invocada al pulsar una tecla con la práctica 1 activa:
// (si la tecla no se procesa en el 'main').
//
//  - devuelve 'true' si la tecla se usa en esta práctica para cambiar
//    entre el cubo, el tetraedro u otros objetos (cambia el valor de
//    'objeto_activo').
//  - devuelve 'false' si la tecla no se usa en esta práctica (no ha
//    cambiado nada)

bool P2_FGE_PulsarTeclaCaracter( unsigned char tecla )
{
   if ( toupper(tecla) != 'O')
      return false ;

   objetoActivo2 = (objetoActivo2+1) % numObjetos2 ;
   cout << "práctica 1: nuevo objeto activo es: " << objetoActivo2 ;
   if ( objetos2[objetoActivo2] != nullptr )
      cout << " (" << objetos2[objetoActivo2]->leerNombre() << ")." << endl ;
   else
      cout << " (objeto no creado)" << endl ;
   return true ;
}

// ---------------------------------------------------------------------
// Función a implementar en la práctica 1  para dibujar los objetos
// se debe de usar el modo de dibujo que hay en el parámetro 'cv'
// (se accede con 'cv.modoVisu')

void P2_DibujarObjetos( ContextoVis & cv ) {
  if ( objetos2[objetoActivo2] != nullptr ) {
   objetos2[objetoActivo2]->visualizarGL( cv );
  }
}
