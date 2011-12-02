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
	ClearImage();
	RequestResize();
	Invalidate();
}

const sf::String& Button::GetLabel() const {
	return m_label;
}

void Button::SetImage( const Image::Ptr& image ) {
	SetLabel( L" " );
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
		SetState( NORMAL );
		return;
	}

	if( button == sf::Mouse::Left ) {
		if( press ) {
			SetState( ACTIVE );
		}
		else {
			SetState( PRELIGHT );
		}
	}
}

sf::Vector2f Button::CalculateRequisition() {
	float padding( Context::Get().GetEngine().GetProperty<float>( "Padding", shared_from_this() ) );
	const std::string& font_name( Context::Get().GetEngine().GetProperty<std::string>( "FontName", shared_from_this() ) );
	unsigned int font_size( Context::Get().GetEngine().GetProperty<unsigned int>( "FontSize", shared_from_this() ) );
	const sf::Font& font( *Context::Get().GetEngine().GetResourceManager().GetFont( font_name ) );

	if( GetChild() ) {
		sf::Vector2f requisition(
			GetChild()->GetRequisition().x + 2 * GetBorderWidth() + 2 * padding,
			GetChild()->GetRequisition().y + 2 * GetBorderWidth() + 2 * padding
		);

		return requisition;
	}

	sf::Vector2f metrics = Context::Get().GetEngine().GetTextMetrics( m_label, font, font_size );
	metrics.y = Context::Get().GetEngine().GetLineHeight( font, font_size );

	sf::Vector2f requisition(
		metrics.x + 2 * GetBorderWidth() + 2 * padding,
		metrics.y + 2 * GetBorderWidth() + 2 * padding
	);

	return requisition;
}

const std::string& Button::GetName() const {
	static const std::string name( "Button" );
	return name;
}

void Button::HandleMouseClick( sf::Mouse::Button button, int /*x*/, int /*y*/ ) {
	if( button != sf::Mouse::Left ) {
		return;
	}

	OnClick();
}

void Button::HandleAdd( const Widget::Ptr& child ) {
	Bin::HandleAdd( child );

	if( GetChild() && GetChild()->GetName() != "Image" ) {
#ifdef SFGUI_DEBUG
		std::cerr << "SFGUI warning: Only an Image can be added to a Button." << std::endl;
#endif

		Remove( GetChild() );
	}
}

void Button::HandleAllocationChange( const sf::FloatRect& /*old_allocation*/ ) {
	Widget::Ptr child = GetChild();

	if( !child ) {
		return;
	}

	float padding( Context::Get().GetEngine().GetProperty<float>( "Padding", shared_from_this() ) );
	float border_width( Context::Get().GetEngine().GetProperty<float>( "BorderWidth", shared_from_this() ) );

	sf::FloatRect allocation( GetAllocation() );

	allocation.Left = padding + border_width;
	allocation.Top = padding + border_width;
	allocation.Width -= 2 * ( padding + border_width );
	allocation.Height -= 2 * ( padding + border_width );

	child->SetAllocation( allocation );
}

void Button::HandleStateChange( State old_state ) {
	Widget::Ptr child = GetChild();

	if( child ) {
		float padding( Context::Get().GetEngine().GetProperty<float>( "Padding", shared_from_this() ) );
		float border_width( Context::Get().GetEngine().GetProperty<float>( "BorderWidth", shared_from_this() ) );

		sf::FloatRect allocation( GetAllocation() );

		allocation.Width -= 2 * ( padding + border_width );
		allocation.Height -= 2 * ( padding + border_width );

		if( GetState() == ACTIVE ) {
			allocation.Left = padding + 2 * border_width;
			allocation.Top = padding + 2 * border_width;
		}
		else {
			allocation.Left = padding + border_width;
			allocation.Top = padding + border_width;
		}

		child->SetAllocation( allocation );
	}

	Bin::HandleStateChange( old_state );
}

}
