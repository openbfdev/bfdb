#include <cassert>
#include <memtable.hpp>

int	main(int argc, char **argv) {
    bfdb::memtable table;
    uint64_t sequence = 1;
    std::string value;
    
    table.put("hello", "world", sequence++, 1);
    table.put("hello", "world1", sequence++, 1);
    table.put("hello", "world2", sequence++, 1);
    table.put("hello", "world3", sequence++, 1);


    table.put("hello1", "world", sequence++, 1);
    table.put("hello2", "world1", sequence++, 1);
    table.put("hello3", "world2", sequence++, 1);
    table.put("hello4", "world3", sequence++, 1);

    

    table.get("hello", value, 1);
    assert(value == "world");

    table.get("hello", value, 2);
    assert(value == "world1");

    table.get("hello", value, 3);
    assert(value == "world2");

    table.get("hello", value, 4);
    assert(value == "world3");
    
    
    table.get("hello1", value, 100);
    assert(value == "world");
    

    table.get("hello4", value, 100);
    assert(value == "world3");

    return 0;
}
