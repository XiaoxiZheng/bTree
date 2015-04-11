
#include"bTree.h"
#include"bTreeNode.h"

int main()
{
    int op = 0;
	int size;
	int key;
	int value;
	cout<<"Please enter the size of bTree"<<endl;
	cin>>size;
    cout<<"Insert,Delete,Find,toStr()"<<endl;
	BTree * BT = new BTree(size);
	while(op < 4) {
		cin >> op;
		switch(op) {
			case 0://Insert
                cout<<"Insert"<<endl;
				cin>>key;
				cin>>value;
//				BT->insert(key,value);
                BT->insert(key);
				break;
			case 1://Delete
			    cout<<"Delete"<<endl;
				cin>>key;
//				BT->delete_key(key);
                BT->delete_key(key);
				break;
			case 2://Find
			    cout<<"Find"<<endl;
				cin>>key;
				BT->find(key);
//				if(BT->find(key,&value))
					//cout<<value<<endl;
				//else
					//cout<<"Key Not Found\n";
				break;
			case 3://toString
			    cout<<"toStr()/Traverse"<<endl;
			    BT->traverse();
//				BT->toStr();
				break;
		}
	}
	return 0;
}
