CMAKE_MINIMUM_REQUIRED (VERSION 3.25.2)
PROJECT (tencent-behaviac-solution
    VERSION 3.6.40
    DESCRIPTION "Fork from Tencent/behaviac to make private build."
    HOMEPAGE_URL "https://github.com/YapingXin-Aerospace/behaviac-3.6.40"
    LANGUAGES CXX C
)

SET (CMAKE_CXX_STANDARD 14)
SET (CMAKE_C_STANDARD 11)

MESSAGE (STATUS "Solution: " ${CMAKE_PROJECT_NAME})
MESSAGE (STATUS "Dev-Root BINARY dir " ${PROJECT_BINARY_DIR})
MESSAGE (STATUS "Dev-Root SOURCE dir " ${PROJECT_SOURCE_DIR})

SET (REPO_Dev_DIR ${PROJECT_SOURCE_DIR})
MESSAGE (STATUS "REPO_Dev_DIR: " ${REPO_Dev_DIR})

# Check Operating System type
MESSAGE(STATUS "CMAKE_SYSTEM_NAME : ${CMAKE_SYSTEM_NAME}")
IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
    ADD_DEFINITIONS( -D _LINUX )
    ADD_DEFINITIONS( -D __GNU__ )
    ADD_DEFINITIONS( -D __linux__ )
ELSEIF (CMAKE_SYSTEM_NAME MATCHES "Windows")
    ADD_DEFINITIONS( -D _WIN32 )
    ADD_DEFINITIONS( -D WIN32 )
    ADD_DEFINITIONS( -D _WINDOWS )
    # ADD_DEFINITIONS( -D _CRT_SECURE_NO_WARNINGS )
    # ADD_DEFINITIONS( -D HAVE_VLD )
    # ADD_DEFINITIONS( -D USE_UNIXem )
ELSE ()
    MESSAGE(FATAL_ERROR "Detecting OS platform : (other than Windows or Linux) : ${CMAKE_SYSTEM_NAME}")
ENDIF ()

MESSAGE(STATUS ">>> CMAKE_GENERATOR          : ${CMAKE_GENERATOR}")


SET(EXECUTABLE_OUTPUT_PATH   ${PROJECT_BINARY_DIR}/../bin)
SET(LIBRARY_OUTPUT_PATH      ${PROJECT_BINARY_DIR}/../lib)


# For Windows OS: Prompt to require CMAKE_BUILD_TYPE parameter in [Debug, Release, MinSizeRel, RelWithDebInfo]
IF (CMAKE_SYSTEM_NAME MATCHES "Windows")
    
    MESSAGE(STATUS ">>> CMAKE_BUILD_TYPE         : ${CMAKE_BUILD_TYPE}")

    IF ((NOT CMAKE_BUILD_TYPE STREQUAL Debug) AND 
        (NOT CMAKE_BUILD_TYPE STREQUAL Release) AND 
        (NOT CMAKE_BUILD_TYPE STREQUAL MinSizeRel) AND 
        (NOT CMAKE_BUILD_TYPE STREQUAL RelWithDebInfo))
        MESSAGE(STATUS ">>> Please provide the CMAKE_BUILD_TYPE parameter in [Debug, Release, MinSizeRel, RelWithDebInfo]")
        MESSAGE(FATAL_ERROR "" )
    ENDIF ()

    IF (MSVC)

        MESSAGE(STATUS ">>> CMAKE_GENERATOR_PLATFORM : ${CMAKE_GENERATOR_PLATFORM}")
        MESSAGE(STATUS ">>> MSVC_VERSION             : ${MSVC_VERSION}")
        MESSAGE(STATUS ">>> MSVC_TOOLSET_VERSION     : ${MSVC_TOOLSET_VERSION}")

        IF ((NOT CMAKE_GENERATOR_PLATFORM STREQUAL x64) AND 
            (NOT CMAKE_GENERATOR_PLATFORM STREQUAL Win32))
            MESSAGE(STATUS ">>> Please provide the CMAKE_GENERATOR_PLATFORM parameter in [Win32, x64]")
            MESSAGE(FATAL_ERROR "" )
        ELSE ()
            SET(EXECUTABLE_OUTPUT_PATH ${EXECUTABLE_OUTPUT_PATH}/${CMAKE_GENERATOR_PLATFORM})
            SET(LIBRARY_OUTPUT_PATH ${LIBRARY_OUTPUT_PATH}/${CMAKE_GENERATOR_PLATFORM})
        ENDIF ()
    ENDIF ()
ENDIF ()


SET (BEHAVIAC_PACKAGE_VERSION "3.6.40")

# Set BUILD_USE_64BITS according to CMAKE_GENERATOR_PLATFORM
IF (CMAKE_GENERATOR_PLATFORM STREQUAL x64)
    SET (BUILD_USE_64BITS ON  CACHE BOOL "CMAKE_GENERATOR is for 64 bit, set to ON accordingly" FORCE)
