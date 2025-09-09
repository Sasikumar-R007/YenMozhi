export default async function handler(req, res) {
  const url = process.env.UPSTASH_REDIS_REST_URL;
  const token = process.env.UPSTASH_REDIS_REST_TOKEN;
  const key = "yenmozhi:micState";

  // GET current value
  const r = await fetch(`${url}/get/${key}`, {
    headers: { Authorization: `Bearer ${token}` },
  });

  let value = await r.text();
  if (value === "null" || !value) value = "off"; // default

  res.status(200).json({ micState: value });
}
