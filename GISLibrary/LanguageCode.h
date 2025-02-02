#pragma once
#include <string>

namespace S100
{
	class LanguageCode : public std::string
	{
	public:
		using std::string::string;
		using std::string::operator=; 

		inline void GetContents(pugi::xml_node& node)
		{
			for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
			{
				const pugi::char_t* instructionName = instruction.name();

				if (!strcmp(instructionName, "lan:LanguageCode"))
				{
					*this = std::string(instruction.child_value());
				}
			}
		}
	};
}