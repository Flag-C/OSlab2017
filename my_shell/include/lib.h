#define SCR_WIDTH  320
#define SCR_HEIGHT 200
#define SCR_SIZE ((SCR_WIDTH) * (SCR_HEIGHT))
#define VMEM_ADDR  ((uint8_t*)0xc00a0000)

void init_intr();
void init_idt();
void draw_string(const char *str, int x, int y, int color);
void draw_pixel_u(int, int, int);
bool query_key_u(int ch);
void init_timer();
int key_query(int index);
void	printf(const char *fmt, ...);
void init_screen();
void shell_enter();
void shell_print_string(char str[], int color);