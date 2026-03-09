#include <crow.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>

using namespace std;

// ─── Zodiac Calculator ────────────────────────────────────────────────────────
string calculateZodiac(int day, int month) {
    if ((month == 3 && day >= 21) || (month == 4 && day <= 19))  return "Aries (Mesh)";
    if ((month == 4 && day >= 20) || (month == 5 && day <= 20))  return "Taurus (Vrishabh)";
    if ((month == 5 && day >= 21) || (month == 6 && day <= 20))  return "Gemini (Mithun)";
    if ((month == 6 && day >= 21) || (month == 7 && day <= 22))  return "Cancer (Kark)";
    if ((month == 7 && day >= 23) || (month == 8 && day <= 22))  return "Leo (Singh)";
    if ((month == 8 && day >= 23) || (month == 9 && day <= 22))  return "Virgo (Kanya)";
    if ((month == 9 && day >= 23) || (month == 10 && day <= 22)) return "Libra (Tula)";
    if ((month == 10 && day >= 23) || (month == 11 && day <= 21)) return "Scorpio (Vrishchik)";
    if ((month == 11 && day >= 22) || (month == 12 && day <= 21)) return "Sagittarius (Dhanu)";
    if ((month == 12 && day >= 22) || (month == 1 && day <= 19)) return "Capricorn (Makar)";
    if ((month == 1 && day >= 20) || (month == 2 && day <= 18))  return "Aquarius (Kumbh)";
    return "Pisces (Meen)";
}

// ─── Lucky Quotes ─────────────────────────────────────────────────────────────
vector<string> getRandomQuotes() {
    vector<string> quotes = {
        "Believe in yourself and shine bright!",
        "Success comes from consistency.",
        "Your stars support your hard work.",
        "Today is a new opportunity to grow.",
        "Confidence is your true power.",
        "The cosmos aligns in your favour.",
        "Trust the journey the universe has laid for you.",
        "Your potential is written in the stars."
    };

    srand((unsigned)time(nullptr));
    vector<string> chosen;
    vector<int> used;

    while ((int)chosen.size() < 3) {
        int idx = rand() % quotes.size();
        bool duplicate = false;
        for (int u : used) if (u == idx) { duplicate = true; break; }
        if (!duplicate) {
            chosen.push_back(quotes[idx]);
            used.push_back(idx);
        }
    }
    return chosen;
}

// ─── Main ─────────────────────────────────────────────────────────────────────
int main() {
    crow::SimpleApp app;

    // ── Health check (Render uses this to verify the service is up) ──────────
    CROW_ROUTE(app, "/")
    ([]() {
        return crow::response(200, "Kundali API is running! POST to /kundali");
    });

    // ── POST /kundali  ────────────────────────────────────────────────────────
    // Expects JSON body: { "name":"...", "day":DD, "month":MM, "year":YYYY, "place":"..." }
    // Returns JSON with zodiac, quotes, and a special message
    CROW_ROUTE(app, "/kundali").methods(crow::HTTPMethod::POST)
    ([](const crow::request& req) {
        auto body = crow::json::load(req.body);

        // ── Validate input ────────────────────────────────────────────────────
        if (!body || !body.has("name") || !body.has("day") ||
            !body.has("month") || !body.has("year") || !body.has("place")) {
            crow::json::wvalue err;
            err["error"] = "Missing fields. Required: name, day, month, year, place";
            return crow::response(400, err);
        }

        string name  = body["name"].s();
        int    day   = body["day"].i();
        int    month = body["month"].i();
        int    year  = body["year"].i();
        string place = body["place"].s();

        // ── Basic range validation ────────────────────────────────────────────
        if (day < 1 || day > 31 || month < 1 || month > 12 || year < 1900 || year > 2100) {
            crow::json::wvalue err;
            err["error"] = "Invalid date values.";
            return crow::response(400, err);
        }

        // ── Build response ────────────────────────────────────────────────────
        string zodiac = calculateZodiac(day, month);
        vector<string> quotes = getRandomQuotes();

        crow::json::wvalue res;
        res["name"]           = name;
        res["place_of_birth"] = place;
        res["date_of_birth"]  = to_string(day) + "-" + to_string(month) + "-" + to_string(year);
        res["zodiac_sign"]    = zodiac;
        res["lucky_quotes"][0] = quotes[0];
        res["lucky_quotes"][1] = quotes[1];
        res["lucky_quotes"][2] = quotes[2];
        res["special_message"] = "The universe has great plans for you, " + name + "!";

        return crow::response(200, res);
    });

    // ── GET /kundali  (friendly usage hint) ──────────────────────────────────
    CROW_ROUTE(app, "/kundali").methods(crow::HTTPMethod::GET)
    ([]() {
        crow::json::wvalue hint;
        hint["usage"]   = "Send a POST request to /kundali";
        hint["example"] = crow::json::load(
            R"({"name":"Arjun","day":15,"month":8,"year":2000,"place":"Delhi"})"
        );
        return crow::response(200, hint);
    });

    // ── Listen on $PORT (Render injects this env var) or default 8080 ─────────
    const char* port_env = getenv("PORT");
    int port = port_env ? atoi(port_env) : 8080;

    app.port(port).multithreaded().run();
    return 0;
}
