# 定义第三方库列表
# 如果目录下存在对应的子目录，就会自动添加
set(THIRD_PARTY_LIBS
    abseil
    # 添加更多第三方库...
)

# 为每个第三方库设置特定的选项
function(set_third_party_options LIB_NAME)
    if(${LIB_NAME} STREQUAL "abseil")
        # set(ABSL_PROPAGATE_CXX_STD ON CACHE BOOL "" FORCE)
        # set(ABSL_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
    elseif(${LIB_NAME} STREQUAL "fmt")
        set(FMT_INSTALL OFF CACHE BOOL "" FORCE)
    elseif(${LIB_NAME} STREQUAL "spdlog")
        set(SPDLOG_INSTALL OFF CACHE BOOL "" FORCE)
    elseif(${LIB_NAME} STREQUAL "googletest")
        set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
        set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    # 添加更多库的选项...
    endif()
endfunction()

# 遍历所有第三方库
foreach(LIB ${THIRD_PARTY_LIBS})
    # 检查库目录是否存在
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${LIB}/CMakeLists.txt")
        message(STATUS "Found third-party library: ${LIB}")
        
        # 设置库特定的选项
        set_third_party_options(${LIB})
        
        # 添加子目录
        add_subdirectory(${LIB})
    else()
        message(STATUS "Third-party library not found: ${LIB}")
        
        # 可选：尝试使用 find_package 查找系统安装的库
        # 例如对于 abseil：
        if(${LIB} STREQUAL "abseil")
            find_package(absl QUIET)
            if(absl_FOUND)
                message(STATUS "Using system-installed Abseil")
            else()
                message(WARNING "Abseil not available. Some features might be disabled.")
            endif()
        endif()
        # 为其他库添加类似的查找逻辑
    endif()
endforeach()

# 创建一个接口库，包含所有第三方库的依赖
add_library(third_party_interface INTERFACE)

# 条件添加依赖，只有当目标存在时才添加
if(TARGET absl::base)
    target_link_libraries(third_party_interface INTERFACE 
        absl::base
        absl::strings
        # 其他常用的 abseil 组件
    )
endif()

# 为其他库添加类似的条件链接
# 例如：
# if(TARGET fmt::fmt)
#     target_link_libraries(third_party_interface INTERFACE fmt::fmt)
# endif()

# if(TARGET spdlog::spdlog)
#     target_link_libraries(third_party_interface INTERFACE spdlog::spdlog)
# endif()

# 可选：自动查找并添加所有第三方目录
# 这会添加列表中未明确指定的库
option(AUTO_DETECT_THIRD_PARTY "Automatically detect and add all third-party libraries" OFF)
if(AUTO_DETECT_THIRD_PARTY)
    file(GLOB EXTRA_DIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)
    foreach(DIR ${EXTRA_DIRS})
        if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${DIR}" 
           AND NOT ${DIR} STREQUAL "CMakeFiles"
           AND NOT ${DIR} IN_LIST THIRD_PARTY_LIBS)
            
            if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${DIR}/CMakeLists.txt")
                message(STATUS "Auto-detected third-party library: ${DIR}")
                add_subdirectory(${DIR})
            endif()
        endif()
    endforeach()
endif()
