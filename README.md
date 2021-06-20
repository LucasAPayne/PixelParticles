# PixelParticles

This project uses the idea of cellular automata to implement simple simulations for particles such as sand and water, similar to the game Noita or a falling sand game.

## Getting Started
This repository uses [Premake](https://github.com/premake/premake-core) as its build system, but no install is necessary, as Premake is included in this repository.

To set up the repository on your local machine, use your preferred method of downloading the repository. Then, on Windows, navigate to the scripts folder and run the `Win-GenProjects.bat` file. There may be a warning dialog that pops up when you try to do this. If so, click "More Info", then click "Run Anyway". This batch file will generate a Visual Studio 2019 solution that is ready to run.

## What's Next
- Adding new types of particles
- Implementing particles as vertex buffers and rendering them all in one draw call to improve performance
- Allowing different types of particles to interact with each other
- Adding more physics elements such as velocity and gravity to make the simulation more realistic and to untie simulation speed from frame rate
- Adding an editor to allow users to place particles
