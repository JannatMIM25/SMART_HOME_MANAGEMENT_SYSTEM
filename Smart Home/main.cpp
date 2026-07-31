#define _WIN32_WINNT 0x0A00
#include <iostream>
#include <string>
#include <fstream>
#include "httplib.h"
#include <cstdio>

using namespace std;
using namespace httplib;
int Stacksize = 0;

// -------- Device Node --------
class Device
{
public:
    int id;
    string name;

    Device *left, *right;

    Device(int i, string n)
    {
        id = i;
        name = n;
        left = right = NULL;
    }
};

// -------- Stack Node --------
class StackNode
{
public:
    int id;
    StackNode *next;

    StackNode(int i)
    {
        id = i;
        next = NULL;
    }
};

// -------- Queue Node --------
class QueueNode
{
public:
    int id;
    QueueNode *next;

    QueueNode(int i)
    {
        id = i;
        next = NULL;
    }
};

// -------- Linked List Node --------
class ListNode
{
public:
    int id;
    string name;
    ListNode *next;

    ListNode(int i, string n)
    {
        id = i;
        name = n;
        next = NULL;
    }
};

// -------- Smart Home --------
class SmartHome
{

public:
    Device *root;

    StackNode *top;

    QueueNode *front, *rear;

    ListNode *head;

    // Sorted Array
    Device *arr[102];
    int size;

    SmartHome()
    {
        root = NULL;

        top = NULL;

        front = rear = NULL;

        head = NULL;

        size = 0;
    }

    // -------- BST Search --------

    bool search(Device *root, int id)
    {
        if (root == NULL)
            return false;

        else if (root->id == id)
            return true;

        else if (id < root->id)
            return search(root->left, id);
        else

            return search(root->right, id);
    }

    // -------- BST Insert --------

    Device *insert(Device *root, int id, string name)
    {
        if (root == NULL)
            return new Device(id, name);

        if (id < root->id)
            root->left = insert(root->left, id, name);

        else if (id > root->id)
            root->right = insert(root->right, id, name);

        return root;
    }

    // -------- Stack --------

    void push(int id)
    {
        StackNode *newNode = new StackNode(id);
        if (top == NULL)
        {
            top = newNode;
            return;
        }

        newNode->next = top;

        top = newNode;
    }

    // -------- Queue --------

    void enqueue(int id)
    {
        QueueNode *newNode = new QueueNode(id);

        if (front == NULL)
        {
            front = rear = newNode;
        }

        else
        {
            rear->next = newNode;
            rear = newNode;
        }
    }

    // -------- Linked List --------

    void addToLinkedList(int id, string name)
    {
        ListNode *newNode = new ListNode(id, name);

        if (head == NULL)
        {
            head = newNode;
        }

        else
        {
            ListNode *temp = head;

            while (temp->next != NULL)
                temp = temp->next;

            temp->next = newNode;
        }
    }

    // -------- Sorted Array --------

    void insertionSort()
    {
        for (int i = 2; i <= size; i++)
        {
            Device *key = arr[i];

            int j = i - 1;

            while (j >= 1 && arr[j]->id > key->id)
            {
                arr[j + 1] = arr[j];
                j--;
            }

            arr[j + 1] = key;
        }
    }

    void addToArray(Device *d)
    {
        if (size < 100)
        {

            arr[++size] = d; // insert first
            insertionSort(); // then sort
        }
    }

    // -------- File Duplicate Check --------

    bool checkDuplicate(int id)
    {
        ifstream file("devices.txt");

        int fileId;

        string name, type;

        while (file >> fileId)
        {
            file.ignore();

            getline(file, name, ',');

            getline(file, type);

            if (fileId == id)
            {
                file.close();

                return true;
            }
        }

        file.close();

        return false;
    }

    // -------- Save File --------

    void saveToFile(int id, string name, string type)
    {
        ofstream file("devices.txt", ios::app);

        file << id << "," << name << "," << type << endl;

        file.close();
    }

