#include <bits/stdc++.h>
using namespace std;
const int MAX_USERS = 1000;

class TreeNode
{
public:
    int memberId;
    string spouse;
    string member_name;
    string dob;
    string a_d;
    string gender;
    vector<TreeNode *> children;
    bitset<10> isalive;
    TreeNode(int mId, string spName, string mName, string DoB, char ad, char gendr)
    {
        memberId = mId;
        spouse = spName;
        member_name = mName;
        dob = DoB;
        a_d = ad;
        if (ad == 'A' || ad == 'a')
        {
            a_d = "ALIVE";
        }
        else if (ad == 'D' || ad == 'd')
        {
            a_d = "DEAD";
        }

        if (gendr == 'M' || gendr == 'm')
        {
            gender = "MALE";
        }
        else if (gendr == 'F' || gendr == 'f')
        {
            gender = "FEMALE";
        }
    }
};

class HashTable // creating hash table
{
public:
    int userId;      // unique
    string userName; // name of the user
    string PassWord; // password
    TreeNode *root;

    HashTable()
    {
        userId = -1;
        userName = "";
        PassWord = "";
    }
    HashTable(string uname, string pass) // constructor to initialise class variables
    {
        userName = uname;
        PassWord = pass;
        userId = -1;
        root = nullptr;
    }
    TreeNode *get_root()
    {
        return root;
    }
    int get_userId()
    {
        return userId;
    }
    string get_uname()
    {
        return userName;
    }
    string get_pass()
    {
        return PassWord;
    }
    void set_uname(string usn)
    {
        userName=usn;
    }
    void set_pass(string psw)
    {
        PassWord=psw;
    }
    void set_userId(int key)
    {
        userId = key;
    }
    void erase_all_data()
    {
        for (int i = 0; i < 1000; i++)
        {
            userName = "";
            PassWord = "";
            userId = -1;
        }
    }
};
HashTable user[MAX_USERS];

int HashFunction(string userName, string PassWord)
{
    long int index = 1;
    string concatenate = userName.substr(0, 3) + PassWord.substr(0, 3);
    int sum = 0;
    for (int i = 0; i < concatenate.size(); i++)
    {
        sum = sum + int(concatenate[i]);
    }
    for (int i = 0; i < concatenate.size(); i++)
    {
        index = (index * sum) % MAX_USERS;
    }
    return index;
}
int member_ctr = 0;

void SaveFamilyTree(TreeNode *currentNode, ofstream &outFile) {
    if (currentNode) {
        outFile << currentNode->memberId << " " << currentNode->member_name << " "
                << currentNode->dob << " " << currentNode->spouse << " " << currentNode->gender << " "
                << currentNode->a_d << " " << currentNode->isalive << endl;

        for (TreeNode *child : currentNode->children) {
            SaveFamilyTree(child, outFile);
        }
    }
}


void SaveUserDetails(HashTable& user,ofstream &userFile) 
{
    if (userFile.is_open()) {
        // Write the user details to the file
        userFile << user.get_uname() << " " << user.get_pass() << "\n";

        // Close the file
        userFile.close();
        cout << "User details saved successfully!\n";
    } 
    else 
    {
        cout << "Error opening user file for saving.\n";
    }
}

TreeNode *FindFamilyMember(TreeNode *currentNode, const string &name)
{
    if (currentNode)
    {
        if (currentNode->member_name == name)
        {
            return currentNode;
        }

        // Recursively search for the family member in the children
        for (TreeNode *child : currentNode->children)
        {
            TreeNode *foundMember = FindFamilyMember(child, name);
            if (foundMember)
            {
                return foundMember;
            }
        }
    }
    return nullptr;
    // Family member not found
}


bool LoadUserDetails(HashTable users[]) {
    ifstream userFile("users.txt");

    if (!userFile) {
        cout << "Error opening user file. Please register first." << endl;
        return false;
    }

    while (!userFile.eof()) {
        string username, password;
        userFile >> username >> password;

        int key = HashFunction(username, password);

        users[key].set_uname(username);
        users[key].set_pass(password);
    }

    userFile.close();
    return true;
}

