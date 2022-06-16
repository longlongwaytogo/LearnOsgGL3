set(TargetName osgTest)
add_executable(osgTest main.cpp)
set_target_properties(${TargetName} PROPERTIES VS_DEBUGGER_ENVIRONMENT "${MY_PATH}")