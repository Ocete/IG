// *********************************************************************
// **
// ** Informática Gráfica, curso 2018-19
// ** Práctica 2  (implementación)
// **
// *********************************************************************

#include "aux.hpp"
#include "tuplasg.hpp"   // Tupla3f
#include "practicas.hpp"
#include "practica3.hpp"
#include "grafo-escena.hpp" // Añadido por mi

using namespace std ;

static constexpr int numObjetos3 = 3;
static NodoGrafoEscenaParam * objetos3[numObjetos3] = {nullptr, nullptr, nullptr};
static unsigned objetoActivo3 = 0 ;

bool animacion_activada = false;
int param_actual = 0;

// ---------------------------------------------------------------------
// Función para implementar en la práctica 1 para inicialización.
// Se llama una vez al inicio, cuando ya se ha creado la ventana e
// incializado OpenGL.

void P3_Inicializar(  )
{
   cout << "Creando objetos de la práctica 3 .... " << flush;
   //vector<Parametro> *p;
  //Atraccion atr (p);

   objetos3[0] = new C(true);
   objetos3[1] = new C(false);
   objetos3[2] = new C2();
   cout << "hecho." << endl << flush ;
}

// ---------------------------------------------------------------------
// Función invocada al pulsar una tecla con la práctica 1 activa:
// (si la tecla no se procesa en el 'main').
//
//  - devuelve 'true' si la tecla se usa en esta práctica para cambiar
//    entre el grado de libertad o incrementar/decrementar valores
//  - devuelve 'false' si la tecla no se usa en esta práctica (no ha
//    cambiado nada)

bool P3_FGE_PulsarTeclaCaracter( unsigned char tecla ) {
  bool res = false  ; // valor devuelto (true si se ha procesado tecla)
  switch ( toupper( tecla ) ) {
    case 'O' :
       objetoActivo3 = (objetoActivo3 + 1) % numObjetos3;
       param_actual = 0;
       cout << "nuevo objeto activo es: " << objetoActivo3 ;
       if ( objetos3[objetoActivo3] != nullptr )
          cout << " (" << objetos3[objetoActivo3]->leerNombre() << ")." << endl ;
       else
          cout << " (objeto no creado)" << endl ;
       res = true;
       break ;

    case 'A' :
       animacion_activada ^= 1;
       res = true;
       if (animacion_activada)
         FijarFuncDesocupado( FGE_Desocupado );
       cout << "animaciones ";
       if (animacion_activada) cout << "activadas"; else cout << "desactivadas";
       cout << endl;
       break ;

    case 'G' :
       param_actual = (param_actual + 1 ) % objetos3[objetoActivo3]->numParametros();
       cout << "Parámetro actual: " <<
            ( objetos3[objetoActivo3]->leerPtrParametro(param_actual) )->leer_descripcion() << endl;
       break ;

    case 'R' :
       objetos3[objetoActivo3]->reset();
       res = true;
       cout << "visualización reseteada" << endl;
       break ;

    case '>' :
       if (animacion_activada) {
         (objetos3[objetoActivo3]->leerPtrParametro(param_actual) )->acelerar();
       } else {
         (objetos3[objetoActivo3]->leerPtrParametro(param_actual) )->incrementar();
       }
       res = true;
       break ;

    case '<' :
       if (animacion_activada) {
         (objetos3[objetoActivo3]->leerPtrParametro(param_actual) )->decelerar();
       } else {
         (objetos3[objetoActivo3]->leerPtrParametro(param_actual) )->decrementar();
       }
       res = true;
      break;

    default:
      break;
  }
  return res;
}

// ---------------------------------------------------------------------
// Función a implementar en la práctica 3  para dibujar los objetos
// se debe de usar el modo de dibujo que hay en el parámetro 'cv'
// (se accede con 'cv.modoVisu')

void P3_DibujarObjetos( ContextoVis & cv ) {
  if (objetos3[objetoActivo3] != nullptr) {
    objetos3[objetoActivo3]->visualizarGL( cv );
  }
}

//--------------------------------------------------------------------------

bool P3_FGE_Desocupado() {
   if (animacion_activada) {
     objetos3[objetoActivo3]->siguienteCuadro();
     redibujar_ventana = true;
   }
   return animacion_activada;
}