void CreateFamilyTree(TreeNode *root)
{
    if (root)
    {
        system("CLS");
        cout << "A family tree already exists. Creating a new tree will overwrite the current tree." << endl;
        return;
    }
    char confirmation;
    cout << "Proceed (Y/N): ";
    cin >> confirmation;
    if (confirmation != 'Y' && confirmation != 'y')
    {
        return;
    }
    root = nullptr;
    system("CLS");
    cout << "New family tree created." << endl;
}

void AddMemberToTree(HashTable &h)
{
    if (!h.get_root())
    {
        string memberName;
        cout << "Enter the name of the new family member: ";
        cin >> memberName;

        string dob;
        cout << "Enter the date of birth (DOB) for " << memberName << ": ";
        cin >> dob;

        char g;
        cout << "ENTER THE GENDER OF MEMBER (M/F): ";
        cin >> g;

        char ad;
        cout << "IS THE MEMBER ALIVE OR DEAD(A/D): ";
        cin >> ad;

        string Spouse;
        cout << "Enter the name of " << memberName << "'s spouse: ";
        cin >> Spouse;

        TreeNode *newMember = new TreeNode(member_ctr++, Spouse, memberName, dob, ad, g);
        h.root = newMember;
        system("CLS");
        cout << memberName << " has been added to the family tree as the root." << endl;
    }
    else
    {
        cout << "Enter the name of the parent family member: ";
        string parentName;
        cin.ignore();
        getline(cin, parentName);

        TreeNode *parent = FindFamilyMember(h.root, parentName);

        if (parent)
        {
            cout << "Enter the name of the new family member: ";
            string memberName;
            getline(cin, memberName);

            cout << "Enter the date of birth (DOB) for " << memberName << ": ";
            string dob;
            getline(cin, dob);

            cout << "Enter the name of " << memberName << "'s spouse: ";
            string Spouse;
            getline(cin, Spouse);

            char g;
            cout << "ENTER THE GENDER OF MEMBER (M/F): ";
            cin >> g;

            char ad;
            cout << "IS THE MEMBER ALIVE OR DEAD(A/D): ";
            cin >> ad;

            TreeNode *newMember = new TreeNode(member_ctr++, Spouse, memberName, dob, ad, g);

            // Add the new family member as a child of the selected parent
            parent->children.push_back(newMember);
            if (ad = 'A' || ad == 'a')
            {
                int index = parent->children.size() - 1;
                parent->isalive.set(index);
            }
            else if (ad == 'D' || ad == 'd')
            {
            }
            else
            {
                system("CLS");
                cout << "Invalid Input! Re-enter.";
                return;
            }
            system("CLS");
            cout << memberName << " has been added to the family tree as a child of " << parentName << "." << endl;
        }
        else
        {
            system("CLS");
            cout << "Parent family member not found in the tree." << endl;
        }
    }
}

TreeNode *FindParentOfFamilyMember(TreeNode *currentNode, const string &memberName)
{
    if (currentNode)
    {
        // Check if the current node has the member as a child
        for (TreeNode *child : currentNode->children)
        {
            if (child->member_name == memberName)
            {
                return currentNode; // Found the parent
            }
        }

        for (TreeNode *child : currentNode->children)
        {
            TreeNode *parent = FindParentOfFamilyMember(child, memberName);
            if (parent)
            {
                return parent;
            }
        }
    }
    return nullptr;
}

