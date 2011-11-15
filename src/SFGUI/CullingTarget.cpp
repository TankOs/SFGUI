#include <SFGUI/CullingTarget.hpp>

namespace sfg {

CullingTarget::CullingTarget( sf::RenderTarget& target ) :
	m_real_target( target ),
	m_cull_count( std::pair<std::size_t, std::size_t>( 0, 0 ) ),
	m_cull( true )
{
	sf::View view = GetView();

	m_view_rect = sf::FloatRect(
		view.GetCenter().x - GetView().GetSize().x / 2.f,
		view.GetCenter().y - GetView().GetSize().y / 2.f,
		view.GetSize().x,
		view.GetSize().y
	);

	m_viewport = GetView().GetViewport();
}

void CullingTarget::SetView( const sf::View& view ) {
	m_real_target.SetView( view );

	m_view_rect = sf::FloatRect(
		view.GetCenter().x - GetView().GetSize().x / 2.f,
		view.GetCenter().y - GetView().GetSize().y / 2.f,
		view.GetSize().x,
		view.GetSize().y
	);

	m_viewport = GetView().GetViewport();
}

const sf::View& CullingTarget::GetView() const {
	return m_real_target.GetView();
}

unsigned int CullingTarget::GetWidth() const {
	return m_real_target.GetWidth();
}

unsigned int CullingTarget::GetHeight() const {
	return m_real_target.GetHeight();
}

const std::pair<std::size_t, std::size_t>& CullingTarget::GetCount() const {
	return m_cull_count;
}

void CullingTarget::ResetCount() {
	m_cull_count.first = 0;
	m_cull_count.second = 0;
}

void CullingTarget::Cull( bool enable ) {
	m_cull = enable;
}

void CullingTarget::Draw( const RenderQueue& queue ) {
	if( !m_cull ) {
		m_real_target.Draw( queue );
		return;
	}

	if( ( m_viewport.Left > 1.f ) ||
	    ( m_viewport.Top > 1.f )  ||
	    ( m_viewport.Left + m_viewport.Width < .0f ) ||
	    ( m_viewport.Top + m_viewport.Height < .0f ) ) {
		m_cull_count.second++;
		return;
	}

	sf::Vector2f position = queue.GetPosition();

	m_view_rect.Left -= position.x;
	m_view_rect.Top -= position.y;

	// Time to cull... :)
	bool discard = true;

	const RenderQueue::DrawablesVector& drawables = queue.GetDrawables();
	std::size_t children_size = drawables.size();

	for( RenderQueue::DrawablesVector::size_type index = 0; index < children_size; ++index ) {
		if( m_view_rect.Left > drawables[index].second.Left + drawables[index].second.Width ) {
			continue;
		}
		if( m_view_rect.Top > drawables[index].second.Top + drawables[index].second.Height ) {
			continue;
		}
		if( drawables[index].second.Left > m_view_rect.Left + m_view_rect.Width ) {
			continue;
		}
		if( drawables[index].second.Top > m_view_rect.Top + m_view_rect.Height ) {
			continue;
		}

		discard = false;
		break;
	}

	m_view_rect.Left += position.x;
	m_view_rect.Top += position.y;

	if( !discard ) {
		m_real_target.Draw( queue );
		m_cull_count.first++;
	}
	else {
		m_cull_count.second++;
	}
}

}
