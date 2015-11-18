cmake_minimum_required(VERSION 3.3)
project(ProjetBaseDonnee)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES

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

add_executable(ProjetBaseDonnee ${SOURCE_FILES})