void DisplaySpecificGeneration(TreeNode *currentNode, TreeNode *rootNode, int targetGeneration, int currentGeneration)
{
    if (currentNode)
    {
        if (currentGeneration == targetGeneration && currentNode->memberId == rootNode->memberId)
        {
            // Print the family member if they belong to the target generation
            cout << currentNode->memberId << " " << currentNode->member_name << " (" << currentNode->dob << ") Spouse: " << currentNode->spouse << endl;
        }
        else if (currentGeneration == targetGeneration && currentNode->memberId != rootNode->memberId)
        {
            TreeNode *parent = FindParentOfFamilyMember(rootNode, currentNode->member_name);
            int index = 0;
            for (int i = 0; i < parent->children.size(); i++)
            {
                if (parent->children[i]->memberId == currentNode->memberId)
                {
                    index = i;
                    break;
                }
            }
            if (parent->isalive.test(index))
            {
                cout << currentNode->memberId << " " << currentNode->member_name << " (" << currentNode->dob << ") "
                     << "Alive/Deceased: Alive"
                     << " Spouse: " << currentNode->spouse << endl;
            }
            else
            {
                cout << currentNode->memberId << " " << currentNode->member_name << " (" << currentNode->dob << ") "
                     << "Alive/Deceased: Deceased"
                     << " Spouse: " << currentNode->spouse << endl;
            }
        }

        for (TreeNode *child : currentNode->children)
        {
            DisplaySpecificGeneration(child, rootNode, targetGeneration, currentGeneration + 1);
        }
    }
    else
    {
        system("CLS");
        cout << "No members found in entered generation!";
        return;
    }
}

void DisplayFamilyTree(TreeNode* currentNode, TreeNode* rootNode, int depth = 0) {
    if (currentNode) 
    { 
        for (int i = 0; i < depth; i++) //indentation for children nodes
        {
            cout << "  ";
        }
        if (currentNode->memberId==rootNode->memberId)
        { cout << "Member ID:"<<currentNode->memberId<<" "<<"Member Name:"<<currentNode->member_name << " (" << currentNode->dob << ")" <<" "<< "Spouse:"<<currentNode->spouse << endl;}
        else if (currentNode->memberId!=rootNode->memberId)
        {
            TreeNode* parent=FindParentOfFamilyMember(rootNode,currentNode->member_name);
            int index=0;
            for (int i=0; i<parent->children.size(); i++)
            {
                if (parent->children[i]->memberId==currentNode->memberId)
                {
                    index=i;
                    break;
                }
            }
            if (parent->isalive.test(index)) //if alive, print status as alive, else deceased
            {
                cout << "Member ID:"<<currentNode->memberId << " " <<"Member Name:"<< currentNode->member_name << " (" << currentNode->dob << ") " << "Alive/Deceased: Alive" << " Spouse:" << currentNode->spouse << endl;
            }
            else
            {
                cout << "Member ID:"<<currentNode->memberId << " " <<"Member Name:"<<currentNode->member_name << " (" << currentNode->dob << ") " << "Alive/Deceased: Deceased" << " Spouse:" << currentNode->spouse << endl;
            }
        }
        for (TreeNode* child : currentNode->children) 
        {
            DisplayFamilyTree(child,rootNode,depth + 1);
        }
    }
    if(currentNode==nullptr && currentNode==rootNode)
    {
        cout<<""<<endl;
        return;
    }
}


void DisplaybyKeyword(TreeNode* currentNode, TreeNode* rootNode, string keyword)
{
    //if root doesnt exist
    if(currentNode==nullptr && currentNode==rootNode)
    {   
        cout<<"No Family tree found. Please create one!"<<endl;
        return;
    }
    else if (currentNode && currentNode->member_name.find(keyword)!=string::npos && currentNode->memberId==rootNode->memberId) //found keyword at rootnode
    { 
        cout << "Member ID:"<<currentNode->memberId<<" "<<"Member Name:"<<currentNode->member_name << " (" << currentNode->dob << ")" <<" "<< "Spouse:"<<currentNode->spouse << endl;
        return;
    }
    else if (currentNode && currentNode->member_name.find(keyword)!=string::npos && currentNode->memberId!=rootNode->memberId) //found keyword inside the tree
    {
        TreeNode* parent=FindParentOfFamilyMember(rootNode,currentNode->member_name);
        int index=0;
        for (int i=0; i<parent->children.size(); i++)
        {
            if (parent->children[i]->memberId==currentNode->memberId)
            {
                index=i;
                break;
            }
        }
        if (parent->isalive.test(index)) //if alive, print status as alive, else deceased
        {
            cout << "Member ID:"<<currentNode->memberId << " " <<"Member Name:"<< currentNode->member_name << " (" << currentNode->dob << ") " << "Alive/Deceased: Alive" << " Spouse:" << currentNode->spouse << endl;
        }
        else
        {
            cout << "Member ID:"<<currentNode->memberId << " " <<"Member Name:"<<currentNode->member_name << " (" << currentNode->dob << ") " << "Alive/Deceased: Deceased" << " Spouse:" << currentNode->spouse << endl;
        }
    }
    else
    {
        //recursively call for currentNode's children
        for (TreeNode* child : currentNode->children) 
        {
            DisplaybyKeyword(child,rootNode,keyword);
        }
    }
    
}



