# Project Setup Guide

## Requirements

- **SDL 2.0.22**  
  Required for `SDL.h`. This is the core library for rendering and window generation.

- **SDL_ttf-devel-2.22.0**  
  Required for `SDL_ttf.h`. Used for rendering fonts.

- **Compiler**  
  GCC or any other compatible C compiler. The project uses standard libraries such as:
  - `time.h`
  - `stdio.h`
  - `stdlib.h`
  - `stdbool.h`
  - `math.h`

- **System Software**  
  Works on **macOS**, **Linux**, or **Windows**.

- **Hardware Specifications**
  - Minimum screen resolution: **1200 × 640 pixels**
  - **Mouse** required for data input.

---

## Instructions for Setup

1. **Install SDL Libraries**  
   - Download and install the required SDL libraries.  
   - Include the library headers in your project.  
   - Copy the `.dll` files to the same directory as your executable.

2. **Configure Your Project**  
   - Include the following in your source files:  
     ```c
     #include <SDL.h>
     #include <SDL_ttf.h>
     ```
   - Link the necessary SDL libraries during compilation.

3. **Add Project Files**  
   - Place the entire Git folder (project directory) into the library folder.

---

**✅ Your environment is now ready for building and running SDL applications.**
