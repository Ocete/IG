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
   id_vbo_normales = 0;
}

// -----------------------------------------------------------------------------

// Calcula las dos tablas de normales
void MallaInd::calcular_normales() {
  using namespace std;
  if (normales_caras.size() == 0) {

    // Calculamos las normales de las caras
    Tupla3f a,b;
    for (int i=0; i<caras.size(); i++) {
      a = vertices[ caras[i](1) ] - vertices[ caras[i](0) ];
      b = vertices[ caras[i](2) ] - vertices[ caras[i](0) ];
      b = a.cross(b);
      // Normalizamos si no es 0
      if (b(0) != 0 || b(1) != 0 || b(2) != 0) {
        normales_caras.push_back( b.normalized() );
      } else {
        normales_caras.push_back( b );
      }
    }

    // Calculamos las normales de los vertices
    a = {0,0,0};
    normales_vertices = std::vector<Tupla3f> (vertices.size(), a);
    for (int i=0; i<caras.size(); i++) {
      b = normales_caras[i];
      for (int j=0; j<2; j++) {
        normales_vertices[ caras[i](j) ] = b + normales_vertices[ caras[i](j) ];
      }
    }
    for (int i=0; i<vertices.size(); i++) {
      // Normalizamos si no es 0
      if (normales_vertices[i](0) != 0 || normales_vertices[i](1) != 0
            || normales_vertices[i](2) != 0) {
        normales_vertices[i] = normales_vertices[i].normalized();
      }
    }
  }
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

  // Repetimos para las normales
  if (normales_vertices.size() > 0) {
    glEnableClientState ( GL_NORMAL_ARRAY );
    glNormalPointer ( GL_FLOAT, 0, normales_vertices.data() );
  }

  // habilitar punteros a vertices y establecer direccion y estructura de estos
  glEnableClientState( GL_VERTEX_ARRAY );
  glVertexPointer( 3, GL_FLOAT, 0, vertices.data() );

  // Dibujar
  glDrawElements( GL_TRIANGLES, caras.size()*3, GL_UNSIGNED_INT, caras.data() );

  // Inhabilitar punteros
  glDisableClientState( GL_VERTEX_ARRAY );
  glDisableClientState( GL_COLOR_ARRAY );
  glDisableClientState( GL_NORMAL_ARRAY );
}

// -----------------------------------------------------------------------------

void MallaInd::visualizarDE_MI_SombreadoPlano( ContextoVis & cv ) {
  glShadeModel( GL_FLAT );
  visualizarDE_MI_Sombreado(cv);
}

// -----------------------------------------------------------------------------

void MallaInd::visualizarDE_MI_SombreadoSuave( ContextoVis & cv ) {
  glShadeModel( GL_SMOOTH );
  visualizarDE_MI_Sombreado(cv);
}

// -----------------------------------------------------------------------------

void MallaInd::visualizarDE_MI_Sombreado( ContextoVis & cv ) {
  //glBegin( GL_TRIANGLES );
  int index;
  /*if (coordenadas_textura.size() == 0) {
    glColor3fv( Tupla3f{0.7,0,0.7} );
  }

  for ( unsigned i=0; i<caras.size(); i++ ) {
    for( unsigned j=0 ; j<3 ; j++ ) {
      index = caras[i](j);
      if ( coordenadas_textura.size() > 0 ) {
        glTexCoord2fv( coordenadas_textura[ index ] );
      }
      if ( normales_vertices.size() > 0 ) {
        glNormal3fv( normales_vertices[ index ] );
      }
      glVertex3fv( vertices[ index ] );
    }
  }
  glEnd();*/

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);

  glVertexPointer(3, GL_FLOAT, 0, vertices.data());
  glTexCoordPointer(2, GL_FLOAT, 0, coordenadas_textura.data());
  glNormalPointer(GL_FLOAT, 0, normales_vertices.data());

  glDrawElements( GL_TRIANGLES, caras.size()*3, GL_UNSIGNED_INT, caras.data() );

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

// -----------------------------------------------------------------------------

void MallaInd::ColoresExamen( std::vector<Tupla3f> &colores_sec, int num_ver ) {
  colores_sec.clear();
  for ( unsigned i=0; i<num_ver; i++ ) {
    colores_sec.push_back( Tupla3f( (float) (rand() % 100) / 100,
                (float) (rand() % 100) / 100, (float) (rand() % 100) / 100));
  }
}

// -----------------------------------------------------------------------------

