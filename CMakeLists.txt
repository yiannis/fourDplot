CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT( fourDplot )

SET( CMAKE_BUILD_TYPE "RelWithDebInfo" )

SET( PARSER_SOURCES
  "../calculator/Expr.cc"
  "../calculator/Function.cc"
  "../calculator/Token.cc"
  "../calculator/Lexer.cc"
  "../calculator/Parser.cc" )

FIND_PACKAGE( Boost       REQUIRED COMPONENTS program_options )
FIND_PACKAGE( ImageMagick REQUIRED COMPONENTS Magick++ )
FIND_PACKAGE( GLUT        REQUIRED )

if (NOT ImageMagick_Magick++_FOUND)
  MESSAGE(FATAL_ERROR
    "Did not find ImageMagick C++ development files.\nOn ubuntu try:\n# apt-get install libmagick++-dev"
    )
endif (NOT ImageMagick_Magick++_FOUND)
if (NOT Boost_FOUND)
  MESSAGE(FATAL_ERROR
    "Did not find Boost program_options.\nOn Ubuntu try:\n# apt-get install libboost-program-options-dev"
    )
endif (NOT Boost_FOUND)
if (NOT GLUT_FOUND)
  MESSAGE(FATAL_ERROR "Did not find Glut development files.\nOn Ubuntu try:\n# apt-get install freeglut3-dev"
    )
endif (NOT GLUT_FOUND)

INCLUDE_DIRECTORIES(
  "${CMAKE_SOURCE_DIR}/../calculator/"
  ${Boost_INCLUDE_DIRS}
  ${ImageMagick_INCLUDE_DIRS}
  ${GLUT_INCLUDE_DIR} )

SET( PLOT_SOURCES
  "Arguments.cc"
  "Color.cc"
  "glCalls.cc"
  "Surface.cc")


##################TARGETS##################
#plot4D#
ADD_EXECUTABLE( plot4D "main.cc" ${PLOT_SOURCES} ${PARSER_SOURCES} )
TARGET_LINK_LIBRARIES( plot4D ${Boost_LIBRARIES} ${ImageMagick_LIBRARIES} ${GLUT_LIBRARIES} )
