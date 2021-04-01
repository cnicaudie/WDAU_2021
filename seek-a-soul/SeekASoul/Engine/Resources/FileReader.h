#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <utility>
#include <SFML/System/Vector2.hpp>

namespace FileReader 
{
	static const std::pair<std::vector<int>, sf::Vector2u> ReadLevelFromFile(const std::string path)
	{
		std::vector<int> level;
		std::ifstream levelFile(path);
		std::string line;

		sf::Vector2u levelSize;
		
		if (levelFile.is_open())
		{
			while (std::getline(levelFile, line))
			{
				++levelSize.y;

				size_t pos = 0; // value separator

				while ((pos = line.find(" ")) != std::string::npos)
				{
					if (levelSize.y == 1) 
					{ 
						++levelSize.x; 
					}

					int tileNumber = std::stoi(line.substr(0, pos));
					level.push_back(tileNumber);
					line.erase(0, pos + 1);
				}
			}
		}

		return std::make_pair(level, levelSize);
	}

	static const std::map<std::string, std::string> ReadConfigFile(const std::string path) 
	{
		std::map<std::string, std::string> configKeymap;

		std::ifstream configFile(path);
		std::string line;

		if (configFile.is_open())
		{
			while (std::getline(configFile, line))
			{
				size_t pos = 0; // value separator

				if ((pos = line.find(":")) != std::string::npos)
				{
					std::string key = line.substr(0, pos);
					std::string value = line.substr(pos + 1);
					configKeymap[key] = value;
				}
			}
		}

		return configKeymap;
	}
}