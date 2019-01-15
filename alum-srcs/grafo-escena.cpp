// *********************************************************************
// **
// ** Gestión de una grafo de escena (implementación)
// ** Copyright (C) 2016 Carlos Ureña
// **
// ** This program is free software: you can redistribute it and/or modify
// ** it under the terms of the GNU General Public License as published by
// ** the Free Software Foundation, either version 3 of the License, or
// ** (at your option) any later version.
// **
// ** This program is distributed in the hope that it will be useful,
// ** but WITHOUT ANY WARRANTY; without even the implied warranty of
// ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// ** GNU General Public License for more details.
// **
// ** You should have received a copy of the GNU General Public License
// ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
// **
// *********************************************************************

#include "aux.hpp"
#include "matrices-tr.hpp"
#include "shaders.hpp"
#include "grafo-escena.hpp"
#include "MallaInd.hpp" // Añadido por mi
#include "MallaRevol.hpp" // Añadido por mi
#include <sstream> // Para el stringstream
//#include "tuplasg.hpp"

using namespace std ;

// *********************************************************************
// Entrada del nodo del Grafo de Escena

// ---------------------------------------------------------------------
// Constructor para entrada de tipo sub-objeto

EntradaNGE::EntradaNGE( Objeto3D * pObjeto )
{
   assert( pObjeto != NULL );
   tipo   = TipoEntNGE::objeto ;
   objeto = pObjeto ;
}
// ---------------------------------------------------------------------
// Constructor para entrada de tipo "matriz de transformación"

EntradaNGE::EntradaNGE( const Matriz4f & pMatriz )
{
   tipo    = TipoEntNGE::transformacion ;
   matriz  = new Matriz4f() ; // matriz en el heap, puntero propietario
   *matriz = pMatriz ;
}

// ---------------------------------------------------------------------
// Constructor para entrada de tipo "matriz de transformación"

EntradaNGE::EntradaNGE( Material * pMaterial )
{
   assert( pMaterial != NULL );
   tipo     = TipoEntNGE::material ;
   material = pMaterial ;
}

// -----------------------------------------------------------------------------
// Destructor de una entrada

EntradaNGE::~EntradaNGE() {
   /**  no fnciona debido a que se hacen copias (duplicados) de punteros
   if ( tipo == TipoEntNGE::transformacion )
   {
      assert( matriz != NULL );
      delete matriz ;
      matriz = NULL ;
   }
   * **/
}

// *****************************************************************************
// Nodo del grafo de escena: contiene una lista de entradas
// *****************************************************************************

// -----------------------------------------------------------------------------
// Visualiza usando OpenGL

void NodoGrafoEscena::visualizarGL( ContextoVis & cv ) {
  // guarda modelview actual
  glMatrixMode( GL_MODELVIEW );
  glPushMatrix() ;
  cv.pilaMateriales.push();
  for( unsigned i = 0 ; i < entradas.size() ; i++ ) {

    // Para la practica 5, fijamos el color actual como el identificador
    // CUIDADO: no activar las entradas de tipo material en el bucle de
    // abajo si modoSeleccion esta activado
    if ( cv.modoSeleccionFBO ) {
      int ident = leerIdentificador();
      //cout << "Color fijado a " << ident << endl;
      if ( ident != -1 ) {
        FijarColorIdent ( ident );
      }
    }

    if( entradas[i].tipo == TipoEntNGE::objeto ) {
      // si la entrada es sub-objeto, visualizarlo
      entradas[i].objeto->visualizarGL( cv ) ;
    } else if ( entradas[i].tipo == TipoEntNGE::transformacion ) {
      // si la entrada es transformación, componerla
      glMatrixMode( GL_MODELVIEW );
      glMultMatrixf( *(entradas[i].matriz) );
    } else if (entradas[i].tipo == TipoEntNGE::material &&
                          !cv.modoSeleccionFBO ) {
      // si la entrada es un material, lo activas
      cv.pilaMateriales.activarMaterial( entradas[i].material );
    }
  }
  // restaura modelview guardada
  cv.pilaMateriales.pop();
  glMatrixMode( GL_MODELVIEW );
  glPopMatrix();
}
// -----------------------------------------------------------------------------
void NodoGrafoEscena::asignarIdentificadores( int &nuevo_ident ) {
  ponerIdentificador ( nuevo_ident );
  nuevo_ident++;
  for (int i=0; i<entradas.size(); i++) {
    if (entradas[i].tipo == TipoEntNGE::objeto) {
      entradas[i].objeto->asignarIdentificadores(nuevo_ident);
    }
  }
}
// -----------------------------------------------------------------------------
NodoGrafoEscena::NodoGrafoEscena() {
  color_fijado = false;
}
// -----------------------------------------------------------------------------
void NodoGrafoEscena::fijarColorNodo( const Tupla3f & nuevo_color ) {
  if (!color_fijado) {
    color_fijado = true;
    for (int i=0; i<entradas.size(); i++) {
      if (entradas[i].tipo == TipoEntNGE::objeto) {
        entradas[i].objeto->fijarColorNodo( nuevo_color );
      }
    }
  }
}
// -----------------------------------------------------------------------------
// Añadir una entrada (al final).
// genérica

