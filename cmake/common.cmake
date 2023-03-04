#--------------------------------------------------------------------------------------
# Copyright (c) longlongwaytogo. All rights reserved.
# longlongway2012@hotmail.com
#--------------------------------------------------------------------------------------

# CreateExe 
# 创建一个基于osg的可执行程序
# targetName; 目标名称
# package： 所属于的包
SET(resource_file "")
function(CreateModule targetName package isLib) 

	if(${isLib})
	message(STATUS "create lib project:${targetName}")
	elseif()
	message(STATUS "create exe project:${targetName}")
	endif()
	FILE(GLOB_RECURSE src . *.cpp *.cxx)
	FILE(GLOB_RECURSE inc  . *.h *.hpp ) 
	
	set(txtFile "")
	FILE(GLOB_RECURSE txtFile  . *.md *.txt) 
	set(all_Shader "")
	FILE(GLOB_RECURSE shader  . *.vs) 
	set(all_Shader ${all_Shader} ${shader})
	FILE(GLOB_RECURSE shader  . *.fs) 
	set(all_Shader ${all_Shader} ${shader})
	FILE(GLOB_RECURSE shader  . *.gs) 
	set(all_Shader ${all_Shader} ${shader})
	FILE(GLOB_RECURSE shader  . *.vert) 
	set(all_Shader ${all_Shader} ${shader})
	FILE(GLOB_RECURSE shader  . *.frag) 
	set(all_Shader ${all_Shader} ${shader})
	FILE(GLOB_RECURSE shader  . *.geom) 
	set(all_Shader ${all_Shader} ${shader})
	FILE(GLOB_RECURSE shader  . *.compute) 
	set(all_Shader ${all_Shader} ${shader})
		FILE(GLOB_RECURSE shader  . *.glsl) 
	set(all_Shader ${all_Shader} ${shader})


	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} PREFIX "Shader Files" FILES ${all_Shader})
	#message(STATUS, "all shader:${all_Shader}")
	IF(${isLib})
	
		string(TOUPPER "${targetName}" TARGETNAME)
		add_library(${targetName} ${src} ${inc}  ${all_Shader} ${txtFile} ${resource_file})
		generate_export_header(${targetName} 
		EXPORT_MACRO_NAME  ${TARGETNAME}_API 
		EXPORT_FILE_NAME "${CMAKE_CURRENT_SOURCE_DIR}/${targetName}_export.h" )
		#message(STATUS "--------------current_dir:"${CMAKE_CURRENT_SOURCE_DIR})
	ELSE()
		ADD_EXECUTABLE(${targetName} ${src} ${inc}  ${all_Shader}  ${txtFile} ${resource_file})
	ENDIF()
 

	SET_PROPERTY(TARGET ${TargetName} PROPERTY FOLDER ${package})
	SET_TARGET_PROPERTIES(${TargetName} PROPERTIES VS_DEBUGGER_ENVIRONMENT "${MY_PATH}")
	add_definitions(-DshaderPath="${CMAKE_CURRENT_SOURCE_DIR}")
	add_definitions(-DsourcePath="${CMAKE_CURRENT_SOURCE_DIR}")
endfunction()

 
 # macro
macro(CreateExe targetName package)
CreateModule(${targetName} ${package} OFF)
endmacro()
# endmacro

 # macro
macro(CreateLib targetName package)
CreateModule(${targetName} ${package} ON)
endmacro()
# endmacro