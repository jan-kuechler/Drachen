#ifdef WIN32
#pragma once
#endif
#ifndef PCH_H
#define PCH_H

// Standard Header
#include <queue>
#include <vector>
#include <stack>
#include <array>
#include <tuple>

#include <string>

#include <fstream>

#include <stdexcept>

// Boost Header
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/bind.hpp>
#include <boost/exception/all.hpp>
#include <boost/algorithm/string.hpp>

// SFML Header
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

using namespace sf;

#endif //PCH_H
