project(VeNo VERSION 0.0.1)

set(BaseTargetName ${PROJECT_NAME})

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

set_target_properties(${BaseTargetName} PROPERTIES CXX_STANDARD 20)

add_subdirectory(Assets)
add_subdirectory(Source/VeNo)
add_subdirectory(Source/VUtils)

target_sources(${BaseTargetName} PRIVATE ${SRCS})

target_compile_definitions(
        ${BaseTargetName}
        PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
)

#enable avx2
if (OSC_ENABLE_SIMD)
    message(STATUS "SIMD acceleration enabled (requesting AVX2)")
    if (MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE /arch:AVX2)
    else ()
        target_compile_options(${PROJECT_NAME} PRIVATE -mavx2 -mfma)
    endif ()
else ()
    message(STATUS "SIMD acceleration disabled")
    target_compile_definitions(${PROJECT_NAME} PRIVATE SIMD_DISABLE=1)

endif ()

target_include_directories(${BaseTargetName} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Source/)
target_include_directories(${BaseTargetName} SYSTEM PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../vendors/juce/modules)
target_include_directories(${BaseTargetName} SYSTEM PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../vendors/fmt/include)

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
        fmt::fmt
)
