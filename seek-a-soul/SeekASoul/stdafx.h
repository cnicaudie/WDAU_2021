#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "imgui.h"
#include "imgui-SFML.h"

#include <Engine/Game.h>
#include <Engine/Animation/Animated.h>
#include <Engine/Collision/BoxCollideable.h>
#include <Engine/Event/EventManager.h>
#include <Engine/Resources/TextureManager.h>
#include <Engine/Input/InputManager.h>

#include <Game/GameManager.h>
#include <Game/Action.h>

#include <iostream>
#include <vector>
#include <memory>
#include <cmath>
#include <algorithm>
#include <typeinfo>