# Hudsight Drawing
Using Hudsight's drawing hook to render anything to the screen

https://hudsight.com/

## How to use
- Add one of the Crosshair image files to Hudsight that fills your screen.
- Configure Hudsight to use Hooks for rendering the crosshair
- Compile & Run the project, and you'll see a rotating square.

## How do I import into my own project?
- Import hudsight.h and hudsight.cpp into your project and initialize the HudsightDrawing class
- Drawing methods must be called between the classes HudsightDrawing::frame_start and HudsightDrawing::frame_end functions
