#include <iostream>
#include <string>
using namespace std;

struct Node { //structure of node
	int data;
	Node* linked;
};

struct Actual { //structure of actual node
	Node* prev = nullptr;
	Node* next = nullptr;
	Node* actual = nullptr;
};

Node* XOR(Node* a, Node* b) {
	return (Node*)((uintptr_t)(a) ^ (uintptr_t)(b)); //return xor of adresses
}

void AddBeg(Node** head, Node** tail, Actual* actual, int data, bool isTail) {
	Node* temp = new Node;
	temp->data = data;
	temp->linked = XOR(nullptr, *head); //create new node

	if (actual->actual == nullptr) actual->actual = temp; //if list is empty then set the actual

	if ((*tail) == nullptr) (*tail) = temp; //and tail

	if ((*head) != nullptr) {
		if (!isTail) {
			if (actual->prev == nullptr) actual->prev = temp; //if adding head then set actual->prev 
		}
		else {
			if (actual->next == nullptr) actual->next = temp; //if adding tail then set actual->next
		}
		

		(*head)->linked = XOR(temp, (*head)->linked); //reconnect the list
	}

	(*head) = temp; //set new node
}

void Next(Node** head, Node** tail, Actual* actual, bool write) {
	if (actual->actual == nullptr) cout << "NULL" << endl; //if empty list cout null
	else if ((actual->actual == *head) && (actual->actual == *tail)) cout << actual->actual->data << endl; //if actual is the only node cout it
	else if (actual->actual == *tail) {
		actual->prev = nullptr; //if head then shift actual to tail
		actual->actual = *head;
		actual->next = XOR(NULL, actual->actual->linked);
		if(write) cout << actual->actual->data << endl;
	}
	else {
		actual->prev = actual->actual; //else just shift the actual
		actual->actual = actual->next;
		actual->next = XOR(actual->prev, actual->actual->linked);
		if(write) cout << actual->actual->data << endl;
	}
}

void Prev(Node** head, Node** tail, Actual* actual, bool write) {
	if (actual->actual == nullptr) cout << "NULL" << endl; //if empty list cout null
	else if ((actual->prev == nullptr) && (actual->next == nullptr)) cout << actual->actual->data << endl; //if actual is the only node cout it
	else if (actual->actual == *head) {
		actual->next = nullptr; //if head then shift actual to tail
		actual->actual = *tail;
		actual->prev = XOR(actual->actual->linked, NULL);
		if(write) cout << actual->actual->data << endl;
	}
	else {
		actual->next = actual->actual; //else just shift the actual
		actual->actual = actual->prev;
		actual->prev = XOR(actual->next, actual->actual->linked);
		if(write) cout << actual->actual->data << endl;
	}
}

void AddAct(Node** head, Node** tail, Actual* actual, int data) {
	Node* temp = new Node;
	temp->data = data;
	temp->linked = XOR(actual->prev, actual->actual);

	if (actual->actual == nullptr) {
		actual->actual = temp; //if empty list set actual
	}
	else if (actual->actual == (*head)) {
		AddBeg(head, tail, actual, data, 0); // if actual is head then just add head
	}
	else {
		actual->prev->linked = XOR(XOR(actual->prev->linked, actual->actual), temp); //add new element and reconnect the list (prev of prev and new node)
		actual->prev = temp;
		actual->actual->linked = XOR(actual->prev, actual->next);
	}
	
	if ((*head) == nullptr) { //if empty list then set head
		(*head) = temp;
	}

	if ((*tail) == nullptr) { //if empty list then set tail
		(*tail) = temp;
	}

}

