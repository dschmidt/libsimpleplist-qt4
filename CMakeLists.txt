CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

## project settings
PROJECT( libsimpleplist-qt4 )
SET( SIMPLEPLIST_MAJOR_VERSION 0 )
SET( SIMPLEPLIST_MINOR_VERSION 0 )
SET( SIMPLEPLIST_PATCH_VERSION 0 )

SET( SIMPLEPLIST_VERSION ${SIMPLEPLIST_MAJOR_VERSION}.${SIMPLEPLIST_MINOR_VERSION}.${SIMPLEPLIST_PATCH_VERSION} )
SET( SIMPLEPLIST_SONAME ${SIMPLEPLIST_MAJOR_VERSION}.${SIMPLEPLIST_MINOR_VERSION} )

MESSAGE(STATUS "Building libsimpleplist-qt4 " ${SIMPLEPLIST_VERSION} )

# cmake modules
SET( CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_SOURCE_DIR}/cmake/modules" )

## required deps
# qt4
FIND_PACKAGE( Qt4 COMPONENTS QtCore QtXml REQUIRED )
INCLUDE( ${QT_USE_FILE} )

# definitions
ADD_DEFINITIONS( ${QT_DEFINITIONS} )
ADD_DEFINITIONS( -DQT_SHARED )
ADD_DEFINITIONS( -DSIMPLEPLIST_MAKEDLL )

# build
INCLUDE_DIRECTORIES(
    .
    ${QT_INCLUDES}
)


SET( SIMPLEPLIST_MOC_HEADERS simplepropertylistreader.h )
SET( SIMPLEPLIST_SOURCES simplepropertylistreader.cpp )
SET( SIMPLEPLIST_HEADERS ${SIMPLEPLIST_MOC_HEADERS} )

QT4_WRAP_CPP( SIMPLEPLIST_MOC ${SIMPLEPLIST_MOC_HEADERS} )

ADD_LIBRARY( simpleplist-qt4 SHARED ${SIMPLEPLIST_SOURCES} ${SIMPLEPLIST_MOC} )

SET_TARGET_PROPERTIES( simpleplist-qt4 PROPERTIES
    VERSION ${SIMPLEPLIST_VERSION}
    SOVERSION ${SIMPLEPLIST_SONAME}
)

TARGET_LINK_LIBRARIES( simpleplist-qt4
    ${QT_LIBRARIES}
)

INSTALL( TARGETS simpleplist-qt4
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib${LIB_SUFFIX}
    ARCHIVE DESTINATION lib${LIB_SUFFIX}
    BUNDLE DESTINATION library
)

INSTALL( FILES ${SIMPLEPLIST_HEADERS}
    DESTINATION include/simpleplist-qt4
)
