﻿#include "file_search.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <cassert>
#include "CppUnitTest.h"

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace unittest
{
	TEST_CLASS(unittest) {
public:

	unittest() {
		filesystem::create_directory("C:/test");
		filesystem::create_directory("C:/test/dir1");
		filesystem::create_directory("C:/test/dir2");
		filesystem::create_directory("C:/test/dir3");
		filesystem::create_directory("C:/test/dir3/dir4");
		ofstream outfile1("C:/test/test.txt");
		ofstream outfile2("C:/test/dir1/test.txt");
		ofstream outfile3("C:/test/dir2/not_test.txt");
		ofstream outfile4("C:/test/dir3/not_test.txt");
		ofstream outfile5("C:/test/dir3/dir4/test.txt");
	}

	~unittest() {
		filesystem::remove_all("C:/test");
	}

	TEST_METHOD(OnlyOneFileInFirstDirTest) {
		streambuf* oldCoutStreamBuf = cout.rdbuf();
		stringstream stringStream;
		cout.rdbuf(stringStream.rdbuf());

		char* argv[] = { "file_search", "test.txt", "C:/test/dir1" };
		int argc = 3;
		file_search(argc, argv);

		string content = stringStream.str();
		string expected = "C:/test/dir1/test.txt\n";
		cout.rdbuf(oldCoutStreamBuf);
		Assert::AreEqual(expected, content);
	}

	TEST_METHOD(SuccessfulSearchExitCodeTest) {
		streambuf* oldCoutStreamBuf = cout.rdbuf();
		stringstream stringStream;
		cout.rdbuf(stringStream.rdbuf());

		char* argv[] = { "file_search", "test.txt", "C:/test/dir1" };
		int argc = 3;
		int code = 0;
		code = file_search(argc, argv);

		cout.rdbuf(oldCoutStreamBuf);
		Assert::AreEqual(code, 0);
	}

	TEST_METHOD(NoMatchesTest) {
		streambuf* oldCoutStreamBuf = cout.rdbuf();
		stringstream stringStream;
		cout.rdbuf(stringStream.rdbuf());

		char* argv[] = { "file_search", "test.txt", "C:/test/dir2" };
		int argc = 3;
		file_search(argc, argv);

		string content = stringStream.str();
		string expected = "";
		cout.rdbuf(oldCoutStreamBuf);
		Assert::AreEqual(expected, content);
	}

	TEST_METHOD(NoMatchesExitCodeTest) {

		char* argv[] = { "file_search", "test.txt", "C:/test/dir2" };
		int argc = 3;
		int code = 0;
		code = file_search(argc, argv);

		Assert::AreEqual(code, 0);
	}

	TEST_METHOD(OnlyOneFileInSecondDirTest) {
		streambuf* oldCoutStreamBuf = cout.rdbuf();
		stringstream stringStream;
		cout.rdbuf(stringStream.rdbuf());

		char* argv[] = { "file_search", "test.txt", "C:/test/dir3" };
		int argc = 3;
		file_search(argc, argv);

		string content = stringStream.str();
		string expected = "C:/test/dir3/dir4/test.txt\n";
		cout.rdbuf(oldCoutStreamBuf);
		Assert::AreEqual(expected, content);
	}

	TEST_METHOD(MultipleFilesTest) {
		streambuf* oldCoutStreamBuf = cout.rdbuf();
		stringstream stringStream;
		cout.rdbuf(stringStream.rdbuf());

		char* argv[] = { "file_search", "test.txt", "C:/test" };
		int argc = 3;
		file_search(argc, argv);

		string content = stringStream.str();
		string expected = "C:/test/test.txt\nC:/test/dir3/dir4/test.txt\nC:/test/dir1/test.txt\n";
		cout.rdbuf(oldCoutStreamBuf);
		Assert::AreEqual(expected, content);
	}

	//Assuming that there are only this occurrences
	TEST_METHOD(AllSystemSearchTest) {
		streambuf* oldCoutStreamBuf = cout.rdbuf();
		stringstream stringStream;
		cout.rdbuf(stringStream.rdbuf());

		char* argv[] = { "file_search", "test.txt", "" };
		int argc = 2;
		file_search(argc, argv);

		string content = stringStream.str();
		string expected = "C:/test/test.txt\nC:/test/dir3/dir4/test.txt\nC:/test/dir1/test.txt\n";
		cout.rdbuf(oldCoutStreamBuf);
		Assert::AreEqual(expected, content);
	}

	TEST_METHOD(IncorrectArgumentTest) {
		streambuf* oldCoutStreamBuf = cerr.rdbuf();
		stringstream stringStream;
		cerr.rdbuf(stringStream.rdbuf());

		char* argv[] = { "file_search", "", ""};
		int argc = 1;
		file_search(argc, argv);

		string content = stringStream.str();
		string expected = "Argument do not correspond signature";
		cerr.rdbuf(oldCoutStreamBuf);
		Assert::AreEqual(expected, content);
	}

	TEST_METHOD(IncorrectArgumentExitCodeTest) {

		char* argv[] = { "file_search", "", ""};
		int argc = 1;
		int code = 0;
		code = file_search(argc, argv);

		Assert::AreEqual(code, 1);
	}

	TEST_METHOD(IncorrectDirectoryTest) {
		streambuf* oldCoutStreamBuf = cerr.rdbuf();
		stringstream stringStream;
		cerr.rdbuf(stringStream.rdbuf());

		char* argv[] = { "file_search", "test.txt", "C:/test/dir5" };
		int argc = 3;
		file_search(argc, argv);

		string content = stringStream.str();
		string expected = "Directory do not exist";
		cerr.rdbuf(oldCoutStreamBuf);
		Assert::AreEqual(content, expected);
	}

	TEST_METHOD(IncorrectDirectoryExitCodeTest) {

		char* argv[] = { "file_search", "test.txt", "C:/test/dir5" };
		int argc = 3;
		int code = 0;
		code = file_search(argc, argv);

		Assert::AreEqual(code, 1);
	}
	};
}
