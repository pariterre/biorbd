macro(FindOrBuildRBDL MATH_BACKEND)
    # Check if MATH_BACKEND is EIGEN or CASADI
    if(NOT (${MATH_BACKEND} STREQUAL "EIGEN" OR ${MATH_BACKEND} STREQUAL "CASADI"))
        message(FATAL_ERROR "FindOrBuildRBDL: Invalid option '${MATH_BACKEND}'. Use 'EIGEN' or 'CASADI'.")
    endif()

    # Try to find RBDL first
    if (${MATH_BACKEND} STREQUAL "EIGEN")
        find_package(RBDL QUIET)
    
    elseif (${MATH_BACKEND} STREQUAL "CASADI")
        find_package(RBDLCasadi QUIET)
        if (RBDLCasadi_FOUND)
            set(RBDL_FOUND TRUE)
            
            # Define include and library paths to mimic RBDL eigen which is the format expected by biorbd
            set(RBDL_INCLUDE_DIR ${RBDLCasadi_INCLUDE_DIR}/rbdl-casadi ${RBDLCasadi_INCLUDE_DIR})
            set(RBDL_LIBRARY ${RBDLCasadi_LIBRARY})
        endif()
    endif()

    if(RBDL_FOUND AND NOT RBDL_IS_BUILT)
        set(RBDL_IS_BUILT FALSE)
    else()
        message(STATUS "RBDL not found, downloading and installing from GitHub")
        include(ExternalProject)

        set(RBDL_IS_BUILT TRUE)
        if (INSTALL_DEPENDENCIES_ON_SYSTEM)
            set(RBDL_INSTALL_DIR ${CMAKE_INSTALL_PREFIX})
            set(RBDL_TARGET_STATIC OFF)
        else()
            set(RBDL_INSTALL_DIR "${CMAKE_BINARY_DIR}/RBDL_install")
            set(RBDL_TARGET_STATIC ON)
        endif()

        if (${MATH_BACKEND} STREQUAL "EIGEN")
            set(RBDL_BUILD_CASADI OFF)
            set(RBDL_LIBRARY_SUFFIX "")
            set(CASADI_ARGS "")
        elseif (${MATH_BACKEND} STREQUAL "CASADI")
            set(RBDL_BUILD_CASADI ON)
            set(RBDL_LIBRARY_SUFFIX "-casadi")
            set(CASADI_ARGS
                -DCasadi_INCLUDE_DIR=${Casadi_INCLUDE_DIR}
                -DCasadi_LIBRARY=${Casadi_LIBRARY}
                -DCasadi_FOUND=${Casadi_FOUND}
            )
        endif()
        
        # Detect correct static library extension (OS-independent)
        if(WIN32)
            set(RBDL_LIB_NAME "rbdl${RBDL_LIBRARY_SUFFIX}.lib") 
        else()
            if(${RBDL_TARGET_STATIC})
                set(RBDL_LIB_NAME "librbdl${RBDL_LIBRARY_SUFFIX}.a")
            else()
                set(RBDL_LIB_NAME "librbdl${RBDL_LIBRARY_SUFFIX}.so")
            endif()
        endif()
        set(RBDL_LIBRARY "${RBDL_INSTALL_DIR}/lib/${RBDL_LIB_NAME}")

        ExternalProject_Add(rbdl_external
            GIT_REPOSITORY https://github.com/pariterre/rbdl.git
            GIT_TAG master
            CMAKE_ARGS
                -DCMAKE_INSTALL_PREFIX=${RBDL_INSTALL_DIR}
                -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                -DRBDL_BUILD_STATIC=${RBDL_TARGET_STATIC}
                -DRBDL_BUILD_CASADI=${RBDL_BUILD_CASADI}
                -DEigen3_DIR=${EIGEN3_DIR}
                -DCasadi_INCLUDE_DIR=${Casadi_INCLUDE_DIR}
                -DCasadi_LIBRARY=${Casadi_LIBRARY}
                -DCasadi_FOUND=${Casadi_FOUND}
                -DCMAKE_POSITION_INDEPENDENT_CODE=ON
            BUILD_BYPRODUCTS "${RBDL_LIBRARY}"
        )

        # Define include and library paths
        set(RBDL_INCLUDE_DIR "${RBDL_INSTALL_DIR}/include")
        if (${MATH_BACKEND} STREQUAL "CASADI")
            # Append "rbdl-casadi" to the include path
            set(RBDL_INCLUDE_DIR "${RBDL_INCLUDE_DIR}/rbdl-casadi")
        endif()

        # Ensure that the library gets built before linking
        add_library(RBDL_BUILD STATIC IMPORTED)
        
        set_target_properties(RBDL_BUILD PROPERTIES
            IMPORTED_LOCATION "${RBDL_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${RBDL_INCLUDE_DIR}"
        )
        add_dependencies(RBDL_BUILD rbdl_external)

        # Windows DLL runtime setup
        if(WIN32)
            set_target_properties(RBDL_BUILD PROPERTIES
                IMPORTED_IMPLIB "${RBDL_LIBRARY}"  # Import library for linking
                IMPORTED_LOCATION "${RBDL_RUNTIME_DIR}/rbdl.dll"  # Runtime DLL location
            )
        endif()

        if (EIGEN3_IS_BUILT)
            add_dependencies(rbdl_external EIGEN3_BUILD)
        endif()
        if (${MATH_BACKEND} STREQUAL "CASADI")
            if (Casadi_IS_BUILT)
                add_dependencies(rbdl_external CASADI_BUILD)
            endif()
            target_include_directories(RBDL_BUILD INTERFACE ${Casadi_INCLUDE_DIR})
            target_link_libraries(RBDL_BUILD INTERFACE ${Casadi_LIBRARY})
        endif()

        set(RBDL_FOUND TRUE)
    endif()


endmacro()