cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 17)
project(anybus)


include(FetchContent)
FetchContent_Declare(
  JetsonGPIO 
  GIT_REPOSITORY https://github.com/pjueon/JetsonGPIO.git 
  GIT_TAG master
)

FetchContent_MakeAvailable(JetsonGPIO)

# ビルドファイル取得
file(GLOB SRC                   RELATIVE ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/*.c)
file(GLOB SRC_ADAPT             RELATIVE ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/abcc_adapt/*.c)
file(GLOB SRC_DRV_SRC           RELATIVE ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/abcc_drv/src/*.c)
file(GLOB SRC_DRV_SRC_PAR       RELATIVE ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/abcc_drv/src/par/*.c)
file(GLOB SRC_DRV_SRC_PAR30     RELATIVE ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/abcc_drv/src/par30/*.c)
file(GLOB SRC_DRV_SRC_SERIAL    RELATIVE ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/abcc_drv/src/serial/*.c)
file(GLOB SRC_DRV_SRC_SPI       RELATIVE ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/abcc_drv/src/spi/*.c)
file(GLOB SRC_OBJ               RELATIVE ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/abcc_obj/*.c)
file(GLOB SRC_OBJ_NW_OBJ        RELATIVE ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/abcc_obj/nw_obj/*.c)
file(GLOB SRC_EXAMPLE_APP       RELATIVE ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/example_app/*.c)
file(GLOB SRC_SPI_DEV           RELATIVE ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/spi_dev/*.cpp)

# テスト出力
#[[
message(${SRC})
message(${SRC_ADAPT})
message(${SRC_DRV_SRC})
message(${SRC_DRV_SRC_PAR})
message(${SRC_DRV_SRC_PAR30})
message(${SRC_DRV_SRC_SPI})
message(${SRC_OBJ})
message(${SRC_OBJ_NW_OBJ})
message(${SRC_EXAMPLE_APP})
message(${SRC_SPI_DEV})
]]

add_executable(anybus   ${SRC}
                        ${SRC_ADAPT}
                        ${SRC_DRV_SRC}
                        ${SRC_DRV_SRC_PAR}
                        ${SRC_DRV_SRC_PAR30}
                        ${SRC_DRV_SRC_SPI}
                        ${SRC_OBJ}
                        ${SRC_OBJ_NW_OBJ}
                        ${SRC_EXAMPLE_APP}
                        ${SRC_SPI_DEV}
                )

# includeフォルダ設定
target_include_directories(anybus PRIVATE ${PROJECT_SOURCE_DIR}/abcc_abp)
target_include_directories(anybus PRIVATE ${PROJECT_SOURCE_DIR}/abcc_adapt)
target_include_directories(anybus PRIVATE ${PROJECT_SOURCE_DIR}/abcc_drv/inc)
target_include_directories(anybus PRIVATE ${PROJECT_SOURCE_DIR}/abcc_drv/src)
target_include_directories(anybus PRIVATE ${PROJECT_SOURCE_DIR}/abcc_drv/src/par)
target_include_directories(anybus PRIVATE ${PROJECT_SOURCE_DIR}/abcc_drv/src/par30)
target_include_directories(anybus PRIVATE ${PROJECT_SOURCE_DIR}/abcc_drv/src/serial)
target_include_directories(anybus PRIVATE ${PROJECT_SOURCE_DIR}/abcc_drv/src/spi)
target_include_directories(anybus PRIVATE ${PROJECT_SOURCE_DIR}/abcc_obj)
target_include_directories(anybus PRIVATE ${PROJECT_SOURCE_DIR}/abcc_obj/nw_obj)
target_include_directories(anybus PRIVATE ${PROJECT_SOURCE_DIR}/example_app)
target_include_directories(anybus PRIVATE ${PROJECT_SOURCE_DIR}/spi_dev)

# link
target_link_libraries(anybus JetsonGPIO::JetsonGPIO)