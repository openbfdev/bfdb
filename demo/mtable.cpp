#include <cassert>
#include <cstdio>
#include <cstring>
#include <memtable.hpp>
#include <stdint.h>
#include <string>
#include <iostream>
#include <stdlib.h>

#define BFDB_OK 0
#define BFDB_ERR -1

static uint64_t sequence = 0;

typedef int (*bfdb_command_pt)(bfdb::memtable &db, int argc, const char *argv[]);
typedef struct bfdb_command_s bfdb_command_t;

struct bfdb_command_s {
    const char *name;
    int min_argc;
    int max_argc;
    bfdb_command_pt op;
};

int bfdb_command_get(bfdb::memtable &db, int argc, const char *argv[]) {
    std::string value;
    const char *key;
    key = argv[1];

    db.get(key, value, sequence);

    std::cout << value << std::endl;
    return BFDB_OK;
}

int bfdb_command_set(bfdb::memtable &db, int argc, const char *argv[]) {
    const char *key, *value;
    key = argv[1];
    value = argv[2];

    return db.put(key, value, ++sequence, bfdb::memtable::MTABLE_PUT_INSERT);
}

int bfdb_command_del(bfdb::memtable &db, int argc, const char *argv[]) {
    const char *key;
    key = argv[1];

    return db.put(key, "", ++sequence, bfdb::memtable::MTABLE_PUT_DELETE);
}

static bfdb_command_t bfdb_cmds[] = {
    {
        .name = "get",
        .min_argc = 2,
        .max_argc = 2,
        .op = bfdb_command_get,
    },
    {
        .name = "set",
        .min_argc = 3,
        .max_argc = 3,
        .op = bfdb_command_set,
    },
    {
        .name = "del",
        .min_argc = 2,
        .max_argc = 2,
        .op = bfdb_command_del
    },
};

int argcv_parser_helper(const char *cmd) {
    const char *p;
    int num=0;
    int word_start = 1;
    p = cmd;
    while(*p) {
        if((*p == '\r') || (*p == '\n')){
            break;
        }
        if((*p == ' ') || (*p == '\t')){
            word_start = 1;
            p++;
            continue;
        }
        if(word_start){
            num++;
            word_start = 0;
        }
        p++;
    }
    return num;
}

int argcv_parser(const char *cmd, char ***argv) {
    int argc = 0, head_size = 0, word_start =1, num = 0;
    char **block, *p;
    argc = argcv_parser_helper(cmd);
    if (argc <= 0) {
        return -1;
    }
    head_size = argc * sizeof(**argv);

    block = (char **)::malloc(strlen(cmd) + 1 + head_size);
    if (!block) {
        return -ENOMEM;
    }
    *argv = block;
    p = (char *)block + head_size;
    strcpy(p, cmd);
    while(*p){
        if((*p == '\r') || (*p == '\n')){
            *p = '\0';
            break;
        }
        if((*p == ' ') || (*p == '\t')){
            word_start = 1;
            *p = '\0';
            p++;
            continue;
        }
        assert(argc >= num);
        if(word_start){
            block[num++] = p;
            word_start = 0;
        }
        p++;
    }

    // printf("command debug: this command is:%s\n", cmd);
    return argc;
}

bfdb_command_t *bfdb_find_command(const char *name) {
    bfdb_command_t *cmd;
    int i;
    for (i = 0; i < 3; i++) {
        cmd = &bfdb_cmds[i];
        if (strcmp(name, cmd->name) == 0) {
            return cmd;
        }
    }

    return NULL;
}

int bfdb_command_argv(bfdb::memtable &table, int argc, const char *argv[]) {
    bfdb_command_t *cmd;
    cmd = bfdb_find_command(argv[0]);
    if (cmd == NULL) {
        return BFDB_ERR;
    }

    if (argc >= cmd->min_argc && argc <= cmd->max_argc) {
        return cmd->op(table, argc, argv);
    }

    std::cout << "command format error" << std::endl;
    return BFDB_ERR;
}

int bfdb_command(bfdb::memtable &table, char *cmd) {
    int argc, rc = 0;
    char **argv;
    argc = argcv_parser(cmd, &argv);
    if (argc > 0) {
        rc = bfdb_command_argv(table, argc, (const char **)argv);
        if (argv) {
            free(argv);
        }
    }

    return rc;
}

int	main(int argc, const char **argv) {
    bfdb::memtable table;
    char buf[1024];

    for (;;) {
        std::cout << "mtable>>";
        std::cin.getline(buf, 1024);
        if (bfdb_command(table, buf) != BFDB_OK) {

            // return -1;
        }
    }
    return 0;
}
