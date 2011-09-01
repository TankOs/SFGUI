#include <SFGUI/Window.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Box.hpp>
#include <SFGUI/Entry.hpp>
#include <SFGUI/Table.hpp>
#include <SFGUI/Label.hpp>
#include <SFGUI/Scale.hpp>
#include <SFGUI/Scrollbar.hpp>
#include <SFGUI/Engines/BREW.hpp>
//#include <SFGUI/Loaders/YAML.hpp>
#include <SFGUI/ThemeLoader.hpp>
#include <SFGUI/Context.hpp>

#include <SFML/Graphics.hpp>
#include <sstream>

class SampleApp {
	public:
		void Run();

	private:
		void OnAddButtonHClick( sfg::Widget::Ptr widget );
		void OnAddButtonVClick( sfg::Widget::Ptr widget );
		void OnNewButtonClick( sfg::Widget::Ptr widget );
		void OnToggleTitlebarClick( sfg::Widget::Ptr widget );
		void OnHideWindowClicked( sfg::Widget::Ptr widget );
		void OnRangeValueChange( sfg::Adjustment::Ptr adjustment );

		sfg::Window::Ptr m_wndmain;
		sfg::Box::Ptr m_boxbuttonsh;
		sfg::Box::Ptr m_boxbuttonsv;
		sfg::Entry::Ptr m_entry;
		sfg::Table::Ptr m_table;
		sfg::Scale::Ptr m_scale;
		sfg::Scrollbar::Ptr m_scrollbar;
		sfg::Label::Ptr m_range_value;
};

void SampleApp::Run() {
	sf::RenderWindow  window( sf::VideoMode( 1024, 768, 32 ), "SFGUI test" );
	sf::Event  event;

	window.SetFramerateLimit( 60 );
	//window.UseVerticalSync( true );

	// Create widgets.
	m_wndmain = sfg::Window::Create();
	m_wndmain->SetName( "wndmain" );
	m_wndmain->SetTitle( L"Example application" );
	m_wndmain->SetBorderWidth( 10.f );

	sfg::Button::Ptr btnaddbuttonh( sfg::Button::Create( L"Add button horizontally" ) );
	sfg::Button::Ptr btnaddbuttonv( sfg::Button::Create( L"Add button vertically" ) );
	sfg::Button::Ptr btntoggletitlebar( sfg::Button::Create( L"Toggle titlebar" ) );
	sfg::Button::Ptr btnhidewindow( sfg::Button::Create( L"Close window" ) );

	m_scale = sfg::Scale::Create( 0.f, 100.f, 1.f, sfg::Scale::Horizontal );
	m_scrollbar = sfg::Scrollbar::Create( m_scale->GetAdjustment(), sfg::Scrollbar::Horizontal );
	m_scrollbar->GetAdjustment()->SetMajorStep( 10.f );
	m_scrollbar->GetAdjustment()->SetPageSize( 20.f );
	m_scrollbar->GetAdjustment()->OnChange.Connect( &SampleApp::OnRangeValueChange, this );

	m_scale->SetLength( 80.f );
	m_scale->SetWidth( 20.f );
	m_scrollbar->SetLength( 80.f );
	m_scrollbar->SetWidth( 20.f );

	m_entry = sfg::Entry::Create( L"Type something!" );
	m_entry->SetRequisition( sf::Vector2f( 100.f, 0.f ) );

	sfg::Label::Ptr test_label( sfg::Label::Create( L"Foobar?" ) );
	sfg::Label::Ptr another_label( sfg::Label::Create( L"Meow?" ) );

	m_range_value = sfg::Label::Create( L"n/a" );
	m_range_value->SetRequisition( sf::Vector2f( 40.f, 0.f ) );

	// Layout.
	sfg::Box::Ptr boxtoolbar( sfg::Box::Create( sfg::Box::Horizontal ) );
	boxtoolbar->SetName( "boxtoolbar" );
	boxtoolbar->SetSpacing( 5.f );
	boxtoolbar->Pack( btnaddbuttonh, false );
	boxtoolbar->Pack( btnaddbuttonv, false );
	boxtoolbar->Pack( btntoggletitlebar, false );
	boxtoolbar->Pack( btnhidewindow, false );
	boxtoolbar->Pack( m_entry, true );
	boxtoolbar->Pack( m_scale, true );
	boxtoolbar->Pack( m_scrollbar, true );
	boxtoolbar->Pack( m_range_value, false );

	m_boxbuttonsh = sfg::Box::Create( sfg::Box::Horizontal );
	m_boxbuttonsh->SetSpacing( 5.f );

	m_boxbuttonsv = sfg::Box::Create( sfg::Box::Vertical );
	m_boxbuttonsv->SetSpacing( 5.f );

	m_table = sfg::Table::Create();
	m_table->Attach( sfg::Label::Create( L"Username:" ), sf::Rect<sf::Uint32>( 0, 0, 1, 1 ), sfg::Table::FILL, sfg::Table::FILL );
	m_table->Attach( sfg::Entry::Create(), sf::Rect<sf::Uint32>( 1, 0, 1, 1 ), sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL );
	m_table->Attach( sfg::Label::Create( L"Password:" ), sf::Rect<sf::Uint32>( 0, 1, 1, 1 ), sfg::Table::FILL, sfg::Table::FILL );
	m_table->Attach( sfg::Entry::Create(), sf::Rect<sf::Uint32>( 1, 1, 1, 1 ), sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::FILL );
	m_table->Attach( sfg::Button::Create( L"Login" ), sf::Rect<sf::Uint32>( 1, 2, 1, 1 ), 0, 0 );
	m_table->SetRowSpacings( 5.f );
	m_table->SetColumnSpacings( 5.f );

	sfg::Box::Ptr  boxmain( sfg::Box::Create( sfg::Box::Vertical ) );
	boxmain->SetSpacing( 5.f );
	boxmain->Pack( boxtoolbar, false );
	boxmain->Pack( m_boxbuttonsh, false );
	boxmain->Pack( m_boxbuttonsv, false );
	boxmain->Pack( m_table );

	m_wndmain->Add( boxmain );

	// Signals.
	btnaddbuttonh->OnClick.Connect( &SampleApp::OnAddButtonHClick, this );
	btnaddbuttonv->OnClick.Connect( &SampleApp::OnAddButtonVClick, this );
	btntoggletitlebar->OnClick.Connect( &SampleApp::OnToggleTitlebarClick, this );
	btnhidewindow->OnClick.Connect( &SampleApp::OnHideWindowClicked, this );

	// Load theme.
	//sfg::ThemeLoader::LoadFromFile<sfg::loaders::YAML>( "data/default.yaml", sfg::Context::Get().GetEngine() );
	// TODO: Reinvalidate widgets when loading theme.

	while( window.IsOpened() ) {
		while( window.PollEvent( event ) ) {
			if( m_wndmain->HandleEvent( event ) == sfg::Widget::EatEvent ) {
				continue;
			}

			if( event.Type == sf::Event::Closed ) {
				window.Close();
			}
		}

		window.Clear( sf::Color( 80, 80, 80 ) );
		m_wndmain->Expose( window );
		window.Display();
	}
}

