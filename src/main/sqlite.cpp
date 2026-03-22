#include <sqlite3.h>

#include <print>

static constexpr auto db_file = std::string_view("./test.db");

int main() {
  sqlite3 *db = nullptr;
  sqlite3_stmt *stmt = nullptr;

  std::println("sqlite3 version:{}", sqlite3_libversion());
  std::println("Database file:{}", db_file);

  // open the database
  if (auto error = sqlite3_open(db_file.data(), &db); error == SQLITE_OK) {
    std::println("Database open!");
  } else {
    std::println("Database error:{}", sqlite3_errstr(error));
  }

  return 0;
}