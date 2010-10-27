#pragma once

namespace sfg {

struct SlotResult {
	template <typename InputIterator>
	bool operator()( InputIterator first, InputIterator last ) const;
};

}

#include "SlotResult.inl"
