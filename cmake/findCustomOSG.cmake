# custom find osg

# pre def 
# ${OpenSceneGraph_ROOT}
# ${OpenSceneGraph_BINARY_ROOT}
include(SelectLibraryConfigurations)
include(FindPackageHandleStandardArgs)

set(ExtDir ${OpenSceneGraph_ROOT} ${OpenSceneGraph_BINARY_ROOT})
set(ExtLibDebugDir lib lib/win64_d lib/x64_d lib/Debug)
set(ExtLibReleaseDir lib lib/win64 lib/x64 lib/Release)



#[=======================================================================[.rst:

Findosg_functions
-----------------

This CMake file contains two macros to assist with searching for OSG
libraries and nodekits.  Please see FindOpenSceneGraph.cmake for full
documentation.
#]=======================================================================]


#
# OSG_FIND_PATH
#
function(OSG_FIND_PATH module header)
  string(TOUPPER ${module} module_uc)

  # Try the user's environment request before anything else.
  find_path(${module_uc}_INCLUDE_DIR ${header}
    HINTS
      ENV ${module_uc}_DIR
      ENV OSG_DIR
      ENV OSGDIR
      ENV OSG_ROOT
      ${ExtDir}
	  ${OSG_DIR}
      ${${module_uc}_DIR}
	  
    PATH_SUFFIXES include
  )
endfunction()


#
# OSG_FIND_LIBRARY
#
function(OSG_FIND_LIBRARY module library)
  string(TOUPPER ${module} module_uc)

  find_library(${module_uc}_LIBRARY_RELEASE
    NAMES ${library}
    HINTS
      ENV ${module_uc}_DIR
      ENV OSG_DIR
      ENV OSGDIR
      ENV OSG_ROOT
	  ${ExtDir}
      ${OSG_DIR}
      ${${module_uc}_DIR}
    PATH_SUFFIXES  ${ExtLibReleaseDir}
  )

  find_library(${module_uc}_LIBRARY_DEBUG
    NAMES ${library}d
    HINTS
      ENV ${module_uc}_DIR
      ENV OSG_DIR
      ENV OSGDIR
      ENV OSG_ROOT
	  ${ExtDir}
      ${OSG_DIR}
      ${${module_uc}_DIR}
    PATH_SUFFIXES ${ExtLibDebugDir}
  )

  select_library_configurations(${module_uc})

  # the variables set by select_library_configurations go out of scope
  # here, so we need to set them again
  set(${module_uc}_LIBRARY ${${module_uc}_LIBRARY} PARENT_SCOPE)
  set(${module_uc}_LIBRARIES ${${module_uc}_LIBRARIES} PARENT_SCOPE)
endfunction()

#
# OSG_MARK_AS_ADVANCED
# Just a convenience function for calling MARK_AS_ADVANCED
#
function(OSG_MARK_AS_ADVANCED _module)
  string(TOUPPER ${_module} _module_UC)
  mark_as_advanced(${_module_UC}_INCLUDE_DIR)
  mark_as_advanced(${_module_UC}_LIBRARY)
  mark_as_advanced(${_module_UC}_LIBRARY_DEBUG)
endfunction()


# find osg 
OSG_FIND_PATH   (OSG osg/PositionAttitudeTransform)
OSG_FIND_LIBRARY(OSG osg)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(osg DEFAULT_MSG OSG_LIBRARY OSG_INCLUDE_DIR)

# osgAnimation
OSG_FIND_PATH   (OSGANIMATION osgAnimation/Animation)
OSG_FIND_LIBRARY(OSGANIMATION osgAnimation)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(osgAnimation DEFAULT_MSG
    OSGANIMATION_LIBRARY OSGANIMATION_INCLUDE_DIR)
	
# osgdb
OSG_FIND_PATH   (OSGDB osgDB/DatabasePager)
OSG_FIND_LIBRARY(OSGDB osgDB)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(osgDB DEFAULT_MSG
    OSGDB_LIBRARY OSGDB_INCLUDE_DIR)

