namespace sf
{

template <typename T>
std::ostream& operator<<( std::ostream& stream, const sf::Vector2<T>& vec )
{
	stream << vec.x << " " << vec.y;
	return stream;
}

template <typename T>
std::istream& operator>>( std::istream& stream, sf::Vector2<T>& vec )
{
	stream >> vec.x >> vec.y;
	return stream;
}

template <typename T>
std::ostream& operator<<( std::ostream& stream, const sf::Rect<T>& rect )
{
	stream
		<< rect.left   << " "
		<< rect.top    << " "
		<< rect.width  << " "
		<< rect.height << " "
	;
	return stream;
}

template <typename T>
std::istream& operator>>( std::istream& stream, sf::Rect<T>& rect )
{
	stream
		>> rect.left
		>> rect.top
		>> rect.width
		>> rect.height
	;
	return stream;
}

}
