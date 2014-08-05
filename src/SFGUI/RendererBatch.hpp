#pragma once

#include <SFGUI/Config.hpp>

namespace sfg {
namespace priv {

struct RendererBatch {
	std::shared_ptr<RendererViewport> viewport;
	std::shared_ptr<Signal> custom_draw_callback;
	int atlas_page;
	int start_index;
	int index_count;
	int min_index;
	int max_index;
	bool custom_draw;
};

}
}
