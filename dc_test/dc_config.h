#define PAGE_SIZE (4096)
//1024 -> 4mb
//1024 * 256 -> 1G
#define NPAGE (1024 * 256)
#define OFFSET (0 * PAGE_SIZE)
#define LENGTH (PAGE_SIZE * NPAGE)

#define MYOPEN 548
#define MYPREAD 549

#define OPEN 2
#define PREAD 17

char buf[LENGTH];
const char *fname = "tmp.txt";
