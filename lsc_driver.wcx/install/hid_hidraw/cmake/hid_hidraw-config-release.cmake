#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "hid_hidraw::hid_hidraw" for configuration "Release"
set_property(TARGET hid_hidraw::hid_hidraw APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(hid_hidraw::hid_hidraw PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libhid_hidraw.so"
  IMPORTED_SONAME_RELEASE "libhid_hidraw.so"
  )

list(APPEND _cmake_import_check_targets hid_hidraw::hid_hidraw )
list(APPEND _cmake_import_check_files_for_hid_hidraw::hid_hidraw "${_IMPORT_PREFIX}/lib/libhid_hidraw.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
