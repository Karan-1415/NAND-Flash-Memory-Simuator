#include <stdio.h>
#include <stdint.h>

#define BLOCKS 8192
#define PAGES 128
#define MAX_WEAR 1000
#define LOGICAL_PAGES 10000
#define WEAR_THRESHOLD 100

typedef enum {
    Page_free,
    Page_valid,
    Page_invalid,
} page_status;

typedef struct {
    int data;
    page_status status;
    uint32_t add;
} page;

typedef struct {
    int block;
    int page;
} PhysicalAddress;

PhysicalAddress L2P[LOGICAL_PAGES];
page memory[BLOCKS][PAGES];
int wear_counter[BLOCKS];

void mem_initialization() {
    for (int i = 0; i < BLOCKS; i++) {
        wear_counter[i] = 0;
        for (int j = 0; j < PAGES; j++) {
            memory[i][j].data = -1;
            memory[i][j].status = Page_free;
            memory[i][j].add = (i << 7) | j;
        }
    }
    for (int i = 0; i < LOGICAL_PAGES; i++) {
        L2P[i].block = -1;
        L2P[i].page = -1;
    }
}

int find_the_least_wear() {
    int min = MAX_WEAR + 1;
    int min_block = -1;
    for (int i = 0; i < BLOCKS; i++) {
        if (memory[i][0].status == Page_free && wear_counter[i] < min) {
            min = wear_counter[i];
            min_block = i;
        }
    }
    return min_block;
}

void update_L2P(int logical_page_number, int block, int page) {
    if (L2P[logical_page_number].block != -1) {
        int old_block = L2P[logical_page_number].block;
        int old_page = L2P[logical_page_number].page;
        memory[old_block][old_page].status = Page_invalid;
    }
    L2P[logical_page_number].block = block;
    L2P[logical_page_number].page = page;
}

int read(int logical_page_number) {
    if (logical_page_number >= LOGICAL_PAGES) {
        printf("Invalid Page\n");
        return -1;
    }

    int block = L2P[logical_page_number].block;
    int page = L2P[logical_page_number].page;

    if (block == -1 || page == -1) {
        printf("No valid mapping\n");
        return -1;
    }

    if (memory[block][page].status != Page_valid) {
        printf("Page not valid\n");
        return -1;
    }

    return memory[block][page].data;
}

void erase_block(int block) {
    for (int j = 0; j < PAGES; j++) {
        memory[block][j].data = -1;
        memory[block][j].status = Page_free;
        memory[block][j].add = (block << 7) | j;
    }
    wear_counter[block]++;
}

void perform_dynamic_wear_leveling() {
    int max_wear = -1, min_wear = MAX_WEAR + 1;
    int hot_block = -1, cold_block = -1;

    for (int i = 0; i < BLOCKS; i++) {
        if (wear_counter[i] > max_wear) {
            max_wear = wear_counter[i];
            hot_block = i;
        }
        if (memory[i][0].status == Page_free && wear_counter[i] < min_wear) {
            min_wear = wear_counter[i];
            cold_block = i;
        }
    }

    if (hot_block == -1 || cold_block == -1 || (max_wear - min_wear < WEAR_THRESHOLD))
        return;

    for (int j = 0; j < PAGES; j++) {
        if (memory[hot_block][j].status == Page_valid) {
            for (int k = 0; k < PAGES; k++) {
                if (memory[cold_block][k].status == Page_free) {
                    memory[cold_block][k].data = memory[hot_block][j].data;
                    memory[cold_block][k].status = Page_valid;

                    for (int l = 0; l < LOGICAL_PAGES; l++) {
                        if (L2P[l].block == hot_block && L2P[l].page == j) {
                            update_L2P(l, cold_block, k);
                            break;
                        }
                    }

                    memory[hot_block][j].status = Page_invalid;
                    break;
                }
            }
        }
    }

    erase_block(hot_block);
}

void main() {
    mem_initialization();

    int logical_page = 42;
    int block = find_the_least_wear();

    if (block == -1) {
        printf("No block available\n");
        return;
    }

    int page = -1;
    for (int j = 0; j < PAGES; j++) {
        if (memory[block][j].status == Page_free) {
            page = j;
            break;
        }
    }

    if (page == -1) {
        printf("No free page in block\n");
        return;
    }

    memory[block][page].data = 1234;
    memory[block][page].status = Page_valid;

    wear_counter[block]++;
    update_L2P(logical_page, block, page);

    printf("L2P[%d] = Block %d, Page %d\n", logical_page, L2P[logical_page].block, L2P[logical_page].page);

    perform_dynamic_wear_leveling();
}
