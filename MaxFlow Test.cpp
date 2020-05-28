#include "pch.h"
#include "CppUnitTest.h"

#include "../MaxFlow algorithm/Graph.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MaxFlowTest
{
	TEST_CLASS(MaxFlowTest)
	{
	public:
		
		TEST_METHOD(Overall__test)
		{
			Graph graph;
			graph.getFrom("C:\\Users\\Кристина\\source\\repos\\MaxFlow algorithm\\MaxFlow algorithm\\Data.txt");
			Assert::IsTrue(graph.getMaxFlow() == 24);
		}

		TEST_METHOD(getFrom_empty_file)
		{
			Graph g;
			try
			{
				g.getFrom("C:\\Users\\Кристина\\source\\repos\\MaxFlow algorithm\\MaxFlow algorithm\\Empty.txt");

			}
			catch (std::exception error)
			{
				Assert::AreEqual(error.what(), "File is empty!");
			}
		}

		TEST_METHOD(getFrom_file_doesnt_exist)
		{
			try
			{
				Graph g;
				g.getFrom("Blablabla.txt");

			}
			catch (std::exception error)
			{
				Assert::AreEqual(error.what(), "No file was found!");
			}
		}

		TEST_METHOD(getFrom_wrong_format_data)
		{
			/////data from wrong file/////
			////A B   /////
			////B C 10////
			////C T 30////
			////K T 10////
			////C K 20////
			try
			{
				Graph g;
				g.getFrom("C:\\Users\\Кристина\\source\\repos\\MaxFlow algorithm\\MaxFlow algorithm\\Wrong_Data.txt");

			}
			catch (std::exception error)
			{
				Assert::AreEqual(error.what(), "Wrong data format in the file!");
			}
		}

		TEST_METHOD(getFrom_wrong_data_test2)
		{
			/////data from wrong file/////
			////A B 4
			////B C 10
			////C T 30
			////K T 12
			////C B 20
			try
			{
				Graph g;
				g.getFrom("C:\\Users\\Кристина\\source\\repos\\MaxFlow algorithm\\MaxFlow algorithm\\Wrong_Data2.txt");

			}
			catch (std::exception error)
			{
				Assert::AreEqual(error.what(), "Last vertex must be 'T'!");
			}
		}

		TEST_METHOD(getFrom_wrong_format_data3)
		{
			/////data from wrong file/////
			///Sa T 3
			///S C 12
			///C G 5
			///G T 2
			///C T 10
			try
			{
				Graph g;
				g.getFrom("C:\\Users\\Кристина\\source\\repos\\MaxFlow algorithm\\MaxFlow algorithm\\Wrong_Data3.txt");

			}
			catch (std::exception error)
			{
				Assert::AreEqual(error.what(), "Wrong data format in the file!");
			}
		}

		TEST_METHOD(getMaxFlow_empty)
		{
			Graph g;
			try
			{
				g.getMaxFlow();
			}
			catch (std::exception error)
			{
				Assert::AreEqual(error.what(), "Vertex list was empty!");
			}
		}

		TEST_METHOD(getMaxFlow_right_data)
		{
			    //S O 3
				//S P 3
				//O Q 3
				//O P 2
				//P R 2
				//Q R 4
				//Q T 2
				//R T 3
			Graph g;
			g.getFrom("C:\\Users\\Кристина\\source\\repos\\MaxFlow algorithm\\MaxFlow algorithm\\Data2.txt");
			Assert::AreEqual(g.getMaxFlow(), (size_t)5);
		}



	};
}