void DisplaySubFamily(TreeNode * root)
{
    system("CLS");
    cout<<"Enter member of family tree from which subtree is to be displayed:";
    string member;
    cin>>member;
    TreeNode* currentNode=FindFamilyMember(root,member);
    if(currentNode==nullptr)
    {
        cout << "Family Member Not Found" << endl;
    }
    else
    {
        DisplayFamilyTree(currentNode,currentNode);
    }
}


void SearchFamilyMember(TreeNode *root)
{
    system("CLS");
    cout << "Enter the name of the family member you want to search for: ";
    string memberName;
    cin >> memberName;
    // Find the family member in the tree
    TreeNode *member = FindFamilyMember(root, memberName);

    if (member && member->memberId == root->memberId)
    {
        cout << "Family member found:" << endl;
        cout << "Name: " << member->member_name << endl;
        cout << "Date of Birth (DOB): " << member->dob << endl;
        cout << "Spouse Name: " << member->spouse << endl;
        cout << "Gender: " << member->gender << endl;
    }
    else if (member && member->memberId != root->memberId)
    {
        TreeNode *parent = FindParentOfFamilyMember(root, member->member_name);
        cout << "Family member found:" << endl;
        cout << "Name: " << member->member_name << endl;
        cout << "Date of Birth (DOB): " << member->dob << endl;
        cout << "Spouse Name: " << member->spouse << endl;
        cout << "Parent Name: " << parent->member_name << endl;
        cout << "Gender: " << member->gender << endl;
        int index = 0;
        for (int i = 0; i < parent->children.size(); i++)
        {
            if (parent->children[i]->memberId == member->memberId)
            {
                index = i;
                break;
            }
        }
        if (parent->isalive.test(index))
        {
            cout << "Member Alive/Deceased:Alive" << endl;
        }
        else
        {
            cout << "Member Alive/Deceased:Deceased" << endl;
        }
    }
    else
    {
        system("CLS");
        cout << "Family member not found in the tree." << endl;
    }
}

void EditFamilyMemberDetails(TreeNode *root)
{
    system("CLS");
    cout << "Enter the name of the family member whose details you want to edit: ";
    string memberName;
    cin >> memberName;

    // Find the family member in the tree
    TreeNode *member = FindFamilyMember(root, memberName);

    if (member)
    {
        cout << "Editing details for " << memberName << ":" << endl;

        // Prompt the user to update information
        cout << "Name:";
        string newName;
        cin >> newName;
        if (!newName.empty())
        {
            member->member_name = newName;
        }

        cout << "DOB:";
        string newDob;
        cin >> newDob;
        if (!newDob.empty())
        {
            member->dob = newDob;
        }

        cout << "Spouse:";
        string newSpouse;
        cin >> newSpouse;
        if (!newSpouse.empty())
        {
            member->spouse = newSpouse;
        }
        if (root->memberId != member->memberId)
        {
            cout << "Alive/Deceased:";
            string newStatus;
            cin >> newStatus;
            TreeNode *parent = FindParentOfFamilyMember(root, member->member_name);
            int index = 0;
            for (int i = 0; i < parent->children.size(); i++)
            {
                if (parent->children[i]->memberId == member->memberId)
                {
                    index = i;
                    break;
                }
            }
            if ((newStatus == "Deceased" && parent->isalive.test(index)) || (newStatus == "Alive" && !parent->isalive.test(index)))
            {
                parent->isalive.flip(index);
            }
        }

        system("CLS");
        cout << "Details for " << memberName << " have been updated." << endl;
    }
    else
    {
        system("CLS");
        cout << "Family member not found in the tree." << endl;
    }
}

