if (NOT CTR_BAREMETAL)
    add_subdirectory(HOS)
endif()

if (RIP_BACKEND STREQUAL "citro3d")
    add_subdirectory(C3D)
endif()