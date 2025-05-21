#include <iostream>
using namespace std;

struct Elem {
    int val;
    Elem* next;
};

// Function to read a list from input
Elem* readList() {
    Elem *head = nullptr, *tail = nullptr;
    int num;
    while (true) {
        cout << "Enter an element (0-end) > ";
        cin >> num;
        if (num == 0)
            break;
        Elem* newElem = new Elem{num, nullptr};
        if (!head) {
            head = tail = newElem;
        } else {
            tail->next = newElem;
            tail = newElem;
        }
    }
    return head;
}

// Recursive function to append lst2 to lst1
Elem* append(Elem* lst1, Elem* lst2) {
    if (!lst1)
        return lst2;
    lst1->next = append(lst1->next, lst2);
    return lst1;
}

// Print list
void printList(Elem* head) {
    while (head) {
        cout << head->val << " ";
        head = head->next;
    }
    cout << endl;
}

int main() {
    cout << "Entering the list 1...\n";
    Elem* lst1 = readList();

    cout << "\nEntering the list 2...\n";
    Elem* lst2 = readList();

    cout << "\nThe list 1: ";
    printList(lst1);

    cout << "\nThe list 2: ";
    printList(lst2);

    Elem* result = append(lst1, lst2);

    cout << "\nBoth lists appended: ";
    printList(result);

    return 0;
}