void RemoveFamilyMember(HashTable &h)
{
    system("CLS");
    cout << "Enter the name of the family member you want to remove: ";
    string memberName;
    cin >> memberName;
    if (memberName == h.root->member_name)
    {
        cout << "Do you want to delete the root of the family tree?(Y/N):";
        char choice;
        cin >> choice;
        if (tolower(choice) == 'y')
        {
            TreeNode *temp = h.root;
            delete temp;
            h.root = nullptr;
            system("CLS");
            cout << "The root has been deleted." << endl;
            return;
        }
    }
    // Find the parent family member to remove the child from
    TreeNode *parent = FindParentOfFamilyMember(h.root, memberName);

    if (parent)
    {
        // Find the family member to remove
        TreeNode *toRemove = nullptr;
        for (size_t i = 0; i < parent->children.size(); ++i)
        {
            if (parent->children[i]->member_name == memberName)
            {
                toRemove = parent->children[i];
                parent->children.erase(parent->children.begin() + i);
                break;
            }
        }

        if (toRemove)
        {
            // Remove the family member from the children vector
            delete toRemove;
            system("CLS");
            cout << memberName << " has been removed from the family tree." << endl;
        }
        else
        {
            system("CLS");
            cout << "Family member not found in the children of " << parent->member_name << "." << endl;
        }
    }
    else
    {
        system("CLS");
        cout << "Family member not found in the tree." << endl;
    }
}

void insertUser(int userId, string username, string password, TreeNode *root, HashTable ht[])
{
    ht[userId].userId = userId;
    ht[userId].userName = username;
    ht[userId].PassWord = password;
    ht[userId].root = root;
}




bool validate(string &username, string &password, int key)
{
    bool flag2 = false;
    bool traversed = false;
    if (user[key].get_pass() == password && user[key].get_uname() == username && !traversed)
    {
        system("CLS");
        cout << "Logged In Successfully! \n";
        flag2 = true;
        return flag2;
    }
    else if (!traversed)
    {
        for (int i = key + 1; i < 1000; i = (key + i * i) % 1000)
        {
            if (user[i].get_pass() == password && user[i].get_uname() == username)
            {
                system("CLS");
                cout << "Logged In Successfully! \n";
                flag2 = true;
                return flag2;
            }
        }
    }
    traversed = true;
    if (traversed && !flag2)
    {
        return false;
    }
}

void FindRelationship(TreeNode *root, string person1, string person2)
{
    TreeNode *person1Node = FindFamilyMember(root, person1);
    TreeNode *person2Node = FindFamilyMember(root, person2);

    if (!person1Node || !person2Node)
    {
        cout << "One or both persons not found in the family tree." << endl;
        return;
    }

   
    TreeNode *commonancestor = nullptr;
    TreeNode *ancestor1 = person1Node;
    TreeNode *ancestor2 = person2Node;

    while (ancestor1 != ancestor2 && ancestor1 && ancestor2)
    {
        if (ancestor1->memberId < ancestor2->memberId)
        {
            ancestor2 = FindParentOfFamilyMember(root, ancestor2->member_name);
        }
        else if (ancestor1->memberId > ancestor2->memberId)
        {
            ancestor1 = FindParentOfFamilyMember(root, ancestor1->member_name);
        }
        else
        {
            commonancestor = ancestor1;
            break;
        }
    }

    if (!commonancestor)
    {
        cout << "Error finding common ancestor." << endl;
        return;
    }

    // Determine the relationship between person1 and person2 based on the common ancestor
    if (commonancestor->memberId == person1Node->memberId || commonancestor->memberId == person2Node->memberId)
    {
        cout << person1 << " is " << person2 << "'s ";
        if (person1Node->memberId == commonancestor->memberId)
        {
            cout << "parent.";
        }
        else
        {
            cout << "child.";
        }
    }
    else
    {
        cout << person1 << " and " << person2 << " are ";
        int depth1 = 0, depth2 = 0;
        TreeNode *temp = person1Node;
        while (temp != commonancestor)
        {
            temp = FindParentOfFamilyMember(root, temp->member_name);
            depth1++;
        }
        temp = person2Node;
        while (temp != commonancestor)
        {
            temp = FindParentOfFamilyMember(root, temp->member_name);
            depth2++;
        }

        if (depth1 > 1)
        {
            cout << "cousins ";
            for (int i = 1; i < depth1 - 1; ++i)
            {
                cout << "removed ";
            }
            if (depth1 > 2)
            {
                cout << depth1 - 2 << " times ";
            }
        }
        else
        {
            cout << "siblings ";
        }

        if (depth1 != depth2)
        {
            cout << "once ";
        }

        if (depth1 < depth2)
        {
            cout << "removed ";
            for (int i = 1; i < depth2 - depth1; ++i)
            {
                cout << "removed ";
            }
            cout << depth2 - depth1 << " times ";
        }

        cout << "from each other.";
    }
}

