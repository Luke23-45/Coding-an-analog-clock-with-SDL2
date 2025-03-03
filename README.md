# Colorful Clock with Particle Effect Animation

This project is a visually engaging analog clock implemented in C++ using SDL2 and SDL2_ttf libraries. It features a dynamic particle effect background, smooth gradient clock hands, multiple color schemes, and toggleable digital time and date displays.

## Getting Started

### Prerequisites
Ensure you have the following installed:
- C++ Compiler (e.g., g++)
- [SDL2](https://www.libsdl.org/)
- [SDL2_ttf](https://www.libsdl.org/projects/SDL_ttf/)

## Project Structure

* `clock.cc`: Main C++ source file containing the clock application logic.
* `Makefile`:  Build configuration file for compiling the project.
* `src/`: Source code directory (intended for better organization).
    * `include/`:  Directory for header files 

### Installation
1. Clone the repository:
    ```bash
    git clone git@github.com:Luke23-45/Coding-an-analog-clock-with-SDL2.git
    ```
## Building

This project uses `make` for building. To build the project, run the following command in your terminal:

1. Navigate to the project directory:
    ```bash
    cd Coding-an-analog-clock-with-SDL2
    ```
3. Compile the code:
    ```bash
    make
    ```
4. Run the executable (Linux/macOS):
    ```bash
    ./main

    ```
5. Run the executable (Windows):
    ```bash
    main.exe
    ```
6. To clean up the build artifacts:
    ```bash
    make clean
    ```

## Features
- **Analog Clock Display:**  Shows time using hour, minute, and second hands.
- **Dynamic Particle Background:** Features a subtle, moving particle effect in the background for visual interest.
- **Smooth Gradients:** Employs smooth gradient lines for the clock's outer circle and hands, adding a visually appealing aesthetic.
- **Multiple Color Schemes:**  Offers different color palettes that can be toggled to change the clock's appearance.
- **Toggleable Digital Clock:**  Allows you to display or hide a digital clock at the bottom of the window.
- **Toggleable Digital Date:**  Allows you to display or hide a digital date above the digital clock.
- **Pulsating Center Dot:**  Includes a small, pulsating dot at the center of the clock, adding a subtle animation.
- **Hour Labels and Tick Marks:** Clearly marked hour labels and minute/hour tick marks around the clock face for easy time reading.
- **Real-time Updates:** Clock hands and digital displays update in real-time, reflecting the current system time.

## Key Controls

| Action                | Key       | Description                                      |
| --------------------- | --------- | ------------------------------------------------ |
| Exit simulation     | `ESC` key | Closes the application.                          |
| Toggle Digital Clock  | `d` key   | Shows or hides the digital time display.         |
| Toggle Digital Date   | `t` key   | Shows or hides the digital date display.          |
| Change Color Scheme   | `c` key   | Cycles through different color schemes for the clock. |


## Code Structure
- **`clock.cc`**: This is the primary source file and contains all the C++ code for the clock application. It includes:
    - **SDL Initialization and Setup**:  Handles the initialization of SDL2, window creation, and renderer setup.
    - **Rendering Functions**:  Functions for drawing different elements of the clock, including:
        - Background gradients and particles (`drawGradientBackground`, `drawParticles`).
        - Clock face elements like the outer circle, hour marks, and labels (`drawCircle`, `drawClockHours`, `drawHourLabels`).
        - Clock hands (`drawHandle`).
        - Center dot (`drawCenterDot`).
        - Digital clock and date (`renderDigitalClock`, `renderDigitalDate`).
    - **Time and Angle Update Logic**: Functions to update the clock hands and retrieve system time (`updateClockAngles`).
    - **Event Handling**:  Processes user input for exiting, toggling digital displays, and changing color schemes.
    - **Main Loop**: The core application loop that handles event processing, updates, and rendering.
- **`Makefile`**: This file is used to automate the compilation process. It likely includes:
    - **Compiler Flags**: Specifies flags for the g++ compiler, including optimization and warning levels.
    - **Linking Instructions**:  Links the necessary SDL2 and SDL2_ttf libraries to create the executable.
    - **Build and Clean Targets**: Defines targets for compiling the code (`make`) and cleaning up compiled files (`make clean`).


## Demo Video
Check out the project demo video on YouTube: https://youtu.be/HDTzYFRzg50
## License

This project is licensed under the MIT License. Feel free to use, modify, and distribute the code.

## Acknowledgements

- SDL2 for graphics rendering.
- SDL2_ttf for font rendering.
