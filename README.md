# tsEngine [![License: GPL v3](https://img.shields.io/badge/License-GPLv3-green.svg)](https://github.com/tonyspan/tsEngine/blob/master/LICENSE)

This project is a small and simple 2D game engine I am building from scratch to learn about how modern engines (and games) are made. Written in `C++17`. My knowledge of 3D graphics is basic at the moment, so I ended up using `SDL2` for rendering (and for events, audio as well), and used `EnTT` for ECS. I read various sources (videos, sites, books, etc.) on how to architect an engine, the various systems, etc. Thanks also to [TheCherno](https://www.youtube.com/c/TheChernoProject) from which I took some knowledge and concepts/ideas.

I built a sandbox project, called AngryBird (yet another Flappy Bird clone) to demo the engine. Also, the engine supports native C++ scripting (at compile time) to add behavior to entities (similar API to Unity's C# scripts).

## Dependencies
* [`spdlog`](https://github.com/gabime/spdlog)
* [`EnTT`](https://github.com/skypjack/entt)
* [`SDL2`](https://www.libsdl.org/) (~~2.0.16~~ 2.0.18)
* [`glm`](https://github.com/g-truc/glm)
* [`ImGui`](https://github.com/ocornut/imgui)
* [`YAML`](https://github.com/jbeder/yaml-cpp/)

## Features
* Logger
* Audio
* Layers (unused)
* Event Handler
* ECS
* Renderer
* Physics
* Asset Manager
* Serializer
* C++ scripting (compile time)

## Getting Started
* Clone recursively this repository.
* Run `WinGenProject.bat` file found in `scripts` folder.
* Launch `tsEngine.sln`.

## Things I might add
* [WIP] Simple game/level editor
* [WIP] Dynamic (runtime) C++ script loading or python scripting. Depends on which is going to work :)
* Particle system
* Box2D

## Notes
* The repository is updated, most of the time, using amend (see changelog below for details).
* Currently works ONLY on Windows.

# Issues
* [Engine] Some unnoticed memory leaks possibly.
* [Engine/Sandbox] ~~UI text flickering.~~ (Fixed: 1/12/21)
* [Engine] ~~Physics colliders size if different that initial~~ (Fixed: 28/1/22).
* [Editor] On window resize or maximize, ImGui (and viewport) don't get resized/aligned properly (maybe switch to ImGui docking?).

# Changelog
<details>
<summary>Details</summary>
(Some dates aren't accurate)
  * [13/9/21] Started Engine project
  * [16/11/21] Finished Sandbox project
  * [1/12/21] Fixed annoying flickering bug (UI text), Sandbox project refactoring
  * [2/12/21] Switched to `SDL2 2.0.18` (to use ImGui)
  * [7/12/21] Engine improvements/refactoring/fixes
  * [9/12/21] Started Editor project
  * [27/12/21] More Engine improvements/refactoring/fixes based on Editor project
  * [3/1/22] Revisited menu class, thus Sandbox project. Editor improvements/refactoring
  * [15/1/22] Finished Serialization
  * [28/1/22] Small fixes
  * [10/3/22] Renderer, Events refactoring
</details>