unsigned NodoGrafoEscena::agregar( const EntradaNGE & entrada ) {
   entradas.push_back( entrada );
   return entradas.size() - 1;
}
// -----------------------------------------------------------------------------
// construir una entrada y añadirla (al final)
// objeto (copia solo puntero)

unsigned NodoGrafoEscena::agregar( Objeto3D * pObjeto ) {
   return agregar( EntradaNGE( pObjeto ) );
}
// ---------------------------------------------------------------------
// construir una entrada y añadirla (al final)
// matriz (copia objeto)

unsigned NodoGrafoEscena::agregar( const Matriz4f & pMatriz ) {
   return agregar( EntradaNGE( pMatriz ) );
}
// ---------------------------------------------------------------------
// material (copia solo puntero)
unsigned NodoGrafoEscena::agregar( Material * pMaterial ) {
   return agregar( EntradaNGE( pMaterial ) );
}

// devuelve el puntero a la matriz en la i-ésima entrada
Matriz4f * NodoGrafoEscena::leerPtrMatriz( unsigned indice )
{
  assert (indice < entradas.size() &&
            entradas[indice].tipo == TipoEntNGE::transformacion );
  return entradas[indice].matriz;
}
// -----------------------------------------------------------------------------
// si 'centro_calculado' es 'false', recalcula el centro usando los centros
// de los hijos (el punto medio de la caja englobante de los centros de hijos)

void NodoGrafoEscena::calcularCentroOC() {
  // COMPLETADO: práctica 5: calcular y guardar el centro del nodo
  // en coordenadas de objeto (hay que hacerlo recursivamente)
  // (si el centro ya ha sido calculado, no volver a hacerlo)

  if ( !centro_calculado ) {
    Matriz4f mat = MAT_Ident();
    vector<Tupla3f> centros;
    Tupla3f centro_leido;

    float n_centros = 0;
    for (int i=0; i<entradas.size(); i++) {
        if (entradas[i].tipo == TipoEntNGE::objeto) {
          entradas[i].objeto->calcularCentroOC();
          centro_leido = entradas[i].objeto->leerCentroOC();
          centro_leido = mat * centro_leido;
          centros.push_back ( centro_leido );
        } else if ( entradas[i].tipo == TipoEntNGE::transformacion ) {
          mat = mat * (*entradas[i].matriz);
        }
    }

    Tupla3f centro = centroCajaEnglobante ( centros );
    ponerCentroOC( centro );
    centro_calculado = true;
  }
}
// -----------------------------------------------------------------------------
// método para buscar un objeto con un identificador y devolver un puntero al mismo

