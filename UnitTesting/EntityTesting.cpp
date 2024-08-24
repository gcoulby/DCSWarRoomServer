#include "pch.h"
#include "CppUnitTest.h"
#include "TestData.h"
#include "Types.h"
#include "MessageParser.h"
#include <regex>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(EntityTesting)
	{
	public:
		MessageParser parser;
		double timeFrame = 0;

		std::string normalizeString(const std::string& input) {
			std::string output = std::regex_replace(input, std::regex("\\s{2,}"), " "); // Replace multiple spaces with one
			output = std::regex_replace(output, std::regex("\\s+\\."), "."); // Remove spaces before periods
			return output;
		}

		TEST_METHOD(GlobalPropertiesCorrectly)
		{
			std::string line = messages[1];

			/*DataMessages entities;
			parser.parseMessage(line, entities, &timeFrame);*/
			parser.parseMessage(line);
			auto entities = parser.entities;

			//first check if the entity is there
			Assert::IsTrue(entities["0"].size() == 11);

			//check if the properties are correct
			Assert::AreEqual("text/acmi/tacview", entities["0"].find("FileType")->second.c_str());
		}


		TEST_METHOD(MultiLineStringsParsedCorrectly)
		{
			std::string line = messages[1];

			/*DataMessages entities;
			parser.parseMessage(line, entities, &timeFrame);*/
			parser.parseMessage(line);
			auto entities = parser.entities;

			//first check if the entity is there
			Assert::IsTrue(entities["0"].size() == 11);

			//check if the properties are correct
			std::string raw = "The Black Sea\\\n24 June 1992\\, Morning\\\nFictional\\\n\\\nThe collapse of the Soviet Union in 1991 led to a renewed conflict in the Caucasus region.  The initial attempts by Russian forces to seize Georgian territory were driven back\\, but that has only strengthened Moscow’s resolve.  The Russians continue to build a sizable ground force near Maykop.  A fully equipped Motor Rifle Brigade has been identified with additional forces continuing to flow in.";
			//std::string raw = "The Black Sea\n24 June 1992, Morning\nFictional\n\nThe collapse of the Soviet Union in 1991 led to a renewed conflict in the Caucasus region.The initial attempts by Russian forces to seize Georgian territory were driven back, but that has only strengthened Moscow’s resolve.The Russians continue to build a sizable ground force near Maykop.A fully equipped Motor Rifle Brigade has been identified with additional forces continuing to flow in.";
			std::string expected = normalizeString(raw);
			std::string actual = normalizeString(entities["0"].find("Comments")->second);

			//check if the properties are correct
			Assert::AreEqual(actual.c_str(), expected.c_str());
		}


		TEST_METHOD(CorrectNumberOfEntities)
		{
			std::string line = messages[1];

			/*DataMessages entities;
			parser.parseMessage(line, entities, &timeFrame);*/
			parser.parseMessage(line);
			auto entities = parser.entities;

			//first check if there are 4 entities
			Assert::IsTrue(entities.size() == 4);

			//check if entity 40000001 has 8 properties
			Assert::IsTrue(entities["40000001"].size() == 8);

			//check if the properties are correct
			Assert::AreEqual("Navaid+Static+Bullseye", entities["40000001"].find("Type")->second.c_str());
		}


		TEST_METHOD(ListOutEntityIds)
		{
			std::string line = messages[2];

			/*DataMessages entities;
			parser.parseMessage(line, entities, &timeFrame);*/
			parser.parseMessage(line);
			auto entities = parser.entities;
			//parse another line to check for changes
			line = messages[4];
			/*parser.parseMessage(line, entities, &timeFrame);*/
			parser.parseMessage(line);
			entities = parser.entities;

			//first check if the new entities have been found
			Assert::IsTrue(entities.size() == 23);

			auto ids = parser.getIds();

			//check if the entity has the same number of properties
			Assert::IsTrue(ids.size() == 23);

		}
	};
}
