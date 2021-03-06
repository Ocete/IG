// *********************************************************************
// **
// ** Informática Gráfica
// ** Práctica 5  (implementaciones)
// **
// *********************************************************************

#include "aux.hpp"
#include "tuplasg.hpp"   // Tupla3f
#include "practicas.hpp"
#include "practica5.hpp"
#include "grafo-escena.hpp"
#include "materiales.hpp"

#include "CamaraInter.hpp"

using namespace std ;

// COMPLETAR: práctica 5: declarar variables de la práctica 5 (static)
//    (escena, viewport, el vector de camaras, y las variables
//      que sirven para gestionar el modo arrastrar)
// ......

// viewport actual (se recalcula al inicializar y al fijar las matrices)
Viewport viewport ;
// true si se está en modo arrastrar, false si no
static bool modo_arrastrar = false ;

// Añadido por mi
static const int num_camaras = 4;
static CamaraInteractiva * camaras[num_camaras] = {nullptr, nullptr, nullptr, nullptr};
static int camara_activa;
static ColFuentesLuz  * colFuentes;

const static float desp = 1;

float xant, yant;
static C4 * c4;

// ---------------------------------------------------------------------

void P5_Inicializar(  int vp_ancho, int vp_alto ) {
  cout << "Creando objetos de la práctica 5 .... " << flush ;
  // COMPLETAR: práctica 5: inicializar las variables de la práctica 5 (incluyendo el viewport)

  c4 = new C4();
  //int ident = 1;
  //c4->asignarIdentificadores( ident );

  viewport = Viewport (0, 0, vp_ancho, vp_alto);

  camara_activa = 0;
/*  camaras[0] = new CamaraInteractiva (false, vp_alto * (1.0) / vp_ancho, 0, 0,
                                      {0,0,0}, false, 135, 2);
  camaras[1] = new CamaraInteractiva (false, vp_alto * (1.0) / vp_ancho, 0, 0,
                                      {0,0,0}, true, 90, 2);
*/

  float aspect = vp_alto * (1.0) / vp_ancho;
  // Planta
  camaras[0] = new CamaraInteractiva (true, aspect, 0, 90,
                                      {0,0,0}, true, 80.0, 8.0);
  // Alzado
  camaras[1] = new CamaraInteractiva (true, aspect, 0, 0,
                                     {0,0,0}, true, 80.0, 10);
  // Perfil
  camaras[2] = new CamaraInteractiva (true, aspect, 90, 0,
                                     {0,0,0}, true, 80, 10);
  // Ortográfica
  camaras[3] = new CamaraInteractiva (true, aspect, 0, 0,
                                      {0,0,0}, false, 80, 5);

  colFuentes = new ColFuentesLuz();
  colFuentes->insertar( new FuentePosicional( {2.5,0,10}, {1,1,1,0} ) );
  //colFuentes->insertar( new FuenteDireccional(-10, 30, {1,1,1,1} ) );
  colFuentes->activar(0);

  cout << "hecho." << endl << flush ;
}

// ---------------------------------------------------------------------

void P5_FijarMVPOpenGL( int vp_ancho, int vp_alto ) {
   // COMPLETADO: práctica 5: actualizar viewport, actualizar y activar la camara actual
   //     (en base a las dimensiones del viewport)

  viewport.ancho = vp_ancho;
  viewport.alto = vp_alto;
  camaras[camara_activa]->ratio_yx_vp = vp_alto * (1.0) / vp_ancho;

  camaras[camara_activa]->calcularViewfrustum();
  camaras[camara_activa]->activar();
}

// ---------------------------------------------------------------------

void P5_DibujarObjetos( ContextoVis & cv ) {
  // COMPLETAR: práctica 5: activar las fuentes de luz y visualizar la escena
  //      (se supone que la camara actual ya está activada)

  // Esto obviamente no tiene que estar, es solo para pruebas
  //cv.modoSeleccionFBO = true;

  if (c4 != nullptr) {
    glEnable( GL_LIGHTING );
    c4->visualizarGL( cv );
    if (cv.modoVis == modoSombreadoPlano || cv.modoVis == modoSombreadoSuave) {
      colFuentes->activar( 0 );
    }
  }

}

// ---------------------------------------------------------------------

bool P5_FGE_PulsarTeclaCaracter(  unsigned char tecla ) {

  bool result = true ;
  string str;

  switch ( toupper( tecla ) ) {
    case 'R':
      camaras[camara_activa]->modoPPR();
      str = camaras[camara_activa]->PPR ? "activado" : "desactivado";
      cout << "Modo PPR " << str << endl;
      break ;

    case 'C':
      camara_activa = (camara_activa + 1) % num_camaras;
      cout << "Cámraa actual cambiada a: " << camara_activa << endl;
      camaras[camara_activa]->activar();
      break ;

    case 'V':
      cout << "Modo de cámara actual cambiado a: ";
      if ( camaras[camara_activa]->examinar ) {
        camaras[camara_activa]->modoPrimeraPersona();
        cout << "Primera Persona" << endl;
      } else {
        camaras[camara_activa]->modoExaminar();
        cout << "Examinar" << endl;
      }
      break ;

    case '-':
      //cout << "Desplaza negativo de la cámara en Z" << endl;
      camaras[camara_activa]->desplaZ( -desp );
      break;

    case '+':
      //cout << "Desplaza positivo de la cámara en Z" << endl;
      camaras[camara_activa]->desplaZ( desp );
      break;

    default:
      result = false ;
      break ;
  }
  return result ;
}
// ---------------------------------------------------------------------

