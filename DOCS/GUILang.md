# GUI Markup Language

GUI Markup Language is parsed line by line

It will interprete the GUI on Runtime for every opening dont make the layout to complex

## Syntax

| Syntax    | Meaning |
|-----------|---------|
| @{0}#{1}  | 0 is the component 1 is the Name and it's optional |
| #{0}      | 0 is the name and not Optional |
| ~         | File Information |
| --        | Comment
| @import   | Import files to avoid code duplication

## Available Components

- Label
- Knob
- Slider
- Select
- Button

## Imports

to use Imports write: @import {name}

if you want to use import variables you can they are splitted by: ; and =

example:
```
@import Bin::Oscillator;osc=1;isActive
```
That example line imports the Binary Oscillator File with variable osc with the value "1" and isActive with value "false"

## Group & Component Properties

- x
- y
- w
- h
- bg (only on Groups currently)
- bind (only on Compoenents)

## Color Values

we allow predefined colors that you can setup software wide or instead use custom bgs

```
primaryBG
secondaryBG
primary
secondary
font
clip
lcdBG
lcd
```

if a invalid color picked then it will fallback to 255,255,255,255 aka white

if you use custom make sure it's rgb or rgba format range 0-255!

the parser dont allow corrupted rgba values! if you specific a invalid color it will skip the bg and use no color

```
RED Color RGBA!
custom color: bg: 255,0,0,255
```

## Component Properties (Not Default ones)

Components are registered with Component Factories that know how to implement them all of the are based on BaseComponent

If a Custom Component is needed ask if we can provide it or build it yourself :P

### Label

| property | value |
|----------|-------|
| text | text to show |

### Knob

| property | value |
|----------|-------|
| label | bottom,top,no,center |
| labelvalue | real, showname |

### Select

| property | value |
|----------|-------|
| preset | detune,filter,clip |
| values | , seperated list of values example: hard,soft,acid |


### Button
| property | value |
|----------|-------|
| mode | push or toggle |
| action | event-listener name (only on push mode) |

### LCD
| property | value |
|----------|-------|
| mode | number (1-4) |
