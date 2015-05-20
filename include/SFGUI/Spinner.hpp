#pragma once

#include <SFGUI/Widget.hpp>

#include <memory>

namespace sfg {

/** Spinner.
 */
class SFGUI_API Spinner : public Widget {
	public:
		typedef std::shared_ptr<Spinner> Ptr; //!< Shared pointer.
		typedef std::shared_ptr<const Spinner> PtrConst; //!< Shared pointer.

		/** Create spinner.
		 * @return Spinner.
		 */
		static Ptr Create();

		const std::string& GetName() const override;

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

		/** Get current stage of the Spinner in the range of [0;steps-1].
		 * @return state of the Spinner in the range of [0;steps-1].
		 */
		unsigned int GetStage() const;

	protected:
		/** Ctor.
		 */
		Spinner();

		std::unique_ptr<RenderQueue> InvalidateImpl() const override;
		sf::Vector2f CalculateRequisition() override;

	private:
		void HandleUpdate( float seconds ) override;

		float m_elapsed_time;

		unsigned int m_stage;

		bool m_started;
};

}
