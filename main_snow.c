#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <fcntl.h> 
#include <sys/time.h>

#define N 200

int px[N];
int py[N];

int MX;
int MY;

int speed = 1;

int top_line = 1;

pthread_t thread;

void init_random()
{
	unsigned int ticks;
	struct timeval tv;
	int fd;
	gettimeofday(&tv, NULL);
	ticks = tv.tv_sec + tv.tv_usec;
	fd = open("/dev/urandom", O_RDONLY);
	if(fd > 0)
	{
		unsigned int r;
		int i;
		for(i = 0; i < 512; i++)
		{
			read(fd, &r, sizeof(r));
			ticks += r;
		}
		close(fd);
	}
	srand(ticks);
	printf("init finished");
}

unsigned int new_rand()
{
	int fd;
	unsigned int n = 0;
	fd = open("/dev/urandom", O_RDONLY);
	if(fd > 0)
	{
		read(fd, &n, sizeof(n));
	}
	close(fd);
	return n;
}

unsigned int new_rand2(int a, int b)
{
	unsigned int n = new_rand();
	unsigned int rs = a + n % (b - a);
	return rs;
}

void error_quit(char *msg)   //错误处理
{
	perror(msg);
	exit(1);
}

void *thread_fn(void* win)
{
	// 利用线程处理按键事件
	while(1)	
	{
		char c = getchar();
		if(c == 'q')
		{
			delwin( (WINDOW*)win );
			endwin();
			exit(0);
		}

	}
	exit(0);
}

int main(int argc,char* argv[])
{
	init_random();

	int i, j;
	WINDOW* win;

	initscr();
	raw();
	noecho();
	curs_set(0);

	MX = LINES;
	MY = COLS;

	for(i = 0; i < N; i++)
	{
		px[i] = (int)new_rand2(top_line, MX);
		py[i] = (int)new_rand2(1, MY);
	}

	if((win=newwin(LINES,COLS,0,0))==NULL)
		error_quit("Create New Window Error!");

	for(i=0;i<N;i++)
	{	
		mvprintw(px[i], py[i], "*");
	}

	refresh();
	box(win, 0, 0);
	wrefresh(win);

	int err = pthread_create(&thread, NULL, thread_fn, win);
	if(err == -1)
		error_quit("Create thread Error!");
	while(1)
	{			
		for(i = 0; i < N; i ++)
		{
			px[i] += speed;
			if(px[i] > MX)
			{
				px[i] = top_line + 1;
				py[i] = (int)new_rand2(top_line, MY);
			}
		}

		wclear(win);

		//mvwprintw(win, 1, 2, "Name:   Snow    version: 1.0    date: 2017/03/11");  
		mvwprintw(win, 1, 2, "ProgramName: Snow, Snow Author: wenlding, Blog: http://blog.csdn.net/qq_26437925");  
		//mvwprintw(win, 3, 2, "Usage:  Press 'f' to speed up, 's' to speed down,'q' to quit.");  
		//mvwprintw(win, 4, 2, "=============================================================="); 

		//mvwprintw(win, x, y, "*");

		for(i = 0; i < N; i ++)
		{	
			mvwprintw(win, px[i], py[i], "*");
		}

		box(win, 0, 0);
		wrefresh(win);

		MX = LINES;
		MY = COLS;

		sleep(1);
	}

	return 0;
}
