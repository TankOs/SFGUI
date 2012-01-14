#pragma once
#include <SFGUI/Widget.hpp>
#include <SFGUI/SharedPtr.hpp>
#include <SFML/System.hpp>

namespace sfg {

/** Spinner.
 */
class SFGUI_API Spinner : public Widget {
	public:
		typedef SharedPtr<Spinner> Ptr; //!< Shared pointer.
		typedef SharedPtr<const Spinner> PtrConst; //!< Shared pointer.

		/** Dtor.
		 */
		~Spinner();

		/** Create spinner.
		 * @return Spinner.
		 */
		static Ptr Create();

		virtual const std::string& GetName() const;

		/** Start the spinner.
		 */
		void Start();

		/** Stop the spinner.
		 */
		void Stop();

		/** Check if spinner is started.
		 * @return true if spinner is started.
		 */
		bool Started() const;

	protected:
		/** Ctor.
		 */
		Spinner();

		RenderQueue* InvalidateImpl() const;
		sf::Vector2f CalculateRequisition();

	private:
		void HandleUpdate( float seconds );

		float m_elapsed_time;

		unsigned int m_stage;

		mutable RenderQueue* m_renderqueue;

		bool m_started;
};

}
