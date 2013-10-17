namespace sfg {

template <class Loader>
std::shared_ptr<const ResourceLoader> ResourceManager::CreateLoader() {
	std::shared_ptr<Loader> loader( new Loader );

	auto loader_iter = m_loaders.find( loader->GetIdentifier() );
	if( loader_iter != m_loaders.end() ) {
		return loader_iter->second;
	}

	m_loaders[loader->GetIdentifier()] = loader;
	return loader;
}

}
