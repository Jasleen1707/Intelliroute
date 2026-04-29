const express = require("express");
const cors = require("cors");
const axios = require("axios");

const app = express();

app.use(cors());
app.use(express.json());

app.post("/route", async (req, res) => {

    const { source, destination } = req.body;

    try {
        const API_KEY = "a6b6a7c2353af77c07a5606c28c717ac"; 

        let response = await axios.get(
            `https://api.aviationstack.com/v1/flights?access_key=${API_KEY}&dep_iata=${source}&arr_iata=${destination}`
        );

        let flights = response.data.data || [];

        let filtered = flights
            .filter(f => f.departure && f.arrival && f.flight)
            .slice(0, 10);

        let note = "";

        // fallback
        if (filtered.length === 0) {
            note = `No direct flights found from ${source} to ${destination}\nShowing alternative routes...\n\n`;

            response = await axios.get(
                `https://api.aviationstack.com/v1/flights?access_key=${API_KEY}`
            );

            flights = response.data.data || [];

            filtered = flights
                .filter(f => f.departure && f.arrival && f.flight)
                .slice(0, 10);
        }

        if (filtered.length === 0) {
            return res.send("No flight data available.");
        }

        // convert
        const routes = filtered.map(f => ({
            flight: f.flight?.iata || "N/A",
            airline: f.airline?.name || "N/A",
            from: f.departure?.iata || "N/A",
            to: f.arrival?.iata || "N/A",
            cost: Math.floor(Math.random() * 5000) + 2000,
            time: Math.floor(Math.random() * 10) + 1
        }));

        // ---------------- PARETO ----------------

        function dominates(a, b) {
            return (a.cost <= b.cost && a.time <= b.time) &&
                   (a.cost < b.cost || a.time < b.time);
        }

        let pareto = [];

        routes.forEach(r => {
            let dominated = false;

            for (let p of pareto) {
                if (dominates(p, r)) {
                    dominated = true;
                    break;
                }
            }

            if (!dominated) {
                pareto = pareto.filter(p => !dominates(r, p));
                pareto.push(r);
            }
        });

        // ---------------- ORIGINAL LOGIC ----------------

        let cheapest = routes.reduce((a, b) => a.cost < b.cost ? a : b);
        let fastest = routes.reduce((a, b) => a.time < b.time ? a : b);

        let balanced = routes.reduce((a, b) =>
            (a.cost + a.time) < (b.cost + b.time) ? a : b
        );

        // ---------------- OUTPUT ----------------

        let result = "✈️ SMART ROUTE RESULTS\n\n";

        result += `🔍 Route: ${source} → ${destination}\n\n`;
        result += note;

        // 🔹 NEW PART (Pareto)
        result += "📊 Pareto Optimal Routes:\n\n";

        pareto.forEach(r => {
            result += `${r.flight} | ${r.airline}\n`;
            result += `From: ${r.from} → To: ${r.to}\n`;
            result += `Cost: ${r.cost}, Time: ${r.time}\n\n`;
        });

        result += "-----------------------------\n";

        // 🔹 OLD OUTPUT (UNCHANGED STYLE)

        result += "💰 Cheapest Flight:\n";
        result += `${cheapest.flight} | ${cheapest.airline}\n`;
        result += `From: ${cheapest.from} → To: ${cheapest.to}\n`;
        result += `Cost: ${cheapest.cost}, Time: ${cheapest.time}\n\n`;

        result += "⚡ Fastest Flight:\n";
        result += `${fastest.flight} | ${fastest.airline}\n`;
        result += `From: ${fastest.from} → To: ${fastest.to}\n`;
        result += `Cost: ${fastest.cost}, Time: ${fastest.time}\n\n`;

        result += "⚖️ Balanced Flight:\n";
        result += `${balanced.flight} | ${balanced.airline}\n`;
        result += `From: ${balanced.from} → To: ${balanced.to}\n`;
        result += `Cost: ${balanced.cost}, Time: ${balanced.time}\n\n`;

        result += "-----------------------------\n";
        result += "📋 Other Available Flights:\n\n";

        routes.forEach(r => {
            result += `${r.flight} | ${r.airline}\n`;
            result += `From: ${r.from} → To: ${r.to}\n`;
            result += `Cost: ${r.cost}, Time: ${r.time}\n\n`;
        });

        res.json({cheapest: `${cheapest.flight} | Cost: ₹${cheapest.cost}, Time: ${cheapest.time} hrs`,
            fastest: `${fastest.flight} | Cost: ₹${fastest.cost}, Time: ${fastest.time} hrs`,
            balanced: `${balanced.flight} | Cost: ₹${balanced.cost}, Time: ${balanced.time} hrs`,
            pareto: pareto.map(p => `${p.flight} (₹${p.cost}, ${p.time} hrs)`).join("<br>")
        });

    } catch (err) {
        console.log(err.response?.data || err.message);
        res.send("Error fetching data");
    }
});

app.listen(3000, () => {
    console.log("Server running on port 3000");
});