#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

WP *watchpoint = NULL;

void cpu_exec(uint32_t);
void cache_test(swaddr_t);
//void cacheL2_test(swaddr_t);
/* We use the ``readline'' library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");

 	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

static int cmd_c(char *args) {
	cpu_exec(-1);
	return 0;
}

static int cmd_q(char *args) {
	return -1;
}

static int cmd_si(char *args) {
	char *p;
	p = strtok(args, " ");
	if (p) {
		int temp = 0;
		int i;
		for (i = 0; p[i]; ++i)
			temp = temp * 10 + (p[i] - '0');
		cpu_exec(temp);	
 	}
	else
		cpu_exec(1);
	return 0;
}

static int cmd_w(char *args) {
	bool success = true;
	watchpoint = new_wp(args);
	int ans = expr(args, &success);
	printf("value: 0x%x\n", ans);
	return 0;
}

static int cmd_d(char *args) {
	int temp = 0;
	int i;
	for (i = 0; args[i]; ++i)
		temp = temp * 10 + (args[i] - '0');
	free_wp(temp);
	return 0;
}

static int cmd_p(char *args) {
	bool success = true;
	int temp = 0;
	temp = expr(args, &success);
	printf("0x%x\n", temp);
	return 0;
}

static int cmd_bt()
{
	uint32_t temp = cpu.ebp;
	int no = 0;
	//printf("ebp: 0x%x\n", cpu.ebp);
	while (temp)
	{
		printf("#%d  ", no);
		if (variable_check(swaddr_read((temp + 4), 4)) == 1)
		{
			//printf("zhanzhenlian\n");
			printf("0x%08x   ", swaddr_read((temp + 4), 4));
			printf("(%d %d %d %d)", swaddr_read((temp + 8), 4), swaddr_read((temp + 12), 4), swaddr_read((temp + 16), 4), swaddr_read((temp + 20), 4));
		}
		temp = swaddr_read((temp), 4);
		no++;
		printf("\n");
	}
	return 0;
}


static int cmd_info(char *args) {
	char *p;
	WP *q;
	p = strtok(args, " ");
	if (strcmp(p, "r") == 0)
	{
 		printf("eax:\t0x%08x\n", cpu.eax);
		printf("edx:\t0x%08x\n", cpu.edx);
		printf("ecx:\t0x%08x\n", cpu.ecx);
		printf("ebx:\t0x%08x\n", cpu.ebx);
		printf("ebp:\t0x%08x\n", cpu.ebp);
		printf("esi:\t0x%08x\n", cpu.esi);
		printf("edi:\t0x%08x\n", cpu.edi);
		printf("esp:\t0x%08x\n", cpu.esp);
		printf("eip:\t0x%08x\n", cpu.eip);		
		return 0;
 	}
 	if (strcmp(p, "w") == 0)
 	{
		printf("Num\tWhat\n");
		for (q = watchpoint; q != NULL; q = q->next)
 		{
			printf("%d\t%s\n", (*q).NO, (*q).expr);
		}
		return 0;
	}
	return 0;
}

static int cmd_x(char *args) {
	char *p;
	p = strtok(args, " ");
	int temp = 0;
	int i;
	for (i = 0; p[i]; ++i)
		temp = temp * 10 + (p[i] - '0');	
	p = strtok(NULL, " ");
	int address = 0;
	if (p[0] == 0)
	{
		for (i = 2; p[i]; ++i)
			address = address * 16 + ((p[i] <= '9' && p[i] >= '0') ? (p[i] - '0') : (p[i] - 'a' + 10));
	}
	else if (strcmp(p, "$eip") == 0)
	{
		address = cpu.eip;
	}
	else if (strcmp(p, "$eax") == 0)
	{
		address = cpu.eax;
	}
	else if (strcmp(p, "$ebx") == 0)
	{
		address = cpu.ebx;
	}
	else if (strcmp(p, "$ecx") == 0)
	{
		address = cpu.ecx;
	}
	else if (strcmp(p, "$edx") == 0)
	{
		address = cpu.edx;
	}
	else if (strcmp(p, "$esp") == 0)
	{
		address = cpu.esp;
	}
	else if (strcmp(p, "$ebp") == 0)
	{
		address = cpu.ebp;
	}
	else if (strcmp(p, "$esi") == 0)
	{
		address = cpu.esi;
	}
	else if (strcmp(p, "$edi") == 0)
	{
		address = cpu.edi;
	}
	int j;
	for (j = 0; j < temp; ++j) {
		printf("0x%08x:\t0x%08x\n",address + 4 * j, swaddr_read(address + 4 * j, 4));	
	}
	return 0;
}

static int cmd_cache(char *args){
	if(!args){
		printf("EXPR missed!\n");
		return 0;
	}
	bool success = false;
	hwaddr_t result = expr(args, &success);
	if(success) {
		cache_test(result);
		//cacheL2_test(result);
	}
	else
		printf("Bad EXPR\n");
	return 0;
}

/*#ifdef USE_CACHE
  static int cmd_cache(char *args) {
  if (!args) {
  printf("invalid argument\n");
  return 0;
  }
  bool succ = false;
  hwaddr_t addr = expr(args, &succ);
  if (!succ) {
  printf("invalid argument\n");
  return 1;
  }
  cache_show(addr);
  return 0;
  }
#endif*/

static int cmd_help(char *args);

static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },
	{ "si", "Single step", cmd_si},
	{ "info", "print the conditon of register or print the information of watch points", cmd_info},
	{ "x", "scan memory", cmd_x},
	{ "p", "print contents", cmd_p},
	{ "w", "set a watchpoint", cmd_w},
	{ "d", "delete a watchpoint", cmd_d},
	{ "bt", "print stack frame", cmd_bt},
	{ "cache", "Examine memory in cache\n\tADDR\tan expression for the memory address to examine", cmd_cache }

	/* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if(arg == NULL) {
		/* no argument given */
		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else {
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

void ui_mainloop() {
	while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if(args >= str_end) {
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif

		int i;
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(cmd, cmd_table[i].name) == 0) {
				if(cmd_table[i].handler(args) < 0) { return; }
				break;
			}
		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}
