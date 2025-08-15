#pragma once

#include <tinyxml2.h>

#include <SFGUI/Widgets.hpp>
#include <string>

#define sfg_cast(className, ptr) std::dynamic_pointer_cast<sfg::className>(ptr)

namespace sfg {
    namespace ui {
        class SFGUI_API XMLLoader : protected ::tinyxml2::XMLDocument {
            public:
            typedef std::shared_ptr<XMLLoader> Ptr; //!< Shared pointer.
		    typedef std::shared_ptr<const XMLLoader> PtrConst; //!< Shared pointer.

            enum class Whitespace {
                PRESERVE_WHITESPACE,
                COLLAPSE_WHITESPACE,
                PEDANTIC_WHITESPACE
            };

            static std::map<std::string, sfg::Signal::SignalID*> SIGNAL;

    		static Ptr Create(bool processEntities = true, Whitespace whitespaceMode = Whitespace::PRESERVE_WHITESPACE);

            bool loadFromFile(std::string fileName);

            std::shared_ptr<sfg::Widget> getWidget();
            operator sfg::Widget::Ptr();


            protected:
            XMLLoader(bool processEntities, Whitespace whitespaceMode);

            private:
            sfg::Widget::Ptr m_widget;
            bool m_reload;

            bool parser();
        };
    }  // namespace ui
}  // namespace sfg