#pragma once

#include <SFGUI/Config.hpp>

#include <sstream>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

namespace sf
{

template <typename T>
std::ostream& operator<<( std::ostream& stream, const sf::Vector2<T>& vec );

template <typename T>
std::istream& operator>>( std::istream& stream, sf::Vector2<T>& vec );

template <typename T>
std::ostream& operator<<( std::ostream& stream, const sf::Rect<T>& rect );

template <typename T>
std::istream& operator>>( std::istream& stream, sf::Rect<T>& rect );

}

#include "StreamOperators.inl"