bool NodoGrafoEscena::buscarObjeto
(
   const int         ident_busc, // identificador a buscar
   const Matriz4f &  mmodelado,  // matriz de modelado
   Objeto3D       ** objeto,     // (salida) puntero al puntero al objeto
   Tupla3f &         centro_wc   // (salida) centro del objeto en coordenadas del mundo
)
{
  // COMPLETADO: práctica 5: buscar un sub-objeto con un identificador
  assert(0<ident_busc);

  if ( !centro_calculado ) {
    calcularCentroOC();
  }

  if ( leerIdentificador() == ident_busc ) {
    centro_wc = mmodelado*leerCentroOC();
    if ( objeto != nullptr ) {
      *objeto = this ;
    }
    return true;
  } else {
    bool found = false;
    for (int i=0; i<entradas.size() && !found; i++) {
      found = entradas[i].tipo == TipoEntNGE::objeto && entradas[i].objeto->buscarObjeto(
            ident_busc, mmodelado, objeto, centro_wc );
    }
    return found;
  }
}

// *****************************************************************************
// Nodo del grafo de escena, con una lista añadida de parámetros
// *****************************************************************************


// -----------------------------------------------------------------------------
// devuelve el numero de grados de libertad
int NodoGrafoEscenaParam::numParametros() {
   return parametros.size();
}
// -----------------------------------------------------------------------------

// devuelve un puntero al i-ésimo grado de libertad
Parametro * NodoGrafoEscenaParam::leerPtrParametro( unsigned i ) {
  assert (i >= 0 && i < parametros.size());
  return &parametros[i];
}
// -----------------------------------------------------------------------------
void NodoGrafoEscenaParam::siguienteCuadro() {
  for (unsigned i=0; i<parametros.size() ; i++) {
    parametros[i].siguiente_cuadro();
  }
}
// -----------------------------------------------------------------------------
void NodoGrafoEscenaParam::reset() {
  for (unsigned i=0; i<parametros.size() ; i++) {
    parametros[i].reset();
  }
}
// -----------------------------------------------------------------------------

C::C(bool atraccion) {
  // cout << "Constructor C comienza" << endl;
  if (atraccion) {
    agregar( new Atraccion( &parametros ) );
    nombre = "Atracción";
  } else {
    agregar( new Colgante( &parametros ) );
    reset();
    nombre = "Araña";
  }
  // cout << "Constructor C termina" << endl;
}

// -----------------------------------------------------------------------------

Asiento::Asiento() {
  // cout << "Constructor Asiento comienza" << endl;
  agregar( MAT_Escalado(0.5,0.3,0.5) );
  agregar( new Cubo );
  // cout << "Constructor Asiento termina" << endl;
}

// -----------------------------------------------------------------------------

AsientoDelantero::AsientoDelantero(float d) {
  // cout << "Constructor AsientoDelantero comienza" << endl;
  agregar( MAT_Traslacion(0,0,d) );
  agregar( new Asiento );
  // cout << "Constructor AsientoDelantero termina" << endl;
}

// -----------------------------------------------------------------------------

AsientoTrasero::AsientoTrasero(float d) {
  // cout << "Constructor AsientoTrasero comienza" << endl;
  agregar( MAT_Traslacion(0,0,-d-0.5) );
  agregar( new Asiento );
  // cout << "Constructor AsientoTrasero termina" << endl;
}

// -----------------------------------------------------------------------------

CilindroAsientos::CilindroAsientos() {
  // cout << "Constructor CilindroAsientos comienza" << endl;
  agregar( MAT_Rotacion(270,0,0,1) );
  agregar( new Cilindro(10,30,true,true,10,1) );
  // cout << "Constructor CilindroAsientos termina" << endl;
}

// -----------------------------------------------------------------------------

FinalBrazoMecanico::FinalBrazoMecanico(vector<Parametro> *v, float d, int num) {
  // cout << "Constructor FinalBrazoMecanico comienza" << endl;
  agregar( MAT_Rotacion(0,0,0,1) ); // Matriz del parámetro
  agregar( MAT_Rotacion(0,1,0,0) ); // Matriz del parámetro
  agregar( MAT_Traslacion(d,0,0) );
  agregar( new CilindroAsientos );
  agregar( MAT_Traslacion(2,0,0) );
  for (int i=0; i<3; i++) {
    agregar( MAT_Traslacion(2,0,0) );
    agregar( new AsientoDelantero(1) );
    agregar( new AsientoTrasero(1) );
  }

  // Parámetro asociado
  stringstream ss;
  ss << "Giro del final del brazo mecanico " << num << " arriba y abajo";
  v->push_back( Parametro (ss.str(), entradas[0].matriz,
                [=](float ang) {return MAT_Rotacion (ang,0,0,1);},
                true, 0, 22.5, 0.1) );

  // Parámetro asociado
  stringstream ss2;
  ss2 << "Giro del final del brazo mecanico " << num << " sobre si mismo";
  v->push_back( Parametro (ss2.str(), entradas[1].matriz,
                [=](float ang) {return MAT_Rotacion (ang,1,0,0);},
                true, 0, 45, 0.1) );

  // cout << "Constructor FinalBrazoMecanico termina" << endl;
}

