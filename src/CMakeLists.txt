# 添加所有源文件并将名称保存到DIR_SRCS中
aux_source_directory(. DIR_SRCS)

add_library(ZRainySTLCore ${DIR_SRCS})
