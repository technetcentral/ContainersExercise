#include <iostream>
#include <iomanip> // for setw
#include <vector>
#include <list>
#include <algorithm> // for max and sort
#include <stdexcept> // for domain_error
#include <assert.h>
using namespace std;

// Functions from OrganizingPrograms

double median(const vector<double>& v) {
    if (v.size() == 0)
        throw domain_error("can't compute median of empty vector");
    vector<double> v2(v);
    sort(v2.begin(), v2.end());
    int mid = v2.size() / 2;
    double median;
    if (v2.size() % 2 == 0)
        median = (v2[mid - 1] + v2[mid]) / 2;
    else
        median = v2[mid];
    return median;
}

double grade(double midterm, double final,
             const vector<double>& homework) {
    try {
        return 0.2 * midterm + 0.4 * final + 0.4 * median(homework);
    } catch (domain_error& e) {
        throw domain_error("need at least one homework");
    }
    return 0; // to quiet the warning
}

// Note, changed to add an "if" and clear.
istream& read_homework(istream& in, vector<double>& homework) {
    if (in) {
        double x;
        homework.clear();
        while (cin >> x)
            homework.push_back(x);
        in.clear(); // clear the error state
    }
    return in;
}

// New structure for grouping student information.

struct student_info {
    string name;
    double midterm, final;
    vector<double> homework;
};

istream& read(istream& in, student_info& s) {
    in >> s.name >> s.midterm >> s.final;
    return read_homework(in, s.homework);
}

double grade(const student_info& s) {
    return grade(s.midterm, s.final, s.homework);
}

bool compare_names(const student_info& s1, const student_info& s2) {
    return s1.name < s2.name;
}

bool failing_grade(const student_info& s) {
    return grade(s) < 60;
}

// first try, not space efficient
vector<student_info>
extract_fails1(vector<student_info>& students) {
    vector<student_info> pass, fail;
    for (int i = 0; i != students.size(); ++i) {
        if (failing_grade(students[i]))
            fail.push_back(students[i]);
        else
            pass.push_back(students[i]);
    }
    students = pass; // copies pass into students
    return fail;
}



// second try, not time efficient
vector<student_info>
extract_fails2(vector<student_info>& students) {
    vector<student_info> fail;
    int i = 0;
    while (i != students.size()) {
        if (failing_grade(students[i])) {
            fail.push_back(students[i]);
            students.erase(students.begin() + i);
        } else
            ++i;
    }
    return fail;
}


// solution: use list instead of vector
list<student_info>
extract_fails(list<student_info>& students) {
    list<student_info> fail;
    list<student_info>::iterator iter = students.begin();
    while (iter != students.end()) {
        if (failing_grade(*iter)) {
            fail.push_back(*iter);
            iter = students.erase(iter);
        } else
            ++iter;
    }
    return fail;
}

// need to update the following to use extract_fails.
int main() {
    list<student_info> students;
    student_info student;
    string::size_type maxlen = 0;
    while (read(cin, student)) {
        maxlen = max(maxlen, student.name.size());
        students.push_back(student);
    }
    extract_fails(students);
    //sort(students.begin(), students.end(), compare_names);
    students.sort(compare_names);
    for (list<student_info>::iterator iter = students.begin();
         iter != students.end(); ++iter) {
        cout << setw(maxlen+1) << iter->name;
        try {
            double course_grade = grade(*iter);
            streamsize old_prec = cout.precision(3);
            cout << " " << course_grade << endl;
            cout.precision(old_prec);
        } catch (const domain_error& e) {
            cout << e.what();
        }
    }
    return 0;
}