// -----------------------------------------------------------------------------

BrazoMecanico::BrazoMecanico(vector<Parametro> *v, float d, int num) {
  // cout << "Constructor BrazoMecanico comienza" << endl;
  agregar( MAT_Rotacion(0,0,0,1) ); // Matriz del parámetro
  agregar( MAT_Traslacion(d,0,0) );
  agregar( new CilindroAsientos );
  agregar( MAT_Traslacion(11,0,0) );
  agregar( new Esfera(20,50,false,false,1) );
  agregar( new FinalBrazoMecanico(v, d, num) );

  // Parámetro asociado
  stringstream ss;
  ss << "Giro del brazo mecanico " << num;
  v->push_back( Parametro (ss.str(), entradas[0].matriz,
                [=](float ang) {return MAT_Rotacion (ang,0,0,1);},
                true, 0, 22.5, 0.1) );
  // cout << "Constructor BrazoMecanico termina" << endl;
}

// -----------------------------------------------------------------------------

BaseBrazos::BaseBrazos(vector<Parametro> *v) {
  // cout << "Constructor BaseBrazos comienza" << endl;
  agregar ( MAT_Rotacion(0,0,1,0) ); // MAT parametro
  for (int i=0; i<4; i++) {
    agregar( new BrazoMecanico(v, 1, i+1) );
    agregar( MAT_Rotacion(90,0,1,0) );
  }
  agregar( new Esfera(20,50,false,false,1) );

  // Parámetro asociado
  v->push_back( Parametro ("Giro de la base de brazos",
                entradas[0].matriz,
                [=](float ang) {return MAT_Rotacion (ang,0,1,0);},
                false, 180, 50, 2) );
  // cout << "Constructor BaseBrazos termina" << endl;
}

// -----------------------------------------------------------------------------

Atraccion::Atraccion(vector<Parametro> *v) {
  // cout << "Constructor Atraccion comienza" << endl;

  agregar( MAT_Escalado(0.1,0.1,0.1) );
  agregar( MAT_Traslacion(0,26.8,0) );
  agregar( MAT_Rotacion(0,0,1,0) ); // MAT del parametro
  agregar( MAT_Rotacion(0,1,0,0) ); // MAT del parametro
  agregar( new Esfera(20,50,false,false,0.8) );
  agregar( MAT_Traslacion(0,-25.8,0) );
  agregar( new Cilindro(10, 30, true, true, 25, 0.3) );
  agregar( MAT_Traslacion(0,-1,0) );
  agregar( new BaseBrazos(v) );

  // Parámetro asociado
  v->push_back( Parametro ("Giro de la atracción",
                entradas[2].matriz,
                [=](float ang) {return MAT_Rotacion (ang,0,1,0);},
                false, 0, 10, 0.1) );

  v->push_back( Parametro ("Giro de la atracción 2",
                entradas[3].matriz,
                [=](float ang) {return MAT_Rotacion (ang,1,0,0);},
                true, 0, 45, 0.1) );

  // cout << "Constructor BaseBrazos termina" << endl;
}
// -----------------------------------------------------------------------------

Ojo::Ojo(float ang) {
  // cout << "Constructor Ojo comienza" << endl;
  agregar( MAT_Rotacion(ang, 0, 1, 0) );
  agregar( MAT_Rotacion(30, 0, 0, 1) );
  agregar( MAT_Traslacion(1, 0, 0) );
  agregar( new Esfera(20,50,false,false,0.1) );
  Tupla3f c = {1,0,0};
  fijarColorNodo( c );
  // cout << "Constructor Ojo termina" << endl;
}

