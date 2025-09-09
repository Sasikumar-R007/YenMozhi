export default async function handler(req, res) {
  if (req.method !== "POST") return res.status(405).end();

  // simple auth for ESP32
  const auth = req.headers.authorization || "";
  if (auth !== `Bearer ${process.env.TOGGLE_SECRET}`) {
    return res.status(401).json({ ok: false, error: "unauthorized" });
  }

  const url = process.env.UPSTASH_REDIS_REST_URL;
  const token = process.env.UPSTASH_REDIS_REST_TOKEN;
  const key = "yenmozhi:micState";

  // read current state
  const r = await fetch(`${url}/get/${key}`, {
    headers: { Authorization: `Bearer ${token}` },
  });
  let current = await r.text();
  current = current === "on" ? "off" : "on";

  // write new state
  await fetch(`${url}/set/${key}/${current}`, {
    method: "POST",
    headers: { Authorization: `Bearer ${token}` },
  });

  res.status(200).json({ ok: true, micState: current });
}
