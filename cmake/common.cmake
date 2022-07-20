#
# CreateExe 
# 创建一个基于osg的可执行程序
# targetName; 目标名称
# package： 所属于的包
SET(resourc_file "")
function(CreateExe targetName package) 

	message(status "targetName")
	FILE(GLOB_RECURSE src . *.cpp)
	FILE(GLOB_RECURSE inc  . *.h) 
	
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

	#message(STATUS, "all shader:${all_Shader}")
	
	ADD_EXECUTABLE(${targetName} ${src} ${inc}  ${all_Shader} ${resourc_file})
	SET_PROPERTY(TARGET ${TargetName} PROPERTY FOLDER ${package})
	SET_TARGET_PROPERTIES(${TargetName} PROPERTIES VS_DEBUGGER_ENVIRONMENT "${MY_PATH}")
	add_definitions(-DshaderPath="${CMAKE_CURRENT_SOURCE_DIR}")
	add_definitions(-DsourcePath="${CMAKE_CURRENT_SOURCE_DIR}")
endfunction()