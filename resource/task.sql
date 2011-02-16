
CREATE TABLE pages (
  id INTEGER PRIMARY KEY,
  url TEXT,
  parsed NUMERIC
);

CREATE TABLE media (
  id INTEGER PRIMARY KEY,
  from_page INTEGER,
  url TEXT,
  downloaded NUMERIC
);