// -----------------------------------------------------------------------------

Pata::Pata(vector<Parametro> *v, int num) {
  // cout << "Constructor Pata comienza" << endl;
  float e = 0.05;
  agregar( MAT_Traslacion(1-e, 0 ,0) );
  agregar( MAT_Escalado(e, e, e) );
  agregar( new BrazoMecanico(v, 1, num) );
  Tupla3f c = {0.2,0.2,0.2};
  fijarColorNodo( c );
  // cout << "Constructor Pata termina" << endl;
}

// -----------------------------------------------------------------------------

ParDePatas::ParDePatas(vector<Parametro> *v, int num) {
  // cout << "Constructor ParDePatas comienza" << endl;
  agregar( new Pata(v, num) );
  agregar( MAT_Rotacion(180, 0, 0, 1) );
  agregar( new Pata(v, num+1) );
  // cout << "Constructor ParDePatas termina" << endl;
}

// -----------------------------------------------------------------------------

Cuerpo::Cuerpo(vector<Parametro> *v) {
  // cout << "Constructor Cuerpo comienza" << endl;
  agregar( MAT_Rotacion(0,0,1,0) ); // MAT parametro
  for (int i=0; i<4; i++) {
    agregar( MAT_Rotacion(30, 0, 1, 0) );
    agregar( new ParDePatas(v, i*2) );
  }
  agregar( MAT_Rotacion(45, 0, 1, 0) );
  agregar( MAT_Escalado(1, 0.3, 1) );
  agregar( new Ojo(15) );
  agregar( new Ojo(-15) );
  agregar( new Esfera(20,50,false,false,1) );

  Tupla3f c = {0,0,0};
  fijarColorNodo( c );
  // Parámetro asociado
  v->push_back( Parametro ("Giro del cuerpo de la araña",
                entradas[0].matriz,
                [=](float ang) {return MAT_Rotacion (ang,0,1,0);},
                false, 180, 50, 2) );

  // cout << "Constructor Cuerpo termina" << endl;
}

// -----------------------------------------------------------------------------

Colgante::Colgante(vector<Parametro> *v) {
  // cout << "Constructor Colgante comienza" << endl;
  //agregar( MAT_Escalado(0.1,0.1,0.1) );
  agregar( MAT_Traslacion(0,25.8 + 0.3,0) );
  agregar( MAT_Rotacion(0,0,1,0) ); // MAT del parametro
  agregar( MAT_Rotacion(0,1,0,0) ); // MAT del parametro
  agregar( new Esfera(20,50,false,false,0.8) );
  agregar( MAT_Traslacion(0,-25.8,0) );
  agregar( new Cilindro(10, 30, true, true, 25, 0.01) );
  agregar( MAT_Traslacion(0,-0.3,0) );
  agregar( new Cuerpo(v) );

  // Parámetro asociado
  v->push_back( Parametro ("Giro colgante",
                entradas[1].matriz,
                [=](float ang) {return MAT_Rotacion (ang,0,1,0);},
                false, 0, 10, 0.1) );

  v->push_back( Parametro ("Giro colgante sobre si misma",
                entradas[2].matriz,
                [=](float ang) {return MAT_Rotacion (ang,1,0,0);},
                true, 0, 5, 0.1) );

  Tupla3f c = {1,1,1};
  fijarColorNodo( c );

  // cout << "Constructor Colgante termina" << endl;
}

// -----------------------------------------------------------------------------

Brazo::Brazo(Tupla3f color) {
  // cout << "Constructor Brazo comienza" << endl;
  agregar( MAT_Rotacion(90, 0, 0, 1) );
  agregar( new Esfera(20,50,false,false,1) );
  agregar( MAT_Traslacion(0, 1, 0) );
  agregar( new Cilindro(10, 30, true, true, 3, 0.25) );
  agregar( MAT_Traslacion(0, 4, 0) );
  agregar( new Esfera(20,50,false,false,1) );

  fijarColorNodo(color);
  // cout << "Constructor Brazo termina" << endl;
}

// -----------------------------------------------------------------------------