void LoginUser(HashTable users[], int& currentUserId,ifstream &userFile) 
{
    string inputUsername, inputPassword;

    cout << "Enter your Username: ";
    cin >> inputUsername;

    cout << "Enter your Password: ";
    cin >> inputPassword;

    if (!userFile.is_open())
    {
        cout<<"ERROR OPENING THE FILE!";
    }
    bool found = false;
    string word;
    while(userFile>>word)
    {
        if(word == inputUsername)
        {
            found = true;
            break;
        }
    }

    userFile.close();

    bool userFound = false;

    // Check if user exists in the loaded user details
    //for (int i = 0; i < MAX_USERS; ++i) {
    //    if (users[i].get_uname() == inputUsername && users[i].get_pass() == inputPassword) {
    //        userFound = true;
    //        currentUserId = i;
    //        break;
    //    }
    //}

    if (!found) {
        cout << "Invalid Username or Password. Please try again." << endl;
        return;
    }

    cout << "Login successful!" << endl;

    bool check = true;
    while (check)
    {
        int choice;
        int key=HashFunction(inputUsername,inputPassword);
        int boxWidth = 90; // Set a fixed width for the box

        cout << setw(boxWidth) << right << "+----------------------------------------+" << endl;
        cout << setw(boxWidth) << right << "|              MENU OPTIONS              |" << endl;
        cout << setw(boxWidth) << right << "+----------------------------------------+" << endl;
        cout << setw(boxWidth) << right << "|  1. Create a new Family Tree           |" << endl;
        cout << setw(boxWidth) << right << "|  2. Display Entire Family Tree         |" << endl;
        cout << setw(boxWidth) << right << "|  3. Display a Specific Generation      |" << endl;
        cout << setw(boxWidth) << right << "|  4. Add Members to Existing Tree       |" << endl;
        cout << setw(boxWidth) << right << "|  5. Edit Details of a Family Member    |" << endl;
        cout << setw(boxWidth) << right << "|  6. Search for a Family Member         |" << endl;
        cout << setw(boxWidth) << right << "|  7. Remove a Family Member             |" << endl;
        cout << setw(boxWidth) << right << "|  8. Search Members by keyword          |" << endl;
        cout << setw(boxWidth) << right << "|  9. FIND RELATION                      |" << endl;
        cout << setw(boxWidth) << right << "| 10. Display Sub-Family Tree            |" << endl;
        cout << setw(boxWidth) << right << "| 11. Return to the main screen          |" << endl;
        cout << setw(boxWidth) << right << "+----------------------------------------+" << endl;

        cout << "\nEnter your choice:";
        cin >> choice;
        switch (choice)
        {
        case 1:
        {
            system("cls");
            CreateFamilyTree(user[key].get_root());
            break;
        }
        case 2:
        {
            system("cls");
            DisplayFamilyTree(user[key].get_root(), user[key].get_root());
            break;
        }
        case 3:
        {
            system("cls");
            int targetgen;
            cout << "Enter the generation you want to display: ";
            cin >> targetgen;
            DisplaySpecificGeneration(user[key].get_root(), user[key].get_root(), targetgen, 0);
            break;
        }
        case 4:
        {
            system("cls");
            AddMemberToTree(user[key]);
            break;
        }
        case 5:
        {
            system("cls");
            EditFamilyMemberDetails(user[key].get_root());
            break;
        }
        case 6:
        {
            system("cls");
            SearchFamilyMember(user[key].get_root());
            break;
        }
        case 7:
        {
            system("cls");
            RemoveFamilyMember(user[key]);
            break;
        }
        case 8:
        {
            system("CLS");
            string keyword;
            cout << "Enter keyword to be searched:";
            cin >> keyword;
            DisplaybyKeyword(user[key].get_root(), user[key].get_root(), keyword);
            break;
        }
        case 9:
        {
            system("cls");
            string person1, person2;
            cout << "Enter the name of the first person: ";
            cin >> person1;
            cout << "Enter the name of the second person: ";
            cin >> person2;
            FindRelationship(user[key].get_root(), person1, person2);
            break;
        }
        case 10:
        {
            system("cls");
            DisplaySubFamily(user[key].get_root());
            break;
        }
        case 11:
        {
            check = false;
            system("CLS");
            return;
        }
        default:
            cout << "Invalid choice. Please re-enter your choice";
        }
    }
}

