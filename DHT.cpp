#include<cstring>
#include<functional>
#include<iostream>
#include<cmath> 
#include<cstdlib>
#include<ctime>
#include<sstream>
#include <fstream>
using namespace std;

string TO_String (int intValue){
	stringstream stringValue;	// converting int to string
	stringValue<<intValue;
	string tempName = stringValue.str();
	return tempName;
}

/* ----------------------------------------------------- AVL Tree ---------------------------------------------------------- */
template <typename dataType>
struct AVLT_Node
{
	string fileName;
	int fileLineNumber; 
	int machineID;
	
	AVLT_Node* leftNode;
	AVLT_Node* rightNode;
	int Height;

	AVLT_Node(){
		fileName = "";
		machineID = -1;
		fileLineNumber = 0;
	
		leftNode = NULL;
		rightNode = NULL;
		Height = -1;
	}
	
};

template <typename dataType>
class AVL_Tree
{
public:
	AVLT_Node<dataType>* rootNode;			// Tree root node
	AVLT_Node<dataType>* tempNode;			// temporary node 
	int counterFileLine;
	// Constructor -> assign NULL to nodes pointers
	AVL_Tree()
	{
		rootNode = NULL;
		tempNode = NULL;
		counterFileLine = 0;
	}

	// This function will check either the tree is empty or not.
	bool IsEmpty()
	{
		if (rootNode == NULL)
			return 1;	// return true if tree is empty else false
		return 0;
	}


	/* This Function is used to calculate Height of nodes in AVL */
	int getHeight(AVLT_Node<dataType>* pNode)
	{
		if (pNode == NULL)
			return -1;
		else
			return max(getHeight(pNode->leftNode), getHeight(pNode->rightNode)) + 1;	// max function will return the maximum value of the two getHeight() AVL Tree Nodes
	}

	/* This function is for calculating balancing factor */
	int getBalancingFactor(AVLT_Node<dataType>* pNode)
	{
		if (pNode->leftNode != NULL && pNode->rightNode != NULL)
		{
			return pNode->leftNode->Height - pNode->rightNode->Height;
		}
		else if (pNode->leftNode == NULL && pNode->rightNode != NULL)
		{
			return (pNode->rightNode->Height) * -1;
		}
		else if (pNode->leftNode != NULL && pNode->rightNode == NULL)
		{
			return pNode->leftNode->Height;
		}
	}

	/* get Machine ID */
	int getNode_MID () const {
		return rootNode->machineID;
	}

	// Right Rotation
	AVLT_Node<dataType>* SRWR(AVLT_Node<dataType>* previousNode)
	{
		AVLT_Node<dataType>* newNode;
		newNode = previousNode->rightNode;
		previousNode->rightNode = newNode->leftNode;
		newNode->leftNode = previousNode;
		previousNode->Height = getHeight(previousNode);
		newNode->Height = getHeight(newNode);
		return newNode;
	}

	// Left Rotation
	AVLT_Node<dataType>* SRWL(AVLT_Node<dataType>* previousNode)
	{
		AVLT_Node<dataType>* newNode;
		newNode = previousNode->leftNode;
		previousNode->leftNode = newNode->rightNode;
		newNode->rightNode = previousNode;
		previousNode->Height = getHeight(previousNode);
		newNode->Height = getHeight(newNode);
		return newNode;
	}

	// Left Right Rotation
	AVLT_Node<dataType>* DRWR(AVLT_Node<dataType>* previousNode)
	{
		previousNode->rightNode = SRWL(previousNode->rightNode);
		return SRWR(previousNode);
	}

	// Right Left Rotation
	AVLT_Node<dataType>* DRWL(AVLT_Node<dataType>* previousNode)
	{
		previousNode->leftNode = SRWR(previousNode->leftNode);
		return SRWL(previousNode);
	}

	/* This function is only to get value from user not root or any other thing */
	void insert_tree( int MachineID, string F_Name){
		rootNode = Insert_AVL(rootNode, MachineID, F_Name);
	}
	
	/* This is the main Insertion Function It will insert values and keep the Tree balanced by performing rotations */
	AVLT_Node<dataType>* Insert_AVL(AVLT_Node<dataType>* rNode, int value, string F_Name){
		if (rNode == NULL){
			counterFileLine++;			
			rNode = new AVLT_Node<dataType>;
			rNode->machineID = value;
			rNode->leftNode = NULL;
			rNode->rightNode = NULL;
			rNode->fileLineNumber = counterFileLine;
			rNode->fileName = F_Name;
		}

		else if (rNode->machineID > value){
			rNode->leftNode = Insert_AVL(rNode->leftNode, value, F_Name);
			if (getBalancingFactor(rNode) == 2 && getBalancingFactor(rNode->leftNode) == 1)
				rNode = SRWL(rNode);
			if (getBalancingFactor(rNode) == -2 && getBalancingFactor(rNode->rightNode) == 1)
				rNode = DRWL(rNode);
		}

		else if (rNode->machineID < value){
			rNode->rightNode = Insert_AVL(rNode->rightNode, value, F_Name);
			if (getBalancingFactor(rNode) == -2 && getBalancingFactor(rNode->rightNode) == -1)
				rNode = SRWR(rNode);
			if (getBalancingFactor(rNode) == 2 && getBalancingFactor(rNode->leftNode) == -1)
				rNode = DRWR(rNode);
		}

		rNode->Height = getHeight(rNode);
		return rNode;
	}