bool P5_FGE_PulsarTeclaEspecial(  int tecla  ) {

  bool result = true ;

  switch ( tecla ) {
    case GLFW_KEY_LEFT:
      //cout << "Desplaza hacia la izquierda la cámara" << endl;
      camaras[camara_activa]->moverHV( -desp, 0 );
      break;

    case GLFW_KEY_RIGHT:
      //cout << "Desplaza hacia la derecha la cámara" << endl;
      camaras[camara_activa]->moverHV( desp, 0 );
      break;

    case GLFW_KEY_UP:
      //cout << "Desplaza hacia arriba la cámara" << endl;
      camaras[camara_activa]->moverHV( 0, desp );
      break;

    case GLFW_KEY_DOWN:
      //cout << "Desplaza hacia abajo la cámara" << endl;
      camaras[camara_activa]->moverHV( 0, -desp );
      break;

    default:
      result = false ;
      break ;
  }
  return result ;
}

// ---------------------------------------------------------------------
// se llama al hacer click con el botón izquierdo

void P5_ClickIzquierdo( int x, int y ) {
  // COMPLETAR: práctica 5: visualizar escena en modo selección y leer el color del pixel en (x,y)

  // 1. crear un 'contextovis' apropiado
  ContextoVis contexto;
  contexto.modoVis = modoSolido;
  contexto.modoSeleccionFBO = true;
  // glColor3ub(0,0,0);

  // 2. visualizar en modo selección (sobre el backbuffer)
  glClearColor(0, 0, 0, 1);                           // color de fondo
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // limpiar pantalla
  c4->visualizarGL( contexto );

  // 3. leer el color del pixel, si es 0 no se hace nada
  int ident_leido = LeerIdentEnPixel(x, y);
  if ( ident_leido <= 0 ) {
    cout << "Ningun objeto seleccionado" << endl;
    return;
  } else if ( ident_leido == 5 ) {
    cout << "bingo! has acertado en el dado" << endl;
  }

  // 4. buscar el objeto en el grafo de escena e informar del mismo
  Objeto3D * p_obj;
  Tupla3f centro (0.0, 0.0, 0.0);
  Matriz4f mat = MAT_Ident();
  if ( !c4->buscarObjeto( ident_leido, mat, &p_obj, centro ) ) {
    cout << "ERROR: Objeto no encontrado en el arbol. ";
    cout << "Identificador: " << ident_leido << endl;
    return;
  }

  camaras[camara_activa]->modoExaminar( centro );
  cout << "Objeto fijado con identificador: " << ident_leido << " y centro " << centro << endl;
}

// ---------------------------------------------------------------------
// se llama al mover el botón en modo arrastrar

void P5_InicioModoArrastrar( int x, int y ) {
   // COMPLETADO: práctica 5: activar bool de modo arrastrar
   // registrar (x,y) de inicio del modo arrastrar
   xant = x;
   yant = y;
   modo_arrastrar = true;
}

// ---------------------------------------------------------------------
// se llama al subir el botón derecho del ratón

void P5_FinModoArrastrar() {
   // COMPLETADO: práctica 5: desactivar bool del modo arrastrar
   modo_arrastrar = false;
}
// ---------------------------------------------------------------------
// se llama durante el modo arrastrar

void P5_RatonArrastradoHasta( int x, int y ) {
   // COMPLETADO: calcular desplazamiento desde inicio de modo arrastrar
   // actualizar la camara (moverHV)
   camaras[camara_activa]->moverHV ( x-xant, y-yant );
   xant = x;
   yant = y;
}
// ---------------------------------------------------------------------
// pulsar/levantar botón del ratón, específico de la práctica 5

bool P5_FGE_ClickRaton( int boton, int estado, int x, int y )
{
   //cout << "P5_FGE_ClickRaton" << endl ;
   if ( estado == GLFW_PRESS && boton == GLFW_MOUSE_BUTTON_LEFT  )
      P5_ClickIzquierdo( x, viewport.alto-y );
   else if ( estado == GLFW_PRESS && boton == GLFW_MOUSE_BUTTON_RIGHT )
      P5_InicioModoArrastrar( x, viewport.alto-y );
   else if ( estado == GLFW_RELEASE && boton == GLFW_MOUSE_BUTTON_RIGHT )
      P5_FinModoArrastrar();
   else
      return false ;

   return true ;
}
// ---------------------------------------------------------------------
bool P5_FGE_RatonMovidoPulsado( int x, int y )
{
   if ( modo_arrastrar )
   {
      P5_RatonArrastradoHasta( x, viewport.alto-y );
      return true ;
   }
   else
      return false ;
}

// ---------------------------------------------------------------------

bool P5_FGE_Scroll( int direction ) {
   // COMPLETADO: acercar/alejar la camara (desplaZ)
   camaras[camara_activa]->desplaZ( direction );
   return true ;
}

// ---------------------------------------------------------------------

void FijarColorIdent( const int ident ) { // 0 ≤ ident < 2^24
  // COMPLETADO: fijar color actual de OpenGL usando 'ident' (glColor3ub)
  const unsigned char
  byteR = ( ident ) % 0x100U,             // rojo = byte menos significativo
  byteG = ( ident / 0x100U ) % 0x100U,    // verde = byte intermedio
  byteB = ( ident / 0x10000U ) % 0x100U;  // azul = byte más significativo
  glColor3ub( byteR, byteG, byteB );
}

//---------------

int LeerIdentEnPixel( int xpix, int ypix ) {
  // COMPLETADO:leer el identificador codificado en el color del pixel (x,y)
  unsigned char bytes[3] ; // para guardar los tres bytes
  // leer los 3 bytes del frame-buffer
  glReadPixels( xpix, ypix, 1, 1, GL_RGB,GL_UNSIGNED_BYTE, (void *)bytes);
  // reconstruir el indentificador y devolverlo:
  return bytes[0] + ( 0x100U* bytes[1] ) + ( 0x10000U* bytes[2] ) ;
}

//---------------
