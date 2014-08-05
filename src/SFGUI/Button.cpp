#include <SFGUI/Button.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>
#include <SFGUI/Image.hpp>
#include <SFGUI/RenderQueue.hpp>

namespace sfg {

Button::Ptr Button::Create( const sf::String& label ) {
	auto ptr = Ptr( new Button );
	ptr->SetLabel( label );
	return ptr;
}

std::unique_ptr<RenderQueue> Button::InvalidateImpl() const {
	return Context::Get().GetEngine().CreateButtonDrawable( std::dynamic_pointer_cast<const Button>( shared_from_this() ) );
}

void Button::SetLabel( const sf::String& label ) {
	m_label = label;
	RequestResize();
	Invalidate();
}

const sf::String& Button::GetLabel() const {
	return m_label;
}

void Button::SetImage( Image::Ptr image ) {
	ClearImage();
	Add( image );
}

Image::PtrConst Button::GetImage() const {
	return std::static_pointer_cast<Image>( GetChild() );
}

void Button::ClearImage() {
	auto child = GetChild();

	if( child ) {
		Remove( child );
	}
}

void Button::HandleMouseEnter( int /*x*/, int /*y*/ ) {
	if( GetState() == State::NORMAL ) {
		SetState( State::PRELIGHT );
	}
}

void Button::HandleMouseLeave( int /*x*/, int /*y*/ ) {
	if( GetState() == State::PRELIGHT ) {
		SetState( State::NORMAL );
	}
}

void Button::HandleMouseButtonEvent( sf::Mouse::Button button, bool press, int /*x*/, int /*y*/ ) {
	if( !IsMouseInWidget() ) {
		if( GetState() == State::ACTIVE ) {
			SetState( State::NORMAL );
		}

		return;
	}

	if( button == sf::Mouse::Left ) {
		if( press ) {
			SetState( State::ACTIVE );
		}
		else if( GetState() == State::ACTIVE ) {
			SetState( State::PRELIGHT );
		}
	}
}

sf::Vector2f Button::CalculateRequisition() {
	float padding( Context::Get().GetEngine().GetProperty<float>( "Padding", shared_from_this() ) );
	float spacing( Context::Get().GetEngine().GetProperty<float>( "Spacing", shared_from_this() ) );
	const std::string& font_name( Context::Get().GetEngine().GetProperty<std::string>( "FontName", shared_from_this() ) );
	unsigned int font_size( Context::Get().GetEngine().GetProperty<unsigned int>( "FontSize", shared_from_this() ) );
	const sf::Font& font( *Context::Get().GetEngine().GetResourceManager().GetFont( font_name ) );

	auto requisition = Context::Get().GetEngine().GetTextStringMetrics( m_label, font, font_size );
	requisition.y = Context::Get().GetEngine().GetFontLineHeight( font, font_size );

	requisition.x += 2 * padding;
	requisition.y += 2 * padding;

	if( GetChild() ) {
		requisition.x += GetChild()->GetRequisition().x;
		requisition.y = std::max( requisition.y, GetChild()->GetRequisition().y + 2 * padding );

		if( GetLabel().getSize() > 0 ) {
			requisition.x += spacing;
		}
	}

	return requisition;
}

const std::string& Button::GetName() const {
	static const std::string name( "Button" );
	return name;
}

bool Button::HandleAdd( Widget::Ptr child ) {
	if( child && child->GetName() != "Image" ) {
#if defined( SFGUI_DEBUG )
		std::cerr << "SFGUI warning: Only an Image can be added to a Button.\n";
#endif

		return false;
	}

	return Bin::HandleAdd( child );
}

void Button::HandleSizeChange() {
	AllocateChild();
}

void Button::AllocateChild() {
	auto child = GetChild();

	if( !child ) {
		return;
	}

	float padding( Context::Get().GetEngine().GetProperty<float>( "Padding", shared_from_this() ) );
	float border_width( Context::Get().GetEngine().GetProperty<float>( "BorderWidth", shared_from_this() ) );

	sf::FloatRect allocation( GetAllocation() );

	allocation.left = padding + border_width;
	allocation.top = padding + border_width;
	allocation.width = child->GetRequisition().x;
	allocation.height -= border_width * 2.f + padding * 2.f;

	if( GetState() == State::ACTIVE ) {
		allocation.left += border_width;
		allocation.top += border_width;
	}

	child->SetAllocation( allocation );
}

void Button::HandleStateChange( State old_state ) {
	AllocateChild();
	Bin::HandleStateChange( old_state );
}

}