	/* This code is for deleting values from an AVL tree. It will rotate nodes after deletion if needed */
	AVLT_Node<dataType>* deleteAVLT_Node(int value)
	{
		return deleteNode(rootNode, value);
	}

	AVLT_Node<dataType>* deleteNode(AVLT_Node<dataType>* pNode, int value)
	{
		if (pNode->leftNode == NULL && pNode->rightNode == NULL)
		{
			if (pNode == this->rootNode)
				this->rootNode = NULL;
			delete pNode;
			return NULL;
		}

		else
		{
			AVLT_Node<dataType>* tempNode;
			if (pNode->machineID < value)
			{
				pNode->rightNode = deleteNode(pNode->rightNode, value);
			}
			else if (pNode->machineID > value)
			{
				pNode->leftNode = deleteNode(pNode->leftNode, value);
			}
			else
			{
				if (pNode->leftNode != NULL)
				{
					tempNode = IP_inpre(pNode->leftNode);
					pNode->machineID = tempNode->machineID;
					pNode->fileLineNumber = tempNode->fileLineNumber;
					pNode->fileName = tempNode->fileName;
					pNode->leftNode = deleteNode(pNode->leftNode, tempNode->machineID);
				}
				else
				{
					tempNode = IS_insuc(pNode->rightNode);
					pNode->machineID = tempNode->machineID;
					pNode->fileLineNumber = tempNode->fileLineNumber;
					pNode->fileName = tempNode->fileName;
					pNode->rightNode = deleteNode(pNode->rightNode, tempNode->machineID);
				}
			}
			if (getBalancingFactor(pNode) == 2 && getBalancingFactor(pNode->leftNode) == 1)
				pNode = SRWL(pNode);
			else if (getBalancingFactor(pNode) == 2 && getBalancingFactor(pNode->leftNode) == -1)
				pNode = DRWR(pNode);
			else if (getBalancingFactor(pNode) == 2 && getBalancingFactor(pNode->leftNode) == 0)
				pNode = SRWL(pNode);
			else if (getBalancingFactor(pNode) == -2 && getBalancingFactor(pNode->rightNode) == 1)
				pNode = DRWL(pNode);
			else if (getBalancingFactor(pNode) == -2 && getBalancingFactor(pNode->rightNode) == -1)
				pNode = SRWR(pNode);
			else if (getBalancingFactor(pNode) == -2 && getBalancingFactor(pNode->rightNode) == 0)
				pNode = SRWL(pNode);
			return pNode;
		}
	}

	void writeAVL_Data () {
		
	}

	AVLT_Node<dataType>* IP_inpre(AVLT_Node<dataType>* pNode)
	{
		while (pNode->rightNode != NULL)
			pNode = pNode->rightNode;
		return pNode;
	}

	AVLT_Node<dataType>* IS_insuc(AVLT_Node<dataType>* pNode)
	{
		while (pNode->leftNode != NULL)
			pNode = pNode->leftNode;
		return pNode;
	}

	/* For searching data in AVL Tree */
	// This function is for searching. It will return the value if value exist in the tree.
	AVLT_Node<dataType>* Search_AVLT(int value)
	{
		tempNode = rootNode;
		while (tempNode != NULL)
		{

			if (tempNode->machineID == value)	// If value matched then this will return it. 
			{
//				cout << "Value Exist !" << endl;
				return tempNode;
			}

			else if (tempNode->machineID > value)	// If the value is lesser than the current node data then it will check on the left side
				tempNode = tempNode->leftNode;
			else if (tempNode->machineID < value) 	// In this case function will check value on the right side of the tree	
				tempNode = tempNode->rightNode;
		}
	
//		cout << "Value doses not exist in AVL !!!" << endl;
		return 0;
	}

	bool Search_AVLT_Two(int value)
	{
		tempNode = rootNode;
		while (tempNode != NULL)
		{

			if (tempNode->machineID == value)	// If value matched then this will return it. 
			{
//				cout << "Value Exist ! AVLT2" << endl;
				return true;
			}

			else if (tempNode->machineID > value)	// If the value is lesser than the current node data then it will check on the left side
				tempNode = tempNode->leftNode;
			else if (tempNode->machineID < value) 	// In this case function will check value on the right side of the tree	
				tempNode = tempNode->rightNode;
		}
//		cout << "Value doses not exist !!! AVLT2" << endl;
		return 0;
	}

