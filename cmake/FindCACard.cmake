# FindCACard.cmake

# CACard_FOUND - System has CACard
# CACard_INCLUDES - The CACard include directories
# CACard_LIBRARIES - The libraries needed to use CACard
# CACard_DEFINITIONS - Compiler switches required for using CACard


IF (CACard_INCLUDE_DIRS AND CACard_LIBRARIES)
   # in cache already
   SET(CACard_FIND_QUIETLY TRUE)
ELSE (CACard_INCLUDE_DIRS AND CACard_LIBRARIES)
   SET(CACard_FIND_QUIETLY FALSE)
ENDIF (CACard_INCLUDE_DIRS AND CACard_LIBRARIES)

IF (NOT WIN32)
   # use pkg-config to get the directories and then use these values
   # in the FIND_PATH() and FIND_LIBRARY() calls
   FIND_PACKAGE(PkgConfig)
   PKG_CHECK_MODULES(PC_CACard libcacard)
   #MESSAGE(STATUS "DEBUG: CACard include directory = ${CACard_INCLUDE_DIRS}")
   #MESSAGE(STATUS "DEBUG: CACard link directory = ${CACard_LIBRARY_DIRS}")
   #MESSAGE(STATUS "DEBUG: CACard CFlags = ${CACard_CFLAGS}")
   SET(CACard_DEFINITIONS  ${PC_CACard_CFLAGS} ${PC_CACard_CFLAGS_OTHER})
ENDIF (NOT WIN32)

FIND_PATH(CACard_INCLUDE_DIR vreader.h
   HINTS ${PC_CACard_INCLUDES} ${PC_CACard_INCLUDE_DIRS}
   PATH_SUFFIXES cacard/
   )

FIND_LIBRARY(_CACardLibs NAMES cacard libcacard
   PATHS
   ${PC_CACard_LIBDIR}
   ${PC_CACard_LIBRARY_DIRS}
   )

list(APPEND CACard_LIBRARIES ${_CACardLibs} )
list(APPEND CACard_INCLUDE_DIRS ${CACard_INCLUDE_DIR} )

IF (CACard_INCLUDE_DIRS AND CACard_LIBRARIES)
   SET(CACard_FOUND TRUE)
ELSE (CACard_INCLUDE_DIRS AND CACard_LIBRARIES)
   SET(CACard_FOUND FALSE)
ENDIF (CACard_INCLUDE_DIRS AND CACard_LIBRARIES)

IF (CACard_FOUND)
   IF (NOT CACard_FIND_QUIETLY)
      MESSAGE(STATUS "Found CACard libraries: ${CACard_LIBRARIES}")
      MESSAGE(STATUS "Found CACard includes : ${CACard_INCLUDE_DIRS}")
   ENDIF (NOT CACard_FIND_QUIETLY)
ELSE (CACard_FOUND)
    IF (CACard_FIND_REQUIRED)
      MESSAGE(STATUS "Could NOT find CACard")
    ENDIF(CACard_FIND_REQUIRED)
ENDIF (CACard_FOUND)

MARK_AS_ADVANCED(CACard_INCLUDE_DIRS CACard_LIBRARIES)
