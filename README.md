# IG

Este repositorio recoge mis prácticas de IG. Actualmente trabajando en la práctica 4 (sin terminar)

Trabajo de las prácticas anteriores:
  - P123: Añadir materiales

Trabajo de la P4:
  - modoMateriales no se ve bien (ahora mismo no existe este modo)
  - Bug: hay un ángulo de la cámara que aunque se mueva no afecta
  - Bug: en la primera vez que cambio a modoSombreadoPlano

Trabajo de la P5:
  - Quitar el modoSeleccion
  - Bug: en modo orbital desplaZ no hace nada.
  - Bug: repasar los calculos del modo ortografico
  - Bug: la seleccion no coge el objeto esperado, obtiene un identificador del color normal.

Como abordar la practica 5:

La practica 5 tiene dos bloques importantes: las cámaras y la selección.
El funcionamiento de la camara esta explicado en el principio del tema 3, las primeras 50 diapos.
Como configurar la cámara para nuestra practica esta explicado en el tema 4.
El funcionamiento del modo selección está explicado en el tema 4.

Centrarse primero en el bloque de la cámara, leer las diapositivas y el guión de ese apartado muy despacio y entendiendolo bien (son pocas diapositivas, solo hay que tener paciencia). Una vez hecho esto, abordar las cámaras. El archivo Camara.cpp está completo de por si (pero nos hace falta entenderlo), completaremos CamaraInteractiva.cpp.
Crear una camara de cada tipo - perspectiva / ortogonal - y probar que funcionan decentemente, mas o menos. Para ver que las camaras van bien conviene implementar las funciones de pulsacion de teclas de practica5.cpp e instanciar un grafo-escena de practicas anteriores.

Una vez las camaras estan mas o menos terminadas, leer el bloque de selección del guión y de las diapositivas. Nos explican dos formas de implementar esto. Utilizaremos la segunda que aparece en las diapositivas (subsección 5.2). La primera es prescindible mirarsela para entender la segunda y no la usaremos. Para la implementacion utilizaremos un modo nuevo que NO se añade a los modos vistos anteriormente. En vez de eso utilizamos modoVis.modoSeleccionFBO.
