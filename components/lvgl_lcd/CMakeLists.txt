file(GLOB_RECURSE SOURCES *.c tft/*.c)

idf_component_register(SRCS ${SOURCES}
                       INCLUDE_DIRS . tft
                       REQUIRES console lvgl)