BrazoOrientado::BrazoOrientado(vector<Parametro> *v, float ang, Tupla3f color) {
  // cout << "Constructor BrazoOrientado comienza" << endl;
  agregar( MAT_Rotacion(0, 0, 0, 1) ); // MAT param
  agregar( MAT_Rotacion(ang, 0, 0, 1) );
  agregar( new Brazo(color) );

  // Parámetro asociado
  v->push_back( Parametro ("Giro del brazo orientado",
                entradas[0].matriz,
                [=](float ang) {return MAT_Rotacion (ang,0,0,1);},
                true, 0, 45, 0.1) );

  // cout << "Constructor BrazoOrientado termina" << endl;
}

// -----------------------------------------------------------------------------

Tronco::Tronco(std::vector<Parametro> *v) {
  // cout << "Constructor Tronco comienza" << endl;
  agregar( MAT_Rotacion(0, 0, 0, 1) ); // MAT param
  agregar( new Brazo(Tupla3f(1,0,0)) );
  agregar( MAT_Traslacion(-5, 0, 0) );
  agregar( new BrazoOrientado(v, 45, Tupla3f(0,0,1) ) );
  agregar( new BrazoOrientado(v, -45, Tupla3f(0,1,0)) );

  v->push_back( Parametro ("Giro del tronco",
                entradas[0].matriz,
                [=](float ang) {return MAT_Rotacion (ang,0,0,1);},
                true, 0, 45, 0.1) );

  // cout << "Constructor Tronco termina" << endl;
}

// -----------------------------------------------------------------------------

C2::C2() {
  // cout << "Constructor C2 comienza" << endl;
  agregar( new Tronco( &parametros ) );
  nombre = "Tronco";
  // cout << "Constructor C2 termina" << endl;
}

// -----------------------------------------------------------------------------

C::C() { }

// -----------------------------------------------------------------------------

C4::C4() {
  ponerNombre( "Clase práctica 4" );
  agregar( new PeonP4(0) );
  agregar( MAT_Rotacion(-45,0,1,0) );
  agregar( new PeonP4(1) );
  agregar( MAT_Rotacion(-45,0,1,0) );
  agregar( new PeonP4(2) );
  agregar( MAT_Escalado(4,4,4) );
  agregar( MAT_Traslacion(0,-0.5,0) );
  agregar( new Lata() );
}

// -----------------------------------------------------------------------------

// 0 = Madera, 1 = Blanco, 2 = Negro
PeonP4::PeonP4(int tipo) {
  agregar( MAT_Traslacion(Tupla3f{5.0,0.0,0.0}) );
  string nombre = "Peon ";
  switch (tipo) {
    case 0:
      agregar( new MaterialPeonMadera()  );
      nombre += "de madera";
      break;
    case 1:
      agregar( new MaterialPeonBlanco()  );
      nombre += "blanco";
      break;
    case 2:
      agregar( new MaterialPeonNegro()  );
      nombre += "negro";
      break;
    default:
      cerr << "\tABORTANDO - Intentando crear peón de tipo desconocido" << endl;
      assert(false);
  }
  ponerNombre( nombre );
  bool textura = tipo == 0;
  agregar( new MallaRevol("../plys/peon.ply", 30, 1, 0, textura) );
  // cout << "Creado: " << nombre << endl;
}

// -----------------------------------------------------------------------------

Lata::Lata () {
  agregar( new TapaSuperiorLata() );
  agregar( new TapaInferiorLata() );
  agregar( new CuerpoLata() );
}

// -----------------------------------------------------------------------------

TapaSuperiorLata::TapaSuperiorLata () {
  agregar( new MaterialTapasLata() );
  agregar( new MallaRevol("../plys/lata-psup.ply", 30, 1, 0, 0) );
}

// -----------------------------------------------------------------------------

TapaInferiorLata::TapaInferiorLata () {
  agregar( new MaterialTapasLata() );
  agregar( new MallaRevol("../plys/lata-pinf.ply", 30, 1, 0, 0) );
}

// -----------------------------------------------------------------------------

CuerpoLata::CuerpoLata () {
  agregar( new MaterialLata() );
  agregar( new MallaRevol("../plys/lata-pcue.ply", 30, 1, 0, 1) );
}
