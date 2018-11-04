// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Declaraciones de la clase Objeto3D.hpp
// **
// *********************************************************************

#include <aux.hpp>
#include <tuplasg.hpp>
#include "MallaInd.hpp"     // declaración de 'ContextoVis'
//#include <file_ply_stl.hpp> // Para  ply::read

// *****************************************************************************
// funciones auxiliares


// *****************************************************************************
// métodos de la clase MallaInd.

MallaInd::MallaInd()
{
   // 'identificador' puesto a 0 por defecto, 'centro_oc' puesto a (0,0,0)
   ponerNombre("malla indexada, anónima");
}
// -----------------------------------------------------------------------------

MallaInd::MallaInd( const std::string & nombreIni )
{
   // 'identificador' puesto a 0 por defecto, 'centro_oc' puesto a (0,0,0)
   ponerNombre(nombreIni) ;
   vbo_creado = false;
   id_vbo_vertices = 0;
   id_vbo_caras = 0;
   id_vbo_colores = 0;
}

// -----------------------------------------------------------------------------

// calcula las dos tablas de normales
void MallaInd::calcular_normales()
{
   // COMPLETAR: en la práctica 2: calculo de las normales de la malla
   // .......


}

// -----------------------------------------------------------------------------

void MallaInd::visualizarDE_MI_Vertex( ContextoVis & cv ) {
  glBegin( GL_TRIANGLES );
  glColor3fv( Tupla3f{0,0.7,0} );
  for ( unsigned i=0; i<caras.size(); i++ ) {
    for( unsigned j=0 ; j<3 ; j++ ) {
      glVertex3fv( vertices[ caras[i](j) ] );
    }
  }
  glEnd();
}

// -----------------------------------------------------------------------------

void MallaInd::visualizarDE_MI_DrawArrays( ContextoVis & cv ) {
    std::vector<Tupla3f> vertices_sec;
    std::vector<Tupla3f> colores_sec;

    for ( unsigned i=0; i<caras.size(); i++ ) {
      for( unsigned j=0 ; j<3 ; j++ ) {
        vertices_sec.push_back( vertices[ caras[i](j) ] );
        colores_sec.push_back( colores[ caras[i](j) ] );
      }
    }

    // Si hay colores, habilitar punteros a colores y establecer direccion y
    // estructura de estos
    if (colores.size() > 0) {
      glEnableClientState( GL_COLOR_ARRAY );
      glColorPointer( 3, GL_FLOAT, 0, colores_sec.data() );
    }

    // habilitar punteros a vertices y establecer direccion y estructura de estos
    glEnableClientState( GL_VERTEX_ARRAY );
    glVertexPointer( 3, GL_FLOAT, 0, vertices_sec.data() );

    // dibujar
    glDrawArrays( GL_TRIANGLES, 0, vertices_sec.size() );

    // inhabilitar punteros
    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_COLOR_ARRAY );
}

// -----------------------------------------------------------------------------

void MallaInd::visualizarDE_MI_DrawElements( ContextoVis & cv ) {
  // Si hay colores, habilitar punteros a colores y establecer direccion y
  // estructura de estos
  if (colores.size() > 0) {
    glEnableClientState( GL_COLOR_ARRAY );
    glColorPointer( 3, GL_FLOAT, 0, colores.data() );
  }

  // habilitar punteros a vertices y establecer direccion y estructura de estos
  glEnableClientState( GL_VERTEX_ARRAY );
  glVertexPointer( 3, GL_FLOAT, 0, vertices.data() );

  // dibujar
  glDrawElements( GL_TRIANGLES, caras.size()*3, GL_UNSIGNED_INT, caras.data() );

  // inhabilitar punteros
  glDisableClientState( GL_VERTEX_ARRAY );
  glDisableClientState( GL_COLOR_ARRAY );
}

// -----------------------------------------------------------------------------

void MallaInd::visualizarDE_MI( ContextoVis & cv ) {
  typedef enum { modoVertex, modoDrawArrays, modoDrawElements} modo_directo;
  modo_directo mod = modoDrawElements;

  switch (mod) {
    case modoVertex:
      visualizarDE_MI_Vertex(cv);
      break;
    case modoDrawArrays:
      visualizarDE_MI_DrawArrays(cv);
      break;
    case modoDrawElements:
      visualizarDE_MI_DrawElements(cv);
  }
}

// ----------------------------------------------------------------------------

GLuint MallaInd::crearVBO( GLuint tipo, GLuint tam, GLvoid * puntero ) {
  assert( tipo == GL_ARRAY_BUFFER || tipo == GL_ELEMENT_ARRAY_BUFFER );
  GLuint id_vbo ;
  // crear nuevo VBO, obtener identificador
  glGenBuffers( 1, &id_vbo );
  // activar el VBO usando su identificador
  glBindBuffer( tipo, id_vbo );
  // transferencia de datos
  glBufferData( tipo, tam, puntero, GL_STATIC_DRAW );
  // desactivación del VBO (activar 0)
  glBindBuffer( tipo, 0 );

  return id_vbo ;
}