void DelBeg(Node** head, Node** tail, Actual* actual, bool isTail) {
	if ((*head) == nullptr) return; //if empty list then do nothing
	else if ((*head) == (*tail)){ //if only one element then clear the list
		Node* toDelete = (*head);
		delete toDelete;
		(*head) = nullptr;
		(*tail) = nullptr;
		actual->actual = nullptr;
		actual->prev = nullptr;
		actual->next = nullptr;
	}
	else {
		if (!isTail) { //if deleteing head
			if ((actual->actual) == (*head)) Prev(head, tail, actual, 0); //shift actual to tail
			else if ((actual->prev) == (*head)) actual->prev = nullptr; //clear the prev if it doesnt exist
		}
		else { //if deleting tail
			if ((actual->actual) == (*head)) Prev(tail, head, actual, 0); //shift back actual
			else if ((actual->next) == (*head)) actual->next = nullptr; //clear next if it doesnt exist
		}
		
		Node* next = XOR(nullptr, (*head)->linked);
		next->linked = XOR(nullptr, XOR((*head), next->linked)); //reconnext the list (next of next and null)
		Node* toDelete = (*head);
		(*head) = next; //shift the head
		delete toDelete; //delete the node
	}
}

void DelAct(Node** head, Node** tail, Actual* actual) {
	if ((actual->actual) == nullptr) return; //if empty list then do nothing
	else if ((actual->actual) == (*head)) {
		DelBeg(head, tail, actual, 0); //if actual is the head, delete head
	}
	else if ((actual->actual) == (*tail)) {
		DelBeg(tail, head, actual, 1); //if actual is the tail, delete tail
	}
	else {
		Node* toDelete = actual->actual;
		actual->prev->linked = XOR(XOR(actual->prev->linked, actual->actual), actual->next); //reconnect the list (prev of prev and next)
		actual->next->linked = XOR(actual->prev, XOR(actual->next->linked, actual->actual)); //next of next and prev
		Prev(head, tail, actual, 0); //shift back actual
		delete toDelete; //delete the node
	}
}

void DelVal(Node** head, Node** tail, Actual* actual, int data) {
	Node* curr = (*head); //initialization
	Node* prev = nullptr;
	Node* next = nullptr;

	while (curr != nullptr) {
		if ((curr->data) == data) {
			if (curr == (*head)) {
				next = XOR(nullptr, (*head)->linked); //iteration
				DelBeg(head, tail, actual, 0); //delete the head
				curr = (*head);
			}
			else if (curr == (*tail)) {
				DelBeg(tail, head, actual, 1); //delete the tail
				break; //break (tail was the last node)
			}
			else if (curr == (actual->actual)) {
				next = XOR(prev, curr->linked); //iteration
				prev = XOR(curr->linked, next);
				Node* toDelete = actual->actual;
				Node* temp1 = XOR(actual->prev->linked, toDelete); // save prev of prev
				Node* temp2 = XOR(actual->next->linked, toDelete); // save next of next
				actual->actual = XOR(toDelete->linked, actual->next); //shift actual
				actual->prev = temp1;
				actual->actual->linked = XOR(temp1, actual->next); //reconnect the list
				actual->next->linked = XOR(actual->actual, temp2);
				delete toDelete; // delete the node with given value
				curr = next;
			}
			else if (curr == (actual->prev)) {
				next = XOR(prev, curr->linked); //iteration
				prev = XOR(curr->linked, next);
				Node* toDelete = actual->prev;
				Node* temp1 = XOR(toDelete->linked, actual->actual); // save prev of prev
				Node* temp2 = XOR(temp1->linked, toDelete); //save prev of (prev of prev)
				actual->prev = temp1; //shift actual
				actual->prev->linked = XOR(temp2, actual->actual); //reconnect the list
				actual->actual->linked = XOR(actual->prev, actual->next);
				delete toDelete; //delete the node with given value
				curr = next;
			}
			else if (curr == (actual->next)) {
				next = XOR(prev, curr->linked);
				prev = XOR(curr->linked, next);
				Node* toDelete = actual->next;
				Node* temp1 = XOR(actual->actual, toDelete->linked); // save next of next
				Node* temp2 = XOR(temp1->linked, toDelete); //save next of (next of next)
				actual->next = temp1; //shift actual
				actual->next->linked = XOR(temp2, actual->actual); //reconnect the list
				actual->actual->linked = XOR(actual->prev, actual->next);
				delete toDelete; //delete the node with given value
				curr = next;
			}
			else {
				next = XOR(prev, curr->linked); //iteration
				prev = XOR(curr->linked, next);
				Node* toDelete = curr;
				Node* temp1 = XOR(curr, next->linked); // save prev of prev
				Node* temp2 = XOR(curr, prev->linked); // save next of next
				prev->linked = XOR(temp1, next); //reconnect the list
				next->linked = XOR(prev, temp2);
				delete toDelete; //delete the node with given value
				curr = next;
			}
		}
		else {
			next = XOR(prev, curr->linked); //iteration
			prev = curr;
			curr = next;
		}
		
	}
}

