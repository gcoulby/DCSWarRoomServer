#include "pch.h"
#include "CppUnitTest.h"
#include "TestData.h"
#include "Types.h"
#include "MessageParser.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace UnitTesting
{
	TEST_CLASS(CoordinateTesting)
	{
	public:
		MessageParser parser;
		double timeFrame = 0;
		TEST_METHOD(ParsedCorrectly)
		{
			std::string line = "14,T=5.0357476|6.6795903|2000|458104.28|-26622.17";

			//DataMessages entities;
			//parser.parseMessage(line, entities, &timeFrame);
			parser.parseMessage(line);
			auto entities = parser.entities;

			//first check if the entity is there
			Assert::IsTrue(entities.size() == 1);

			//check if the entity has the correct number of properties
			Assert::IsTrue(entities["14"].size() == 5);

			//check if the properties are correct
			Assert::AreEqual("5.0357476", entities["14"].find("Longitude")->second.c_str());
		}

		TEST_METHOD(ParsedWithoutAltitude)
		{
			std::string line = "14,T=5.0357476|6.6795903||458104.28|-26622.17";

			/*DataMessages entities;
			parser.parseMessage(line, entities, &timeFrame);*/
			parser.parseMessage(line);
			auto entities = parser.entities;

			//first check if the entity is there
			Assert::IsTrue(entities.size() == 1);

			//check if the entity has the correct number of properties
			Assert::IsTrue(entities["14"].size() == 4);

			//check if the longitude correct
			Assert::AreEqual("5.0357476", entities["14"].find("Longitude")->second.c_str());

			//check if altitude exists
			Assert::IsTrue(entities["14"].find("Altitude") == entities["14"].end());
		}


		TEST_METHOD(CorrectCoordinateParsedFromStream)
		{
			std::string line = messages[2];

			/*DataMessages entities;
			parser.parseMessage(line, entities, &timeFrame);*/
			parser.parseMessage(line);
			auto entities = parser.entities;

			//first check if the correct number of entities have been discovered
			Assert::IsTrue(entities.size() == 10);

			//check if the entity has the correct number of properties
			Assert::IsTrue(entities["101"].size() == 14);

			//check if the longitude correct
			Assert::AreEqual("4.7790453", entities["101"].find("Longitude")->second.c_str());
		}


		TEST_METHOD(CoordinatesChangedAfterUpdate)
		{
			std::string line = messages[2];

			/*DataMessages entities;
			parser.parseMessage(line, entities, &timeFrame);*/
			parser.parseMessage(line);
			auto entities = parser.entities;

			//first check if the correct number of entities have been discovered
			Assert::IsTrue(entities.size() == 10);

			//check if the entity has the correct number of properties
			Assert::IsTrue(entities["101"].size() == 14);

			//check if the longitude correct
			Assert::AreEqual("4.7790453", entities["101"].find("Longitude")->second.c_str());

			//parse another line to check for changes
			line = messages[4];
			parser.parseMessage(line);
			entities = parser.entities;

			//first check if the new entities have been found
			Assert::IsTrue(entities.size() == 23);

			//check if the entity has the same number of properties
			Assert::IsTrue(entities["101"].size() == 16);

			//check if the longitude has been successfully updated
			Assert::AreEqual("4.7790568", entities["101"].find("Longitude")->second.c_str());
		}
	};
}
