add_executable            (swim-mem-mem mem/mem.c)
target_link_libraries     (swim-mem-mem swim)
target_include_directories(swim-mem-mem PRIVATE ../include)