void MallaInd::visualizarDE_MI_DrawExamen( ContextoVis & cv ) {
    std::vector<Tupla3f> vertices_sec;
    std::vector<Tupla3f> colores_sec;

    for ( unsigned i=0; i<caras.size(); i++ ) {
      for( unsigned j=0 ; j<3 ; j++ ) {
        vertices_sec.push_back( vertices[ caras[i](j) ] );
      }
    }

    ColoresExamen(colores_sec, vertices.size());

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

void MallaInd::visualizarDE_MI( ContextoVis & cv ) {

  switch (cv.modoVis) {
    case modoExamen:
      glDisable( GL_LIGHTING );
      glDisable( GL_TEXTURE_2D );
      visualizarDE_MI_DrawExamen(cv);
      break;
    case modoSombreadoPlano:
      glEnable( GL_LIGHTING );
      glEnable( GL_TEXTURE_2D );
      visualizarDE_MI_SombreadoPlano(cv);
      break;
    case modoSombreadoSuave:
      glEnable( GL_LIGHTING );
      glEnable( GL_TEXTURE_2D );
      visualizarDE_MI_SombreadoSuave(cv);
      break;
    default:
      glDisable( GL_LIGHTING );
      glDisable( GL_TEXTURE_2D );
      visualizarDE_MI_DrawElements(cv);
      // visualizarDE_MI_Vertex(cv);
      // visualizarDE_MI_DrawArrays(cv);
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
  tam_texturas = sizeof(float)* coordenadas_textura.size() *2L;

  id_vbo_vertices = crearVBO (GL_ARRAY_BUFFER, tam_vertices, vertices.data());
  id_vbo_caras = crearVBO (GL_ELEMENT_ARRAY_BUFFER, tam_caras, caras.data());

  if (colores.size() > 0)
    id_vbo_colores = crearVBO (GL_ARRAY_BUFFER, tam_vertices, colores.data());

  if (normales_vertices.size() > 0)
    id_vbo_normales = crearVBO(GL_ARRAY_BUFFER, tam_vertices,
          normales_vertices.data());

  if (coordenadas_textura.size() > 0)
    id_vbo_texturas = crearVBO (GL_ARRAY_BUFFER, tam_texturas,
          coordenadas_textura.data());

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
  }

  // Configurar VBO de normales
  if ( normales_vertices.size() > 0 ) {
    glBindBuffer( GL_ARRAY_BUFFER, id_vbo_normales );
    glNormalPointer( GL_FLOAT, 0, 0 );
    glEnableClientState( GL_NORMAL_ARRAY );
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
  glDisableClientState( GL_COLOR_ARRAY );
  glDisableClientState( GL_NORMAL_ARRAY );
}

// -----------------------------------------------------------------------------

void MallaInd::visualizarDE_VBOs_NT( ContextoVis & cv ) {
  if ( !vbo_creado ) {
    inicializarVBOs();
  }

  // Configurar VBO de normales
  if ( normales_vertices.size() > 0 ) {
    glBindBuffer( GL_ARRAY_BUFFER, id_vbo_normales );
    glNormalPointer( GL_FLOAT, 0, 0 );
    glEnableClientState( GL_NORMAL_ARRAY );
  }

  // Configurar VBO de texturas
  if ( coordenadas_textura.size() > 0 ) {
    glBindBuffer( GL_ARRAY_BUFFER, id_vbo_texturas );
    glTexCoordPointer( 2, GL_FLOAT, 0, 0 );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
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
  glDisableClientState( GL_NORMAL_ARRAY );
  glDisableClientState( GL_TEXTURE_COORD_ARRAY );
}

// -----------------------------------------------------------------------------

void MallaInd::PolygonMode ( ContextoVis & cv ) {
  GLenum mode;
  switch(cv.modoVis) {
    case modoSolido:
    case modoExamen:
    case modoSombreadoPlano:
    case modoSombreadoSuave:
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
    if (!vbo_creado) {
      inicializarVBOs();
    }
    if (cv.modoVis == modoSombreadoPlano || cv.modoVis == modoSombreadoSuave) {
      visualizarDE_VBOs_NT(cv);
    } else {
      visualizarDE_VBOs(cv);
    }
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

// -----------------------------------------------------------------------------

void MallaInd::fijarColorNodo(const Tupla3f &color) {
  colores.clear();
  for (int i=0; i<=vertices.size(); i++) {
    colores.push_back( color );
  }
}

// *****************************************************************************

// *****************************************************************************

Cubo::Cubo() : Cubo(1) {}

Cubo::Cubo(float lado) : MallaInd( "malla cubo" ) {
  vertices = { Tupla3f(0,0,0) , lado*Tupla3f(1,0,0), lado*Tupla3f(0,1,0),
                lado*Tupla3f(1,1,0), lado*Tupla3f(0,0,1), lado*Tupla3f(1,0,1),
                lado*Tupla3f(0,1,1), lado*Tupla3f(1,1,1)};

  caras = { Tupla3i(0,2,1), Tupla3i(1,2,3), Tupla3i(0,1,4), Tupla3i(1,5,4),
            Tupla3i(1,3,5), Tupla3i(3,7,5), Tupla3i(2,7,3), Tupla3i(2,6,7),
            Tupla3i(0,4,2), Tupla3i(2,4,6), Tupla3i(4,5,6), Tupla3i(5,7,6)};

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
            Tupla3i(0,3,2), Tupla3i(1,2,3)};

  // Esta llamada tiene que realizarse despues de asignar los vertices
  asignarColores();
}
// *****************************************************************************
