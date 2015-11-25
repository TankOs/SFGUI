#include <SFGUI/ListBox.hpp>
#include <SFGUI/Context.hpp>
#include <SFGUI/Engine.hpp>
#include <SFGUI/RenderQueue.hpp>
#include <SFGUI/Widgets.hpp>
#include <SFML/Graphics.hpp>

float h = 0;
float padding;
int odds_last_sel = -1;
std::vector<int> sel;

namespace sfg {

Signal::SignalID ListBox::OnSelect = 0;

ListBox::Ptr ListBox::Create(  ) {
	auto ptr = Ptr( new ListBox );
	return ptr;
}

std::unique_ptr<sfg::RenderQueue> ListBox::InvalidateImpl() const {

    auto background_color = sfg::Context::Get().GetEngine().GetProperty<sf::Color>( "BackgroundColor", shared_from_this() );
    const auto& font_name = sfg::Context::Get().GetEngine().GetProperty<std::string>( "FontName", shared_from_this() );
    auto font_size = sfg::Context::Get().GetEngine().GetProperty<unsigned int>( "FontSize", shared_from_this() );
    padding = Context::Get().GetEngine().GetProperty<float>( "Padding", shared_from_this() );

    auto inverted_color = sf::Color::White - background_color;
    inverted_color.a = 255;

    sf::Color med_color(background_color.r/2+inverted_color.r/2, background_color.g/2+inverted_color.g/2, background_color.b/2+inverted_color.b/2, 255);

    const auto& font = sfg::Context::Get().GetEngine().GetResourceManager().GetFont( font_name );

    std::unique_ptr<sfg::RenderQueue> queue( new sfg::RenderQueue );

    queue->Add(
            sfg::Renderer::Get().CreatePane(
                sf::Vector2f( 0.f, 0.f ),
                sf::Vector2f( GetAllocation().width, GetAllocation().height ),
                1.f,
                background_color,
                sf::Color(0, 0, 0, 128),
                5.f
            )
        );

    h = sfg::Context::Get().GetEngine().GetFontLineHeight( *font, font_size );

    sel.clear();
    for(unsigned i=0; i<m_entries.size(); i++)
    {
        bool hover = false, selected = false;
        sf::Text text( m_entries[i], *font, font_size );
        text.setColor( sf::Color::White );

        float y = (h+padding*2-1)*i;

        if(widget_hover && hovered_element==i)
            hover = true;

        if((i>=selection_begin && i<=selection_end) || (i>=selection_end && i<=selection_begin)) selected = true;

        if(selection_odds[i]) selected = !selected;

        if(selected) sel.push_back(i);

        text.setPosition(
            padding,
            y + padding
        );

        queue->Add(
            sfg::Renderer::Get().CreatePane(
                sf::Vector2f( 0.f, y ),
                sf::Vector2f( GetAllocation().width, h+padding*2 ),
                1.f,
                (selected?sf::Color::Blue:(hover?med_color:background_color)),
                sf::Color(0, 0, 0, 128),
                5.f
            )
        );

        queue->Add( sfg::Renderer::Get().CreateText( text ) );
    }

    return queue;
}

sf::Vector2f ListBox::CalculateRequisition() {
	padding = Context::Get().GetEngine().GetProperty<float>( "Padding", shared_from_this() );
	float spacing( Context::Get().GetEngine().GetProperty<float>( "Spacing", shared_from_this() ) );
	const std::string& font_name( Context::Get().GetEngine().GetProperty<std::string>( "FontName", shared_from_this() ) );
	unsigned int font_size( Context::Get().GetEngine().GetProperty<unsigned int>( "FontSize", shared_from_this() ) );
	const sf::Font& font( *Context::Get().GetEngine().GetResourceManager().GetFont( font_name ) );

    h = sfg::Context::Get().GetEngine().GetFontLineHeight( font, font_size );
	sf::Vector2f ret(100, 100);

	for(unsigned i=0; i<m_entries.size(); i++)
    {
        auto requisition = Context::Get().GetEngine().GetTextStringMetrics( m_entries[i], font, font_size );
        requisition.x += 2 * padding;

        if(requisition.x > ret.x) ret.x = requisition.x;
    }

    float lines_h = (h+2 * padding-1) * m_entries.size();
    if(lines_h>ret.y) ret.y = lines_h;

	return ret;
}

const std::string& ListBox::GetName() const {
	static const std::string name( "ListBox" );
	return name;
}

unsigned ListBox::GetSelectedItemsCount()
{
    return sel.size();
}

unsigned ListBox::GetSelectedItemIndex(unsigned i)
{
    if(i>sel.size()) return (unsigned)(-1);
    return sel[i];
}

const std::string& ListBox::GetSelectedItemText(unsigned i)
{
    if(i>sel.size()) return 0;
    return m_entries[sel[i]];
}

void ListBox::AppendItem(std::string str)
{
    m_entries.push_back(str);
    selection_odds.push_back(false);
    if(resize_automatically)
    {
        RequestResize();
        Invalidate();
    }
}

void ListBox::PrependItem(std::string str)
{
    m_entries.insert(m_entries.begin(),str);
    selection_odds.insert(selection_odds.begin(), false);
}

void ListBox::RemoveItem(unsigned index)
{
    m_entries.erase(m_entries.begin()+index);
    selection_odds.erase(selection_odds.begin()+index);
}

void ListBox::Clear()
{
    m_entries.clear();
    selection_odds.clear();
}

void ListBox::HandleMouseEnter( int /*x*/, int /*y*/ ) {
    widget_hover = true;
}

void ListBox::HandleMouseLeave( int /*x*/, int /*y*/ ) {
    widget_hover = false;
    pressed_on_widget = false;
    hovered_element = -1;
    Invalidate();
}

void ListBox::HandleMouseMoveEvent( int x, int y ) {
    if( ( x == std::numeric_limits<int>::min() ) || ( y == std::numeric_limits<int>::min() ) ) {
            pressed_on_widget = false; return;
        }

    if(widget_hover)
    {
        int hov_el_cpy = hovered_element;
        hovered_element = (h?((y-padding+1)/(h+padding*2)-1):0);
        if(pressed_on_widget && hov_el_cpy!=hovered_element)
        {
            if(multiselect){ selection_end=hovered_element; }
            else { selection_begin=selection_end=hovered_element; }
        }
        Invalidate();

    }
}

void ListBox::HandleMouseButtonEvent( sf::Mouse::Button button, bool press, int /*x*/, int /*y*/ ) {
    if(press && button==sf::Mouse::Left && hovered_element != -1)
    {
        pressed_on_widget = true;
        bool shift = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && !sf::Keyboard::isKeyPressed(sf::Keyboard::LControl);
        bool control = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && !sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
        if(multiselect && shift && selection_begin!=-1)
        {
            if(odds_last_sel==-1)
            {
                selection_end = hovered_element;
                int beg = std::min(selection_begin, selection_end);
                int end = std::max(selection_begin, selection_end);
                for (std::vector<bool>::iterator it = selection_odds.begin()+beg; it != selection_odds.begin()+end; ++it)
                    *it = false;
            }
            else
            {
                selection_begin = odds_last_sel;
                odds_last_sel = -1;
                selection_end = hovered_element;
                for (std::vector<bool>::iterator it = selection_odds.begin(); it != selection_odds.end(); ++it)
                    *it = false;
            }

        }
        else if(multiselect && control && selection_begin!=-1)
        {
            selection_odds[hovered_element] = !selection_odds[hovered_element];
            odds_last_sel = hovered_element;
        }
        else
        {
            selection_begin = selection_end = hovered_element;
            for (std::vector<bool>::iterator it = selection_odds.begin(); it != selection_odds.end(); ++it)
                *it = false;
        }
        Invalidate();
    }
    if(pressed_on_widget && !press && button==sf::Mouse::Left && hovered_element != -1)
    {
        pressed_on_widget = false;
        GetSignals().Emit( OnSelect );
    }
}


}
