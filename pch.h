#pragma once
#ifndef PCH_H
#define PCH_H

// Alle normalerweise genutzten Header Dateien geh�ren hier rein

#pragma warning(disable: 4345)

// Standard Header
#include <queue>
#include <vector>
#include <stack>

#include <string>

#include <fstream>

#include <stdexcept>

// Boost Header
#include <boost/lexical_cast.hpp>

#include <boost/filesystem.hpp>

// SFML Header
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

using namespace sf;

#endif //PCH_H