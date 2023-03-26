
set(TargetName "001_postmortems")
CreateExe(${TargetName} "f-GPUPro1")
include_directories( ${CMAKE_SOURCE_DIR}
 ${CMAKE_SOURCE_DIR}/src
 ${CMAKE_SOURCE_DIR}/src/e-graphicAlgorithm
 ${CMAKE_SOURCE_DIR}/src/e-graphicAlgorithm/EffectCompositor
 ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${TargetName}
common
Frame
EffectCompositor)