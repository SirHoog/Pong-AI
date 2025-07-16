# Pong AI
## Overview
### What?
I used a [Genetic Algorithm](https://www.geeksforgeeks.org/dsa/genetic-algorithms/)
(a type of evolutionary algorithm) to train neural networks which control paddles in multiple instances of the game Pong running in parallel.

You can customize most aspects of the training process from rewards and penalties to loading and saving neural networks for later use or custom match-making.

You can control one the paddles and load in a saved neural network to play against. Or you can load in two AIs you've saved before and have them face off in your own tournament.

Now, AIs playing pong might seem basic and boring... and it is... but I've worked hard on this project, so please check it out :sweat_smile:.
### Why?
Before this, I've literally never coded a complete, functioning project (only broken parts of ambitious dreams) so I wanted to try something really simple while learning C++ and machine learning at the same time. 
This was the perfect project to learn, apply my knowledge, and have fun.

Although using neural networks to play Pong is really overkill (because simply tracking the ball is sufficient),
implementing Pong wasn't the focus in the first place. I wanted to quickly learn how to train a neural network to learn a simple task, using a simple algorithm (GA). I finally learned how to make my ideas into a reality.
### Tools:
* [SDL 3](https://www.libsdl.org): For graphics and rendering
* [SDL_image](): For loading PNG textures
* [Dear ImGui](https://github.com/ocornut/imgui): For creating UI really easily
* [ImPlot](https://github.com/epezent/implot): For AI fitness plotting
* [MiniDNN](https://github.com/yixuan/MiniDNN): For easy integration of neural networks in C++
* [Eigen](https://eigen.tuxfamily.org/): Dependency of MiniDNN for linear algebra and vector math in C++
## Building Step-by-Step (using CMake)
**NOTE:** I'm a lazy bum, so this project **<u>only supports Windows</u>** (so far).

If you want to use Linux or Mac, I think it should be easy enough. You might just need to change `src/CMakeLists.txt` for moving the built SDL files.

However, this should be easy to build yourself as it only uses CMake and FetchContent.

This repository contains a `CMakePresets.json` which you can edit, such as changing the generator from Ninja (default) to MSVC if you want.

### Visual Studio 2022
*(I'm not sure if this may build on older versions of Visual Studio)*

Clone this repository,
open in Folder View in Visual Studio,
wait for Ninja to automatically generate CMake files,
select the `PongAI.exe` startup item,
and build as x64 Debug or x64 Release. And you're done!

The executable should appear in `out/build/x64-debug/src/Debug` if you built for x64 Debug, and of course it should appear in `out/build/x64-release/src/Release` if you built for x64 Release.

### In the Terminal (using CMake Presets)
```cmd
cmake --preset <PRESET_NAME>
cmake --build --preset <PRESET_NAME>
```
Use `x64-debug` or `x64-release` in place of `<PRESET_NAME>`

## Did I use AI to help me code?
Yes. Of course I did. But I only used it to learn *how* I should use C++ to implement what I wanted, not what to mindlessly copy and paste.
So please don't think that I'm some brainrotted vibe-coder, okay? Thank you.

I only used ChatGPT for suggesting high-level changes to my code (because I would never rely on it to generate good code for more specific implementations. It hallucinates way too much).

I also used Perplexity for debugging, suggesting changes to my implementations of specific features, and asking how the libraries I was using worked, especially ImGui, to speed up my progress. Perplexity was really helpful and reliable compared to ChatGPT as it hallucinated less and gave me accurate answers.

I used the free version of GitHub Copilot in Visual Studio for auto-completion, but it was pretty buns ngl.

**But I'd say that at least 90% of my code was written by me. The rest might just be modified snippets from the AIs' responses.**

## Contact
If you want to reach out to me (please do :pray:), you can DM on Discord. Username: @sirhoog

I'd highly appreciate any feedback or invitations to work together. I'm a lonely programmer who might really enjoy working on projects together with you.