void PrintForward(Node* head) {
	Node* curr = head; //init
	Node* prev = nullptr;
	Node* next;
	if (curr == nullptr) { //empty list == null
		cout << "NULL" << endl;
		return;
	}
	while (curr != nullptr) { //cout node and iterate
		cout << curr->data << " ";
		next = XOR(prev, curr -> linked);
		prev = curr;
		curr = next;
	}
	cout << endl;
}

enum commands { 
	ACTUAL,
	NEXT,
	PREV,
	ADD_BEG,
	ADD_END,
	ADD_ACT,
	DEL_BEG,
	DEL_END,
	DEL_VAL,
	DEL_ACT,
	PRINT_FORWARD,
	PRINT_BACKWARD,
	UNKNOWN

};

commands FindCommand(string* s) {
	if (*s == "ACTUAL") return ACTUAL;
	if (*s == "NEXT") return NEXT;
	if (*s == "PREV") return PREV;
	if (*s == "ADD_BEG") return ADD_BEG;
	if (*s == "ADD_END") return ADD_END;
	if (*s == "ADD_ACT") return ADD_ACT;
	if (*s == "DEL_BEG") return DEL_BEG;
	if (*s == "DEL_END") return DEL_END;
	if (*s == "DEL_VAL") return DEL_VAL;
	if (*s == "DEL_ACT") return DEL_ACT;
	if (*s == "PRINT_FORWARD") return PRINT_FORWARD;
	if (*s == "PRINT_BACKWARD") return PRINT_BACKWARD;
	return UNKNOWN;
}

int main() {
	ios_base::sync_with_stdio(0); 
	cin.tie(0);

	Node* head = nullptr; //init
	Node* tail = nullptr;
	Actual actual;
	int value;
	string command;

	while (cin >> command) {
		switch (FindCommand(&command)) {
		case ACTUAL:
			if (actual.actual != nullptr) cout << actual.actual->data << endl;
			else cout << "NULL" << endl;
			break;
		case NEXT:
			Next(&head, &tail, &actual, 1);
			break;
		case PREV:
			Prev(&head, &tail, &actual, 1);
			break;
		case ADD_BEG:
			cin >> value;
			AddBeg(&head, &tail, &actual, value, 0);
			break;
		case ADD_END:
			cin >> value;
			AddBeg(&tail, &head, &actual, value, 1);
			break;
		case ADD_ACT:
			cin >> value;
			AddAct(&head, &tail, &actual, value);
			break;
		case DEL_BEG:
			DelBeg(&head, &tail, &actual, 0);
			break;
		case DEL_END:
			DelBeg(&tail, &head, &actual, 1);
			break;
		case DEL_VAL:
			cin >> value;
			DelVal(&head, &tail, &actual, value);
			break;
		case DEL_ACT:
			DelAct(&head, &tail, &actual);
			break;
		case PRINT_FORWARD:
			PrintForward(head);
			break;
		case PRINT_BACKWARD:
			PrintForward(tail);
			break;
		default:
			cout << "UNKNOWN COMMAND" << endl;
			break;
		}
	}
	
	return 0;
}