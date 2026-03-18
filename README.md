# CNC Pencil Writing Machine

A two-axis CNC machine built mostly from scratch — wood frame, 
purchased linear rails and stepper motors, custom motion control 
firmware written from scratch in Arduino C.

[Full writeup and video](https://portfolio-seven-mu-24.vercel.app/projects/cnc-pencil-machine)

## How it works

Two stepper motors drive a gantry system along X and Y axes via GT2 
timing belts and linear rails. The firmware handles all motion 
control without any external library.

**Homing** — on startup, both axes drive toward their home limit 
switches, back off, then re-approach slowly for a precise home 
position. Coordinates are zeroed after homing.

**Coordinate navigation** — `goTo(x, y)` moves the gantry to any 
target coordinate using Bresenham's line algorithm, which distributes 
steps across both axes to produce smooth diagonal motion without 
skipping or stutter.

**Drawing primitives** — built on top of `goTo()`:
- `drawCenteredSquare(size, x, y)`
- `drawCenteredTriangle(side, x, y)`  
- `drawCircle(centerX, centerY, radius, resolution)`

## Hardware

| Component | Details |
|---|---|
| Frame | Pine wood |
| Rails | Linear rails with carriage blocks |
| Motion | GT2 timing belts + pulleys |
| Motors | 2x NEMA 17 stepper motors (main axes) |
| Controller | Arduino Uno |
| Drivers | A4988 stepper drivers |
| Limit switches | 4x (both ends of each axis) |

## Pin mapping

| Arduino Pin | Connection |
|---|---|
| D12 | Dir — Motor 1 (X axis) |
| D13 | Step — Motor 1 (X axis) |
| D10 | Dir — Motor 2 (Y axis) |
| D11 | Step — Motor 2 (Y axis) |
| D1 | Limit switch 1A (X close) |
| D2 | Limit switch 1B (X far) |
| D3 | Limit switch 2B (Y close) |
| D4 | Limit switch 2A (Y far) |

## Files

- `SpinTilLimit.ino` — Main firmware: homing, coordinate navigation, 
  drawing primitives (squares, triangles, circles)
- `back_and_forth.ino` — Early axis test: moves both motors 700 steps 
  forward and back in a loop
- `LimSwitchTest.ino` — Limit switch diagnostic: prints Open/Closed 
  state of pin 2 to serial

## What's next

- G-code parser — would allow any vector image to be fed as a draw 
  path instead of hardcoded shapes
- Aluminum extrusion frame for better rigidity
- Pen lift servo for travel moves without drawing
