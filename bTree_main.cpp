
#include"bTree.h"
#include"bTreeNode.h"

int main()
{
    int op=0;
	int size;
	string key;
	string value;
	cout<<"Please enter the size of bTree"<<endl;
	cin>>size;
    cout<<"Insert,Delete,Find,toStr()"<<endl;
	BTree * BT = new BTree(size);
	while(op < 4) {
		cin >> op;
		switch(op) {
			case 0://Insert
				cin>>key;
				cin>>value;
//				BT->insert(key,value);
                cout<<"Insert():"+key<<endl;
                BT->insert(key);
				break;
			case 1://Delete
			    cin>>key;
			    cout<<"Delete():" +key<<endl;
				BT->delete_key(key);

				break;
			case 2://Find
                cin>>key;
			    cout<<"Find(): "+key<<endl;
				//cout<<BT->find(key)<<endl;
				if(BT->find(key,&value))
					cout<<value<<endl;
				else
					cout<<"Key Not Found\n";
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