	// This is the first display function of Inorder traversal
	// This will check either the root is empty or not
	// It will call another inorder function that will actually display data
	void inorder_display()
	{
		if (IsEmpty())
		{
			cout << "Tree Is Empty !!!" << endl;
			return;
		}
		else
		{
			tempNode = rootNode;
			inorder(tempNode);
		}
	}

	//	This is inorder function used to display values of tree
	void inorder(AVLT_Node<dataType>*& tNode) const
	{
		if (tNode != NULL)
		{
			inorder(tNode->leftNode);
			cout << "Machine ID = " << tNode->machineID << endl;
			cout << "File Name = " << tNode->fileName << endl;
			cout << "Line Number = " << tNode->fileLineNumber << endl;
			inorder(tNode->rightNode);

		}
	}

/*
	// This is the first display function of Preorder traversal
	// This will check either the root is empty or not
	// It will call another Preorder function that will actually display data
	void preorder_display()
	{
		if (IsEmpty())
		{
			cout << "Tree Is Empty !!!" << endl;
			return;
		}
		else
		{
			tempNode = rootNode;
			preorder(tempNode);
		}
	}

	//	This is the preorder function used to display values of tree
	void preorder(AVLT_Node<dataType>*& tNode) const
	{
		if (tNode != NULL)
		{
			cout << "Machine ID = " << tNode->machineID << endl;
			cout << "File Name = " << tNode->fileName << endl;
			cout << "Line Number = " << tNode->fileLineNumber << endl;
			preorder(tNode->leftNode);
			preorder(tNode->rightNode);
		}
	}

	// This is the first display function of Post_order traversal
	// This will check either the root is empty or not
	// It will call another Postorder function that will actually display data
	void postorder_display()
	{
		if (IsEmpty())
		{
			cout << "Tree Is Empty !!!" << endl;
			return;
		}
		else
		{
			tempNode = rootNode;
			postorder(tempNode);
		}
	}

	//This is the postorder function used to display values of tree
	void postorder(AVLT_Node<dataType>*& tNode) const
	{
		if (tNode != NULL)
		{
			preorder(tNode->leftNode);
			preorder(tNode->rightNode);
			cout << "Machine ID = " << tNode->machineID << endl;
			cout << "File Name = " << tNode->fileName << endl;
			cout << "Line Number = " << tNode->fileLineNumber << endl;
		}
	}
*/
	~AVL_Tree()		//Destructor
	{
	}
};


// -----------------------------------------------------Doubly Linked List --------------------------------------------------------- //

template<typename T>
struct doublyNode
{
	T value;
	doublyNode* next;
	doublyNode* previous;
	doublyNode()
	{
		next = NULL;
		previous = NULL;
	}
};

template<typename T>
class doublyLinkedList
{
	int size;
	doublyNode<T>* head;
	doublyNode<T>* node;
	doublyNode<T>* temp;
public:
	doublyLinkedList()	{
		size = 0;
		head = NULL;
	}
	bool isEmpty()
	{
		if (head == NULL)
			return true;
		return false;
	}
	
	void insertNode(T value)
	{
		temp = new doublyNode<T>;
		temp->value = value;

		if (isEmpty())
		{
			head = temp;
			size++;
		}
		else 
		{
			node = head;
			while (node->next != NULL)
			{
				node = node->next;
			}
			node->next = temp;
			temp->previous = node;
			size++;
		}
	}

	int getSize()
	{
		return size;
	}

	void deleteNode()
	{
		if (!isEmpty())
		{
			if (head->next == NULL)
			{
				temp = head;
				head = NULL;
				delete temp;
				size--;
				return;
			}
			else
			{
				node = head;
				while (node->next != NULL)
				{
					node = node->next;
				}
				temp = node;
				node->previous->next = NULL;
				delete temp;
				size--;
			}
		}
	}

	void displayList()
	{
		if (isEmpty())
		{
			cout << "The list is empty..." << endl;
		}
		else
		{
			node = head;
			while (node != NULL)
			{
				cout << node->value<<" ";
				node = node->next;
			}
			cout << endl;
		}
	}

	void setValueAt (int index, int val) {
		if (index >= 0 && index < size){
			node = head;
			int count = 0;
			while (count != index){
				node = node->next;
				count++;
			}

			if (node)
				node->value = val;
			else{
				cout<<"Node is Empty !!!"<<endl;
				return;
			}
		}
		
		else {
			cout<<"Index Not in Range !!! setValueAt"<<endl;
			return;
		}
	}

	int getValueAt (int index) {
		if (index >=0 && index<size){
			int count = 0;
			node = head;
			while (count != index){
				node = node->next;
				count++;
			}
		
			return node->value;
		}
		
		else{
			cout<<"Index Not In Range getValueAt"<<endl;
			return -1;
		}
	}

