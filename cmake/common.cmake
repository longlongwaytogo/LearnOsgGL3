#
# CreateExe 
# ����һ������osg�Ŀ�ִ�г���
# targetName; Ŀ������
# package�� �����ڵİ�

function(CreateExe targetName package) 

	message(status "targetName")
	FILE(GLOB_RECURSE src . *.cpp)
	FILE(GLOB_RECURSE inc  . *.h) 
	ADD_EXECUTABLE(${targetName} ${src} ${inc})
	SET_PROPERTY(TARGET ${TargetName} PROPERTY FOLDER ${package})
	SET_TARGET_PROPERTIES(${TargetName} PROPERTIES VS_DEBUGGER_ENVIRONMENT "${MY_PATH}")
endfunction()