#ifdef WIN32
#pragma once
#endif
#ifndef PCH_H
#define PCH_H

// Alle normalerweise genutzten Header Dateien gehören hier rein

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
#include <boost/range/algorithm.hpp>
#include <boost/bind.hpp>
#include <boost/exception/all.hpp>
#include <boost/algorithm/string.hpp>

// SFML Header
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

using namespace sf;

#endif //PCH_H
