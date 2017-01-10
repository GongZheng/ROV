# Try to find librtmp for pi
# Once done this will define
# LIBRTMP_FOUND - System has librtmp
# LIBRTMP_INCLUDE_DIRS 
# LIBRTMP_LIBRARIES

FIND_PATH(LIBRTMP_INCLUDE_AMF_DIRS
  NAMES librtmp/amf.h
  PATHS 
  /usr/include 
  /usr/local/include
  )

FIND_PATH(LIBRTMP_INCLUDE_RTMP_DIRS
  NAMES librtmp/rtmp.h
  PATHS 
  /usr/include 
  /usr/local/include
  )

FIND_PATH(LIBRTMP_INCLUDE_HTTP_DIRS
  NAMES librtmp/http.h
  PATHS 
  /usr/include 
  /usr/local/include
  )

FIND_PATH(LIBRTMP_INCLUDE_LOG_DIRS
  NAMES librtmp/log.h
  PATHS 
  /usr/include 
  /usr/local/include
  )


FIND_LIBRARY(LIBRTMP_INCLUDE_DIRS_RTMP
    NAMES librtmp.so
    PATHS
    /usr/lib
    /usr/local/lib
    )


INCLUDE(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LIBRTMP DEFAULT_MSG 
   LIBRTMP_INCLUDE_AMF_DIRS  LIBRTMP_INCLUDE_RTMP_DIRS LIBRTMP_INCLUDE_HTTP_DIRS
    LIBRTMP_INCLUDE_LOG_DIRS LIBRTMP_INCLUDE_DIRS_RTMP)
    
SET(LIBRTMP_INCLUDE_DIRS ${LIBRTMP_INCLUDE_AMF_DIRS} ${LIBRTMP_INCLUDE_RTMP_DIRS}
     ${LIBRTMP_INCLUDE_HTTP_DIRS}   ${LIBRTMP_INCLUDE_LOG_DIRS}  )
SET(LIBRTMP_LIBRARIES ${LIBRTMP_INCLUDE_DIRS_RTMP} )

MARK_AS_ADVANCED(LIBRTMP_INCLUDE_DIRS LIBRTMP_LIBRARIES)
