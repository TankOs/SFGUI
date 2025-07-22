#include <SFGUI/XMLLoader.hpp>
#include <algorithm>
#include <string>
#include <unistr.h>
#include <uniconv.h>

// convert string to utf32 to support special characters
static std::u32string str_to_utf32(std::string str) {
    std::u32string result;

    size_t size;
    uint32_t* utf32 = u8_to_u32(reinterpret_cast<const uint8_t*>(str.data()), str.size(), nullptr, &size);
    if(!utf32) return U"";

    result.assign(utf32, utf32 + size);
    free(utf32);
    return result;
}

// pass string to lowercase
static std::string toLowercase(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return str;
}

// Register the RadioButton groups to perform the grouping.
static std::map<std::string, std::shared_ptr<sfg::RadioButtonGroup>> RadioGroupMap;

// Recursive function to each XMLElements and generate widgets
sfg::Widget::Ptr generateWidget(tinyxml2::XMLElement* element, sfg::Widget::Ptr widget){
    std::string elementName(element->Name());
    std::map<std::string, std::string> elementAttributes;

    // collect attributes in xml tag and save in elementAttributes with string
    for(auto attr = element->FirstAttribute(); attr != nullptr; attr = attr->Next()){
        elementAttributes[toLowercase(attr->Name())] = toLowercase(std::string(attr->Value()));
    }

    sfg::Widget::Ptr newWidget = nullptr;

    if(!elementName.compare("Box")){ // create Box widget
        newWidget = sfg::Box::Create();

        // set orientation to Box widget
        if(elementAttributes.find("orientation") != elementAttributes.end()){
            if(!elementAttributes["orientation"].compare("vertical")){
                sfg_cast(Box,newWidget)->SetOrientation(sfg::Box::Orientation::VERTICAL);
            }
        }

        // set spacing to Box widget
        if(elementAttributes.find("spacing") != elementAttributes.end()){
            float spacing = 0;
            sscanf(elementAttributes["spacing"].c_str(), "%f", &spacing);
            sfg_cast(Box,newWidget)->SetSpacing(spacing);
        }

        // Iterate XMLElements to create and add childs to Box
        for(auto child = element->FirstChildElement(); child != nullptr; child = child->NextSiblingElement()){
            generateWidget(child, newWidget);
        }
    }else if(!elementName.compare("Button")){ // create Button widget
        newWidget = sfg::Button::Create();
        if(element->GetText()){
            sfg_cast(Button, newWidget)->SetLabel(str_to_utf32(std::string(element->GetText())));
        }
    }else if(!elementName.compare("Canvas")){ // create Canvas widget
        newWidget = sfg::Canvas::Create();
    }else if(!elementName.compare("CheckButton")){ // create CheckButton
        newWidget = sfg::CheckButton::Create(L"");

        // add a label to CheckButton
        if(elementAttributes.find("label") != elementAttributes.end()){
            sfg_cast(CheckButton, newWidget)->SetLabel(str_to_utf32(std::string(elementAttributes["label"])));
        }

        // Mark CheckButton as checked
        if(elementAttributes.find("checked") != elementAttributes.end()){
            if(!elementAttributes["checked"].compare("1") || !elementAttributes["checked"].compare("true")){
                sfg_cast(CheckButton, newWidget)->SetActive(true);
            }
        }
    }else if(!elementName.compare("ComboBox")){ // create ComboBox
        newWidget = sfg::ComboBox::Create();

        // this iteraction is find chils with Item tag name to add items to ComboBox
        for(auto child = element->FirstChildElement(); child != nullptr; child = child->NextSiblingElement()){
            if(!std::string(child->Name()).compare("Item")){
                sfg_cast(ComboBox, newWidget)->AppendItem(str_to_utf32(std::string(child->GetText())));
            }
        }

        // select item in ComboBox
        if(elementAttributes.find("selected") != elementAttributes.end()){
            int selected = 0;
            sscanf(elementAttributes["selected"].c_str(), "%d", &selected);
            sfg_cast(ComboBox, newWidget)->SelectItem(selected);
        }
    }else if(!elementName.compare("Entry")){ // create Entry widget
        newWidget = sfg::Entry::Create();

        // add text in entry with value attribute in Entry tag
        if(elementAttributes.find("value") != elementAttributes.end()){
            sfg_cast(Entry, newWidget)->SetText(str_to_utf32(elementAttributes["value"]));
        }
    }else if(!elementName.compare("Fixed")){ // create Fixed widget
        newWidget = sfg::Fixed::Create();

        // add childs in Fixed
        for(auto child = element->FirstChildElement(); child != nullptr; child = child->NextSiblingElement()){
            generateWidget(child, newWidget);
        }
    }else if(!elementName.compare("Frame")){ // create Frame widget
        newWidget = sfg::Frame::Create();

        if(elementAttributes.find("label") != elementAttributes.end()){
            sfg_cast(Frame, newWidget)->SetLabel(str_to_utf32(elementAttributes["label"]));
        }

        // add child element to Frame
        auto child = element->FirstChildElement();
        if(child){
            generateWidget(child, newWidget);
        }
    }else if(!elementName.compare("Image")){ // create Image widget
        newWidget = sfg::Image::Create();

        // load image from path in src attribute
        if(elementAttributes.find("src") != elementAttributes.end()){
            sf::Image img;
            if(img.loadFromFile(elementAttributes["src"])){
                sfg_cast(Image, newWidget)->SetImage(img);
            }
        }
    }else if(!elementName.compare("Label")){ // create Label widget
        newWidget = sfg::Label::Create();
        if(element->GetText()){
            sfg_cast(Label, newWidget)->SetText(str_to_utf32(std::string(element->GetText())));
        }
    }else if(!elementName.compare("Notebook")){ // create Notebook widget
        newWidget = sfg::Notebook::Create();

        // iterate childs
        for(auto child = element->FirstChildElement(); child != nullptr; child = child->NextSiblingElement()){
            sfg::Label::Ptr pageLabel = sfg::Label::Create(); // label used in Notebook page

            std::map<std::string, std::string> childAttributes;
            // read child attributes to add page label
            for(auto attr = child->FirstAttribute(); attr != nullptr; attr = attr->Next()){
                childAttributes[toLowercase(attr->Name())] = toLowercase(std::string(attr->Value()));
            }

            if(childAttributes.find("label") != childAttributes.end()){
                pageLabel->SetText(str_to_utf32(std::string(childAttributes["label"])));
            }

            sfg::Widget::Ptr page = generateWidget(child, nullptr);
            if(page) sfg_cast(Notebook, newWidget)->AppendPage(page, pageLabel);
        }

        // set one page as selected
        if(elementAttributes.find("selected") != elementAttributes.end()){
            int selected = 0;
            sscanf(elementAttributes["selected"].c_str(), "%d", &selected);
            sfg_cast(Notebook, newWidget)->SetCurrentPage(selected);
        }
    }else if(!elementName.compare("ProgressBar")){ // create ProgressBar widget
        newWidget = sfg::ProgressBar::Create();

        // set vertical orientation to ProgressBar
        if(elementAttributes.find("orientation") != elementAttributes.end()){
            if(!elementAttributes["orientation"].compare("vertical")){
                sfg_cast(ProgressBar, newWidget)->SetOrientation(sfg::ProgressBar::Orientation::VERTICAL);
            }
        }

        float value = 0.0;
        if(elementAttributes.find("value") != elementAttributes.end()){
            sscanf(elementAttributes["value"].c_str(), "%f", &value);
        }
        sfg_cast(ProgressBar, newWidget)->SetFraction(value);
    }else if(!elementName.compare("RadioButton")){ // create RadioButton widget
        newWidget = sfg::RadioButton::Create(L"");

        if(elementAttributes.find("label") != elementAttributes.end()){
            sfg_cast(RadioButton, newWidget)->SetLabel(str_to_utf32(elementAttributes["label"]));
        }

        // generate and set groups to RadioButtons
        if(elementAttributes.find("group") != elementAttributes.end()){
            if(RadioGroupMap.find("group") != RadioGroupMap.end()){
                sfg_cast(RadioButton, newWidget)->SetGroup(RadioGroupMap[elementAttributes["group"]]);
            }else{
                RadioGroupMap[elementAttributes["group"]] = sfg_cast(RadioButton, newWidget)->GetGroup();
            }
        }
    }else if(!elementName.compare("Scale")){ // create Scale widget
        sfg::Scale::Orientation orientation = sfg::Scale::Orientation::HORIZONTAL;

        if(elementAttributes.find("orientation") != elementAttributes.end()){
            if(!elementAttributes["orientation"].compare("vertical")){
                orientation = sfg::Scale::Orientation::VERTICAL;
            }
        }

        float min = 0.0,
              max = 0.0,
              step = 2.0,
              value = 0.0;

        // get attributes min, max, step and value to set to Scale
        if(elementAttributes.find("min") != elementAttributes.end()){
            sscanf(elementAttributes["min"].c_str(), "%f", &min);
        }
        if(elementAttributes.find("max") != elementAttributes.end()){
            sscanf(elementAttributes["max"].c_str(), "%f", &max);
        }
        if(elementAttributes.find("step") != elementAttributes.end()){
            sscanf(elementAttributes["step"].c_str(), "%f", &step);
        }
        if(elementAttributes.find("value") != elementAttributes.end()){
            sscanf(elementAttributes["value"].c_str(), "%f", &value);
        }

        newWidget = sfg::Scale::Create(orientation);
        sfg_cast(Scale, newWidget)->SetRange(min, max);
        sfg_cast(Scale, newWidget)->SetIncrements(step, step);
        sfg_cast(Scale, newWidget)->SetValue(value);
    }else if(!elementName.compare("Scrollbar")){ // create Scrollbar widget
        sfg::Scrollbar::Orientation orientation = sfg::Scrollbar::Orientation::HORIZONTAL;

        if(elementAttributes.find("orientation") != elementAttributes.end()){
            if(!elementAttributes["orientation"].compare("vertical")){
                orientation = sfg::Scrollbar::Orientation::VERTICAL;
            }
        }

        newWidget = sfg::Scrollbar::Create(orientation);
    }else if(!elementName.compare("ScrolledWindow")){ // create ScrolledWindow
        newWidget = sfg::ScrolledWindow::Create();

        sfg_cast(ScrolledWindow, newWidget)->SetScrollbarPolicy(sfg::ScrolledWindow::HORIZONTAL_ALWAYS | sfg::ScrolledWindow::VERTICAL_AUTOMATIC);

        auto child = element->FirstChildElement();
        if(child){
            generateWidget(child, newWidget);
        }
    }else if(!elementName.compare("Separator")){ // create Separator widget
        sfg::Separator::Orientation orientation = sfg::Separator::Orientation::HORIZONTAL;

        if(elementAttributes.find("orientation") != elementAttributes.end()){
            if(!elementAttributes["orientation"].compare("vertical")){
                orientation = sfg::Separator::Orientation::VERTICAL;
            }
        }

        newWidget = sfg::Separator::Create(orientation);
    }else if(!elementName.compare("Spinner")){ // create Spinner widget
        newWidget = sfg::Spinner::Create();
    }else if(!elementName.compare("SpinButton")){ // create SpinButton widget
        newWidget = sfg::SpinButton::Create(0.0, 10.0, 1);

        float min = 0.0,
              max = 0.0,
              step = 2.0,
              value = 0.0;

        // get attributes min, max, step and value to set to SpinButton
        if(elementAttributes.find("min") != elementAttributes.end()){
            sscanf(elementAttributes["min"].c_str(), "%f", &min);
        }
        if(elementAttributes.find("max") != elementAttributes.end()){
            sscanf(elementAttributes["max"].c_str(), "%f", &max);
        }
        if(elementAttributes.find("step") != elementAttributes.end()){
            sscanf(elementAttributes["step"].c_str(), "%f", &step);
        }
        if(elementAttributes.find("value") != elementAttributes.end()){
            sscanf(elementAttributes["value"].c_str(), "%f", &value);
        }

        sfg_cast(SpinButton, newWidget)->SetRange(min, max);
        sfg_cast(SpinButton, newWidget)->SetStep(step);
        sfg_cast(SpinButton, newWidget)->SetValue(value);
    }else if(!elementName.compare("Table")){ // create Table
        newWidget = sfg::Table::Create();

        unsigned int col=0, row=0;

        // Iteract rows and cols to define widget position in table
        for(auto rowElement = element->FirstChildElement(); rowElement != nullptr; rowElement = rowElement->NextSiblingElement()){
            if(!std::string(rowElement->Name()).compare("Row")){
                col = 0;
                for(auto colElement = rowElement->FirstChildElement(); colElement != nullptr; colElement = colElement->NextSiblingElement()){
                    if(!std::string(colElement->Name()).compare("Col")){

                        // get child in element in Col tag
                        auto child = colElement->FirstChildElement();
                        if(child){
                            sf::Vector2u crSpan(1, 1);
                            std::map<std::string, std::string> childAttributes;

                            // read and save child attributes
                            for(auto attr = child->FirstAttribute(); attr != nullptr; attr = attr->Next()){
                                childAttributes[toLowercase(attr->Name())] = toLowercase(std::string(attr->Value()));
                            }

                            // set colspan and rowspan to widget
                            if(childAttributes.find("cols") != childAttributes.end()){
                                sscanf(childAttributes["cols"].c_str(), "%u", &crSpan.x);
                            }
                            if(childAttributes.find("rows") != childAttributes.end()){
                                sscanf(childAttributes["rows"].c_str(), "%u", &crSpan.y);
                            }

                            auto childWidget = generateWidget(child, nullptr);
                            if(childWidget){
                                sfg_cast(Table, newWidget)->Attach(childWidget, sf::Rect<std::uint32_t>( { col, row }, {crSpan.x, crSpan.y}));
                            }
                        }
                        col++;
                    }
                }
                row++;
            }
        }
    }else if(!elementName.compare("ToggleButton")){ // create ToggleButton widget
        newWidget = sfg::ToggleButton::Create();

        if(element->GetText()){
            sfg_cast(Button, newWidget)->SetLabel(str_to_utf32(std::string(element->GetText())));
        }
    }else if(!elementName.compare("Window")){ // create Window widget
        newWidget = sfg::Window::Create();
        char style = sfg::Window::Style::TOPLEVEL;

        // read and set window style
        if(elementAttributes.find("style") != elementAttributes.end()){
            std::string styles(elementAttributes["style"]);
            if(styles.find("toplevel") == std::string::npos){
                style = 0;

                if(styles.find("no_style") != std::string::npos){ style += sfg::Window::Style::NO_STYLE; }
                if(styles.find("titlebar") != std::string::npos){ style += sfg::Window::Style::TITLEBAR; }
                if(styles.find("background") != std::string::npos){ style += sfg::Window::Style::BACKGROUND; }
                if(styles.find("resize") != std::string::npos){ style += sfg::Window::Style::RESIZE; }
                if(styles.find("shadow") != std::string::npos){ style += sfg::Window::Style::SHADOW; }
                if(styles.find("close") != std::string::npos){ style += sfg::Window::Style::CLOSE; }

                sfg_cast(Window, newWidget)->SetStyle(style);
            }
        }

        if(elementAttributes.find("title") != elementAttributes.end()){
            sfg_cast(Window, newWidget)->SetTitle(str_to_utf32(elementAttributes["title"]));

        }

        auto child = element->FirstChildElement();
        if(child){
            generateWidget(child, newWidget);
        }
    }else{
        std::cerr << "Error to render \"" << element->Name() << "\"" << " component not exists!" << std::endl;
        return nullptr;
    }

    if(!widget){
        widget = newWidget; // pass newWidget to widget as root widget if widget is null
    }else{
        std::string widgetName(widget->GetName());
        // verify name of widget to know how to add childs
        if(!widgetName.compare("Window")){
            std::dynamic_pointer_cast<sfg::Window>(widget)->Add(newWidget);
        }else if(!widgetName.compare("Box")){
            std::dynamic_pointer_cast<sfg::Box>(widget)->Pack(newWidget);
        }else if(!widgetName.compare("Frame")){
            std::dynamic_pointer_cast<sfg::Frame>(widget)->Add(newWidget);
        }else if(!widgetName.compare("ScrolledWindow")){
            std::dynamic_pointer_cast<sfg::ScrolledWindow>(widget)->AddWithViewport(newWidget);
        }else if(!widgetName.compare("Fixed")){
            std::dynamic_pointer_cast<sfg::Fixed>(widget)->Put(newWidget, sf::Vector2f(0.0, 0.0));
        }
    }

    // set basic attributes to newWidget, id, class, pos and size
    if(elementAttributes.find("id") != elementAttributes.end()){
        newWidget->SetId(elementAttributes["id"]);
    }
    if(elementAttributes.find("class") != elementAttributes.end()){
        newWidget->SetClass(elementAttributes["class"]);
    }
    if(elementAttributes.find("pos") != elementAttributes.end()){
        sf::Vector2f pos;
        sscanf(elementAttributes["pos"].c_str(), "%f %f", &pos.x, &pos.y);
        newWidget->SetPosition(pos);
    }
    if(elementAttributes.find("size") != elementAttributes.end()){
        sf::Vector2f size;
        sscanf(elementAttributes["size"].c_str(), "%f %f", &size.x, &size.y);
        newWidget->SetRequisition(size);
    }

    return widget;
};

