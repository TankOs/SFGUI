namespace sfg {

template <class Loader>
ResourceLoader& ResourceManager::CreateLoader() {
	Loader* loader( new Loader );

	LoaderMap::iterator loader_iter( m_loaders.find( loader->GetIdentifier() ) );
	if( loader_iter != m_loaders.end() ) {
		delete loader;
		return *loader_iter->second;
	}

	m_loaders[loader->GetIdentifier()] = loader;
	return *m_loaders[loader->GetIdentifier()];
}

}