    // -------- Load File --------
    void loadFromFile()
    {
        ifstream file("devices.txt");

        if (!file.is_open())
            return;

        int id;
        string name, type;

        while (file >> id)
        {
            file.ignore();
            getline(file, name, ',');
            getline(file, type);

            // Strip trailing \r or whitespace from Windows line endings
            while (!type.empty() && (type.back() == '\r' || type.back() == '\n' || type.back() == ' '))
                type.pop_back();

            while (!name.empty() && (name.back() == '\r' || name.back() == '\n'))
                name.pop_back();

            Device *newDevice = new Device(id, name);

            if (type == "tree")
                root = insert(root, id, name);
            else if (type == "stack")
                push(id);
            else if (type == "queue")
                enqueue(id);
            else if (type == "list")
                addToLinkedList(id, name);

            addToArray(newDevice);
        }

        file.close();
    }

    // -------- Add Device --------

    string addDevice(int id, string name, string type)
    {

        // File based duplicate check

        if (checkDuplicate(id))
        {
            return "Device Added Failed!\nID already exists.";
        }

        Device *newDevice = new Device(id, name);

        if (type == "tree")
        {
            root = insert(root, id, name);
        }

        else if (type == "stack")
        {
            push(id);
        }

        else if (type == "queue")
        {
            enqueue(id);
        }

        else if (type == "list")
        {
            addToLinkedList(id, name);
        }

        // Sorted Array

        addToArray(newDevice);

        // Save permanently

        saveToFile(id, name, type);

        return "Device Added Successfully!\n\nDevice ID: " + to_string(id) + "\nDevice Name: " + name + "\nStored In: " + type;
    }

    // Updated File After any operation
    void updateFileAfterDelete(int id)
    {
        ifstream file("devices.txt");
        ofstream updated("updated.txt");

        int fileId;
        string name, type;

        while (file >> fileId)
        {
            file.ignore();

            getline(file, name, ',');
            getline(file, type);

            if (fileId != id)
            {
                updated << fileId << ","
                        << name << ","
                        << type << endl;
            }
        }

        file.close();
        updated.close();

        remove("devices.txt");
        rename("updated.txt", "devices.txt");
    }
    void saveActivity(string action, string details)
    {
        ofstream file("activity.txt", ios::app);

        file << action << "|" << details << endl;

        file.close();
    }
};
// delete.cpp functions
bool Delete_From_Linkedlist(ListNode *&head, int id)
{
    bool track1 = false;
    ListNode *temp = head;
    if (temp == NULL)
    {
        return track1;
    }
    if (head->id == id)
    {
        track1 = true;
        ListNode *temp = head;
        head = head->next;
        delete temp;
    }
    else
    {
        while (temp->next && temp->next->id != id)
        {
            temp = temp->next;
        }
        if (temp->next)
        {
            ListNode *del = temp->next;
            track1 = true;
            temp->next = temp->next->next;
            delete del;
        }
    }
    return track1;
}
bool Delete_From_sorted_array(Device *arr[], int &size, int id)
{
    bool track2 = false;

    for (int i = 1; i <= size; i++)
    {
        if (arr[i]->id == id)
        {
            int j = i;
            track2 = true;
            for (j = i; j < size; j++)
            {
                arr[j] = arr[j + 1];
            }
            size = size - 1;
            break;
        }
    }
    return track2;
}
bool Delete_From_Stack(StackNode *&Top)
{
    if (Top == NULL)
    {
        return false;
    }
    StackNode *temp = Top;
    Top = Top->next;
    delete temp;
    return true;
}
bool delete_from_Queue(QueueNode *&front, QueueNode *&rear)
{
    if (front == NULL)
    {
        return false;
    }
    QueueNode *temp = front;
    front = front->next;
    delete temp;
    if (front == NULL)
    {
        rear = NULL;
    }
    return true;
}
Device *delete_from_BST(Device *&root, int id)
{
    if (root == NULL)
    {
        return NULL;
    }
    if (root->id > id)
    {
        root->left = delete_from_BST(root->left, id);
    }
    else if (root->id < id)
    {
        root->right = delete_from_BST(root->right, id);
    }
    else
    {
        if (root->left == NULL && root->right == NULL)
        {
            delete root;
            return NULL;
        }
        else if (root->left == NULL)
        {
            Device *temp = root->right;
            delete root;
            return temp;
        }
        else if (root->right == NULL)
        {
            Device *temp = root->left;
            delete root;
            return temp;
        }
        else
        {
            Device *child = root->right;

            while (child->left)
            {

                child = child->left;
            }
            root->id = child->id;
            root->name = child->name;

            root->right = delete_from_BST(root->right, child->id);
        }
    }
    return root;
}
StackNode *Search_From_Stack(StackNode *top, int id, int &pos)
{
    StackNode *temp = top;

    if (top == NULL)
        return NULL;
    pos = 1;
    while (temp != NULL)
    {
        if (temp->id == id)
        {
            return temp;
        }
        temp = temp->next;
        pos++;
    }
    pos = -1;

    return NULL;
}

