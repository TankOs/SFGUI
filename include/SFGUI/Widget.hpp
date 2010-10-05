#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/Signals.hpp>
#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/signals2.hpp>
#include <boost/bind.hpp>
#include <string>

namespace sfg {

class SFGUI_API Widget : public boost::noncopyable, boost::enable_shared_from_this<Widget> {
	public:
		enum State {
			Normal = 0,
			Active,
			Prelight,
			Selected,
			Insensitive
		};

		typedef boost::signals2::signal<void( Widget& )>  EnterSignal;
		typedef boost::signals2::signal<void( Widget& )>  LeaveSignal;

		/*struct EnterSignal_Wrap {
			void Connect( const EnterSignal& slot ) {
				m_signal.connect( slot );
			}

			private:
				EnterSignal m_signal;
		} OnEnter;

		virtual ~Widget();*/

		bool IsSensitive() const;
		bool IsVisible() const;
		bool HasFocus() const;

		void SetName( const std::string& name );
		const std::string& GetName() const;

		template <>
		struct EnterSignal_Wrap {
			void Connect( const EnterSignal& slot ) {
				m_signal.connect( slot );
			}

			private:
				EnterSignal m_signal;
		};
		
		//__REIGSTER_SIGNAL( EnterSignal_Wrap, EnterSignal, OnEnter );
		//__REGISTER_SIGNAL( EnterSignal, m_onenter );
		//__REGISTER_SIGNAL( LeaveSignal, m_onleave );

	private:
		Widget();

		EnterSignal  m_onenter;
		LeaveSignal  m_onleave;

		bool  m_sensitive;
		bool  m_visible;
		bool  m_focus;

		State  m_state;

		std::string  m_name;
};

}
