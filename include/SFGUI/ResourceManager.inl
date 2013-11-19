namespace sfg {

template <class Loader>
std::shared_ptr<const ResourceLoader> ResourceManager::CreateLoader() {
	auto loader = std::make_shared<Loader>();

	auto loader_iter = m_loaders.find( loader->GetIdentifier() );
	if( loader_iter != m_loaders.end() ) {
		return loader_iter->second;
	}

	m_loaders[loader->GetIdentifier()] = loader;
	return loader;
}

}
