#pragma once

#include <string>

class TextureManager 
{
public:
	TextureManager();

	inline const sf::Texture& GetTextureFromName(const std::string& name) const
	{
		return m_Textures.at(name);
	}

	inline const sf::Vector2f GetTextureSizeFromName(const std::string& name) const 
	{
		const sf::Vector2f size(static_cast<float>(m_Textures.at(name).getSize().x), static_cast<float>(m_Textures.at(name).getSize().y));
		return size;
	}

private:
	std::map<std::string, sf::Texture> m_Textures;
};