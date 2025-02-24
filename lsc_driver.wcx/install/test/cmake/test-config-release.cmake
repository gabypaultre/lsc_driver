#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "test::test" for configuration "Release"
set_property(TARGET test::test APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(test::test PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/test"
  )

list(APPEND _cmake_import_check_targets test::test )
list(APPEND _cmake_import_check_files_for_test::test "${_IMPORT_PREFIX}/bin/test" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
