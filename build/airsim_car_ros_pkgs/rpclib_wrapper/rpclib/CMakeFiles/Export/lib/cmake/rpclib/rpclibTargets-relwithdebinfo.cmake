#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "rpclib::rpc" for configuration "RelWithDebInfo"
set_property(TARGET rpclib::rpc APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(rpclib::rpc PROPERTIES
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/librpc.so"
  IMPORTED_SONAME_RELWITHDEBINFO "librpc.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS rpclib::rpc )
list(APPEND _IMPORT_CHECK_FILES_FOR_rpclib::rpc "${_IMPORT_PREFIX}/lib/librpc.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
