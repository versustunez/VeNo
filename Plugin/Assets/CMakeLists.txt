set(
        FILES
        lcd.TTF
        fontAwesome.ttf
        arvo.ttf
        Logo.svg
)
set(
        UIFILES
        UIFiles/Footer.vui
        UIFiles/FXSection.vui
        UIFiles/LFO.vui
        UIFiles/MainGui.vui
        UIFiles/OSC.vui
        UIFiles/Sidebar.vui
)
juce_add_binary_data(assets HEADER_NAME Assets.h SOURCES ${FILES})
juce_add_binary_data(uifiles HEADER_NAME UIFiles.h NAMESPACE VeNo::UI SOURCES ${UIFILES})
set_target_properties(assets PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
set_target_properties(uifiles PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