#osgFX
OSG_FIND_PATH   (OSGFX osgFX/BumpMapping)
OSG_FIND_LIBRARY(OSGFX osgFX)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(osgFX DEFAULT_MSG
    OSGFX_LIBRARY OSGFX_INCLUDE_DIR)

#osgGA
OSG_FIND_PATH   (OSGGA osgGA/FlightManipulator)
OSG_FIND_LIBRARY(OSGGA osgGA)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(osgGA DEFAULT_MSG
    OSGGA_LIBRARY OSGGA_INCLUDE_DIR)
	
#osgManipulator
OSG_FIND_PATH   (OSGMANIPULATOR osgManipulator/TrackballDragger)
OSG_FIND_LIBRARY(OSGMANIPULATOR osgManipulator)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(osgManipulator DEFAULT_MSG
    OSGMANIPULATOR_LIBRARY OSGMANIPULATOR_INCLUDE_DIR)
	
#osgParticle
OSG_FIND_PATH   (OSGPARTICLE osgParticle/FireEffect)
OSG_FIND_LIBRARY(OSGPARTICLE osgParticle)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(osgParticle DEFAULT_MSG
    OSGPARTICLE_LIBRARY OSGPARTICLE_INCLUDE_DIR)

#osgShadow
OSG_FIND_PATH   (OSGSHADOW osgShadow/ShadowTexture)
OSG_FIND_LIBRARY(OSGSHADOW osgShadow)
 
FIND_PACKAGE_HANDLE_STANDARD_ARGS(osgShadow DEFAULT_MSG
    OSGSHADOW_LIBRARY OSGSHADOW_INCLUDE_DIR)

#osgSim
OSG_FIND_PATH   (OSGSIM osgSim/ImpostorSprite)
OSG_FIND_LIBRARY(OSGSIM osgSim)


FIND_PACKAGE_HANDLE_STANDARD_ARGS(osgSim DEFAULT_MSG
    OSGSIM_LIBRARY OSGSIM_INCLUDE_DIR)
	
#osgTerrain
OSG_FIND_PATH   (OSGTERRAIN osgTerrain/Terrain)
OSG_FIND_LIBRARY(OSGTERRAIN osgTerrain)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(osgTerrain DEFAULT_MSG
    OSGTERRAIN_LIBRARY OSGTERRAIN_INCLUDE_DIR)

#osgText
OSG_FIND_PATH   (OSGTEXT osgText/Text)
OSG_FIND_LIBRARY(OSGTEXT osgText)

	
FIND_PACKAGE_HANDLE_STANDARD_ARGS(osgText DEFAULT_MSG
    OSGTEXT_LIBRARY OSGTEXT_INCLUDE_DIR)

# osgUtil
OSG_FIND_PATH   (OSGUTIL osgUtil/SceneView)
OSG_FIND_LIBRARY(OSGUTIL osgUtil)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(osgUtil DEFAULT_MSG
    OSGUTIL_LIBRARY OSGUTIL_INCLUDE_DIR)

#osgViewer
OSG_FIND_PATH   (OSGVIEWER osgViewer/Viewer)
OSG_FIND_LIBRARY(OSGVIEWER osgViewer)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(osgViewer DEFAULT_MSG
    OSGVIEWER_LIBRARY OSGVIEWER_INCLUDE_DIR)
	
#osgVolume
OSG_FIND_PATH   (OSGVOLUME osgVolume/Volume)
OSG_FIND_LIBRARY(OSGVOLUME osgVolume)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(osgVolume DEFAULT_MSG
    OSGVOLUME_LIBRARY OSGVOLUME_INCLUDE_DIR)
	