	bool find(T val)
	{
		if (!isEmpty())
		{
			if (head->next == NULL)
			{
				if (head->value == val)
				{
					cout << "FOUND" << endl;
					return true;
				}
				cout << "NOT FOUND" << endl;
				return false;
			}
			else
			{
				node = head;
				while (node != NULL)
				{
					if (node->value == val)
					{
						cout << "FOUND" << endl;
						return true;
					}
					node = node->next;
				}
				cout << "NOT FOUND" << endl;
				return false;
			}
		}
		else
		{
			cout << "NOT FOUND" << endl;
			return false;
		}
	}

	void clearList_DLL(){
		int tempSize = getSize();
		for (int i = 0; i < tempSize; i++)
			deleteNode();
	}

	~doublyLinkedList()
	{
	}
};





// ----------------------------------------------------- Circular List ---------------------------------------------------- //

template<typename T>
struct circularNode
{
	int machineID;
	AVL_Tree<T> AVLT_Data;
	doublyLinkedList<T> DL_RoutingTable;

	circularNode* next;
	circularNode()
	{
		next = NULL;
	}
	
};

template<typename T>
class circularLinkedList	// This is the class that store machines IDs.
{
	int size;	// current size of circular list
	int totalMachines;		// total number of machines that can be the list 	
	int maxSize;	// maximum size -> number of bits
	int activeMachines;		// Machines that will store data
	
	circularNode<T>* head;
	circularNode<T>* node;
	circularNode<T>* temp;
public:
	
	// Default Constructor

	circularLinkedList()
	{
		size = 0;
		maxSize = 0;
		totalMachines = 0;
		activeMachines = 0;
		head = NULL;
	}

	// Parameterize Constructor

	circularLinkedList(int maxS, int active=1)
	{
		size = 0;
		maxSize = maxS;
		totalMachines = pow(2,maxS);
		if (active < 0 || active > totalMachines){
			activeMachines = 1;
			cout<<"Active Machines Not in Range !!!"<<endl;
		}
		else 
			activeMachines = active;
		head = NULL;
	}
	
	
	// set Maximum Size
	
	void setMaximumSize (int size_val){
		if (size_val < 0 || size_val > 160){
			cout<<"Nodes Size Not in Range !!!"<<endl;
			return;
		}

		maxSize = size_val;
		setTotalMachines(pow(2,maxSize));
	}

	// set Active Machines
	
	void setActiveMachines (int active){
		if (active < 0 || active > totalMachines){
			activeMachines = 1;
			cout<<"Active Machines Not in Range !!!"<<endl;
		}
		else
			activeMachines = active;
	}
	
	// set Total Machines
	
	void setTotalMachines (int tm){
		totalMachines = tm;
	}

	// getters Functions

	int getSize()
	{
		return size;
	}
	
	int getMaxSize () const {
		return maxSize;
	}
	
	int getTotalMachines () const{
		return totalMachines;
	}
	
	int getActiveMachines () const {
		return activeMachines;
	}

	// getters Functions End

	/* This function checks whether the ccl is empty or not */
	
	bool isEmpty()
	{
		if (head == NULL)
			return true;
		return false;
	}

	/* This Function Insert Machine ID at the end of list ccl */

	void insert(T id)
	{
		if (size <= totalMachines && size < activeMachines){
			temp = new circularNode<T>;
			temp->machineID = id;
	
			if (head == NULL)
			{
				head = temp;
				head->next = head;
				size++;
			}
	
			else
			{
				node = head;
				while (node->next != head)
				{
					node = node->next;
				}
				node->next = temp;
				node->next->next = head;
				size++;
			}
		}
		
		else {
			cout<<"Cannot Insert Data "<<endl;
		}
	}

	/* This Function Insert Machine ID at the given Index of list ccl */
	
	void insertAtIndex(T id, int ind)
	{
		
		if (size <= totalMachines && size < activeMachines){
			if (ind <= size - 1)
			{
				if (isEmpty() && ind == 0)
				{
					head = temp;
					head->next = head;
					size++;
				}
				else if (ind == 0)
				{
					head->machineID = id;
				}
				else
				{
					int index = 0;
					node = head;
					for (int i = 0;i < getSize();index++)
					{
						if (index == ind)
						{
							node->machineID = id;
							return;
						}
						node = node->next;
					}
				}
			}
		}
	}

	/* This function return the circular list Node at specific index */	

	circularNode<T> getListAt (int indexNum) {
		if (isEmpty()){
			cout<<"List is Empty !!!"<<endl;
		}
		
		else if (indexNum >=0 && indexNum < size){
			int count = 0;
			node = head;
			while (node->next != head && node != NULL && count != indexNum){
				node = node->next;
				count++;
			}
			
			return *node;
		}
	}

	/* This function return the circular list Node at specific index */	

	int getMachineAt (const int& indexNum) {
		if (isEmpty()){
			cout<<"List is Empty !!!"<<endl;
		}
		
		else if (indexNum >=0 && indexNum < size){
			int count = 0;
			node = head;
			while (count != indexNum){
				node = node->next;
				count++;
			}
			
			return (node->machineID);
		}
	}

	
	// Display Tree Function
	
