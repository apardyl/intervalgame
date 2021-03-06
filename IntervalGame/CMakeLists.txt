CMAKE_MINIMUM_REQUIRED( VERSION 3.0 )
PROJECT( IntervalGame CXX )

IF( ${UNIX} )
  SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -pthread -std=c++11 -fvisibility=hidden" )
  SET( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb" )
  SET( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast -flto -march=native" )
ELSEIF( ${MSVC} )
  SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" )
  SET( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}" )
  SET( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ox" )
ENDIF()
    
SET( PROJECT_SOURCE_DIR "IntervalGame" )

SET(IntervalGame_SOURCES
  ${PROJECT_SOURCE_DIR}/Array4Bit.cpp
  ${PROJECT_SOURCE_DIR}/IntervalGraph.cpp
  ${PROJECT_SOURCE_DIR}/IntervalGame.cpp
)

ADD_EXECUTABLE( game ${IntervalGame_SOURCES} )
