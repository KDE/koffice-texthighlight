project(Highlight)

add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(MacroLibrary)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES})

set(highlight_SRCS
    HighlightPlugin.cpp
    Highlight.cpp
    HighlightFactory.cpp
)

kde4_add_plugin(kotexthighlight ${highlight_SRCS})

target_link_libraries(kotexthighlight
    ${KODFTEXT_LIBS}
    ${KDE4_KDEUI_LIBS}
    ktexteditor
)

install(TARGETS kotexthighlight DESTINATION ${PLUGIN_INSTALL_DIR})

install( FILES kotexthighlight.desktop DESTINATION ${SERVICES_INSTALL_DIR} )