	void displayTree () {
		node = head;
		int count = 1;
		do{
			cout<<" ::::::::::::::::::::::: Tree Number "<<count<<" :::::::::::::::::::::"<<endl;	
			node->AVLT_Data.inorder_display();
			cout<<endl;
			node = node->next;
			count++;
		} while (node != head);
	}

	/* This fucntion is used to create a new file name */
	string createFileName (int idofmachine){
		stringstream str;	// converting int to string
		str<<idofmachine;
		string tempName = str.str();
		string temp = "Machine_"+tempName+".txt";
		return temp; // return filename
	}

	/* This Function is used to add data on the machines */
	// Files will be created for the machines that are active
	// Machines that are not active their data will be store in the next active machine

	bool addData_CLL (int machine_ID, string data_value){
		int nextActiveMachinePos;
		nextActiveMachinePos = getNextActiveMachinePos(machine_ID);			
		
		node  = head;
		for (int i=0; i<nextActiveMachinePos; i++)
			node = node->next;		
		
		if (node == NULL){
			cout<<"NODE IS NULL"<<endl;
		}
		
		bool checkMachinePresence = node->AVLT_Data.Search_AVLT_Two(machine_ID);
		if (! checkMachinePresence){
			string F_Name = createFileName(node->machineID);
			node->AVLT_Data.insert_tree(machine_ID, F_Name);
			writeDataIntoFile(node,machine_ID, data_value);
			return 1;
		}
	
		else {
			cout<<"DATA with same key exist !!!"<<endl;
			return 0;
		}
	}

	/* This Function is used to write data into file */
	void writeDataIntoFile(circularNode<T>* &tNode, int datakey ,string data_value){
		AVLT_Node<T> *tempNode;
		tempNode = tNode->AVLT_Data.Search_AVLT(datakey);

		if (tempNode == NULL){
			cout<<"NODE RETURN NULL"<<endl;
			return;
		}		
		
		string NameOfFile = tempNode->fileName;

		char* fname = new char [NameOfFile.length()];
		for (int i=0; i<NameOfFile.length(); i++)
			fname[i] = NameOfFile[i];
		
		ofstream writeData;
		writeData.open(fname, ios::app);
		writeData<<data_value<<endl;
		writeData.close();	
	}

	/* This Function is for deleting Node from AVL Tree */
	
	bool removeData_CLL (int machine_ID) {
		int nextActiveMachinePos;
		int activeMachine;
		nextActiveMachinePos = getNextActiveMachinePos(machine_ID);			
		node = head;
		for (int i=0; i<nextActiveMachinePos; i++)	
			node = node->next;			
				
		if (node->AVLT_Data.Search_AVLT_Two(machine_ID)){
			node->AVLT_Data.deleteAVLT_Node(machine_ID);	
			return 1;
		}

		return 0;
	}

	// This function is for searching data
	
	string readData_CLL (int mID_One, int mID_two) {	// searching start from machine one and it search machine two data 
		if (checkMachineActivation(mID_One)) {	
			circularNode<T> *tempNode;
			int mONE_pos = getNextActiveMachinePos(mID_One);
			int mTWO_active = getMachineAt(getNextActiveMachinePos(mID_two));
			int nextMachine = 0;
			bool foundcheck = 0;

			tempNode = head;
			for (int i=0; i<mONE_pos; i++)	
				tempNode = tempNode->next;
			
			for (int i=0; i<totalMachines; i++){
				foundcheck = 0;
				for (int j=0; j<maxSize; j++){
					
					if (mTWO_active > tempNode->DL_RoutingTable.getValueAt(j)){	// it checks Machine Two Active Machine ID with the routing table of Machine One		
						nextMachine = tempNode->DL_RoutingTable.getValueAt(j);
						foundcheck = 1;
					}
					
					else if (mTWO_active < tempNode->DL_RoutingTable.getValueAt(j)){
						j = maxSize + 1; // it breaks from the second loop 	
					}
					
					else {	// IDs matched
						nextMachine = tempNode->DL_RoutingTable.getValueAt(j);
						nextMachine = getNextActiveMachinePos(nextMachine);
						tempNode = head;
	
						for (int i=0; i<nextMachine; i++)
							tempNode = tempNode->next;
						AVLT_Node<T>* tempTreeNode = tempNode->AVLT_Data.Search_AVLT(mID_two);					
						
						if (!tempTreeNode){
							string tempStr = TO_String(mID_two);
							return "Data Not Found (ID = "+tempStr+")\n";
						}
						
						int lineNum = tempTreeNode->fileLineNumber;
						string str = readFromFile(tempNode, lineNum);
						return str;
					}			
				}
				
				if (foundcheck){
					nextMachine = getNextActiveMachinePos(nextMachine);
					tempNode = head;
					for (int i=0; i<nextMachine; i++)
						tempNode = tempNode->next;
				}
				
				else {
					tempNode = tempNode->next;				
				}
			}
			return "DATA NOT FOUND!\n";
		}
		
		else {
			cout<<"Machine from which you want to search is not Active !!!!"<<endl;	
			return "Reading Failed !!!";
		}		
	}	

