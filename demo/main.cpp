#include <db.hpp>
int	main(int argc, char **argv) {
    bfdb::bfdb db("hello.db");

    if (db.open() != BFDB_OK) {
        return -1;
    }

    if (db.put("hello", "world") != BFDB_OK) {
        return -1;
    }

    return 0;
}
