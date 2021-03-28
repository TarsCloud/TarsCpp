#获取tag上的版本号
macro(get_version _major _minor _patch)
    find_package(Git QUIET)     # 查找Git，QUIET静默方式不报错
    set(RET -1)
    if (GIT_FOUND)
        execute_process(          # 执行一个子进程
                COMMAND ${GIT_EXECUTABLE} rev-list --tags --max-count=1
                RESULT_VARIABLE RET                 # 返回值存入变量
                OUTPUT_VARIABLE _hash            # 输出字符串存入变量
                OUTPUT_STRIP_TRAILING_WHITESPACE    # 删除字符串尾的换行符
                WORKING_DIRECTORY                   # 执行路径
                ${CMAKE_CURRENT_SOURCE_DIR}
                )

        
        if (RET EQUAL 0)
            execute_process(          # 执行一个子进程
                    COMMAND ${GIT_EXECUTABLE} describe --tags ${_hash}
                    RESULT_VARIABLE RET                 # 返回值存入变量
                    OUTPUT_VARIABLE _version            # 输出字符串存入变量
                    OUTPUT_STRIP_TRAILING_WHITESPACE    # 删除字符串尾的换行符
                    WORKING_DIRECTORY                   # 执行路径
                    ${CMAKE_CURRENT_SOURCE_DIR}
                    )
        endif ()
    endif ()

    if (RET EQUAL 0)
        # message("---------------${_version}-------", ${RET})
        string(SUBSTRING "${_version}" 1 -1 _version)
        string(REPLACE "." ";" _version_list ${_version})

        list(LENGTH _version_list version_length)
        if (version_length EQUAL 3)
            list(GET _version_list 0 major)
            if (major MATCHES "^[1-9]\\d*|0$")
            else ()
                set(RET -1)
            endif ()

            list(GET _version_list 1 minor)
            if (minor MATCHES "^[1-9]\\d*|0$")
            else ()
                set(RET -1)
            endif ()

            list(GET _version_list 2 patch)
            if (patch MATCHES "^[1-9]\\d*|0$")
            else ()
                set(RET -1)
            endif ()

            if (RET EQUAL 0)
                set(${_major} ${major})
                set(${_minor} ${minor})
                set(${_patch} ${patch})
            endif ()
        endif ()
    endif ()
endmacro()

macro(get_timestamp _commit_time _build_time _version_time)
    find_package(Git QUIET)     # 查找Git，QUIET静默方式不报错
    set(RET 0)


    if (GIT_FOUND)
        execute_process(          # 执行一个子进程
                #                COMMAND ${GIT_EXECUTABLE} log -1 --format=%cd --date=format:%Y%m%d%H%M%S
                COMMAND ${GIT_EXECUTABLE} log -n 1 --pretty=format:%ad --date=iso # 命令
                RESULT_VARIABLE RET                 # 返回值存入变量
                OUTPUT_VARIABLE commit_time     # 输出字符串存入变量
                OUTPUT_STRIP_TRAILING_WHITESPACE    # 删除字符串尾的换行符
                WORKING_DIRECTORY                   # 执行路径
                ${CMAKE_CURRENT_SOURCE_DIR}
                )
    endif ()

    string(TIMESTAMP ${_build_time} "%Y%m%d%H%M%S") # 获取生成时间

    if (NOT RET EQUAL 0)
        message(AUTHOR_WARNING "can't find .git, use build date instead of commit date!!!")
        set(${_version_time} ${${_build_time}})
    else ()
        string(SUBSTRING ${commit_time} 0 4 year)
        string(SUBSTRING ${commit_time} 5 2 month)
        string(SUBSTRING ${commit_time} 8 2 day)
        string(SUBSTRING ${commit_time} 11 2 hour)
        string(SUBSTRING ${commit_time} 14 2 minute)
        string(SUBSTRING ${commit_time} 17 2 second)
        set(${_version_time} ${year}${month}${day}${hour}${minute}${second})
        set(${_commit_time} ${year}${month}${day}${hour}${minute}${second})
    endif ()
endmacro()

# get git hash
macro(get_git_hash _git_hash)   # 宏的开始
    find_package(Git QUIET)     # 查找Git，QUIET静默方式不报错
    if (GIT_FOUND)
        execute_process(          # 执行一个子进程
                COMMAND ${GIT_EXECUTABLE} log -1 --pretty=format:%h # 命令
                OUTPUT_VARIABLE ${_git_hash}        # 输出字符串存入变量
                OUTPUT_STRIP_TRAILING_WHITESPACE    # 删除字符串尾的换行符
                ERROR_QUIET                         # 对执行错误静默
                WORKING_DIRECTORY                   # 执行路径
                ${CMAKE_CURRENT_SOURCE_DIR}
                )
    endif ()
endmacro()

# get git branch
macro(get_git_branch _git_branch)   # 宏的开始
    find_package(Git QUIET)     # 查找Git，QUIET静默方式不报错
    if (GIT_FOUND)
        execute_process(          # 执行一个子进程
                COMMAND ${GIT_EXECUTABLE} symbolic-ref --short -q HEAD
                OUTPUT_VARIABLE ${_git_branch}        # 输出字符串存入变量
                OUTPUT_STRIP_TRAILING_WHITESPACE    # 删除字符串尾的换行符
                ERROR_QUIET                         # 对执行错误静默
                WORKING_DIRECTORY                   # 执行路径
                ${CMAKE_CURRENT_SOURCE_DIR}
                )
    endif ()
endmacro()