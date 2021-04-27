#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <utility>
#include <SFML/System/Vector2.hpp>

namespace SeekASoul
{
	namespace Engine
	{
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

						size_t pos = 0;

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

			static const std::map<std::string, std::vector<std::string>> ReadConfigFile(const std::string path)
			{
				std::map<std::string, std::vector<std::string>> configKeymap;

				std::ifstream configFile(path);
				std::string line;

				if (configFile.is_open())
				{
					while (std::getline(configFile, line))
					{
						size_t pos = 0;

						if ((pos = line.find(":")) != std::string::npos)
						{
							std::string key = line.substr(0, pos);
					
							// Safe duplicate check 
							if (configKeymap.find(key) != configKeymap.end()) 
							{
								LOG_WARNING("The key " << key << " already exists, please verify the following config file : \n" << path);
								continue;
							}
					
							line = line.substr(pos + 1);

							std::vector<std::string> values;
							while ((pos = line.find(" ")) != std::string::npos)
							{
								values.push_back(line.substr(0, pos));
								line.erase(0, pos + 1);
							}

							configKeymap[key] = values;
						}
					}
				}

				return configKeymap;
			}
		}
	}
}