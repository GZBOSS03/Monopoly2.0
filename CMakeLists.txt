cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(Monopoly2.0)

include_directories(include) 

add_library(Giocatori
    include/Giocatore.h
    src/Giocatore.cpp
    include/Computer.h
    src/Computer.cpp
    include/Human.h
    src/Human.cpp
)

add_library(Tabellone
    include/Tabellone.h
    src/Tabellone.cpp
    include/Casella.h
    src/Casella.cpp
    include/Casella_Terreno.h
    src/Casella_Terreno.cpp
    include/Casella_Stazione.h
    include/Casella_Tasse.h
    include/Casella_Societa.h
    include/Casella_Prob_Impr.h
)

add_executable(Game src/Game.cpp)

target_link_libraries(Game Tabellone Giocatori)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
install(TARGETS Game DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)