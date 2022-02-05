set(
  FILES
  lcd.TTF
  fontAwesome.ttf
  arvo.ttf
  Logo.svg
)
juce_add_binary_data(assets HEADER_NAME Assets.h SOURCES ${FILES})
set_target_properties(assets PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
