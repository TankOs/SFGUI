#pragma once

#include <SFGUI/Widgets.hpp>
#include <tinyxml2.h>
#include <string>

#define sfg_cast(className, ptr) std::dynamic_pointer_cast<sfg::className>(ptr)

namespace sfg{

    class XMLWidget: protected ::tinyxml2::XMLDocument{
        public:

        enum class Whitespace {
            PRESERVE_WHITESPACE,
            COLLAPSE_WHITESPACE,
            PEDANTIC_WHITESPACE
        };

        XMLWidget(bool processEntities = true, Whitespace whitespaceMode = Whitespace::PRESERVE_WHITESPACE);
        XMLWidget(std::string fileName, bool processEntities = true, Whitespace whitespaceMode = Whitespace::PRESERVE_WHITESPACE);

        bool loadFromFile(std::string fileName);

        sfg::Widget::Ptr getWidget();
        operator sfg::Widget::Ptr();

        private:
        sfg::Widget::Ptr m_widget;
        bool m_reload;

        bool parser();
    };
}