// ----------------------------------------------------------------------------

void MallaInd::inicializarVBOs() {
  tam_vertices = sizeof(float)* vertices.size() *3L;
  tam_caras = sizeof(int)* caras.size() *3L;

  id_vbo_vertices = crearVBO (GL_ARRAY_BUFFER, tam_vertices, vertices.data());
  id_vbo_caras = crearVBO (GL_ELEMENT_ARRAY_BUFFER, tam_caras, caras.data());

  if (colores.size() > 0)
    id_vbo_colores = crearVBO (GL_ARRAY_BUFFER, tam_vertices, colores.data());

  vbo_creado = true;
}

// ----------------------------------------------------------------------------

void MallaInd::visualizarDE_VBOs( ContextoVis & cv ) {
  if ( !vbo_creado ) {
    inicializarVBOs();
  }

  // Configurar VBO de colores
  if ( colores.size() > 0 ) {
    glBindBuffer( GL_ARRAY_BUFFER, id_vbo_colores );
    glColorPointer( 3, GL_FLOAT, 0, 0 );
    glEnableClientState( GL_COLOR_ARRAY );
    glBindBuffer( GL_ARRAY_BUFFER, 0);
  }
  // Configurar VBO de vertices
  glBindBuffer( GL_ARRAY_BUFFER, id_vbo_vertices );
  glVertexPointer( 3, GL_FLOAT, 0, 0 );
  glBindBuffer( GL_ARRAY_BUFFER, 0 ); // creo que esto sobra
  glEnableClientState( GL_VERTEX_ARRAY );

  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, id_vbo_caras );
  glDrawElements( GL_TRIANGLES, 3L*caras.size(), GL_UNSIGNED_INT, 0 ) ;
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

  glDisableClientState( GL_VERTEX_ARRAY );
  if (colores.size() > 0)
    glDisableClientState( GL_COLOR_ARRAY );
}

// -----------------------------------------------------------------------------

void MallaInd::PolygonMode ( ContextoVis & cv ) {
  GLenum mode;
  switch(cv.modoVis) {
    case modoSolido:
      mode = GL_FILL;
      break;
    case modoPuntos:
      mode = GL_POINT;
      break;
    default:
      mode = GL_LINE;
  }
  glPolygonMode( GL_FRONT_AND_BACK, mode);
}

// -----------------------------------------------------------------------------

void MallaInd::visualizarGL( ContextoVis & cv ) {
  PolygonMode( cv );

  if (cv.usarVBOs) {
   visualizarDE_VBOs(cv);
  } else {
   visualizarDE_MI(cv);
  }
}

// -----------------------------------------------------------------------------

void MallaInd::asignarColores() {
  int n = vertices.size()-1;
  for (int i=0; i<=n; i++) {
    colores.push_back( Tupla3f( (float) (n-i)/n, 0, (float) i/n ) );
  }
}

// *****************************************************************************

// *****************************************************************************

Cubo::Cubo() : Cubo(1) {}

Cubo::Cubo(float lado) : MallaInd( "malla cubo" ) {
  vertices = { Tupla3f(0,0,0) , lado*Tupla3f(1,0,0), lado*Tupla3f(0,1,0),
                lado*Tupla3f(1,1,0), lado*Tupla3f(0,0,1), lado*Tupla3f(1,0,1),
                lado*Tupla3f(0,1,1), lado*Tupla3f(1,1,1)};

  caras = { Tupla3i(0,2,1), Tupla3i(1,2,3), Tupla3i(0,4,1), Tupla3i(1,4,5),
            Tupla3i(1,5,3), Tupla3i(3,5,7), Tupla3i(2,3,7), Tupla3i(2,7,6),
            Tupla3i(0,4,2), Tupla3i(2,4,6), Tupla3i(4,6,5), Tupla3i(5,6,7)};

  // Esta llamada tiene que realizarse despues de asignar los vertices
  asignarColores();
}
// *****************************************************************************

Tetraedro::Tetraedro() : Tetraedro(1) {}

Tetraedro::Tetraedro(float lado) : MallaInd( "malla tetraedro") {
  vertices = { Tupla3f(0,0,0) , lado*Tupla3f(1,0,0),
              lado*Tupla3f(0.5,sqrt(0.75),0) ,
              lado*Tupla3f(0.5,sqrt(0.75/3),2*sqrt(2*0.75)/3)};

  caras = { Tupla3i(0,2,1), Tupla3i(0,1,3),
            Tupla3i(0,2,3), Tupla3i(1,2,3)};

  // Esta llamada tiene que realizarse despues de asignar los vertices
  asignarColores();
}
// *****************************************************************************
