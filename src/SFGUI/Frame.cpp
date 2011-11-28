#include <SFGUI/Frame.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>

namespace sfg {

Frame::Frame() :
	Bin(),
	m_alignment( .0f )
{
}

Frame::Ptr Frame::Create( const sf::String& label ) {
	Frame::Ptr ptr( new Frame );

	ptr->SetLabel( label );

	return ptr;
}

RenderQueue* Frame::InvalidateImpl() const {
	return Context::Get().GetEngine().CreateFrameDrawable( DynamicPointerCast<const Frame>( shared_from_this() ) );
}

void Frame::SetLabel( const sf::String& label ) {
	m_label = label;
	RequestResize();
	Invalidate();
}

const sf::String& Frame::GetLabel() const {
	return m_label;
}

void Frame::SetAlignment( float alignment ) {
	m_alignment = std::max( 0.f, std::min( 1.f, alignment ) );
}

float Frame::GetAlignment() const {
	return m_alignment;
}

sf::Vector2f Frame::CalculateRequisition() {
	float padding( Context::Get().GetEngine().GetProperty<float>( "Padding", shared_from_this() ) );
	const std::string& font_name( Context::Get().GetEngine().GetProperty<std::string>( "FontName", shared_from_this() ) );
	unsigned int font_size( Context::Get().GetEngine().GetProperty<unsigned int>( "FontSize", shared_from_this() ) );
	const sf::Font& font( *Context::Get().GetEngine().GetResourceManager().GetFont( font_name ) );
	float label_padding( Context::Get().GetEngine().GetProperty<float>( "LabelPadding", shared_from_this() ) );
	float border_width( Context::Get().GetEngine().GetProperty<float>( "BorderWidth", shared_from_this() ) );

	sf::Vector2f requisition( Context::Get().GetEngine().GetTextMetrics( m_label, font, font_size ) );
	requisition.x += 2 * label_padding + 4 * border_width;
	requisition.y = Context::Get().GetEngine().GetLineHeight( font, font_size ) + 4 * border_width;

	Widget::Ptr child = GetChild();
	if( child ) {
		requisition.x += 2.f * padding + child->GetRequisition().x;
		requisition.y += 2.f * padding + child->GetRequisition().y;
	}

	return requisition;
}

const std::string& Frame::GetName() const {
	static const std::string name( "Frame" );
	return name;
}

void Frame::HandleAllocationChange( const sf::FloatRect& /*old_allocation*/ ) {
	Widget::Ptr child = GetChild();
	if( !child ) {
		return;
	}

	float padding( Context::Get().GetEngine().GetProperty<float>( "Padding", shared_from_this() ) );
	float border_width( Context::Get().GetEngine().GetProperty<float>( "BorderWidth", shared_from_this() ) );
	const std::string& font_name( Context::Get().GetEngine().GetProperty<std::string>( "FontName", shared_from_this() ) );
	unsigned int font_size( Context::Get().GetEngine().GetProperty<unsigned int>( "FontSize", shared_from_this() ) );
	const sf::Font& font( *Context::Get().GetEngine().GetResourceManager().GetFont( font_name ) );
	float line_height( Context::Get().GetEngine().GetLineHeight( font, font_size ) );

	sf::FloatRect allocation( GetAllocation() );

	allocation.Left = padding + 2 * border_width;
	allocation.Top = line_height + padding + 2 * border_width;
	allocation.Width -= 2 * padding + 4 * border_width;
	allocation.Height -= line_height + 2 * padding + 4 * border_width;

	child->SetAllocation( allocation );
}

}