ELSE ()
    SET (BUILD_USE_64BITS OFF CACHE BOOL "Purpose: set to ON to use 64 bits, otherwise set to OFF.")	
ENDIF ()
MESSAGE(STATUS ">>> BUILD_USE_64BITS         : ${BUILD_USE_64BITS}")


# Set BEHAVIAC_VERSION_MODE, BEHAVIAC_RELEASE_CUSTOMIZED, BEHAVIAC_RELEASE_ON according to CMAKE_BUILD_TYPE
IF (CMAKE_BUILD_TYPE STREQUAL Debug)
    SET (BEHAVIAC_RELEASE_CUSTOMIZED 1)
    SET (BEHAVIAC_RELEASE_ON 0)
    SET (BEHAVIAC_VERSION_MODE "ForceUseDev" CACHE STRING "version mode, force to use dev/release version or default(dev version on Debug config and release version on Release config)")
ELSEIF (CMAKE_BUILD_TYPE STREQUAL Release)
    SET (BEHAVIAC_RELEASE_CUSTOMIZED 1)
    SET (BEHAVIAC_RELEASE_ON 1)
    SET (BEHAVIAC_VERSION_MODE "ForeUseRelease" CACHE STRING "version mode, force to use dev/release version or default(dev version on Debug config and release version on Release config)")
ELSE ()
    SET (BEHAVIAC_RELEASE_CUSTOMIZED 0)
    SET (BEHAVIAC_RELEASE_ON 0)
    SET (BEHAVIAC_VERSION_MODE "Default" CACHE STRING "version mode, force to use dev/release version or default(dev version on Debug config and release version on Release config)")
ENDIF ()
MESSAGE(STATUS ">>> _RELEASE_CUSTOMIZED      : ${BEHAVIAC_RELEASE_CUSTOMIZED}")
MESSAGE(STATUS ">>> BEHAVIAC_RELEASE_ON      : ${BEHAVIAC_RELEASE_ON}")
MESSAGE(STATUS ">>> BEHAVIAC_VERSION_MODE    : ${BEHAVIAC_VERSION_MODE}")

