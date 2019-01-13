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
      std::vector<Tupla3f> normales_caras;
      std::vector<Tupla3f> normales_vertices;
      std::vector<Tupla2f> coordenadas_textura;

      // Tamaños de las tablas de vertices y caras en bytes
      unsigned tam_vertices;
      unsigned tam_caras;
      unsigned tam_texturas;

      // Almacena si los VBOs se han creado ya
      bool vbo_creado = false;

      // Identificador del VBO
      GLuint id_vbo_vertices;
      GLuint id_vbo_caras;
      GLuint id_vbo_colores;
      GLuint id_vbo_normales;
      GLuint id_vbo_texturas;

      // calculo de las normales de esta malla
      void calcular_normales();

      // visualizar en modo inmediato
      void visualizarDE_MI( ContextoVis & cv );
      // visualizar en modo inmediato con 'glVertex3fv'
      void visualizarDE_MI_Vertex( ContextoVis & cv );
      // visualizar en modo inmediato con 'glDrawArrays'
      void visualizarDE_MI_DrawArrays( ContextoVis & cv );
      // visualizar en modo inmediato con 'glDrawElements'
      void visualizarDE_MI_ColoresNormales( ContextoVis & cv );
      // visualizar en modo inmediato con sombreado plano
      void visualizarDE_MI_SombreadoPlano( ContextoVis & cv );
      // visualizar en modo inmediato con sombreado suave (Gouroud)
      void visualizarDE_MI_SombreadoSuave( ContextoVis & cv );
      // visualizar en modo inmediato con sombreado suave (Gouroud)
      void visualizarDE_MI_Texturas( ContextoVis & cv );
      // visualizar en modo inmediato con 'glDrawElements'
      void visualizarDE_MI_Raw_DrawElements( ContextoVis & cv );



      // visualizar en modo inmediato para el Examen
      void visualizarDE_MI_DrawExamen( ContextoVis & cv );

      void ColoresExamen( std::vector<Tupla3f> &colores_sec, int num_ver );

      // Crea un único VBO
      GLuint crearVBO( GLuint tipo, GLuint tam, GLvoid * puntero );

      // Inicializa los VBOs
      void inicializarVBOs();
      // visualizar con 'draw elements', en modo diferido (con VBOS)
      void visualizarDE_VBOs( ContextoVis & cv );
      // visualizar con VBO utilizando las texturas
      void visualizarDE_VBOs_NT( ContextoVis & cv );


      // configurar el modo de los polígonos
      void PolygonMode ( ContextoVis & cv );

      // incializa el array de colores
      void asignarColores();
      virtual void fijarColorNodo(const Tupla3f &nuevo_color);

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
