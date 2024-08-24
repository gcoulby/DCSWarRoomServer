#include "pch.h"
#include "CppUnitTest.h"
#include "TestData.h"
#include "Types.h"
#include "MessageParser.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace UnitTesting
{
	TEST_CLASS(TimeFrameTesting)
	{
	public:
		MessageParser parser;
		//double timeFrame = 0;
		TEST_METHOD(ParsedCorrectly)
		{
			std::string line = "#127.4";

			/*DataMessages entities;
			parser.parseMessage(line, entities, &timeFrame);*/
			parser.parseMessage(line);
			auto entities = parser.entities;
			auto timeFrame = parser.timeFrame;

			//first check if the entity is there
			Assert::IsTrue(entities.size() == 0);

			//check if the time frame is correct
			Assert::AreEqual(127.4, timeFrame);
		}

		TEST_METHOD(TimeFrameExtractedFromMessage)
		{
			std::string line = messages[3];

			/*DataMessages entities;
			parser.parseMessage(line, entities, &timeFrame);*/
			parser.parseMessage(line);
			auto entities = parser.entities;
			auto timeFrame = parser.timeFrame;

			//first check if the entity is there
			Assert::IsTrue(entities.size() == 1);

			//check if the time frame is correct
			Assert::AreEqual(90.77, timeFrame);

		}
	};
}