namespace sfg{
    XMLWidget::XMLWidget(bool processEntities, Whitespace whitespaceMode) : tinyxml2::XMLDocument(processEntities, static_cast<tinyxml2::Whitespace>(whitespaceMode)){
        this->m_widget = nullptr;
        this->m_reload = false;
    }
    XMLWidget::XMLWidget(std::string fileName, bool processEntities, Whitespace whitespaceMode) : tinyxml2::XMLDocument(processEntities, static_cast<tinyxml2::Whitespace>(whitespaceMode)){
        this->m_widget = nullptr;
        this->m_reload = false;
        this->loadFromFile(fileName);
    }

    bool XMLWidget::loadFromFile(std::string fileName){ // load xmlfile using tinyxml2
        tinyxml2::XMLError result = LoadFile(fileName.c_str());
        if (result != tinyxml2::XML_SUCCESS) {
            std::cerr << "Failed to load XML file: " << fileName << " (Error: " << result << ")" << std::endl;
            return false;
        }
        this->m_reload = true;
        return true;
    }

    sfg::Widget::Ptr XMLWidget::getWidget(){
        if(this->m_reload) this->parser();

        return this->m_widget;
    }

    XMLWidget::operator sfg::Widget::Ptr(){
        return this->getWidget();
    }

    bool XMLWidget::parser(){ // make parse from xml to widget
        tinyxml2::XMLElement* element = this->RootElement();

        this->m_widget = generateWidget(element, nullptr);

        this->m_reload = false;
        return true;
    }
}