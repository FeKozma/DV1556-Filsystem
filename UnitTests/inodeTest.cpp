#include "stdafx.h"
#include "CppUnitTest.h"

#include "../src/inode.h"

#include <vector>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{		
	TEST_CLASS(inodeTest)
	{
	private:
		std::vector<std::string> pathSpliter(std::string path)
		{
			std::vector<std::string> retPath;

			int start = 0;

			for (int i = 0; i < path.length(); ++i) {
				if (path[i] == '/') {
					retPath.push_back(path.substr(start, i - start));
					start = i + 1;
				}
			}

			if (start != path.length()) {
				retPath.push_back(path.substr(start));
			}

			return retPath;
		}

	public:
		
		TEST_METHOD(TestPathSplitterRoot)
		{
			std::string correctPath[] = { "", "home", "user1", "..", "user2", "some", "folder" };
			std::string path = "/home/user1/../user2/some/folder/";
			std::vector<std::string> pathList = pathSpliter(path);
			
			Assert::AreEqual(7, (int)pathList.size());

			for (int i = 0; i < pathList.size(); ++i) {
				Assert::AreEqual(correctPath[i], pathList.at(i));
			}
		}

		TEST_METHOD(TestPathSplitter)
		{
			std::string correctPath[] = { "home", "user1", "..", "user2", "some", "folder" };
			std::string path = "home/user1/../user2/some/folder/";
			std::vector<std::string> pathList = pathSpliter(path);

			Assert::AreEqual(6, (int)pathList.size());

			for (int i = 0; i < pathList.size(); ++i) {
				Assert::AreEqual(correctPath[i], pathList.at(i));
			}
		}
	};
}