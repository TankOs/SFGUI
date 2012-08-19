#include <SFGUI/Button.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>

namespace sfg {

Button::Button() :
	Bin()
{
}

Button::~Button() {
}

Button::Ptr Button::Create( const sf::String& label ) {
	Button::Ptr ptr( new Button );

	ptr->SetLabel( label );

	return ptr;
}

RenderQueue* Button::InvalidateImpl() const {
	return Context::Get().GetEngine().CreateButtonDrawable( DynamicPointerCast<const Button>( shared_from_this() ) );
}

void Button::SetLabel( const sf::String& label ) {
	m_label = label;
	RequestResize();
	Invalidate();
}

const sf::String& Button::GetLabel() const {
	return m_label;
}

void Button::SetImage( const Image::Ptr& image ) {
	ClearImage();
	Add( image );
}

const Image::Ptr Button::GetImage() const {
	return StaticPointerCast<Image>( GetChild() );
}

void Button::ClearImage() {
	if( GetChild() ) {
		Remove( GetChild() );
	}
}

void Button::HandleMouseEnter( int /*x*/, int /*y*/ ) {
	if( GetState() == NORMAL ) {
		SetState( PRELIGHT );
	}
}

void Button::HandleMouseLeave( int /*x*/, int /*y*/ ) {
	if( GetState() == PRELIGHT ) {
		SetState( NORMAL );
	}
}

void Button::HandleMouseButtonEvent( sf::Mouse::Button button, bool press, int /*x*/, int /*y*/ ) {
	if( !IsMouseInWidget() ) {
		if( GetState() == ACTIVE ) {
			SetState( NORMAL );
		}

		return;
	}

	if( button == sf::Mouse::Left ) {
		if( press ) {
			SetState( ACTIVE );
		}
		else if( GetState() == ACTIVE ) {
			SetState( PRELIGHT );
		}
	}
}

sf::Vector2f Button::CalculateRequisition() {
	float padding( Context::Get().GetEngine().GetProperty<float>( "Padding", shared_from_this() ) );
	float spacing( Context::Get().GetEngine().GetProperty<float>( "Spacing", shared_from_this() ) );
	const std::string& font_name( Context::Get().GetEngine().GetProperty<std::string>( "FontName", shared_from_this() ) );
	unsigned int font_size( Context::Get().GetEngine().GetProperty<unsigned int>( "FontSize", shared_from_this() ) );
	const sf::Font& font( *Context::Get().GetEngine().GetResourceManager().GetFont( font_name ) );

	sf::Vector2f requisition = Context::Get().GetEngine().GetTextMetrics( m_label, font, font_size );
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

void Button::HandleAdd( const Widget::Ptr& child ) {
	Bin::HandleAdd( child );

	if( GetChild() && GetChild()->GetName() != "Image" ) {
#ifdef SFGUI_DEBUG
		std::cerr << "SFGUI warning: Only an Image can be added to a Button.\n";
#endif

		Remove( GetChild() );
	}
}

void Button::HandleSizeChange() {
	AllocateChild();
}

void Button::AllocateChild() {
	Widget::Ptr child = GetChild();

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

	if( GetState() == ACTIVE ) {
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
