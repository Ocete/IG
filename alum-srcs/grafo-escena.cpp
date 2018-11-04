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

void NodoGrafoEscena::visualizarGL( ContextoVis & cv )
{
  // guarda modelview actual
  glMatrixMode( GL_MODELVIEW );
  glPushMatrix() ;
  for( unsigned i = 0 ; i < entradas.size() ; i++ ) {
    if( entradas[i].tipo == TipoEntNGE::objeto ) {
      // si la entrada es sub-objeto, visualizarlo
      entradas[i].objeto->visualizarGL( cv ) ;
    } else if ( entradas[i].tipo == TipoEntNGE::transformacion ) {
      // si la entrada es transformación, componerla
      glMatrixMode( GL_MODELVIEW );
      glMultMatrixf( *(entradas[i].matriz) );
    }
  }
  // restaura modelview guardada
  glMatrixMode( GL_MODELVIEW );
  glPopMatrix();
}
// -----------------------------------------------------------------------------
NodoGrafoEscena::NodoGrafoEscena() {
   // COMPLETAR: práctica 3: inicializar un nodo vacío (sin entradas)
   // ........

}
// -----------------------------------------------------------------------------

void NodoGrafoEscena::fijarColorNodo( const Tupla3f & nuevo_color )
{
   // COMPLETAR: práctica 3: asignarle un color plano al nodo, distinto del padre
   // ........

}

// -----------------------------------------------------------------------------
// Añadir una entrada (al final).
// genérica

unsigned NodoGrafoEscena::agregar( const EntradaNGE & entrada )
{
   entradas.push_back( entrada );
   return entradas.size() - 1;
}
// -----------------------------------------------------------------------------
// construir una entrada y añadirla (al final)
// objeto (copia solo puntero)

unsigned NodoGrafoEscena::agregar( Objeto3D * pObjeto )
{
   return agregar( EntradaNGE( pObjeto ) );
}
// ---------------------------------------------------------------------
// construir una entrada y añadirla (al final)
// matriz (copia objeto)

unsigned NodoGrafoEscena::agregar( const Matriz4f & pMatriz )
{
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

void NodoGrafoEscena::calcularCentroOC()
{

   // COMPLETAR: práctica 5: calcular y guardar el centro del nodo
   //    en coordenadas de objeto (hay que hacerlo recursivamente)
   //   (si el centro ya ha sido calculado, no volver a hacerlo)
   // ........

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
   // COMPLETAR: práctica 5: buscar un sub-objeto con un identificador
   // ........

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

C::C() {
  cout << "Constructor C comienza" << endl;
  agregar( new Atraccion( &parametros ) );
  cout << "Constructor C termina" << endl;
}

// -----------------------------------------------------------------------------

Asiento::Asiento() {
  cout << "Constructor Asiento comienza" << endl;
  agregar( MAT_Escalado(0.5,0.3,0.5) );
  agregar( new Cubo );
  cout << "Constructor Asiento termina" << endl;
}

// -----------------------------------------------------------------------------

AsientoDelantero::AsientoDelantero(float d) {
  cout << "Constructor AsientoDelantero comienza" << endl;
  agregar( MAT_Traslacion(0,0,d) );
  agregar( new Asiento );
  cout << "Constructor AsientoDelantero termina" << endl;
}

// -----------------------------------------------------------------------------

AsientoTrasero::AsientoTrasero(float d) {
  cout << "Constructor AsientoTrasero comienza" << endl;
  agregar( MAT_Traslacion(0,0,-d-0.5) );
  agregar( new Asiento );
  cout << "Constructor AsientoTrasero termina" << endl;
}

// -----------------------------------------------------------------------------

CilindroAsientos::CilindroAsientos() {
  cout << "Constructor CilindroAsientos comienza" << endl;
  agregar( MAT_Rotacion(270,0,0,1) );
  agregar( new Cilindro(10,30,true,true,10,1) );
  cout << "Constructor CilindroAsientos termina" << endl;
}

// -----------------------------------------------------------------------------

FinalBrazoMecanico::FinalBrazoMecanico(vector<Parametro> *v, float d, int num) {
  cout << "Constructor FinalBrazoMecanico comienza" << endl;
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

  cout << "Constructor FinalBrazoMecanico termina" << endl;
}

// -----------------------------------------------------------------------------

BrazoMecanico::BrazoMecanico(vector<Parametro> *v, float d, int num) {
  cout << "Constructor BrazoMecanico comienza" << endl;
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
  cout << "Constructor BrazoMecanico termina" << endl;
}

// -----------------------------------------------------------------------------

BaseBrazos::BaseBrazos(vector<Parametro> *v) {
  cout << "Constructor BaseBrazos comienza" << endl;
  agregar ( MAT_Rotacion(0,0,1,0) );
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
  cout << "Constructor BaseBrazos termina" << endl;
}

// -----------------------------------------------------------------------------

Atraccion::Atraccion(vector<Parametro> *v) {
  cout << "Constructor Atraccion comienza" << endl;

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

  cout << "Constructor BaseBrazos termina" << endl;

}

// -----------------------------------------------------------------------------