void SampleApp::OnAddButtonHClick( sfg::Widget::Ptr /*widget*/ ) {
	sfg::Button::Ptr  button( sfg::Button::Create( L"New ->" ) );

	boost::shared_dynamic_cast<sfg::Label>( button->GetChild() )->SetAlignment( sf::Vector2f( 1.f, .5f ) );
	button->OnClick.Connect( &SampleApp::OnNewButtonClick, this );

	m_boxbuttonsh->Pack( button, true );
}

void SampleApp::OnAddButtonVClick( sfg::Widget::Ptr /*widget*/ ) {
	sfg::Button::Ptr  button( sfg::Button::Create( L"<- New" ) );

	boost::shared_dynamic_cast<sfg::Label>( button->GetChild() )->SetAlignment( sf::Vector2f( 0.f, .5f ) );
	button->OnClick.Connect( &SampleApp::OnNewButtonClick, this );

	m_boxbuttonsv->Pack( button, false );
}

void SampleApp::OnNewButtonClick( sfg::Widget::Ptr widget ) {
	sfg::Button::Ptr  button( boost::shared_dynamic_cast<sfg::Button>( widget ) );

	button->SetLabel( "Ouch" );
}

void SampleApp::OnToggleTitlebarClick( sfg::Widget::Ptr /*widget*/ ) {
	m_wndmain->SetStyle( m_wndmain->GetStyle() ^ sfg::Window::Titlebar );
}

void SampleApp::OnHideWindowClicked( sfg::Widget::Ptr /*widget*/ ) {
	m_wndmain->Show( !m_wndmain->IsVisible() );
}

void SampleApp::OnRangeValueChange( sfg::Adjustment::Ptr adjustment ) {
	std::stringstream ss;
	ss << adjustment->GetValue();
	m_range_value->SetText( ss.str() );
}

int main() {
	SampleApp  app;
	app.Run();
	return 0;
}
