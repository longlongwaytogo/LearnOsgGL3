# EffectorPositoer 位于osgExtern子目录下
SET(CPP_RELATIVE_PATH_NAME "osgExtern")
SET(H_RELATIVE_PATH_NAME "osgExtern")



set(TargetName "EffectCompositor")
CreateLib(${TargetName} "e-graphicAlgorithm")
 
 include_directories(
 ${CMAKE_SOURCE_DIR}
 ${CMAKE_SOURCE_DIR}/src
 ${CMAKE_SOURCE_DIR}/src/e-graphicAlgorithm
 ${CMAKE_CURRENT_SOURCE_DIR})
TARGET_LINK_LIBRARIES(EffectCompositor common )
#TARGET_LINK_LIBRARIES(EffectCompositor RenderDocEvent )
 


