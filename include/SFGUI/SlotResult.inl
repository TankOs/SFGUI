namespace sfg {

template <typename InputIterator>
bool SlotResult::operator()( InputIterator first, InputIterator last ) const {
	for( ; first != last; ++first ) {
		if( *first == true ) {
			return true;
		}
	}

	return false;
}

}
