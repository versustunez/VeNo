project(VeNo VERSION 0.0.1)

set(BaseTargetName ${PROJECT_NAME})
option(VeNo_ENABLE_PROFILE "Enable InEngine Profiling" OFF)

juce_add_plugin(
        "${BaseTargetName}"
        COMPANY_NAME "VstZ"
        IS_SYNTH FALSE
        NEEDS_MIDI_INPUT TRUE
        NEEDS_MIDI_OUTPUT FALSE
        IS_MIDI_EFFECT FALSE
        VST3_CATEGORIES Instrument
        AU_MAIN_TYPE kAudioUnitType_Generator
        EDITOR_WANTS_KEYBOARD_FOCUS FALSE
        COPY_PLUGIN_AFTER_BUILD FALSE
        PLUGIN_MANUFACTURER_CODE VstZ
        PLUGIN_CODE VeNo
        FORMATS VST3 Standalone
        PRODUCT_NAME "VeNo Synth"
)

set_target_properties(${BaseTargetName} PROPERTIES CXX_STANDARD 17)

add_subdirectory(Assets)
add_subdirectory(Source/VeNo)
add_subdirectory(Source/VUtils)
add_subdirectory(Source/vendors)

target_sources(${BaseTargetName} PRIVATE ${SRCS})

target_compile_definitions(
        ${BaseTargetName}
        PUBLIC
        JUCE_DISPLAY_SPLASH_SCREEN=0
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
)

target_include_directories(${BaseTargetName} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Source/)
target_include_directories(${BaseTargetName} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Source/vendors)

target_link_libraries(
        ${BaseTargetName} PRIVATE
        assets
        uifiles
        juce::juce_audio_utils
        juce::juce_dsp
        juce::juce_opengl
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

if (VeNo_ENABLE_PROFILE)
    target_compile_definitions(${BaseTargetName} PRIVATE VENO_PROFILE=1)
endif()