all: bTree

bTree: bTree_main.cpp bTree.h bTreeNode.h bTreeNode.cpp

	g++ -o bTree bTree_main.cpp bTree.h bTreeNode.h bTreeNode.cpp


clean:
	rm -rf bTree.exe
