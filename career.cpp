#include "crow/crow.h"
#include <vector>
#include <ctime>
#include <cstdlib>
#include <string>

using namespace std;

// ─── Zodiac Calculator ────────────────────────────────────────────────────────
string getZodiac(int day, int month) {
    if ((month==3 && day>=21) || (month==4 && day<=19))  return "Aries";
    if ((month==4 && day>=20) || (month==5 && day<=20))  return "Taurus";
    if ((month==5 && day>=21) || (month==6 && day<=20))  return "Gemini";
    if ((month==6 && day>=21) || (month==7 && day<=22))  return "Cancer";
    if ((month==7 && day>=23) || (month==8 && day<=22))  return "Leo";
    if ((month==8 && day>=23) || (month==9 && day<=22))  return "Virgo";
    if ((month==9 && day>=23) || (month==10 && day<=22)) return "Libra";
    if ((month==10 && day>=23) || (month==11 && day<=21)) return "Scorpio";
    if ((month==11 && day>=22) || (month==12 && day<=21)) return "Sagittarius";
    if ((month==12 && day>=22) || (month==1 && day<=19)) return "Capricorn";
    if ((month==1 && day>=20) || (month==2 && day<=18))  return "Aquarius";
    return "Pisces";
}

// ─── Career Suggestions ───────────────────────────────────────────────────────
vector<string> getRandomCareers() {
    vector<string> careers = {
        "Software Engineer", "Doctor", "Data Scientist", "Entrepreneur",
        "Teacher", "Astrologer", "Digital Marketer", "Civil Engineer",
        "Graphic Designer", "Psychologist", "Content Creator",
        "Research Scientist", "Business Analyst", "Lawyer"
    };

    srand((unsigned)time(nullptr));
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
    return chosen;
}

// ─── Main ─────────────────────────────────────────────────────────────────────
int main() {
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

        string name  = body["name"].s();
        string dob   = body["dob"].s();
        string place = body["place"].s();

        // Parse dob
        int day = 0, month = 0, year = 0;
        sscanf(dob.c_str(), "%d-%d-%d", &day, &month, &year);

        if (day < 1 || day > 31 || month < 1 || month > 12 || year < 1900 || year > 2100) {
            crow::json::wvalue err;
            err["error"] = "Invalid date. Use format DD-MM-YYYY";
            return crow::response(400, err);
        }

        string zodiac = getZodiac(day, month);
        vector<string> careers = getRandomCareers();

        crow::json::wvalue res;
        res["name"]           = name;
        res["dob"]            = dob;
        res["place_of_birth"] = place;
        res["zodiac_sign"]    = zodiac;
        res["career_suggestions"][0] = careers[0];
        res["career_suggestions"][1] = careers[1];
        res["career_suggestions"][2] = careers[2];
        res["career_suggestions"][3] = careers[3];
        res["astro_message"]  = "Your stars indicate strong potential, " + name + "! Choose a career that matches your passion and dedication.";

        return crow::response(200, res);
    });

    const char* port_env = getenv("PORT");
    int port = port_env ? atoi(port_env) : 8080;

    app.port(port).multithreaded().run();
    return 0;
}
