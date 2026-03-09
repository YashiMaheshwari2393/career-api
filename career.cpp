#include "httplib.h"
#include "json.hpp"
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;
using json = nlohmann::json;

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

    // Function to generate zodiac sign
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

    // Validate date
    bool isValid()
    {
        int day, month, year;
        sscanf(dob.c_str(), "%d-%d-%d", &day, &month, &year);
        return day >= 1 && day <= 31 &&
               month >= 1 && month <= 12 &&
               year >= 1900 && year <= 2100;
    }

    // Career generator
    json generateCareer()
    {
        vector<string> careers =
        {
            "Software Engineer", "Doctor", "Data Scientist", "Entrepreneur",
            "Teacher", "Astrologer", "Digital Marketer", "Civil Engineer",
            "Graphic Designer", "Psychologist", "Content Creator",
            "Research Scientist", "Business Analyst", "Lawyer"
        };

        srand((unsigned)time(0));
        vector<string> chosen;
        vector<int> used;
        while ((int)chosen.size() < 4)
        {
            int index = rand() % careers.size();
            bool duplicate = false;
            for (int u : used) if (u == index) { duplicate = true; break; }
            if (!duplicate)
            {
                chosen.push_back(careers[index]);
                used.push_back(index);
            }
        }

        json result;
        result["name"]               = name;
        result["dob"]                = dob;
        result["place_of_birth"]     = place;
        result["zodiac_sign"]        = getZodiac();
        result["career_suggestions"] = chosen;
        result["astro_message"]      = "Your stars indicate strong potential, " + name +
                                       "! Choose a career that matches your passion and dedication.";
        return result;
    }
};

int main()
{
    httplib::Server svr;

    svr.Get("/", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("AstroVision Career API is running! POST to /career", "text/plain");
    });

    svr.Get("/career", [](const httplib::Request&, httplib::Response& res) {
        json hint;
        hint["usage"]   = "Send a POST request to /career";
        hint["example"] = {{"name","Yashi"},{"dob","23-03-2000"},{"place","Rajasthan"}};
        res.set_content(hint.dump(2), "application/json");
    });

    svr.Post("/career", [](const httplib::Request& req, httplib::Response& res) {
        json body;
        try {
            body = json::parse(req.body);
        } catch (...) {
            json err;
            err["error"] = "Invalid JSON body";
            res.status = 400;
            res.set_content(err.dump(), "application/json");
            return;
        }

        if (!body.contains("name") || !body.contains("dob") || !body.contains("place")) {
            json err;
            err["error"] = "Missing fields. Required: name, dob (DD-MM-YYYY), place";
            res.status = 400;
            res.set_content(err.dump(), "application/json");
            return;
        }

        AstroVision user(body["name"], body["dob"], body["place"]);

        if (!user.isValid()) {
            json err;
            err["error"] = "Invalid date. Use format DD-MM-YYYY";
            res.status = 400;
            res.set_content(err.dump(), "application/json");
            return;
        }

        json result = user.generateCareer();
        res.set_content(result.dump(2), "application/json");
    });

    const char* port_env = getenv("PORT");
    int port = port_env ? atoi(port_env) : 8080;

    cout << "AstroVision API running on port " << port << endl;
    svr.listen("0.0.0.0", port);
    return 0;
}
