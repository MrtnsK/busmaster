set(sources
  AcceptanceFilterDlg.cpp
  CAN_AGCO.cpp
  CANRegAnalysis.cpp
  ChangeRegisters.cpp
  HardwareListing.cpp
  ../Application/MultiLanguage.cpp)

set(headers
  AcceptanceFilterDlg.h
  CAN_AGCO.h
  CAN_AGCO_Extern.h
  CAN_AGCO_Resource.h
  CAN_AGCO_stdafx.h
  ChangeRegDefines.h
  ChangeRegisters.h
  ContrConfigPeakUsbDefs.h
  HardwareListing.h
  ../Application/MultiLanguage.h)

set(resources
  CAN_AGCO.rc)

add_library(CAN_AGCO SHARED ${sources} ${headers} ${resources})

include_directories(
  ..
  ${ICONV_INCLUDE_DIR}
  ${LIBXML2_INCLUDE_DIR}
  ${ATL_INCLUDE_DIRS}
  ${MFC_INCLUDE_DIRS})

# compiler options
add_definitions(-D_USRDLL)

# linker options
set_target_properties(CAN_AGCO PROPERTIES LINK_FLAGS "/NODEFAULTLIB:daouuid")
target_link_libraries(CAN_AGCO
  Advapi32
  DataTypes
  Utils)

# installer options
add_custom_command(
  TARGET CAN_AGCO
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} ARGS -E make_directory ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy $<TARGET_FILE:CAN_AGCO> ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/
  COMMAND ${CMAKE_COMMAND} ARGS -E copy ${CMAKE_CURRENT_SOURCE_DIR}/EXTERNAL/vxlapi.dll ${PROJECT_SOURCE_DIR}/BIN/${CMAKE_BUILD_TYPE}/)
