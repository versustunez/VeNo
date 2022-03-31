// The Parameters can be Generated by the helper/guiFiles/generate.js!
#include <VeNo/GUI/GUIFiles.h>

namespace VeNo::GUI
{
// Empty STRING Reference
std::string Files::EMPTY;

std::string Files::MainGui = R""(~Author: Maurice Grönwoldt (VersusTuneZ)
~Version: 1.0
~Name: MainTheme
~Look: Flat

-- ROOT
w: 1200
h: 700
bg: rootBG
position-x: automatic

-- Layout
#Sidebar {
    header: Sidebar
    w: 25%
    h: 100%
    bg: primaryBG
    position-y: automatic
    @import Sidebar
}
#Content {
    position-y: automatic
    w: 75%
    -- MAIN TAB BAR
    #Tab {
        h: 65%
        position-x: automatic
        @Tabbed {
            w: 50%
            [4](
                @Tab {
                    name: OSC{it0}
                    color: 0,0,0,0
                    @import OSC;i={it0}
                }
            )
        }
        @Tabbed {
            w: 50%
            @Tab {
                name: Filter
            }
            @Tab {
                name: FX
            }
            @Tab {
                name: Config
            }
        }
    }
    #BottomGroup {
        h: 35%
        position-y: automatic
        #ModulatorsTabs {
            h: 80%
            position-x: automatic
            @Tabbed {
                width: 50%
                [4](
                    @Tab#Env{it0} {
                        #Flex {
                            height: 90%
                            flex: true
                            justify-content: space-around
                            align-items: center
                            @Knob#Attack {
                                bind: env{it0}__attack
                                width: 80
                                height: 100
                            }
                            @Knob#Decay {
                                bind: env{it0}__decay
                                width: 80
                                height: 100
                            }
                            @Knob#Sustain {
                                bind: env{it0}__sustain
                                width: 80
                                height: 100
                            }
                            @Knob#Release {
                                bind: env{it0}__release
                                width: 80
                                height: 100
                            }
                        }
                    }
                )
            }
            @Tabbed {
                width: 50%
                [2](
                    @Tab#LFO{it0} {
                    }
                )
                [2](
                    @Tab#Random{it0} {
                    }
                )
            }
        }

        #Controls {
            h: 20%
            width: 100%
            bg: primaryBG
            position-x: automatic
            @import Footer
        }
    }
})"";
std::string Files::OSC = R""(#OSCGroup {
    position-y: automatic
    #CTRL {
        x: 0
        y: 0
        background: secondaryBG
        width: 100%
        height: 30

        @Switch {
            bind: osc{i}__active
            noText: true
            w: 20
            h: 20
            x: 10
            y: 5
        }
        @Button {
            mode: push
            text: Copy
            w: 70
            h: 20
            x: 40
            y: 5
            action: cpyOsc
        }
        @Button {
            mode: push
            text: Paste
            w: 70
            h: 20
            x: 120
            y: 5
            action: pstOsc
        }
    }

    #WaveForm {
        header: true
        background: primary
        width: 100%
        height: 160
    }

    #WaveFormControls {
        width: 100%
        height: 55
        background: primaryBG
        flex: true
        align-items: center
        justify-content: space-around

        #PhaseGroup {
            width: 105
            position-x: automatic
            @Label {
                text: Phase
                width: 40
            }
            @Knob {
                bind: osc{i}__phase
                tooltip: true
                label: no
                width: 40
            }
        }

        @Button {
            filled: true
            text: Editor
            width: 80
            height: 30
        }

        @Switch#Random {
            bind: osc{i}__random_phase
            width: 80
            height: 30
        }

        #WavePosition {
            width: 105
            position-x: automatic
            @Label {
                text: Wave
                width: 40
            }
            @Knob {
                bind: osc{i}__wave_position
                tooltip: true
                label: no
                width: 40
            }
        }
    }

    #OSCControls {
        #OSCPartOne {
            background: secondaryBG
            x: 0
            width: 100
            height: 180

            @Knob#Level {
                bind: osc{i}__level
                label-value: semi
                w: 80
                h: 70
                x: 10
                y: 10
            }
            @Knob#Pan {
                bind: osc{i}__panning
                label-value: semi
                w: 80
                h: 70
                x: 10
                y: 90
            }
        }
        #OSCPartTwo {
            background: secondaryBG
            flex: true
            justify-content: space-around
            align-items: center
            x: 100
            height: 40
            width: 350
            #SemitonesGroup {
                width: 100
                height: 80%
                position-x: automatic
                @Label {
                    text: Semi
                    width: 40
                }
                @ValueBox {
                    bind: osc{i}__semitones
                    full-width: true
                    tooltip: false
                    label: no
                    width: 60
                }
            }
            #FineGroup {
                width: 100
                height: 80%
                position-x: automatic
                @Label {
                    text: Fine
                    width: 40
                }
                @ValueBox {
                    bind: osc{i}__cents
                    full-width: true
                    tooltip: false
                    label: no
                    width: 60
                }
            }
            #StereoGroup {
                width: 100
                height: 80%
                position-x: automatic
                @Label {
                    text: Wide
                    width: 40
                }
                @ValueBox {
                    bind: osc{i}__stereo
                    full-width: true
                    tooltip: false
                    label: no
                    width: 60
                }
            }

        }
        #OSCPartThree {
            background: secondaryBG
            x: 100
            y: 40
            height: 140
            width: 350
            flex: true
            justify-content: space-around
            align-items: center

            #LeftBox {
                width: 100
                height: 80%
                position-x: automatic
                flex: true
                flex-direction: column
                @ValueBox#Voices {
                    bind: osc{i}__voices
                    height: 50
                    full-width: true
                    tooltip: false
                }

                @Select#Detune Mode {
                    bind: osc{i}__detune_mode
                    height: 50
                    preset: detune
                }
            }

            @Knob#Detune Amount {
                bind: osc{i}__detune_amount
                label-value: semi
                width: 80
                height: 80
            }
            @Knob#Detune Dense {
                bind: osc{i}__detune_dense
                width: 80
                height: 80
            }
        }
    }
}
)"";
std::string Files::WaveEditor = R""(~Author: Maurice Grönwoldt (VersusTuneZ)
~Version: 1.0
~Name: WaveEditor
~Look: Flat

w: 560
h: 320
bg: secondaryBG
position-x: automatic

#EditorSpace {
    width: 450
    @WaveEditor {
        x: 10
        y: 10
        width: 430
        height: 280
        -- WaveEditor is the only Component that will listen to the background property
        background: 33,33,33,255
    }
    #Bar {
        background: primaryBG
        height: 20
        y: 300
        position-x: automatic
        #LabelContainer {
            width: 100
            position-x: automatic
            @Label {
                width: 20
                text: X:
            }
            @Label {
                id: xPoint
                text-align: centredLeft
                text: 0.0000
            }
        }
        #LabelContainer {
            width: 100
            position-x: automatic
            @Label {
                width: 20
                text: Y:
            }
            @Label {
                id: yPoint
                text-align: centredLeft
                text: 0.0000
            }
        }
    }
}