#osgWidget
OSG_FIND_PATH   (OSGWIDGET osgWidget/Widget)
OSG_FIND_LIBRARY(OSGWIDGET osgWidget)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(osgWidget DEFAULT_MSG
    OSGWIDGET_LIBRARY OSGWIDGET_INCLUDE_DIR)

# find openthread
find_path(OPENTHREADS_INCLUDE_DIR OpenThreads/Thread
    HINTS
        ENV OPENTHREADS_INCLUDE_DIR
        ENV OPENTHREADS_DIR
        ENV OSG_INCLUDE_DIR
        ENV OSG_DIR
        ENV OSGDIR
        ENV OpenThreads_ROOT
        ENV OSG_ROOT
        ${OPENTHREADS_DIR}
        ${OSG_DIR}
		${ExtDir}
    PATH_SUFFIXES include 
)

#MESSAGE("opentrhead:${OPENTHREADS_INCLUDE_DIR}")

find_library(OPENTHREADS_LIBRARY_RELEASE
    NAMES OpenThreads OpenThreadsWin32
    HINTS
        ENV OPENTHREADS_LIBRARY_DIR
        ENV OPENTHREADS_DIR
        ENV OSG_LIBRARY_DIR
        ENV OSG_DIR
        ENV OSGDIR
        ENV OpenThreads_ROOT
        ENV OSG_ROOT
        ${OPENTHREADS_DIR}
        ${OSG_DIR}
		${ExtDir}
    PATH_SUFFIXES ${ExtLibReleaseDir}
)
#MESSAGE("opentrhead release:${ExtLibReleaseDir}---${OPENTHREADS_LIBRARY_RELEASE}")

find_library(OPENTHREADS_LIBRARY_DEBUG
    NAMES OpenThreadsd OpenThreadsWin32d
    HINTS
        ENV OPENTHREADS_DEBUG_LIBRARY_DIR
        ENV OPENTHREADS_LIBRARY_DIR
        ENV OPENTHREADS_DIR
        ENV OSG_LIBRARY_DIR
        ENV OSG_DIR
        ENV OSGDIR
        ENV OpenThreads_ROOT
        ENV OSG_ROOT
        ${OPENTHREADS_DIR}
        ${OSG_DIR}
		${ExtDir}
    PATH_SUFFIXES ${ExtLibDebugDir}
)

select_library_configurations(OPENTHREADS)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(OpenThreads DEFAULT_MSG
    OPENTHREADS_LIBRARY OPENTHREADS_INCLUDE_DIR)
	
#IF(NOT OPENSCENEGRAPH_INC_DIRS)
set(OPENSCENEGRAPH_INC_DIRS ${OPENSCENEGRAPH_INC_DIR} ${OpenSceneGraph_ROOT}/include ${OpenSceneGraph_BINARY_ROOT}/include)
#ENDIF()
#IF(NOT OPENSCENEGRAPH_LIBRARIES)
SET(OPENSCENEGRAPH_LIBRARIES 
${OSG_LIBRARIES}
${OSGDB_LIBRARIES}
${OSGVIEWER_LIBRARIES}
${OSGGA_LIBRARIES}
${OSGUTIL_LIBRARIES}
${OSGFX_LIBRARIES}
${OSGMANIPULATOR_LIBRARIES}
${OSGANIMATION_LIBRARIES}
${OSGTERRAIN_LIBRARIES}
${OSGPARTICLE_LIBRARIES}
${OPENTHREADS_LIBRARIES}
${OSGTEXT_LIBRARIES}
${OSGSHADOW_LIBRARIES}
${OSGSIM_LIBRARIES}
${OSGWIDGET_LIBRARIES}
)
#ENDIF()
MESSAGE("${OSGFX_LIBRARIES}")
MESSAGE("${OSGUTIL_LIBRARIES}")
MESSAGE("----lib:${OPENSCENEGRAPH_LIBRARIES}")
MESSAGE("----inc:${OPENSCENEGRAPH_INC_DIRS}")