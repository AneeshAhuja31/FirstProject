#include<iostream>
#include<fstream>
#include<algorithm>
#include<string>
#include<sstream>
#include<vector>
#include<iomanip>
using namespace std;

class Student {
private:
    string name;
    int rollno;
    float marks;
    float percentage;

public:
    //Constructor
    Student() : name(""), rollno(0), marks(0.0), percentage(0.0) {}
    //Constructor with arguments
    Student(string name,int rollno,float marks){
        this->name=name;
        this->rollno=rollno;
        this->marks=marks;
        this->percentage=(this->marks*100)/100;
    }
    //Function to read data from user
    void readData(){
        //cin ignore to input multi letter string
        cin.ignore();
        cout<<"Enter Name: ";
        getline(cin,name);
        //Input rollno till integral value is entered
        while(true){
            cout<<"Enter rollno: ";
            while(!(cin>>rollno)){
                cout<<"Invalid input. Please enter an Integral value: ";
                cin.clear(); //clear the error flag
                cin.ignore(numeric_limits<streamsize>::max(),'\n'); //discard unread characters
                //numeric_limits<streamsize>::max() tells the function to ignore all remaining characters in the input buffer up to the specified limit '\n'
            }
            //Check if inputed roll number already exists
            if(Student::rollnoExist(rollno)){
                cout<<"Roll number "<<rollno<<" already exist! Please enter a different roll number.\n";
            }
            //If input rollno is an integer and does not exist beforehand break the loop
            else break;
        }
        //Input Marks
        cout<<"Enter marks: ";
        //Keeping asking for input until float value is entered
        while(true){
            cin>>marks;
            if(cin.fail()){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(),'\n');
                cout<<"Invalid input. Please enter a number :";
            }
            //float value must lie between 0 and 100
            else if(marks<0 || marks>100)
                cout<<"Invalid input. Please enter marks between 0 and 100: ";
            
            else break; //valid input exit the loop
        }
        percentage = (marks/100)*100;
    }
    //Function to display student data
    void displayData() const {
        cout<<"Name: "<<name<<", Rollno: "<<rollno<<", Marks: "<<marks<<", Percentage: "<<percentage<<"%"<<endl;
    }
    //Function to return rollno
    int getRollNo() const{
        return rollno;
    }
    //Function to save student data to a file
    void saveToFile() const{
        //If rollno already exists do not save student data
        if(Student::rollnoExist(rollno)){
            cout<<"Roll Number: "<<rollno<<" already exists! Pls enter different roll number: ";
            return;
        }
        //edit file using ofstream
        ofstream file("student.txt",ios::app); // append mode
        if(file.is_open()){
            file<< name << "," << rollno << "," << marks << "," << percentage << endl;
            file.close();
            cout<<"Data saved successfully.\n";
        }
        else cout<<"Error opening file\n";
    }
    
    // Function to display all students in increasing order by roll number
    static void displayAll() {
        // Read file using ifstream
        ifstream file("student.txt");
        vector<Student> students; // Vector to store student records

        if (file.is_open()) {
            string line;

            // Read data from the file
            while (getline(file, line)) {
                if (!line.empty()) {
                    stringstream ss(line);
                    Student student;
                    
                    // Read data from each line
                    getline(ss, student.name, ',');
                    ss >> student.rollno;
                    ss.ignore(); // Ignore the comma
                    ss >> student.marks;
                    ss.ignore(); // Ignore the comma
                    ss >> student.percentage;

                    // Add the student record to the vector
                    students.push_back(student);
                }
            }
            file.close();

            // Sort students by roll number
            sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
                return a.rollno < b.rollno; // Sort in increasing order of roll number
            });

            // Print the table header
            cout << left << setw(20) << "Name" 
                << setw(10) << "Roll No" 
                << setw(10) << "Marks" 
                << setw(15) << "Percentage" << endl;
            cout << "----------------------------------------------------------" << endl;

            // Check if any records were found
            if (students.empty()) {
                cout << "No student records found.\n"; // If file is empty
            } else {
                // Print data in tabular format
                for (const auto& student : students) {
                    cout << left << setw(20) << student.name
                        << setw(10) << student.rollno
                        << setw(10) << student.marks
                        << setw(15) << student.percentage << '%' << endl;
                }
            }
        } else {
            cout << "Error opening file!\n";
        }
    }

    //Function to search for a student by roll no
    static void searchByRollNo(int x){ //static functions can be called without creating instance of class
        //read file
        ifstream file("student.txt");
        //bool to check if given rollno exists
        bool found = false;
        if(file.is_open()){
            string line;
            while(getline(file,line)){
                //using stringstream we can further breakdown and examine our string/line
                stringstream ss(line);
                string name;
                int rollno;
                float marks, percentage;
                //partition string into name,rollno,marks,percentage and ignore the "," character
                if(getline(ss,name,',') && ss>>rollno && ss.ignore() && ss>>marks && ss.ignore() && ss>>percentage){
                    if(rollno == x){
                        cout<<"Student Found - Name: "<<name<<", Rollno: "<<rollno<<", Marks: "<<marks<<", Percentage: "<<percentage<<"%"<<endl;
                        found=true;
                        break;
                    }
                }
            }
            file.close();
            if(found == false) //if rollno doesnt exist
                cout<<"Student with RollNo: "<<x<<" not found!\n";
        }
        else cout<<"Error opening file!\n";
    }
    //Check if roll number already exists in the file
    static bool rollnoExist(int rollno) {
        //read file
        ifstream file("student.txt");
        if(file.is_open()){
            string line;
            while(getline(file,line)){ //check every line
                stringstream ss(line);
                int existingRollNo;
                ss.ignore(numeric_limits<streamsize>::max(), ','); // Skip name
                ss>>existingRollNo; //only use rollno part of the line
                if(existingRollNo == rollno){
                    file.close();
                    return true; //return true if rollno is found
                }
            }
            file.close();
        }
        else cout <<"Error opening file!\n";

        return false;
    }
    //Function to update student data
    static void updateStudent(){
        int rollno;
        cout<<"Enter RollNo of student to update: ";
        cin>>rollno;
        ifstream file("student.txt");
        vector<Student> students; //create vector of type student object to input student data into .txt file later on
        bool found = false; //bool to check if rollno exists or not

        if(file.is_open()){
            string line;
            while(getline(file,line)){ //iterate through every line
                stringstream ss(line); 
                string name;
                int existingRollNo;
                float marks, percentage;

                if(getline(ss,name,',') && ss>>existingRollNo && ss.ignore() && ss>>marks && ss.ignore() && ss>>percentage){//partitioning string respected order
                    if(existingRollNo==rollno){
                        found = true; //bool is true if rollno exists
                        // Display current data
                        cout << "Current Data: ";
                        cout << "Name: " << name << ", Rollno: " << existingRollNo << ", Marks: " << marks << ", Percentage: " << percentage << "%" << endl;
                        //Update your details
                        cout << "Update your details: \n"<<"Enter new Name: ";
                        //previous code from read function
                        cin.ignore(); 
                        getline(cin,name);
                        cout<<"Enter new marks: ";
                        while(true){
                            cin>>marks;
                            if(cin.fail()){
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(),'\n');
                                cout<<"Invalid input. Please enter a number :";
                            }
                            else if(marks<0 || marks>100)
                                cout<<"Invalid input. Please enter marks between 0 and 100: ";
                            
                            else break; //valid input exit the loop
                        }
                        percentage = (marks/100)*100;

                    }
                    //Add any updated/unchanged object to vector
                    students.push_back(Student(name,existingRollNo,marks)); 
                }
            }
            file.close();

            //Rewrite file with updated records
            ofstream outFile("student.txt");
            for(const auto& student : students){  //auto& is used to refer to each student without copying it
                outFile << student.name << ','<<student.rollno<<','<<student.marks<<','<<student.percentage<<endl; // output printed on .txt file
            }
            outFile.close();

            if(found) cout<<"Student record updated successfully.\n";
            else cout<<"Error opening file!\n";
        }

    }
    //Delete Student Data function
    static void deleteStudent(){
        int rollno;
        cout<<"Enter RollNo of student to delete: ";
        cin>>rollno;
        ifstream file("student.txt");
        vector<Student> students; //create vector of type student object to input student data into .txt file later on
        bool found = false; // bool set to false

        if(file.is_open()){
            string line;
            while(getline(file,line)){ //iterate through every line
                stringstream ss(line);
                string name;
                int existingRollNo;
                float marks, percentage;

                if(getline(ss,name,',') && ss>>existingRollNo && ss.ignore() && ss>>marks && ss.ignore() && ss>>percentage){//partition line in the respective order
                    if(existingRollNo==rollno){
                        found = true;
                        // Display current data
                        cout << "Data being deleted:\n ";
                        cout << "Name: " << name << ", Rollno: " << existingRollNo << ", Marks: " << marks << ", Percentage: " << percentage << "%" << endl;
                    }
                    else //add all student objects to the vector other than the one being deleted 
                        students.push_back(Student(name,existingRollNo,marks)); // Add existing or updated student to vector
                }
            }
            file.close();

            // Rewrite the file without the deleted student
            ofstream outFile("student.txt");
            for(const auto& student : students){  //auto& is used to refer to each student without copying it
                outFile << student.name << ','<<student.rollno<<','<<student.marks<<','<<student.percentage<<endl; //output printed on .txt file
            }
            outFile.close();

            if(found) cout<<"Student record deleted successfully.\n";
            else cout<<"Error opening file!\n";
        }

    }
    //Function to calculate Average Marks
    static void calculateAvgMarks(){
        ifstream file("student.txt"); //read mode
        float totalMarks = 0;
        int studentCount = 0;
         if(file.is_open()){
            string line;
            while(getline(file,line)){ //iterate through every line
                stringstream ss(line); //partition line
                string name;
                int rollno;
                float marks,percentage;
                //read data from each line
                getline(ss,name,',');
                ss >> rollno;
                ss.ignore(); //ignore comma
                ss>>marks; //extract marks

                totalMarks += marks;
                studentCount++;
            }
            file.close();

            if(studentCount>0){
                float averageMarks = totalMarks/studentCount;
                cout<<"Average Marks of all students: "<<fixed<<setprecision(2)<<averageMarks<<endl;
            }
            else{
                cout<<"No student records to calculate average.\n";
            }
        }
        else{
            cout<<"Error in opening file!\n";
        }
    }
    //Function to find total students
    static void calculateTotalStudents(){
        ifstream file("student.txt"); //read mode
        int studentCount = 0;
         if(file.is_open()){
            string line;
            while(getline(file,line)){ //iterate through every line
                studentCount++;
            }
            file.close();

            if(studentCount>0){
                
                cout<<"Total Number of students are: "<<studentCount<<endl;
            }
            else{
                cout<<"No student records found.\n";
            }
        }
        else{
            cout<<"Error in opening file!\n";
        }
    }
    //Function to find top performer
    static void findTopPerformer(){
        ifstream file("student.txt"); //read mode
        float topMarks = -1.0;
        vector<Student> topPerformers; //create a vector of object Student to collect top performers
        int studentCount = 0;
         if(file.is_open()){
            string line;
            while(getline(file,line)){ //iterate through every line
                stringstream ss(line); //partition line
                string name;
                int rollno;
                float marks,percentage;
                //read data from each line
                getline(ss,name,',');
                ss >> rollno;
                ss.ignore(); //ignore comma
                ss>>marks; //extract marks
                studentCount++;
                if(marks>topMarks){
                    topMarks = marks;
                    topPerformers.clear(); //Reset vector if higher marks are found
                    topPerformers.push_back(Student(name,rollno,marks));
                }
                else if (marks == topMarks) {
                    topPerformers.push_back(Student(name, rollno, marks));
                }
            }
            file.close();
            
            if(studentCount>0){
                if(topPerformers.size()==1) //for single topper
                    cout<<"\nThe top performer is: "<<topPerformers[0].name<<" with marks: "<<topMarks<<endl;
                else{  //for multiple top performers
                    cout<<"\nThe Top Performers are:\n";
                    cout << left << setw(20) << "Name"
                         << setw(10) << "Roll No"
                         << setw(10) << "Marks"
                         << setw(15) << "Percentage" << endl;
                    cout << "----------------------------------------------------------" << endl;
                    for(const auto& student : topPerformers){ //loop to display details of objects in topPerformer vector
                        cout<<left<<setw(20)<<student.name
                            <<setw(10)<<student.rollno
                            <<setw(10)<<student.marks
                            <<setw(15)<<student.percentage<<endl;
                    }
                }
            }
            else cout<<"No student records found.\n";
            
        }
        else{
            cout<<"Error in opening file!\n";
        }
    }
    //Function to display Ranklist
    static void displayRankList(){
        ifstream file("student.txt");
        vector<Student> students;
        if(file.is_open()){
            string line;
            while(getline(file,line)){
                stringstream ss(line);
                string name;
                int rollno;
                float marks,percentage;

                //Extract Student Data
                getline(ss,name,',');
                ss>>rollno;
                ss.ignore();
                ss>>marks;
                ss.ignore();
                ss>>percentage;

                students.push_back(Student(name,rollno,marks));
            }
            file.close();

            sort(students.begin(),students.end(),[](const Student &a, const Student &b){
                return a.marks>b.marks;
            });

            //Display rank list
            cout<<"\nRank List:\n";
            cout << left << setw(5) << "Rank"
                << setw(20) << "Name"
                << setw(10) << "Roll No"
                << setw(10) << "Marks"
                << setw(15) << "Percentage" << endl;
            cout << "----------------------------------------------------------" << endl;

            int rank=1;
            for(const auto& student : students){
                cout<<left<<setw(5)<<rank++
                    <<setw(20)<<student.name
                    <<setw(10)<<student.rollno
                    <<setw(10)<<student.marks
                    <<setw(15)<<student.percentage<<'%'<<endl;
            }
        }
        else{
            cout<<"Error opening file!\n";
        }
    }
};
int main() {
    Student s;
    int choice;

    do {
        cout << "\nStudent Management System\n";
        cout << "1. Add Student\n";
        cout << "2. Display All Students\n";
        cout << "3. Search Student by Roll No\n";
        cout << "4. Update Student Data\n";
        cout << "5. Delete Student Data\n";
        cout << "6. Calculate Average Marks\n";
        cout << "7. Calculate Total Students\n";
        cout << "8. Find Top Performer\n";
        cout << "9. Display Rank List\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                s.readData();
                s.saveToFile();
                break;
            case 2:
                Student::displayAll();
                break;
            case 3:
                cout << "Enter Roll No to search: ";
                int rollno;
                cin >> rollno;
                Student::searchByRollNo(rollno);
                break;
            case 4:
                Student::updateStudent();
                break;
            case 5:
                Student::deleteStudent();
                break;
            case 6:
                Student::calculateAvgMarks();
                break;
            case 7:
                Student::calculateTotalStudents();
                break;
            case 8:
                Student::findTopPerformer();
                break;
            case 9:
                Student::displayRankList();
                break;
            case 0:
                cout << "Exiting the system. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice! Please enter a number between 0 and 9.\n";
                break;
        }
    } while (choice != 0);

    return 0;
}
