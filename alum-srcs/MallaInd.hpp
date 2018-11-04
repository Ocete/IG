// *********************************************************************
// **
// ** Informática Gráfica, curso 2016-17
// ** Declaraciones de la clase Objeto3D.hpp
// **
// *********************************************************************

#ifndef IG_MALLAIND_HPP
#define IG_MALLAIND_HPP

#include <vector>          // usar std::vector

#include "Objeto3D.hpp"   // declaración de 'Objeto3D'
// ---------------------------------------------------------------------
// clase para objetos gráficos genéricos

class MallaInd : public Objeto3D
{

   protected:
      std::vector<Tupla3f> vertices;
      std::vector<Tupla3f> colores;
      std::vector<Tupla3i> caras;

      // Tamaños de las tablas de vertices y caras en bytes
      unsigned tam_vertices;
      unsigned tam_caras;

      // Almacena si los VBOs se han creado ya
      bool vbo_creado = false;

      // Identificador del VBO
      GLuint id_vbo_vertices;
      GLuint id_vbo_caras;
      GLuint id_vbo_colores;

      // calculo de las normales de esta malla
      void calcular_normales();

      // visualizar en modo inmediato
      void visualizarDE_MI( ContextoVis & cv );
      // visualizar en modo inmediato con 'glVertex3fv'
      void visualizarDE_MI_Vertex( ContextoVis & cv );
      // visualizar en modo inmediato con 'glVertex3fv'
      void visualizarDE_MI_DrawArrays( ContextoVis & cv );
      // visualizar en modo inmediato con 'glVertex3fv'
      void visualizarDE_MI_DrawElements( ContextoVis & cv );

      // Crea un único VBO
      GLuint crearVBO( GLuint tipo, GLuint tam, GLvoid * puntero );

      // Inicializa los VBOs
      void inicializarVBOs();
      // visualizar con 'draw elements', en modo diferido (con VBOS)
      void visualizarDE_VBOs( ContextoVis & cv );

      // configurar el modo de los polígonos
      void PolygonMode ( ContextoVis & cv );

      // incializa el array de colores
      void asignarColores();

   public:
      // crea una malla vacía (nombre: "malla indexada nueva vacía")
      MallaInd() ;
      // crea una malla vacía con un nombre concreto:
      MallaInd( const std::string & nombreIni );
      // visualizar el objeto con OpenGL
      virtual void visualizarGL( ContextoVis & cv ) ;
} ;
// ---------------------------------------------------------------------

class Cubo : public MallaInd
{
   public:
      Cubo();
      Cubo (float lado);
};
// ---------------------------------------------------------------------

class Tetraedro : public MallaInd
{
   public:
      Tetraedro ();
      Tetraedro (float lado);
};


#endif