IF (BEHAVIAC_RELEASE_ON)
    IF (MSVC)
        SET (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Gy /GL")
        SET (CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} /OPT:ICF /OPT:REF /LTCG")
        SET (CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS} /OPT:ICF /OPT:REF /LTCG")
    ELSE ()
        include( CheckCXXCompilerFlag )
        check_cxx_compiler_flag( -flto GXX_HAS_LTO_FLAG )

        message( STATUS "GXX_HAS_LTO_FLAG = ${GXX_HAS_LTO_FLAG}" )

        if( CMAKE_BUILD_TYPE MATCHES Release AND GXX_HAS_LTO_FLAG )
            find_program(CMAKE_GCC_AR NAMES ${_CMAKE_TOOLCHAIN_PREFIX}gcc-ar${_CMAKE_TOOLCHAIN_SUFFIX} HINTS ${_CMAKE_TOOLCHAIN_LOCATION})
            find_program(CMAKE_GCC_NM NAMES ${_CMAKE_TOOLCHAIN_PREFIX}gcc-nm HINTS ${_CMAKE_TOOLCHAIN_LOCATION})
			find_program(CMAKE_GCC_RANLIB NAMES ${_CMAKE_TOOLCHAIN_PREFIX}gcc-ranlib HINTS ${_CMAKE_TOOLCHAIN_LOCATION})

			if( CMAKE_GCC_AR AND CMAKE_GCC_NM AND CMAKE_GCC_RANLIB )
				message( STATUS "SET LTO Flags" )

				# set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto -fno-fat-lto-objects -O3 -ffunction-sections -fdata-sections -fvisibility=hidden -fvisibility-inlines-hidden" )
				set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto -O3" )
				
				check_cxx_compiler_flag( -fno-fat-lto-objects GXX_HAS_NO_FAT_LTO_FLAG )
				
				message( STATUS "GXX_HAS_NO_FAT_LTO_FLAG = ${GXX_HAS_NO_FAT_LTO_FLAG}" )
				
				if (GXX_HAS_NO_FAT_LTO_FLAG)
					set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-fat-lto-objects" )
				endif()
				
				set( CMAKE_AR "${CMAKE_GCC_AR}" )
				set( CMAKE_NM "${CMAKE_GCC_NM}" )
				set( CMAKE_RANLIB "${CMAKE_GCC_RANLIB}" )
				
				# set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -Wl,--gc-sections")
				# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections")
			else()
				message( WARNING "GCC indicates LTO support, but binutils wrappers could not be found. Disabling LTO." )
			endif()
        endif()

        # -fipa-icf
		# -finline-limit=64 
		# -fno-exceptions -fno-rtti
		# -fwhole-program
		# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffunction-sections -fdata-sections -fvisibility=hidden -O3 -flto")
		# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os -flto -fno-fat-lto-objects")
		# set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -Wl,--gc-sections -flto")
		# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections -flto")
		# set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS_RELEASE} -flto")
		
		# find_package(Gold)
		 
		# if (GOLD_FOUND)
			# SET(LINKER_SCRIPT -fuse-ld=gold -Wl,--icf=safe)
		# endif()
    ENDIF ()
ENDIF ()

IF (BUILD_USE_64BITS)
    IF (MSVC)
        IF (NOT CMAKE_GENERATOR_PLATFORM STREQUAL x64)
            MESSAGE( FATAL_ERROR "You need to specify '${CMAKE_GENERATOR_PLATFORM}' as x64 to generate project file for 64 bit" )
        ENDIF ()
    ELSE ()
        SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")
    ENDIF ()
ENDIF ()


# MESSAGE(STATUS ">>> Output Artifacts")
MESSAGE(STATUS "EXECUTABLE_OUTPUT_PATH : ${EXECUTABLE_OUTPUT_PATH}")
MESSAGE(STATUS "LIBRARY_OUTPUT_PATH    : ${LIBRARY_OUTPUT_PATH}")
# MESSAGE(STATUS "RUNTIME_OUTPUT_DIRECTORY : ${RUNTIME_OUTPUT_DIRECTORY}")
# MESSAGE(STATUS "ARCHIVE_OUTPUT_DIRECTORY : ${ARCHIVE_OUTPUT_DIRECTORY}")
# MESSAGE(STATUS "<<< Output Artifacts")



# exclude_dir = "/exclude_dir/"
macro (ExcludeDir src_files exclude_dir)
	foreach (TMP_PATH ${src_files})
		string (FIND ${TMP_PATH} ${exclude_dir} EXCLUDE_DIR_FOUND)
		if (NOT ${EXCLUDE_DIR_FOUND} EQUAL -1)
			list (REMOVE_ITEM ${src_files} ${TMP_PATH})
		endif ()
	endforeach(TMP_PATH)
endmacro()

# usage: GroupSources("${PROJECT_SOURCE_DIR}/inc/", "inc")
macro(GroupSources dir prefix)
    # file(GLOB_RECURSE elements RELATIVE ${dir} *.h *.hpp *.c *.cpp *.cc)
    file(GLOB_RECURSE elements RELATIVE ${dir} *.*)

    foreach(element ${elements})
      # Extract filename and directory
      get_filename_component(element_name ${element} NAME)
      get_filename_component(element_dir ${element} DIRECTORY)

      if (NOT ${element_dir} STREQUAL "")
        # If the file is in a subdirectory use it as source group.
          # Use the full hierarchical structure to build source_groups.
          string(REPLACE "/" "\\" group_name ${element_dir})
          source_group("${prefix}\\${group_name}" FILES ${dir}/${element})
      else()
        # If the file is in the root directory, place it in the root source_group.
        source_group("${prefix}\\" FILES ${dir}/${element})
      endif()
    endforeach()
endmacro()


MESSAGE(STATUS "PROJECT_SOURCE_DIR     : ${PROJECT_SOURCE_DIR}")
MESSAGE(STATUS "PROJECT_BINARY_DIR     : ${PROJECT_BINARY_DIR}")


CONFIGURE_FILE (
    "${PROJECT_SOURCE_DIR}/build/_config.h.in"
    "${PROJECT_SOURCE_DIR}/inc/behaviac/common/_config.h"
    NEWLINE_STYLE WIN32
)


INCLUDE_DIRECTORIES(
    "${PROJECT_SOURCE_DIR}/inc"
)


file(GLOB_RECURSE BEHAVIAC_INC  "${PROJECT_SOURCE_DIR}/inc/*.*")
file(GLOB_RECURSE BEHAVIAC_SRC  "${PROJECT_SOURCE_DIR}/src/*.*")

if (MSVC)
    GroupSources("${PROJECT_SOURCE_DIR}/inc/" "inc")
    GroupSources("${PROJECT_SOURCE_DIR}/src/" "src")
endif ()

if (MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
endif ()


# set( SHARED_LIB "" )
# if (BEHAVIAC_ANDROID_STUDIO)
#     set( SHARED_LIB "SHARED" )
# endif()
# MESSAGE(STATUS "SHARED_LIB     : ${SHARED_LIB}")

set (LINKER_SCRIPT "")

add_library(behaviac SHARED ${BEHAVIAC_INC} ${BEHAVIAC_SRC} ${LINKER_SCRIPT})
target_compile_definitions(behaviac PUBLIC "BEHAVIACDLL_EXPORTS" "BEHAVIAC_DLL")
