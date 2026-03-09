#include "crow.h"
#include <vector>
#include <string>
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

    // Career generator - returns JSON response
    crow::response generateCareer()
    {
        if (!isValid()) {
            crow::json::wvalue err;
            err["error"] = "Invalid date. Use format DD-MM-YYYY";
            return crow::response(400, err);
        }

        vector<string> careers = {
            "Software Engineer", "Doctor", "Data Scientist", "Entrepreneur",
            "Teacher", "Astrologer", "Digital Marketer", "Civil Engineer",
            "Graphic Designer", "Psychologist", "Content Creator",
            "Research Scientist", "Business Analyst", "Lawyer"
        };

        srand((unsigned)time(0));
        vector<string> chosen;
        vector<int> used;
        while ((int)chosen.size() < 4) {
            int idx = rand() % careers.size();
            bool duplicate = false;
            for (int u : used) if (u == idx) { duplicate = true; break; }
            if (!duplicate) {
                chosen.push_back(careers[idx]);
                used.push_back(idx);
            }
        }

        crow::json::wvalue res;
        res["name"]           = name;
        res["dob"]            = dob;
        res["place_of_birth"] = place;
        res["zodiac_sign"]    = getZodiac();
        res["career_suggestions"][0] = chosen[0];
        res["career_suggestions"][1] = chosen[1];
        res["career_suggestions"][2] = chosen[2];
        res["career_suggestions"][3] = chosen[3];
        res["astro_message"]  = "Your stars indicate strong potential, " + name +
                                "! Choose a career that matches your passion and dedication.";

        return crow::response(200, res);
    }
};

// ─── Main ────────────────────────────────────────────────────────────────────
int main()
{
    crow::SimpleApp app;

    // Health check
    CROW_ROUTE(app, "/")
    ([]() {
        return crow::response(200, "AstroVision Career API is running! POST to /career");
    });

    // GET /career - usage hint
    CROW_ROUTE(app, "/career").methods(crow::HTTPMethod::GET)
    ([]() {
        crow::json::wvalue hint;
        hint["usage"]   = "Send a POST request to /career";
        hint["example"] = crow::json::load(
            R"({"name":"Yashi","dob":"23-03-2000","place":"Rajasthan"})"
        );
        return crow::response(200, hint);
    });

    // POST /career
    CROW_ROUTE(app, "/career").methods(crow::HTTPMethod::POST)
    ([](const crow::request& req) {
        auto body = crow::json::load(req.body);

        if (!body || !body.has("name") || !body.has("dob") || !body.has("place")) {
            crow::json::wvalue err;
            err["error"] = "Missing fields. Required: name, dob (DD-MM-YYYY), place";
            return crow::response(400, err);
        }

        AstroVision user(body["name"].s(), body["dob"].s(), body["place"].s());
        return user.generateCareer();
    });

    const char* port_env = getenv("PORT");
    int port = port_env ? atoi(port_env) : 8080;

    app.port(port).multithreaded().run();
    return 0;
}
