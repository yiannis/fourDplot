CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT( fourDplot )

SET( CMAKE_BUILD_TYPE "RelWithDebInfo" )

## Find the Parser files ##
FIND_PATH( PARSER_SOURCE_DIR
           NAMES Expr.cc Function.cc Token.cc Lexer.cc Parser.cc
           PATHS "../calculator/" "../" "/usr/local/src/"
           DOC "Parser source directory" )

if(PARSER_SOURCE_DIR)
  MESSAGE( "-- Parser found: ${PARSER_SOURCE_DIR}" )
else(PARSER_SOURCE_DIR)
    MESSAGE(FATAL_ERROR "Parser NOT found. Please see the README file." )
endif(PARSER_SOURCE_DIR)

SET( PARSER_SOURCES
  "${PARSER_SOURCE_DIR}/ASTVisitorExecutor.cc"
  "${PARSER_SOURCE_DIR}/ASTVisitorLLVMIR.cc"
  "${PARSER_SOURCE_DIR}/Compiler.cc"
  "${PARSER_SOURCE_DIR}/Expr.cc"
  "${PARSER_SOURCE_DIR}/Function.cc"
  "${PARSER_SOURCE_DIR}/Lexer.cc"
  "${PARSER_SOURCE_DIR}/Parser.cc"
  "${PARSER_SOURCE_DIR}/Token.cc"
)


# For llvm
FIND_PROGRAM(LLVM_CONFIG_EXECUTABLE
    NAMES llvm-config llvm-config-3.0
)
EXEC_PROGRAM(${LLVM_CONFIG_EXECUTABLE} ARGS --includedir OUTPUT_VARIABLE LLVM_INCLUDE_DIRS )
EXEC_PROGRAM(${LLVM_CONFIG_EXECUTABLE} ARGS --cxxflags   OUTPUT_VARIABLE LLVM_CXX_FLAGS )
EXEC_PROGRAM(${LLVM_CONFIG_EXECUTABLE} ARGS --ldflags    OUTPUT_VARIABLE LLVM_LDFLAGS )
EXEC_PROGRAM(${LLVM_CONFIG_EXECUTABLE} ARGS --libs       OUTPUT_VARIABLE LLVM_LIBS )
SET( LLVM_LIBRARIES ${LLVM_LIBS} ${LLVM_LDFLAGS} )
SET( LLVM_CXX_FLAGS "${LLVM_CXX_FLAGS} -fexceptions" )
SET_SOURCE_FILES_PROPERTIES(
  "Surface.cc"
  "glCalls.cc"
  "${PARSER_SOURCE_DIR}/ASTVisitorLLVMIR.cc"
  "${PARSER_SOURCE_DIR}/Compiler.cc"
  PROPERTIES COMPILE_FLAGS ${LLVM_CXX_FLAGS}
)

FIND_PACKAGE( Boost       REQUIRED COMPONENTS program_options )
FIND_PACKAGE( ImageMagick REQUIRED COMPONENTS Magick++ )
FIND_PACKAGE( GLUT        REQUIRED )
FIND_PACKAGE( OpenGL      REQUIRED )

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
if (NOT OPENGL_GLU_FOUND)
  MESSAGE(FATAL_ERROR "Did not find Mesa OpenGL utility library (GLU) development files.\nOn Ubuntu try:\n# apt-get install libglu1-mesa-dev"
    )
endif (NOT OPENGL_GLU_FOUND)

INCLUDE_DIRECTORIES(
  ${LLVM_INCLUDE_DIRS}
  ${PARSER_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${ImageMagick_INCLUDE_DIRS}
  ${GLUT_INCLUDE_DIR} )

#LINK_DIRECTORIES(
#  ${PARSER_SOURCE_DIR}
#)

SET( PLOT_SOURCES
  "Arguments.cc"
  "Color.cc"
  "glCalls.cc"
  "Surface.cc"
)

##################TARGETS##################
#plot4D#
ADD_LIBRARY( plot SHARED ${PARSER_SOURCES} ${PLOT_SOURCES} )
ADD_EXECUTABLE( plot4D "main.cc" )
SET_TARGET_PROPERTIES( plot4D PROPERTIES COMPILE_FLAGS ${LLVM_CXX_FLAGS} )
TARGET_LINK_LIBRARIES( plot4D
  "plot"
  ${LLVM_LIBRARIES}
  ${Boost_LIBRARIES}
  ${ImageMagick_LIBRARIES}
  ${GLUT_LIBRARIES}
  ${OPENGL_glu_LIBRARY}
)