#WaveSidebar {
    w: 110
    h: 320
    background: primaryBG

    @WaveThumbnails {
       x: 0
       y: 10
       height: 240
    }

    #Buttons {
        y: 250
        width: 100%
        height: 70

        @Button {
            filled: true
            text: Save
            x: 10
            y: 10
            width: 90
            height: 20
        }
        @Button {
            filled: true
            text: Load
            y: 42
            x: 10
            width: 90
            height: 20
        }
    }
})"";
std::string Files::Footer = R""(#Pitch  {
    y: 5%
    width: 20%
    height: 90%
    position-x: automatic

    @Label {
        text: Pitch Range
        width: 50%
    }

    @ValueBox#Up {
        y: 12.5%
        bind: pitchbend__up
        height: 75%
        width: 25%
        label: no
        tooltip: false
    }

    @ValueBox#Down {
        y: 12.5%
        bind: pitchbend__down
        height: 75%
        width: 25%
        label: no
        tooltip: false
    }
}

@Label#Tooltip {
    width: 60%
    event: tooltip
}

#Legato/Porta {
    width: 20%
    y: 5%
    height: 90%
    position-x: automatic

    @Switch {
        bind: mono_legato
        width: 50%
    }
    @Knob {
        bind: portamento
        width: 50%
        height: 100%
        label: no
        tooltip: true
    }
})"";
std::string Files::Sidebar = R""(@Logo {
    height: 60
    width: 100%
}

#LCD {
    -- @TODO: Implement LCD Component -> it will listen to the bg :>
    h: 160
    bg: lcdBG
}

#Preset {
    -- @TODO: Implement Preset Component
    header: true
    h: 50
    bg: secondaryBG
}

#Mixer {
    h: 130
    flex: true
    justify-content: space-around
    bg: 0,0,0,70

    [4](
        #OSC{it0} {
            w: 20%
            header: true
            position-y: automatic
            @Switch {
                h: 30
                x: 5%
                w: 90%
                bind: osc{it0}__active
                on: On
                off: Off
            }
            @Knob#Volume {
                h: 70
                bind: osc{it0}__level
                label: bottom
                label-value: semi
            }
        }
    )
}

#Master Volume {
    y-offset: 10
    w: 50%
    h: 175
    x: 25%

    @Knob#Master {
        bind: master__volume
        label: top
        label-value: semi
    }
}

#ModKnobs {
    h: 90
    position-x: automatic
    y-offset: 20
    flex: true
    justify-content: space-around

    [4](
        @Knob {
            w: 20%
            bind: mod{it0}
        }
    )
})"";

} // VeNo::GUI