	/* This Function is for reading data from the file */
	string readFromFile(circularNode<T> *tNode, int lineNum){
		string NameOfFile = createFileName(tNode->machineID);
		string tempStr;
		
		char* fname = new char [NameOfFile.length()];
		for (int i=0; i<NameOfFile.length(); i++)
			fname[i] = NameOfFile[i];
		
		ifstream readData;
		readData.open(fname);
		for (int i=0; i < lineNum; i++)
			getline (readData, tempStr);	
		readData.close();
		
		return tempStr;
	}


	//	This function is used to calculate next active machine
	
	int getNextActiveMachinePos (const int& Machine_ID){
		if (isEmpty()){
			return -1;
		}

		sortMachines();		
		node = head;
		
		if (Machine_ID == 0 && node->machineID >=0)
			return 0;
			
		int count = 0;
		do {
			if (Machine_ID > node->machineID && Machine_ID <= node->next->machineID){
				return count+1;
			}
			node = node->next;
			count++;
		} while (node->next != head);
		
		return 0;
	}

	
	/* This Function is used to sort cll according to machines id */
	
	void sortMachines () {
		int tempNum;	
		node = head;	
		do {
			temp = node->next;				
			do {

				if (node->machineID > temp->machineID){
					tempNum = node->machineID;
					node->machineID = temp->machineID;
					temp->machineID = tempNum;
				}				

				temp = temp->next;
			} while (temp != head);

			node = node->next;
		} while (node->next != head);
	}
	
	
	/* This return true if given machine is active */
	
	bool checkMachineActivation (int mID){
		node = head;
		do {
			if (node->machineID == mID)
				return 1;			
			node = node->next;
		} while (node != head);
		
		return 0;
	}


	// This function will display cll
	
	void displayList()
	{
		if (isEmpty())
		{
			cout << "The list is empty..." << endl;
		}
		
		if (head != NULL)
		{
			cout << head->machineID << " ";
			node = head->next;
			while (node != head)
			{
				cout << node->machineID << " ";
				node = node->next;
			}
			cout << endl;
		}
	}
	
	
	// search specific machine by id
	
	bool find(T id)
	{
		node = head;
		for (int i = 0;i < getSize();i++)
		{
			if (node->machineID == id)
			{
				cout << "FOUND" << endl;
				return true;
			}
			node = node->next;
		}
		cout << "NOT FOUND" << endl;
		return false;
	}
	
	
	// This Function deletes machine node from the end of list
	// Also decrease current size (size) value

	void deleteNode()
	{
		if (!isEmpty())
		{
			if (head->next == head)
			{
				temp = head;
				head = NULL;
				delete temp;
				size--;
				return;
			}
			else
			{
				node = head;
				while (node->next->next != head)
				{
					node = node->next;
				}
				temp = node->next;
				node->next = head;
				delete temp;
				size--;
				return;
			}
		}
	}
	
	
	// This deletes machines by using indexes

	void deleteNodeAtIndex(int ind)
	{
		if (!isEmpty() && ind <= getSize() - 1)
		{
			if (head->next == head && ind == 0)
			{
				temp = head;
				head = NULL;
				delete temp;
				size--;
			}
			else if (ind == 0)
			{
				temp = head;
				node = head;
				while (node->next != head)
				{
					node = node->next;
				}
				node->next = head->next;
				head = head->next;
				delete temp;
				size--;
			}
			else if (ind < size - 1)
			{
				int index = 0;
				node = head;
				for (int i = 0;i < getSize() - 1;index++)
				{
					if (index == ind - 1)
					{
						temp = node->next;
						node->next = node->next->next;
						delete temp;
						size--;
						return;
					}
					node = node->next;
				}
			}
			else if (ind == getSize() - 1)
			{
				node = head;
				while (node->next->next != head)
				{
					node = node->next;
				}
				temp = node->next;
				node->next = head;
				delete temp;
				size--;
			}
		}
	}

