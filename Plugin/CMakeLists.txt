project(VeNo VERSION 0.0.1)

set(BaseTargetName ${PROJECT_NAME})

juce_add_plugin("${BaseTargetName}"
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
        PRODUCT_NAME "VeNo Synth")

set_target_properties(${BaseTargetName} PROPERTIES CXX_STANDARD 17)

target_sources(${BaseTargetName} PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp)

target_compile_definitions(${BaseTargetName}
        PUBLIC
        JUCE_DISPLAY_SPLASH_SCREEN=0
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(${BaseTargetName} PRIVATE
        juce::juce_audio_utils
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
