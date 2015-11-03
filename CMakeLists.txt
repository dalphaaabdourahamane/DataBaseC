cmake_minimum_required(VERSION 3.3)
project(c__)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    .idea/dictionaries/DIALLO_Alpha_Abdoura.xml
    .idea/.name
    .idea/misc.xml
    .idea/modules.xml
    .idea/ProjetBaseDonnee.iml
    .idea/vcs.xml
    .idea/workspace.xml
    HEADER/Affichage.h
    HEADER/DataBase.h
    HEADER/Metadonnee.h
    HEADER/Structure.h
    HEADER/Utile.h
    CMakeLists.txt
    main.cpp)

add_executable(c__ ${SOURCE_FILES})