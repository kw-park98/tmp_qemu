#define PAGE_SIZE (4096)
//1024 -> 4mb
#define NPAGE (1024 * 64)
#define OFFSET (0 * PAGE_SIZE)
#define LENGTH (PAGE_SIZE * NPAGE)

#define NTHREAD 3
#define TIME 5


#define MYOPEN 548
#define MYPREAD 549

#define OPEN 2
#define PREAD 17

const char *fname = "tmp.txt";
const char *cname = "compare.txt";
