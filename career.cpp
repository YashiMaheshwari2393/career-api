
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

class AstroVision
{
private:
    string name;
    string dob;
    string place;

public:

    // Constructor
    AstroVision(string n, string d, string p)
    {
        name = n;
        dob = d;
        place = p;
    }

    // Function to generate zodiac sign (simple logic)
    string getZodiac()
    {
        int day, month, year;
        sscanf(dob.c_str(), "%d-%d-%d", &day, &month, &year);

        if((month==3 && day>=21) || (month==4 && day<=19))
            return "Aries";
        else if((month==4 && day>=20) || (month==5 && day<=20))
            return "Taurus";
        else if((month==5 && day>=21) || (month==6 && day<=20))
            return "Gemini";
        else if((month==6 && day>=21) || (month==7 && day<=22))
            return "Cancer";
        else if((month==7 && day>=23) || (month==8 && day<=22))
            return "Leo";
        else if((month==8 && day>=23) || (month==9 && day<=22))
            return "Virgo";
        else if((month==9 && day>=23) || (month==10 && day<=22))
            return "Libra";
        else if((month==10 && day>=23) || (month==11 && day<=21))
            return "Scorpio";
        else if((month==11 && day>=22) || (month==12 && day<=21))
            return "Sagittarius";
        else if((month==12 && day>=22) || (month==1 && day<=19))
            return "Capricorn";
        else if((month==1 && day>=20) || (month==2 && day<=18))
            return "Aquarius";
        else
            return "Pisces";
    }

    // Career generator
    void generateCareer()
    {
        vector<string> careers =
        {
            "Software Engineer",
            "Doctor",
            "Data Scientist",
            "Entrepreneur",
            "Teacher",
            "Astrologer",
            "Digital Marketer",
            "Civil Engineer",
            "Graphic Designer",
            "Psychologist",
            "Content Creator",
            "Research Scientist",
            "Business Analyst",
            "Lawyer"
        };

        srand(time(0));

        cout << "\n------ Career Prediction ------\n";
        cout << "Name: " << name << endl;
        cout << "Date of Birth: " << dob << endl;
        cout << "Place of Birth: " << place << endl;
        cout << "Zodiac Sign: " << getZodiac() << endl;

        cout << "\nSuggested Career Options:\n";

        for(int i=0;i<4;i++)
        {
            int index = rand() % careers.size();
            cout << "- " << careers[index] << endl;
        }

        cout << "\nAstro Message:\n";
        cout << "Your stars indicate strong potential. Choose a career that matches your passion and dedication.\n";
    }
};

int main()
{
    string name,dob,place;

    cout<<"Enter Name: ";
    getline(cin,name);

    cout<<"Enter Date of Birth (DD-MM-YYYY): ";
    getline(cin,dob);

    cout<<"Enter Place of Birth: ";
    getline(cin,place);

    AstroVision user(name,dob,place);

    user.generateCareer();

    return 0;
} 
 