	/* ::::::::::::: This Function is used to get starting Information ::::::::::::: */ 
	void input_CLL () {
		int active_Machines;
		int machine_ID;
		bool sameMachineExistenceCheck;
		int optionFlag;
		do {
			cout<<"Enter Total Active Machines : "; cin>>active_Machines;
			if (active_Machines < 0 || active_Machines > totalMachines){
				cout<<"---------------- --------------"<<endl;
				cout<<"Active Machines Value is not In Range !!!"<<endl;	
			} 
		} while (active_Machines < 0 || active_Machines > totalMachines);	
		setActiveMachines(active_Machines);	

		//Taking option from the user to manually assign IDs to each machine
		cout << "Press 1 to assign IDs to machines manually or Press 2 to let the IDs be assigned automatically" << endl;
		do 
		{
			cin >> optionFlag;
			if (optionFlag != 1 && optionFlag != 2)
				cout << "You have selected an incorrected option, press the appropriate key" << endl;
		} while (optionFlag != 1 && optionFlag != 2);
				
		
		if (optionFlag == 1){
			int* tempArray = new int [activeMachines];
			for (int i=0; i<activeMachines; i++)
				tempArray[i] = -1;
				
			for (int i=0; i<activeMachines; i++){
				do{
					do {
						cout<<"Enter Machine "<<"["<<i+1<<"] ID : "; cin>>machine_ID;
						if (machine_ID < 0 || machine_ID >= totalMachines){
							cout<<"||||||||||||||| |||||||||||||||||"<<endl;
							cout<<"Machine ID is not in Range !!!"<<endl;
						}
					} while (machine_ID < 0 || machine_ID > totalMachines);
					
					sameMachineExistenceCheck = 0;
					for (int j=0; j<activeMachines; j++){
						if (machine_ID == tempArray[j]){
							sameMachineExistenceCheck = 1;
							j = activeMachines + 2;
						}					
					}
	
					if (sameMachineExistenceCheck)
						cout<<"Machine With Same ID Already Exist !!!!"<<endl;	
					
				} while (sameMachineExistenceCheck);
				tempArray[i] = machine_ID;
				insert(machine_ID);
			}
		}
	
		else if (optionFlag == 2){
			int* tempArray = new int [activeMachines];
			for (int i=0; i<activeMachines; i++)
				tempArray[i] = -1;
				
			for (int i=0; i<activeMachines; i++){
				do{

					machine_ID = rand()%totalMachines;					

					sameMachineExistenceCheck = 0;
					for (int j=0; j<activeMachines; j++){
						if (machine_ID == tempArray[j]){
							sameMachineExistenceCheck = 1;
							j = activeMachines + 2;
						}					
					}
	
					if (sameMachineExistenceCheck)
						cout<<"Machine With Same ID Already Exist !!!!"<<endl;	
					
				} while (sameMachineExistenceCheck);
				tempArray[i] = machine_ID;
				insert(machine_ID);
			}

		}

		createFingerTable(maxSize);
		sortMachines();	
	}
	
	/* ||||||||||||||||||||||| Function that add data ||||||||||||||||||||||| */
	
	void write_data () {
		string str_data;
		int mID;
		char tempChar;
		system("cls");
		cin.ignore();
		cout<<"Enter Data : "; getline(cin, str_data);
		
		unsigned int data_key = rand();
//		data_key = hash<string>{}(str_data);
		data_key %= totalMachines;
		cout<<"Data Key = "<<data_key<<endl;
		addData_CLL(data_key, str_data);		
	}
	
	void createFingerTable(int bits)
	{
		node = head;
		for (int i = 0;i < getSize();i++)
		{
			int power = 0;
			for (int j = 1;j <= bits;j++)
			{
				int value = node->machineID + pow(2, power);
				if (value >= pow(2, bits)) {
					value %= int(pow(2, bits));
				}

				node->DL_RoutingTable.insertNode(value);
				power++;
			}

			node = node->next;
		}

		createRoutingTable();
	}

	void createRoutingTable () {
		int newValue = 0;
		int newValue2 = 0;
		int currentValue;
		circularNode<T> *tempNode;
		tempNode = head;

		for (int i=0; i<getSize(); i++){

			for (int i=0; i<tempNode->DL_RoutingTable.getSize(); i++){
				currentValue = tempNode->DL_RoutingTable.getValueAt(i);
				newValue = getNextActiveMachinePos(currentValue);
				newValue2 = getMachineAt(newValue);
				tempNode->DL_RoutingTable.setValueAt(i, newValue2);	
			}
			tempNode = tempNode->next;
		}
	}

	void displayFingerTable()
	{
		node = head;
		do{
			node->DL_RoutingTable.displayList();
			node = node->next;
		} while (node != head);
	}


	void displayRoutingTableByID (int ID){
		if (checkMachineActivation(ID)){
			circularNode<T> *tempNode = head;
			int machinePosition = getNextActiveMachinePos(ID);
			for (int i=0; i<machinePosition; i++)
				tempNode = tempNode->next;	
			cout<<"Routing Table of Machine ID = "<<ID<<endl;
			tempNode->DL_RoutingTable.displayList();
		}

		else {
			cout<<"Machine is not active !!!"<<endl;
		}		
	}

	void clearRoutingTable () {
		node = head;
		for (int i=0; i<getSize();i++){
			node->DL_RoutingTable.clearList_DLL();
			node=node->next;
		}
	}

	void printSpecificAVL_Tree (int id) {
		int activationCheck = checkMachineActivation(id);
		if (activationCheck == 0){
			cout<<"Machine Is Not Active !!!"<<endl;
			return;
		} 
		
		else {
			int machinePos = getNextActiveMachinePos(id);
			node = head;
			for (int i=0; i<machinePos; i++)
				node = node->next;
			
			cout<<":::::::::::: AVL Tree Of ID = "<<id<<" :::::::::::: "<<endl;				
			node->AVLT_Data.inorder_display();
		}
	}

