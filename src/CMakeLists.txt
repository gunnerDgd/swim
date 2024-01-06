
if (WIN32)
    add_library(swim STATIC
        res.h
        details/win32_res.c

        root.h
        details/win32_root.c

        map.h
        details/win32_map.c                                  

        mem.h
        mem.c

        swim.h
        swim.c
    )
    target_link_libraries(swim PRIVATE build build-arch build-arch-asm)
endif()