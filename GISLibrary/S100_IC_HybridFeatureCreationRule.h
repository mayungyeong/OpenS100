#pragma once
#include "XML_Collection.h"

namespace S100 
{
	class S100_IC_HybridFeatureCreationRule
	{
	public:
		std::shared_ptr<int> InteroperabilityLevel;
		std::string  RuleIdentifier;

		inline void GetContents(pugi::xml_node& node) 
		{
			for (pugi::xml_node instruction = node.first_child(); instruction; instruction = instruction.next_sibling())
			{
				const pugi::char_t* instructionName = instruction.name();

				if (!strcmp(instructionName, "interoperabilityLevel"))
				{
					InteroperabilityLevel = std::make_shared<int>(atoi(instruction.child_value()));
				}
				else if (!strcmp(instructionName, "ruleIdentifier"))
				{
					RuleIdentifier = instruction.child_value();
				}
				else
				{
					std::wstring content = pugi::as_wide(instructionName);
					content.append(L"is another data");
				}
			}
		}
	};
}
