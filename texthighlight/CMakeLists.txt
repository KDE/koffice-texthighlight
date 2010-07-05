project(Highlight)

cmake_minimum_required(VERSION 2.6)

find_package(KDE4 REQUIRED)
add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)
include(MacroLibrary)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ${KOTEXT_INCLUDES} ${KOMAIN_INCLUDES} )

set(highlight_SRCS
    HighlightPlugin.cpp
    Highlight.cpp
    HighlightFactory.cpp
)

kde4_add_plugin(kotexthighlight ${highlight_SRCS})

target_link_libraries(kotexthighlight
    ${KOTEXT_LIBS}
    ${KDE4_KDEUI_LIBS}
    ktexteditor
)

install(TARGETS kotexthighlight DESTINATION ${PLUGIN_INSTALL_DIR})

install( FILES kotexthighlight.desktop DESTINATION ${SERVICES_INSTALL_DIR} )