QueueNode *Search_from_Queue(QueueNode *front, int id, int &qpos)
{
    if (front == NULL)
        return NULL;
    qpos = 1;
    QueueNode *temp = front;
    while (temp != NULL)
    {
        if (temp->id == id)
            return temp;
        temp = temp->next;
        qpos++;
    }
    qpos = -1;

    return NULL;
}
ListNode *Search_From_Linkedlist(ListNode *&head, int id)
{
    if (head == NULL)
        return NULL;

    ListNode *temp = head;

    if (head->id == id)
    {

        ListNode *temp = head;

        return temp;
    }
    else
    {
        while (temp->next && temp->next->id != id)
        {
            temp = temp->next;
        }
        if (temp->next)
        {
            ListNode *del = temp->next;

            return del;
        }
    }
    return NULL;
}
/*Device *Search_From_sorted_array(Device *arr[], int &size, int id)
{

    for (int i = 1; i <= size; i++)
    {
        if (arr[i]->id == id)
        {
            return arr[i];
        }
    }
    return NULL;
}*/
Device *Search_From_BST(Device *root, int id)
{
    if (root == NULL)
        return NULL;

    else if (root->id == id)
        return root;

    else if (id < root->id)
        return Search_From_BST(root->left, id);
    else

        return Search_From_BST(root->right, id);
}

// -------- Traversal Functions --------

string Display_BST(Device *root)
{
    if (root == NULL)
        return "";

    string result = "";

    result += Display_BST(root->left);

    result += "ID: " + to_string(root->id) + " | Name: " + root->name + "\n";

    result += Display_BST(root->right);

    return result;
}

string Display_Stack(StackNode *top)
{
    string result = "";
    StackNode *temp = top;

    while (temp != NULL)
    {
        result += "ID: " + to_string(temp->id) + "\n";
        temp = temp->next;
    }

    return result;
}

string Display_Queue(QueueNode *front)
{
    string result = "";
    QueueNode *temp = front;

    while (temp != NULL)
    {
        result += "ID: " + to_string(temp->id) + "\n";
        temp = temp->next;
    }

    return result;
}

string Display_LinkedList(ListNode *head)
{
    string result = "";
    ListNode *temp = head;

    while (temp != NULL)
    {
        result += "ID: " + to_string(temp->id) + " | Name: " + temp->name + "\n";
        temp = temp->next;
    }

    return result;
}

string Display_Array(Device *arr[], int size)
{
    string result = "";

    for (int i = 1; i <= size; i++)
    {
        result += "ID: " + to_string(arr[i]->id) + " | Name: " + arr[i]->name + "\n";
    }

    return result;
}
// -------- MAIN --------

