# 添加第三方库
add_subdirectory(third_party)

# 定义子模块列表
set(MODULES
    pimp
    # 添加更多模块...
)

foreach(MODULE ${MODULES})
    file(GLOB MODULE_SOURCES "${MODULE}/*.cc")
    file(GLOB MODULE_HEADERS "${MODULE}/*.h")

    add_library(${MODULE} STATIC ${MODULE_SOURCES} ${MODULE_HEADERS})

    target_include_directories(${MODULE} PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR/MODULE}
    )
    # 链接第三方库接口
    target_link_libraries(${MODULE} PRIVATE third_party_interface)
endforeach()

# 创建主可执行文件
add_executable(${PROJECT_NAME} awesomecpp.cc)

# 链接所有模块
target_link_libraries(${PROJECT_NAME} PRIVATE ${MODULES})