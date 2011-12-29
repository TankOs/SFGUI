#include <SFGUI/CullingTarget.hpp>

namespace sfg {

CullingTarget::CullingTarget( sf::RenderTarget& target ) :
	m_cull_count( std::pair<std::size_t, std::size_t>( 0, 0 ) ),
	m_real_target( target ),
	m_last_view_id( 0 ),
	m_current_layer( 0 ),
	m_cull( true ),
	m_out_of_view( false )
{
	PushView( m_real_target.GetView() );

	m_current_frame_buffer = new FrameBuffer;
	m_layer_buffer[m_current_layer] = m_current_frame_buffer;
}

CullingTarget::~CullingTarget() {
	LayerBuffer::const_iterator layer_iter( m_layer_buffer.begin() );
	LayerBuffer::const_iterator layer_end( m_layer_buffer.end() );

	for( ; layer_iter != layer_end; ++layer_iter ) {
		delete layer_iter->second;
	}
}

void CullingTarget::PushView( const sf::View& view ) {
	m_view_stack.push_back( view );

	UpdateView();
}

void CullingTarget::PopView() {
	if( m_view_stack.size() == 1 ) {
#ifdef SFGUI_DEBUG
		std::cerr << "SFGUI warning: Can't pop last view from view stack." << std::endl;
#endif
		return;
	}

	m_view_stack.pop_back();

	UpdateView();
}

void CullingTarget::UpdateView() {
	const sf::View& view = m_view_stack[ m_view_stack.size() - 1 ];

	m_real_target.SetView( view );

	m_view_rect = sf::FloatRect(
		view.GetCenter().x - GetView().GetSize().x / 2.f,
		view.GetCenter().y - GetView().GetSize().y / 2.f,
		view.GetSize().x,
		view.GetSize().y
	);

	m_view_aabb.Left = static_cast<int>( m_view_rect.Left + .5f );
	m_view_aabb.Top = static_cast<int>( m_view_rect.Top + .5f );
	m_view_aabb.Width = static_cast<int>( m_view_rect.Left + m_view_rect.Width + .5f ); // Right
	m_view_aabb.Height = static_cast<int>( m_view_rect.Top + m_view_rect.Height + .5f ); // Bottom

	m_viewport = view.GetViewport();

	if( ( m_viewport.Left > 1.f ) ||
	    ( m_viewport.Top > 1.f )  ||
	    ( m_viewport.Left + m_viewport.Width < .0f ) ||
	    ( m_viewport.Top + m_viewport.Height < .0f ) ) {
		m_out_of_view = true;
	}
	else {
		m_out_of_view = false;
	}

	std::size_t view_cache_size = m_view_cache.size();

	// Check if view AABB is already in cache
	for( std::size_t index = 0; index < view_cache_size; ++index ) {
		const ViewCachePair& cache_pair = m_view_cache[ index ];
		if( m_view_aabb.Left == cache_pair.aabb.Left &&
				m_view_aabb.Top == cache_pair.aabb.Top &&
				m_view_aabb.Width == cache_pair.aabb.Width &&
				m_view_aabb.Height == cache_pair.aabb.Height  ) {
				m_current_view_id = cache_pair.id;

				m_view_map[m_current_view_id] = view;

				return;
		}
	}

	// Kick out an old view AABB and add new view AABB to cache
	if( view_cache_size >= 8 ) {
		m_view_cache.erase( m_view_cache.begin() );
	}

	ViewCachePair view_cache_pair = { ++m_last_view_id, m_view_aabb };

	m_view_cache.push_back( view_cache_pair );

	m_current_view_id = m_last_view_id;

	m_view_map[m_current_view_id] = view;
}

const sf::View& CullingTarget::GetView() const {
	return m_view_stack[ m_view_stack.size() - 1 ];
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

void CullingTarget::Draw( RenderQueue* queue ) {
	if( !m_cull ) {
		queue->SetAssociatedViewID( m_current_view_id );
		AddToFrame( queue );
		return;
	}

	if( m_out_of_view ) {
		m_cull_count.second++;
		return;
	}

	if( queue->GetCheckedViewID() == m_current_view_id ) {
		if( queue->GetCullPass() ) {
		AddToFrame( queue );
			m_cull_count.first++;
			return;
		}
		else {
			m_cull_count.second++;
			return;
		}
	}

	sf::Vector2f position = queue->GetPosition();

	m_view_aabb.Left -= static_cast<int>( position.x + .5f );
	m_view_aabb.Top -= static_cast<int>( position.y + .5f );
	m_view_aabb.Width -= static_cast<int>( position.x + .5f );
	m_view_aabb.Height -= static_cast<int>( position.y + .5f );

	// Time to cull... :)
	bool discard = true;

	sf::IntRect* aabbs = queue->GetAABBs();
	std::size_t aabbs_size = queue->GetAABBSize();

	for( std::size_t index = 0; index < aabbs_size; ++index ) {
		sf::IntRect& aabb = aabbs[index];

		if( m_view_aabb.Left <= aabb.Width &&
			  m_view_aabb.Top <= aabb.Height &&
			  aabb.Left <= m_view_aabb.Width &&
			  aabb.Top <= m_view_aabb.Height ) {
			discard = false;
			break;
		}
	}

	m_view_aabb.Left += static_cast<int>( position.x + .5f );
	m_view_aabb.Top += static_cast<int>( position.y + .5f );
	m_view_aabb.Width += static_cast<int>( position.x + .5f );
	m_view_aabb.Height += static_cast<int>( position.y + .5f );

	queue->SetCheckedViewID( m_current_view_id );

	if( !discard ) {
		AddToFrame( queue );
		m_cull_count.first++;
		queue->SetCullPass( true );
	}
	else {
		m_cull_count.second++;
		queue->SetCullPass( false );
	}
}

void CullingTarget::AddToFrame( RenderQueue* queue ) {
	int layer = queue->GetZOrder();

	if( layer != m_current_layer ) {
		m_current_layer = layer;

		m_current_frame_buffer = m_layer_buffer[m_current_layer];

		if( !m_current_frame_buffer ) {
			m_current_frame_buffer = new FrameBuffer;
			m_layer_buffer[m_current_layer] = m_current_frame_buffer;
		}
	}

	m_current_frame_buffer->push_back( queue );
}

void CullingTarget::Display() {
	unsigned int current_view_id = 0;

	LayerBuffer::const_iterator layer_iter( m_layer_buffer.begin() );
	LayerBuffer::const_iterator layer_end( m_layer_buffer.end() );

	sf::Transform transform;

	for( ; layer_iter != layer_end; ++layer_iter ) {
		FrameBuffer* frame_buffer = layer_iter->second;

		std::size_t frame_buffer_size = frame_buffer->size();

		for( std::size_t index = 0; index < frame_buffer_size; ++index ) {
			unsigned int associated_view_id = (*frame_buffer)[index]->GetAssociatedViewID();

			if( associated_view_id != current_view_id ) {
				current_view_id = associated_view_id;
				m_real_target.SetView( m_view_map[current_view_id] );
			}

			// TODO: Cache somewhere around here...
			transform = sf::Transform::Identity;
			transform.Translate( (*frame_buffer)[index]->GetPosition() );

			m_real_target.Draw( *( (*frame_buffer)[index] ), transform );
		}

		frame_buffer->clear();
	}

	m_view_map.clear();
	m_view_map.insert( m_view_map.begin(), std::pair<unsigned int, sf::View>( m_current_view_id, m_view_stack[ m_view_stack.size() - 1 ] ) );
}

}