int main()
{

    SmartHome home;

    // Load previous devices

    home.loadFromFile();

    Server svr;

    svr.set_mount_point("/", "./www");
    svr.set_file_extension_and_mimetype_mapping("html", "text/html; charset=utf-8");

    // Login

    svr.Post("/login",
             [&](const Request &req, Response &res)
             {
                 string user = req.get_param_value("user");

                 string pass = req.get_param_value("pass");

                 if (user == "admin" && pass == "1234")
                 {
                     res.set_redirect("/dashboard.html");
                 }

                 else
                 {
                     res.set_content(
                         "Wrong Username or Password",
                         "text/plain");
                 }
             });

    // Add Device
    svr.Post("/add",
             [&](const Request &req, Response &res)
             {
                 int id = stoi(req.get_param_value("id"));
                 string name = req.get_param_value("name");
                 string type = req.get_param_value("type");

                 string result = home.addDevice(id, name, type);

                 bool failed = (result.find("Failed") != string::npos);

                 if (failed)
                 {
                     home.saveActivity(
                         "ADD",
                         "Failed to add Device ID: " + to_string(id) +
                             " | Reason: ID already exists");
                 }
                 else
                 {
                     home.saveActivity(
                         "ADD",
                         "Device ID: " + to_string(id) +
                             " | Name: " + name +
                             " | Method: " + type);
                 }

                 res.set_redirect("/activity.html");
             });
    // Delete Device (by ID)

    svr.Post("/delete",
             [&](const Request &req, Response &res)
             {
                 int id = stoi(req.get_param_value("id"));
                 string type = req.get_param_value("method");

                 bool deleted = false;
                 int deleted_id;

                 if (type == "tree")
                 {
                     // check first
                     if (home.search(home.root, id))
                     {
                         deleted_id = id;
                         home.root = delete_from_BST(home.root, id);

                         deleted = true;
                     }
                 }

                 else if (type == "list")
                 {
                     deleted_id = id;
                     deleted = Delete_From_Linkedlist(home.head, id);
                 }

                 else if (type == "array")
                 {
                     deleted_id = id;
                     deleted = Delete_From_sorted_array(home.arr, home.size, id);
                 }

                 if (deleted)
                 {
                     home.updateFileAfterDelete(deleted_id);

                     home.saveActivity(
                         "DELETE",
                         "Device ID: " + to_string(deleted_id) +
                             " removed using " + type);

                     res.set_redirect("/activity.html");
                 }

                 else
                 {
                     home.saveActivity(
                         "DELETE",
                         "Device ID: " + to_string(id) +
                             " not found using " + type);

                     res.set_redirect("/activity.html");
                     ;
                 }
             });

    // Remove From Structure (Stack pop / Queue dequeue)

    svr.Post("/remove-structure",
             [&](const Request &req, Response &res)
             {
                 string type = req.get_param_value("type");

                 bool removed = false;
                 int removed_id = -1;

                 if (type == "stack")
                 {
                     if (home.top != NULL)
                     {
                         removed_id = home.top->id;
                         removed = Delete_From_Stack(home.top);
                     }
                 }

                 else if (type == "queue")
                 {
                     if (home.front != NULL)
                     {
                         removed_id = home.front->id;
                         removed = delete_from_Queue(home.front, home.rear);
                     }
                 }

                 if (removed)
                 {
                     home.updateFileAfterDelete(removed_id);
                     home.saveActivity(
                         "DELETE",
                         "Device ID: " + to_string(removed_id) +
                             " removed using " + type);

                     res.set_redirect("/activity.html");
                 }
                 else
                 {
                     home.saveActivity(
                         "DELETE",
                         "Device ID: " + to_string(removed_id) +
                             " not found using " + type);

                     res.set_redirect("/activity.html");
                 }
             });

    /// Find Device
    /// Find Device
    svr.Get("/find",
            [&](const Request &req, Response &res)
            {
                int id = stoi(req.get_param_value("id"));
                string mode = req.get_param_value("mode"); // "search" or "fast"
                string method = req.get_param_value("method");

                int pos = -1, qpos = -1, apos = -1;
                bool found = false;
                string details = "Device ID: " + to_string(id) + " | Mode: " + mode;

                if (mode == "fast")
                {
                    // Only BST and Array allowed — fastest lookups
                    if (Search_From_BST(home.root, id))
                    {
                        found = true;
                        details += " | Found in Tree (O(log n))";
                    }
                    else
                    {
                        for (int i = 1; i <= home.size; i++)
                        {
                            if (home.arr[i]->id == id)
                            {
                                apos = i;
                                found = true;
                                details += " | Found in Array at pos " + to_string(apos) + " (O(1) direct)";
                                break;
                            }
                        }
                    }
                }
                else // mode == "search" -> use the selected method only
                {
                    if (method == "tree")
                    {
                        found = (Search_From_BST(home.root, id) != NULL);
                        details += " | Method: Tree | " + string(found ? "Found" : "Not Found");
                    }
                    else if (method == "stack")
                    {
                        Search_From_Stack(home.top, id, pos);
                        found = (pos != -1);
                        details += " | Method: Stack | " + string(found ? "Found at pos " + to_string(pos) : "Not Found");
                    }
                    else if (method == "queue")
                    {
                        Search_from_Queue(home.front, id, qpos);
                        found = (qpos != -1);
                        details += " | Method: Queue | " + string(found ? "Found at pos " + to_string(qpos) : "Not Found");
                    }
                    else if (method == "array")
                    {
                        for (int i = 1; i <= home.size; i++)
                        {
                            if (home.arr[i]->id == id)
                            {
                                apos = i;
                                break;
                            }
                        }
                        found = (apos != -1);
                        details += " | Method: Array | " + string(found ? "Found at pos " + to_string(apos) : "Not Found");
                    }
                }

                if (!found)
                    details += " | Result: Not Found";

                home.saveActivity("SEARCH", details);
                res.set_redirect("/activity.html");
            });

    svr.Get("/display",
            [&](const Request &req, Response &res)
            {
                string method = req.get_param_value("method");
                string result = "";

                if (method == "tree")
                    result = Display_BST(home.root);
                else if (method == "stack")
                    result = Display_Stack(home.top);
                else if (method == "queue")
                    result = Display_Queue(home.front);
                else if (method == "list")
                    result = Display_LinkedList(home.head);
                else if (method == "array")
                    result = Display_Array(home.arr, home.size);

                if (result == "")
                    result = "No devices found.";

                string safeResult = "";
                for (char c : result)
                {
                    if (c == '\n')
                        safeResult += "<br>";
                    else
                        safeResult += c;
                }

               home.saveActivity("DISPLAY (" + method + ")", safeResult);
                res.set_redirect("/activity.html");
            });

    // Activity Page Data
    svr.Get("/activity",
            [&](const Request &req, Response &res)
            {
                ifstream file("activity.txt");

                string line;
                string html = "";

                while (getline(file, line))
                {
                    if (line.find("|") == string::npos)
                        continue;

                    size_t pos = line.find("|");

                    string action = line.substr(0, pos);
                    string details = line.substr(pos + 1);

                    string box = "display";

                    if (action == "ADD")
                        box = "add";

                    else if (action == "DELETE")
                        box = "delete";

                    html +=
                        "<div class='activity-box " + box + "'>"
                                                            "<h3>" +
                        action + "</h3>"
                                 "<p class='details'>" +
                        details + "</p>"
                                  "</div>";
                }

                file.close();

                res.set_content(html, "text/html");
            });

    cout << "Server running at http://localhost:8080/index.html\n";

    svr.listen("0.0.0.0", 8080);

    return 0;
}