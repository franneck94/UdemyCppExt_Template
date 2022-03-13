add_executable(${EXE_NAME}
        cycle.cc
        Main.cc)
target_set_warnings(TARGET ${EXE_NAME} ENABLE ON AS_ERROR OFF)

target_link_libraries(${EXE_NAME} PUBLIC
    ${DATA_LOADER_LIB}
    ${RENDER_LIB}
    ${IMPL_LIB}
    ${IMGUI_LIB}
    ${IMPLOT_LIB}
    ${ADAS_LIB})
