#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/Widget.hpp>

#include <string>
#include <stdexcept>

namespace sfg {

/** Selector.
 * The selector is used for building paths to widgets using widget names, IDs,
 * classes and states.
 */
class SFGUI_API Selector {
	public:
		typedef std::shared_ptr<Selector> Ptr; ///< Shared pointer.
		typedef std::shared_ptr<Selector> PtrConst; ///< Shared pointer (const object).

		enum HierarchyType {
			Invalid = 0, //!< Invalid hierarchy type;
			Root = 1 << 0, //!< Current simple selector is the root of the selector.
			Child = 1 << 1, //!< Current simple selector is a child of the parent.
			Descendant = 1 << 2 //!< Current simple selector is a descendant of the parent.
		};

		/** Create selector.
		 * @param str String to parse.
		 * @return Selector or NULL in case of a parser error.
		 */
		static Ptr Create( const std::string& str );

		/** Create selector.
		 * @param widget Widget string.
		 * @param id ID string.
		 * @param class_ Class string.
		 * @param state State string.
		 * @param hierarchy Hierarchy type.
		 * @param parent Selector parent.
		 * @return Selector.
		 */
		static Ptr Create( const std::string& widget, const std::string& id, const std::string& class_, const std::string& state, int hierarchy, Ptr parent );

		/** Get widget name.
		 * @return Widget name or empty if all.
		 */
		const std::string& GetWidgetName() const;

		/** Get ID.
		 * @return ID or empty if all.
		 */
		const std::string& GetId() const;

		/** Get class.
		 * @return Class or empty if all.
		 */
		const std::string& GetClass() const;

		/** Get state.
		 * @return State or -1 if all.
		 */
		int GetState() const;

		/** Get parent selector.
		 * @return Parent or NULL if none set.
		 */
		PtrConst GetParent() const;

		/** Build full selector string.
		 * @return Selector string.
		 */
		std::string BuildString() const;

		/** Compare.
		 * Two selectors are equal if they both contain the exact path in the exact
		 * order with exact IDs, classes and states set. You can only check two
		 * selectors against each other that have been created through Create().
		 * Otherwise this method will always return false.
		 * @param other Other selector.
		 * @return true if equal.
		 */
		bool operator==( const Selector& other );

		/** Check if a selector matches to a widget.
		 * @param widget Widget.
		 * @return true if matches.
		 */
		bool Matches( Widget::PtrConst widget ) const;

	private:
		typedef std::runtime_error ParserException;

		Selector();
		Selector( const Selector& other );
		Selector& operator=( const Selector& other );

		static std::string ParseWidget( std::string::const_iterator& begin, const std::string::const_iterator& end );
		static std::string ParseId( std::string::const_iterator& begin, const std::string::const_iterator& end );
		static std::string ParseClass( std::string::const_iterator& begin, const std::string::const_iterator& end );
		static int ParseState( std::string::const_iterator& begin, const std::string::const_iterator& end );
		static void EatWhitespace( std::string::const_iterator& begin, const std::string::const_iterator& end );

		Ptr m_parent;

		int m_hierarchy_type;

		std::string m_widget;
		std::string m_id;
		std::string m_class;
		int m_state;

		std::size_t m_hash;
};

}