	bool addNewMachine (int newMachineID) {
		bool checkMachinePresence = checkMachineActivation(newMachineID);
		if (checkMachinePresence == 1){
			cout<<"Machine With Same ID Already Present !!!\nMachine Not Added !!!"<<endl;
			return 0;
		}
		
		else {
			save_AVLT_Data();
			activeMachines++;
			insert(newMachineID);
			sortMachines();
			clearRoutingTable();
			createFingerTable(maxSize);
			return 1;	
		}		
	}

	bool removeMachine (int delMachineID) {
		bool checkMachinePresence = checkMachineActivation(delMachineID);
		if (checkMachinePresence == 0){
			cout<<"Machine With ID Is Not Active !!!\nMachine Not Removed !!!"<<endl;
			return 0;
		}
		
		else {
			int machinePos = getNextActiveMachinePos(delMachineID);
			deleteNodeAtIndex(machinePos);
			activeMachines--;
			sortMachines();
			clearRoutingTable();
			createFingerTable(maxSize);
			return 1;	
		}		
	}

	void save_AVLT_Data () {
		string str="";	
		ofstream writeAVLTData;
		writeAVLTData.open("AVL_Tree_Data.txt");

		node = head;
		do {
			writeAVLTData<<node->AVLT_Data.rootNode->machineID<<endl;
			node = node->next;
		} while (node != head);

		writeAVLTData.close();	
	}

	// Destructor
	~circularLinkedList(){
		
	}
};

void block() {
	char temp; 
	cout<<"\n::::::::::::::::::::::::::::::::::\nPress Any Key To Continue ... "; temp = getchar();
	cin.ignore();
}

int main()
{
	srand(time(NULL));
	int numberOfIdentifiers;
	cout<<"Enter Size : "; cin>>numberOfIdentifiers;
	circularLinkedList<int> cll(numberOfIdentifiers);
	cll.input_CLL();
	
	int choice = 0;	
	int randomVariable;
	do {
		system("cls");
		cout<<"1- Add Data"<<endl;
		cout<<"2- Remove Data"<<endl;
		cout<<"3- Display Circular List Data"<<endl;
		cout<<"4- Display Doubly Link Data"<<endl;
		cout<<"5- Display AVL Tree Data"<<endl;
		cout<<"6- Read Data"<<endl;
		cout<<"7- Display Routing Tables of Specific Machine "<<endl;
		cout<<"8- Add New Machine "<<endl;
		cout<<"9- Remove Machine "<<endl;
		cout<<"10- Print AVL Tree of Specific Machine"<<endl;
		cout<<endl;
		cout<<"Choice :- "; cin>>choice;
		
		if (choice == 1) {
			system("cls");
			cll.write_data();
			block();			
		}
		
		else if (choice == 2) {
			system("cls");
			cout<<"Enter Machine ID : "; cin>>randomVariable;
			bool check1 = cll.removeData_CLL(randomVariable);
			if (check1)	
				cout<<"\nData Removed Successfully"<<endl;
			else 
				cout<<"\nData Removation Failed !!!!"<<endl;	

			block();	
		}
		
		else if (choice == 3) {
			system("cls");
			cll.displayList();
			block();
		}

		else if (choice == 4) {
			system("cls");
			cll.displayFingerTable();
			block();			
		}

		else if (choice == 5) {
			system("cls");
			cll.displayTree();
			block();
		}

		else if (choice == 6) {
			system("cls");
			cout<<"Enter Machine ID From Which You Want To Start Searching : "; cin>>choice;
			cout<<"Enter Machine ID Whose Data You Want To Read : "; cin>>randomVariable;
			cout<<"Data Reterived : "<<cll.readData_CLL(choice, randomVariable);
			choice = 6; // restoring choice value;
			block();
		}

		else if (choice == 7) {
			system("cls");
			cout<<"Enter Machine ID : "; cin>>randomVariable;
			cll.displayRoutingTableByID(randomVariable);
			block();
		}

		else if (choice == 8) {
			system("cls");
			cout<<"Enter New Machine ID : "; cin>>randomVariable;
			bool check = cll.addNewMachine(randomVariable);
			if (check)
				cout<<"New Machine Successfully Added "<<endl;	
			else 
				cout<<"New Machine Not Added"<<endl;
			block();
		}

		else if (choice == 9) {
			system("cls");
			cout<<"Enter Machine ID : "; cin>>randomVariable;
			bool check = cll.removeMachine(randomVariable);
			if (check)
				cout<<"Machine Successfully Removed "<<endl;	
			else 
				cout<<"Machine Not Deleted"<<endl;
			block();
		}

		else if (choice == 10) {
			system("cls");
			cout<<"Enter Machine ID : "; cin>>randomVariable;
			cll.printSpecificAVL_Tree(randomVariable); 
			block();
		}
		
	} while (choice != 0);	
	return 0;
}
