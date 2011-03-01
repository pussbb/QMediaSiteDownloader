PRAGMA synchronous = OFF;
PRAGMA temp_store = MEMORY;
PRAGMA quick_check; 
PRAGMA journal_mode =  MEMORY;
CREATE TABLE pages (
  id INTEGER PRIMARY KEY,
  url TEXT UNIQUE,
  parsed NUMERIC
);

CREATE TABLE media (
  id INTEGER PRIMARY KEY,
  from_page INTEGER,
  url TEXT UNIQUE,
  downloaded NUMERIC
);

