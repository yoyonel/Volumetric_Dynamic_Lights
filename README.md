# Volumetric_Dynamic_Lights
Demo Volumetric Dynamic Lights

08 Juin 2015
Travaux sur le scattering et lumière volumique
Trouvé des travaux intéressant sur le temporal filtering + lumière volumique (en 3D)
A approfondir.
J’ai mis en place une version Linux/Mint via CMAKE.
Volumetric Light Scattering and Shadowing with Temporal Up-sampling

http://sebastien.hillaire.free.fr/index.php?option=com_content&view=article&id=72&Itemid=106 (à lire très intéressant)
https://www.youtube.com/watch?feature=player_embedded&v=Zpx7eShJf_Y
Une des ces inspirations issues de GPUGEM: 
Light scattering with openGL

http://fabiensanglard.net/lightScattering/ (ça devrait aussi de compiler assez facilement sous Linux/CMAKE à faire !)

# Pour la version Linux 
je suis parti des sources de la version Windows et je me suis inspiré des CMakeLists.txt de iSpace&Time.
Ça se fait relativement bien mais j’ai eu une erreur un peu mystique lié à OpenGL et NVIDIA: 
Error: “Inconsistency detected by ld.so: dl-version.c: 224”
http://stackoverflow.com/questions/20007961/error-running-a-compiled-c-file-uses-opengl-error-inconsistency-detected
Du coup j’ai spécifié à la main ou chopper la lib OpenGL depuis le cmake-gui: OPENGL_gl_LIBRARY : /usr/lib/nvidia-331-updates/libGL.so
