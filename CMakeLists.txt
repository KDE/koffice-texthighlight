project(texthighlight)

find_package(KDE4 REQUIRED)
find_package(KOfficeLibs REQUIRED)

include (KDE4Defaults)

include_directories(${KDE4_INCLUDES} ${QT_INCLUDES} ${KOFFICELIBS_INCLUDE_DIR})

add_subdirectory(texthighlight)

