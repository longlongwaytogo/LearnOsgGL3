#--------------------------------------------------------------------------------------
# Copyright (c) longlongwaytogo. All rights reserved.
# longlongway2012@hotmail.com
#--------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.0)
project(LearnOsgGL3)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(use_gl3 "use gl3" ON)
 
set(CMAKE_CXX_STANDARD 17)
cmake_policy(SET CMP0074 NEW)

set(OpenSceneGraph_ROOT "" CACHE PATH "osg root path")
set(OpenSceneGraph_BINARY_ROOT "" CACHE PATH "osg binary root path")

if(NOT OpenSceneGraph_ROOT OR NOT OpenSceneGraph_BINARY_ROOT)
	message(FATAL_ERROR "OpenSceneGraph_ROOT is null")
endif()
SET( include_dir "")

#message(STATUS "modulePath:${CMAKE_MODULE_PATH}")

#EXECUTABLE_OUTPUT_PATH
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

#LIBRARY_OUTPUT_PATH
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)


include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/common.cmake) 
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/findCustomOSG.cmake)

FIND_PACKAGE(OpensceneGraph  COMPONENTS osg osgDB osgUtil osgViewer osgText osgGA osgManipulator osgVolume)
IF(NOT OPENSCENEGRAPH_FOUND)
	MESSAGE("------------------is openscengraph found:${OPENSCENEGRAPH_FOUND}")
ELSE()
	MESSAGE("------------------is openscengraph found:${OPENSCENEGRAPH_FOUND}")
ENDIF()
 # find_package(osgViewer)
 # find_package(osgText) 	
 # find_package(osgUtil)
 # find_package(osgDB)
 # find_package(osgGA)
 # find_package(osgManipulator)
 # find_package(osgVolume)
 # find_package(openThreads)
 # find_package(osgShadow)
 # FIND_PACKAGE(osgFX)

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/include") 
INCLUDE_DIRECTORIES(${OPENSCENEGRAPH_INCLUDE_DIRS} ${OpenSceneGraph_BINARY_ROOT}/include)

Set(OSGLIBPATH ""  CACHE FILEPATH "")
LINK_DIRECTORIES(
	${OPENSCENEGRAPH_LIBRARIES_DIRS}
	${OSGDB_LIBRARIES_DIRS}
	${OSGTEXT_LIBRARIES_DIRS}
	${OSGUTIL_LIBRARIES_DIRS}
	${OSGGA_LIBRARIES_DIRS}
	${OSGMANIPULATOR_LIBRARIES_DIRS}
	${OSG_LIBRARIES_DIRS}
	${OPENTHREADS_LIBRARIES_DIRS}
	${OSGSHADOW_LIBRARIES_DIRS}
	${OSGFX_LIBRARIES_DIRS}
	${OSGLIBPATH}
	 )
	 


#MESSAGE("${OSGLIBPATH}")
#MESSAGE(-----------------------------------)
#MESSAGE("include:${OPENSCENEGRAPH_INCLUDE_DIRS}")
#MESSAGE(-----------------------------------)
#MESSAGE("${OPENSCENEGRAPH_LIBRARIES}")
#MESSAGE(-----------------------------------)
# MESSAGE( ${OPENTHREADS_LIBRARIES_DIRS} )

#MESSAGE("LINK_LIB")
#MESSAGE("${OPENSCENEGRAPH_LIBRARIES}")

LINK_LIBRARIES(${OPENSCENEGRAPH_LIBRARIES})

link_libraries(
${OSG_LIBRARIES}
${OSGVIEWER_LIBRARIES}
${OSGDB_LIBRARIES} 
${OSGUTIL_LIBRARIES}
${OSGTEXT_LIBRARIES}
${OSGGA_LIBRARIES}
${OSGMANIPULATOR_LIBRARIES}
${OPENTHREADS_LIBRARIES}
${OSGSHADOW_LIBRARIES}
${OSGFX_LIBRARIES}
 )
 
 set(MY_CUSTOM_PATH ${OpenSceneGraph_BINARY_ROOT}/bin)
 set(MY_PATH "PATH=${MY_CUSTOM_PATH};%PATH%")
 #message(STATUS "modulePath:${MY_CUSTOM_PATH}")
 include(GenerateExportHeader)
# add cmake module
add_custom_target("cmake_module" SOURCES cmake/common.cmake cmake/findCustomOSG.cmake)
# test
add_subdirectory(src/osgTest)
add_subdirectory(src/common)

# base
add_subdirectory(src/a-base/01-osgSimple3)
add_subdirectory(src/a-base/01-osgSimple3-2)

# sim
add_subdirectory(src/b-sim/02-phong)
add_subdirectory(src/b-sim/02-clip)



# advanced
add_subdirectory(src/d-advanced/01-normalVisualizer)
# geometry shader test
add_subdirectory(src/d-advanced/03-gemoetry-buttlefly)


# graphicAlgorithm
add_subdirectory(src/e-graphicAlgorithm/EffectCompositor)
add_subdirectory(src/e-graphicAlgorithm/Frame)
add_subdirectory(src/e-graphicAlgorithm/SH_Sampler) # 球谐函数 Spherical Harmonics
add_subdirectory(src/e-graphicAlgorithm/DepthToPositionAndNormal) # 深度重建
add_subdirectory(src/e-graphicAlgorithm/001_ReflectiveShadowMap)  # 基于阴影的反射光照
add_subdirectory(src/e-graphicAlgorithm/WindFiled)  # 基于阴影的反射光照

# GPU Pro1
add_subdirectory(src/f-GPUPro1)


# Advance
add_subdirectory(src/z-advance/wind)