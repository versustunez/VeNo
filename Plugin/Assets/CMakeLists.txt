set(FILES
        lcd.TTF
        fontAwesome.ttf
        arvo.ttf
        LogoVeNo.png
        )
juce_add_binary_data(assets HEADER_NAME Assets.h SOURCES ${FILES})