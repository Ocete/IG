// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Práctica 2  (implementación)
// **
// *********************************************************************

#include "aux.hpp"
#include "tuplasg.hpp"   // Tupla3f
#include "practicas.hpp"
#include "practica3.hpp"
#include "grafo-escena.hpp"

using namespace std ;

static C4 * c4;
static ColFuentesLuz  * colFuentes;
int angulo_activo;

// ---------------------------------------------------------------------
// Función para implementar en la práctica 4 para inicialización.
// Se llama una vez al inicio, cuando ya se ha creado la ventana e
// incializado OpenGL.

void P4_Inicializar( ContextoVis & cv ) {
   cout << "Creando objetos de la práctica 4 .... " << flush ;

   c4 = new C4();

   colFuentes = new ColFuentesLuz();
   angulo_activo = 0;
   colFuentes->activar(0);
   colFuentes->insertar( new FuenteDireccional(-10, 30, VectorRGB(0.4,0.4,0.4,1) ) );
   colFuentes->insertar( new FuentePosicional( {2.5,0,10}, {0.4,0.4,0.4,0} ) );

   //glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE );

   cout << "hecho." << endl << flush ;
}

// ---------------------------------------------------------------------
// Función invocada al pulsar una tecla con la práctica 4 activa:
// (si la tecla no se procesa en el 'main').
//
//  - devuelve 'true' si la tecla se usa en esta práctica para cambiar
//    entre el grado de libertad o incrementar/decrementar valores
//  - devuelve 'false' si la tecla no se usa en esta práctica (no ha
//    cambiado nada)

bool P4_FGE_PulsarTeclaCaracter( unsigned char tecla, ContextoVis & cv ) {
   bool res = false  ;
   int key = -1;

  switch ( toupper( tecla ) ) {
    case 'G' :
      angulo_activo  = ( angulo_activo + 1 ) % 2;
      break ;

    case '>' :
      key = angulo_activo == 0 ? GLFW_KEY_UP : GLFW_KEY_RIGHT;
      break ;

    case '<' :
      key = angulo_activo == 0 ? GLFW_KEY_DOWN : GLFW_KEY_LEFT;
      break ;
    default :
      break ;
  }
  if (key != -1) {
    res = colFuentes->ptrFuente(0)->gestionarEventoTeclaEspecial(key);
  }

  return res ;
}

// ---------------------------------------------------------------------
// Función a implementar en la práctica 4  para dibujar los objetos
// se debe de usar el modo de dibujo que hay en el parámetro 'cv'
// (se accede con 'cv.modoVisu')

void P4_DibujarObjetos( ContextoVis & cv ) {
  if (c4 != nullptr) {
    glEnable( GL_LIGHTING );
    if (cv.modoVis == modoSombreadoPlano || cv.modoVis == modoSombreadoSuave) {
      colFuentes->activar( 0 );
    }
    c4->visualizarGL( cv );
  }
}
