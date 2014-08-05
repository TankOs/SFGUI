#include <SFGUI/SpinButton.hpp>
#include <SFGUI/Adjustment.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/RenderQueue.hpp>
#include <SFGUI/Engine.hpp>

#include <SFML/Graphics/Font.hpp>
#include <cmath>
#include <sstream>
#include <iomanip>

namespace sfg {

// Signals.
Signal::SignalID SpinButton::OnValueChanged = 0;

SpinButton::SpinButton() :
	m_elapsed_time( 0.f ),
	m_digits( 0 ),
	m_decrease_pressed( false ),
	m_increase_pressed( false ),
	m_repeat_wait( true )
{
}

SpinButton::Ptr SpinButton::Create( float minimum, float maximum, float step ) {
	auto adjustment = Adjustment::Create();
	adjustment->Configure( minimum, minimum, maximum, step, 0.f, 0.f );

	auto ptr = Ptr( new SpinButton );
	ptr->SetAdjustment( adjustment );

	return ptr;
}

SpinButton::Ptr SpinButton::Create( Adjustment::Ptr adjustment ) {
	if( !adjustment ) {
		return Create( 0.f, 0.f, 0.f );
	}

	auto ptr = Ptr( new SpinButton );
	ptr->SetAdjustment( adjustment );

	return ptr;
}

std::unique_ptr<RenderQueue> SpinButton::InvalidateImpl() const {
	return Context::Get().GetEngine().CreateSpinButtonDrawable( std::dynamic_pointer_cast<const SpinButton>( shared_from_this() ) );
}

sf::Vector2f SpinButton::CalculateRequisition() {
	const std::string& font_name( Context::Get().GetEngine().GetProperty<std::string>( "FontName", shared_from_this() ) );
	unsigned int font_size( Context::Get().GetEngine().GetProperty<unsigned int>( "FontSize", shared_from_this() ) );
	float border_width( Context::Get().GetEngine().GetProperty<float>( "BorderWidth", shared_from_this() ) );
	float text_padding( Context::Get().GetEngine().GetProperty<float>( "Padding", shared_from_this() ) );
	const sf::Font& font( *Context::Get().GetEngine().GetResourceManager().GetFont( font_name ) );
	auto line_height = Context::Get().GetEngine().GetFontLineHeight( font, font_size );

	return sf::Vector2f( 2 * (border_width + text_padding), line_height + 2 * ( border_width + text_padding ) );
}

const std::string& SpinButton::GetName() const {
	static const std::string name( "SpinButton" );
	return name;
}

bool SpinButton::IsDecreaseStepperPressed() const {
	return m_decrease_pressed;
}

bool SpinButton::IsIncreaseStepperPressed() const {
	return m_increase_pressed;
}

void SpinButton::HandleMouseButtonEvent( sf::Mouse::Button button, bool press, int x, int y ) {
	float border_width( Context::Get().GetEngine().GetProperty<float>( "BorderWidth", shared_from_this() ) );
	float stepper_aspect_ratio( Context::Get().GetEngine().GetProperty<float>( "StepperAspectRatio", shared_from_this() ) );

	if( button != sf::Mouse::Left ) {
		return;
	}

	auto stepper_height = ( GetAllocation().height / 2.f ) - border_width;
	auto stepper_width = ( GetAllocation().height / 2.f ) * stepper_aspect_ratio;

	if( press ) {
		// Top stepper.
		sf::FloatRect rect;
		rect.left = GetAllocation().left + GetAllocation().width - border_width - stepper_width;
		rect.top = GetAllocation().top + border_width;
		rect.width = stepper_width;
		rect.height = stepper_height;

		if( rect.contains( static_cast<float>( x ), static_cast<float>( y ) ) ) {
			GrabFocus( Widget::Ptr() );

			m_adjustment->Increment();

			m_elapsed_time = 0.f;
			m_increase_pressed = true;
			m_repeat_wait = true;

			Invalidate();
			return;
		}

		// Bottom stepper.
		rect.top = GetAllocation().top + border_width + stepper_height;

		if( rect.contains( static_cast<float>( x ), static_cast<float>( y ) ) ) {
			GrabFocus( Widget::Ptr() );

			m_adjustment->Decrement();

			m_elapsed_time = 0.f;
			m_decrease_pressed = true;
			m_repeat_wait = true;

			Invalidate();
			return;
		}
	}
	else {
		if( m_decrease_pressed || m_increase_pressed ) {
			Invalidate();
		}

		m_decrease_pressed = false;
		m_increase_pressed = false;
	}

	Entry::HandleMouseButtonEvent( button, press, x, y );
}

void SpinButton::HandleUpdate( float seconds ) {
	float stepper_speed( Context::Get().GetEngine().GetProperty<float>( "StepperSpeed", shared_from_this() ) );

	Entry::HandleUpdate( seconds );

	m_elapsed_time += seconds;

	if( m_elapsed_time < ( 1.f / stepper_speed ) ) {
		return;
	}

	if( m_repeat_wait ) {
		sf::Uint32 stepper_repeat_delay( Context::Get().GetEngine().GetProperty<sf::Uint32>( "StepperRepeatDelay", shared_from_this() ) );

		if( m_elapsed_time < ( static_cast<float>( stepper_repeat_delay ) / 1000.f ) ) {
			return;
		}

		m_repeat_wait = false;
	}

	m_elapsed_time = 0.f;

	// Increment / Decrement value while one of the steppers is pressed
	if( m_decrease_pressed ) {
		m_adjustment->Decrement();
		return;
	}
	else if( m_increase_pressed ) {
		m_adjustment->Increment();
		return;
	}
}

void SpinButton::HandleTextEvent( sf::Uint32 character ) {
	if( isdigit( static_cast<int>( character ) ) ) {
		Entry::HandleTextEvent( character );
		return;
	}

	if( ( character == '.' ) && ( GetText().find( "." ) == sf::String::InvalidPos ) ) {
		Entry::HandleTextEvent( character );
		return;
	}

	if( ( character == '-' ) && ( GetText().find( "-" ) == sf::String::InvalidPos ) && ( GetCursorPosition() == 0 ) ) {
		Entry::HandleTextEvent( character );
		return;
	}
}

void SpinButton::HandleKeyEvent( sf::Keyboard::Key key, bool press ) {
	Entry::HandleKeyEvent( key, press );

	if( !press || !HasFocus() ) {
		return;
	}

	if( key == sf::Keyboard::Return ) {
		GrabFocus( Widget::Ptr() );
	}
}

void SpinButton::HandleSizeChange() {
	float stepper_aspect_ratio( Context::Get().GetEngine().GetProperty<float>( "StepperAspectRatio", shared_from_this() ) );

	SetTextMargin( GetAllocation().height / 2.f * stepper_aspect_ratio );

	Entry::HandleSizeChange();
}

void SpinButton::HandleFocusChange( Widget::Ptr focused_widget ) {
	Entry::HandleFocusChange( focused_widget );

	if( focused_widget != shared_from_this() ) {
		UpdateAdjustmentFromText();
	}

	UpdateTextFromAdjustment();
}

Adjustment::Ptr SpinButton::GetAdjustment() const {
	return m_adjustment;
}

void SpinButton::SetAdjustment( Adjustment::Ptr adjustment ) {
	if( !adjustment ) {
		return;
	}

	if( m_adjustment ) {
		m_adjustment->GetSignal( Adjustment::OnChange ).Disconnect( m_adjustment_signal_serial );
	}

	m_adjustment = adjustment;

	auto weak_this = std::weak_ptr<Widget>( shared_from_this() );

	m_adjustment_signal_serial = m_adjustment->GetSignal( Adjustment::OnChange ).Connect( [weak_this] {
		auto shared_this = weak_this.lock();

		if( !shared_this ) {
			return;
		}

		auto spin_button = std::dynamic_pointer_cast<SpinButton>( shared_this );

		if( !spin_button ) {
			return;
		}

		spin_button->UpdateTextFromAdjustment();
	} );

	UpdateTextFromAdjustment();
}

float SpinButton::GetValue() const {
	return m_adjustment->GetValue();
}

void SpinButton::SetValue( float new_value ) {
	m_adjustment->SetValue( new_value );

	UpdateTextFromAdjustment();
}

float SpinButton::GetStep() const {
	return m_adjustment->GetMinorStep();
}

void SpinButton::SetStep( float step ) {
	m_adjustment->SetMinorStep( step );

	UpdateTextFromAdjustment();
}

void SpinButton::SetRange( float minimum, float maximum ) {
	m_adjustment->SetLower( minimum );
	m_adjustment->SetUpper( maximum );

	UpdateTextFromAdjustment();
}

unsigned int SpinButton::GetDigits() const {
	return m_digits;
}

void SpinButton::SetDigits( unsigned int digits ) {
	m_digits = digits;

	UpdateTextFromAdjustment();
}

void SpinButton::Configure( Adjustment::Ptr adjustment, float step, unsigned int digits ) {
	SetAdjustment( adjustment );
	SetStep( step );
	SetDigits( digits );
}

void SpinButton::UpdateTextFromAdjustment() {
	std::ostringstream sstr;
	sstr << std::fixed << std::setprecision( static_cast<int>( m_digits ) ) << m_adjustment->GetValue();
	Entry::SetText( sstr.str() );

	GetSignals().Emit( OnValueChanged );
}

void SpinButton::UpdateAdjustmentFromText() {
	if( !GetText().getSize() ) {
		m_adjustment->SetValue( m_adjustment->GetLower() );
		return;
	}

	std::istringstream text( GetText().toAnsiString() );

	float value;

	if( !( text >> value ) || !text.eof() ) {
		m_adjustment->SetValue( m_adjustment->GetLower() );
		return;
	}

	auto offset = value - m_adjustment->GetLower();
	auto multiple = ( m_adjustment->GetMinorStep() != 0.f ) ? ( offset / m_adjustment->GetMinorStep() ) : 0.f;
	auto step_offset = offset - std::floor( multiple ) * m_adjustment->GetMinorStep();

	if( step_offset >= m_adjustment->GetMinorStep() / 2.f ) {
		value = m_adjustment->GetLower() + std::ceil( multiple ) * m_adjustment->GetMinorStep();
	}
	else {
		value = m_adjustment->GetLower() + std::floor( multiple ) * m_adjustment->GetMinorStep();
	}

	m_adjustment->SetValue( value );
}

}
