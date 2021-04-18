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

add_subdirectory(Assets)
set(SOURCE_FILES
        # VENDOR
        headers/vendor/tsl/robin_growth_policy.h
        headers/vendor/tsl/robin_hash.h
        headers/vendor/tsl/robin_map.h
        headers/vendor/tsl/robin_set.h

        # VENO!
        Source/VeNo/PluginProcessor.cpp
        Source/VeNo/PluginEditor.cpp
        #Generated Parts
        Source/VeNo/Generated/ParameterHandlerSetup.cpp
        #Core Part
        Source/VeNo/Core/Parameter/Handler.cpp
        Source/VeNo/Core/Parameter/Parameter.cpp
        Source/VeNo/Core/Parameter/ModulateParameter.cpp
        Source/VeNo/Core/Config.cpp
        Source/VeNo/Core/Instance.cpp
        Source/VeNo/Core/State/InstanceState.cpp
        Source/VeNo/Core/State/ComponentStates.cpp
        Source/VeNo/Core/Theme/Theme.cpp)

set(SOUND_FILES
        Source/VeNo/Sound/Generation/WaveTables/Structs.cpp
        Source/VeNo/Sound/Generation/WaveTables/RawTable.cpp
        Source/VeNo/Sound/Generation/WaveTables/Creator/Generator.cpp
        Source/VeNo/Sound/Generation/WaveTables/Creator/TableCreatorHelper.cpp)

set(GUI_FILES
        # GUI-Lang
        Source/VeNo/Generated/GUIFiles.cpp
        Source/VeNo/GUI/GUIUtils.cpp
        Source/VeNo/GUI/GuiLang/GUIHandler.cpp
        Source/VeNo/GUI/GuiLang/GuiInterpreter.cpp
        Source/VeNo/GUI/GuiLang/GUIInit.cpp

        #Factory
        Source/VeNo/GUI/Factory/ComponentFactory.cpp
        Source/VeNo/GUI/Factory/LabelComponentFactory.cpp
        Source/VeNo/GUI/Factory/KnobComponentFactory.cpp
        Source/VeNo/GUI/Factory/SliderComponentFactory.cpp
        Source/VeNo/GUI/Factory/SelectComponentFactory.cpp

        # Components
        Source/VeNo/GUI/Components/BaseComponent.cpp
        Source/VeNo/GUI/Components/ComponentGroup.cpp
        Source/VeNo/GUI/Components/Knob.cpp
        Source/VeNo/GUI/Components/Label.cpp)

set(UTILS_FILES
        Source/VUtils/Environment.cpp
        Source/VUtils/FileHandler.cpp
        Source/VUtils/StringUtils.cpp
        Source/VUtils/Math.cpp
        Source/VUtils/Logging.cpp)

target_sources(${BaseTargetName} PRIVATE ${SOURCE_FILES} ${GUI_FILES} ${SOUND_FILES} ${UTILS_FILES})

target_compile_definitions(${BaseTargetName}
        PUBLIC
        JUCE_DISPLAY_SPLASH_SCREEN=0
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_include_directories(${BaseTargetName} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/headers/)

target_link_libraries(${BaseTargetName} PRIVATE
        assets
        juce::juce_audio_utils
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)