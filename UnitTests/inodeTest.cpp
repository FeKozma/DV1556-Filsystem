#include "stdafx.h"
#include "CppUnitTest.h"

#include "../src/inode.cpp"

#include <vector>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{		
	TEST_CLASS(inodeTest)
	{
	private:
		std::vector<std::string> pathSplitter(std::string path)
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
			std::vector<std::string> pathList = pathSplitter(path);
			
			Assert::AreEqual(7, (int)pathList.size());

			for (int i = 0; i < pathList.size(); ++i) {
				Assert::AreEqual(correctPath[i], pathList.at(i));
			}
		}

		TEST_METHOD(TestPathSplitter)
		{
			std::string correctPath[] = { "home", "user1", "..", "user2", "some", "folder" };
			std::string path = "home/user1/../user2/some/folder/";
			std::vector<std::string> pathList = pathSplitter(path);

			Assert::AreEqual(6, (int)pathList.size());

			for (int i = 0; i < pathList.size(); ++i) {
				Assert::AreEqual(correctPath[i], pathList.at(i));
			}
		}

		TEST_METHOD(TestAddFolder)
		{
			inode iNode("");
			Assert::AreEqual(iNode.addFolder("folder1", "/"), true);
			Assert::AreEqual(iNode.addFolder("folder2", "/"), true);
			Assert::AreEqual(iNode.addFolder("folder1", "/"), false);
			Assert::AreEqual(iNode.addFolder("folder2", "/"), false);
			Assert::AreEqual(iNode.addFolder("folder3", "/"), true);
		}

		TEST_METHOD(TestGoToFolder)
		{
			inode iNode("");
			iNode.addFolder("folder1", "");
			iNode.addFolder("folder2", "folder1");
			iNode = *iNode.goToFolder("folder1");
			Assert::AreEqual(iNode.getFolderName(), (std::string)"folder1");
			
			iNode.addFolder("folder3", "/");
			iNode = *iNode.goToFolder("/folder2");
			Assert::AreNotEqual(iNode.getFolderName(), (std::string)"folder2");

			iNode = *iNode.goToFolder("folder3");
			Assert::AreEqual(iNode.getFolderName(), (std::string)"folder3");
			
			iNode = *iNode.goToFolder("/folder1/../folder1/folder2");
			Assert::AreEqual(iNode.getFolderName(), (std::string)"folder3");
		}
	};
}