#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/SharedPtr.hpp>

#include <SFML/Graphics.hpp>

namespace sfg {

class RendererViewport {
	public:
		typedef SharedPtr<RendererViewport> Ptr;

		RendererViewport();

		void SetSourceOrigin( const sf::Vector2f& origin );

		const sf::Vector2f& GetSourceOrigin() const;

		void SetDestinationOrigin( const sf::Vector2f& origin );

		const sf::Vector2f& GetDestinationOrigin() const;

		void SetSize( const sf::Vector2f& size );

		const sf::Vector2f& GetSize() const;

	private:
		sf::Vector2f m_source_origin;
		sf::Vector2f m_destination_origin;
		sf::Vector2f m_size;

		std::size_t m_id;

		static std::size_t last_id;
};

}
