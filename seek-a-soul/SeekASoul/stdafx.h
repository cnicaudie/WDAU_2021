#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "imgui.h"
#include "imgui-SFML.h"

#include <Engine/Game.h>
#include <Engine/Collision/BoxCollideable.h>
#include <Engine/Collision/CollisionDirection.h>
#include <Engine/Event/EventManager.h>
#include <Engine/Resources/TextureManager.h>
#include <Engine/Input/InputManager.h>
#include <Engine/Log/Log.h>

#include <Game/GameManager.h>
#include <Game/Action.h>

#include <UI/UIViewModel.h>

#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <algorithm>
#include <typeinfo>
#include <chrono>