#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "lsc_servocontrol::lsc_servocontrol" for configuration "Release"
set_property(TARGET lsc_servocontrol::lsc_servocontrol APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(lsc_servocontrol::lsc_servocontrol PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/liblsc_servocontrol.so"
  IMPORTED_SONAME_RELEASE "liblsc_servocontrol.so"
  )

list(APPEND _cmake_import_check_targets lsc_servocontrol::lsc_servocontrol )
list(APPEND _cmake_import_check_files_for_lsc_servocontrol::lsc_servocontrol "${_IMPORT_PREFIX}/lib/liblsc_servocontrol.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