void RegisterUser(HashTable users[],ofstream &outFile) 
{
    string username, password;

    cout << "Enter a Username: ";
    cin >> username;

    cout << "Enter a Password: ";
    cin >> password;

    if (username.size() < 3 || password.size() < 3) {
        cout << "At least 3 characters are required for both Username and Password!\n";
        return;
    }

    int flag1 = 0;
    for (int i = 0; i < password.size(); i++) {
        if (int(password[i]) >= 33 && int(password[i] <= 47)) {
            flag1 = 1;
            break;
        }
    }

    if (flag1 == 0) 
    {
        cout << "Password must contain at least one special character!";
        return;
    }

    int key = HashFunction(username, password);
    for (int i=0; i<1000; i++)
    {
        if (users[i].get_uname() == username)
        {
            system("CLS");
            cout << "User Already Exists!\n";
            return;
        }
    }
    HashTable obj(username,password);
    if (users[key].get_pass()=="" && users[key].get_uname()=="" && users[key].get_userId()==-1)
    {
        users[key]=obj;
        obj.set_userId(key);
    }
    else
    {
        for (int i=key+1; i<1000; i=(key + i*i)%1000)
        {
            if (users[i].get_pass()=="" && users[i].get_uname()=="" && users[i].get_userId()==-1)
            {
                users[i]=obj;
                obj.set_userId(i); 
                break;
            }
        }
    }
    system("CLS");

    
    SaveUserDetails(users[key],outFile);
    cout << "Registration successful!\n";
}



int main()
{
    system("CLS");
    int currentUserId=-1;
    int choice;
    string filename ="familytree.txt";
    ofstream outFile(filename);
    ifstream infile(filename);
    bool check = true;
    cout << setw(80) << "Family Tree Generator" << endl;
    while (check)
    {
        cout << setw(90) << "+----------------------------------------+" << endl;
        cout << setw(90) << "|              MENU OPTIONS              |" << endl;
        cout << setw(90) << "+----------------------------------------+" << endl;
        cout << setw(90) << "|  1. User Registration (For new user)   |" << endl;
        cout << setw(90) << "|  2. User Login (For existing user)     |" << endl;
        cout << setw(90) << "|  3. Exit                               |" << endl;
        cout << setw(90) << "+----------------------------------------+" << endl;
        cout << "\nEnter your choice : ";
        cin >> choice;
        switch (choice) {
            case 1:
                system("CLS");
                RegisterUser(user,outFile);
                break;
            case 2:
                system("CLS");
                LoginUser(user, currentUserId,infile);
                break;
            case 3:
                system("CLS");
                cout << "Exiting...\n";
                return 0;
        default:
        {
            system("CLS");
            cout << "Invalid Choice!";
            break;
        }
        }
    }
    return 0;
}