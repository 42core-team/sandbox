#include "con_lib.h"

void	ft_init_func(void *ptr);
void	ft_user_loop(void *ptr);

int	main(int argc, char **argv)
{
	// ft_enable_debug();
	ft_init_con("Martin", &argc, argv);
	ft_loop(&ft_init_func, &ft_user_loop, NULL);
	ft_close_con();
	return (0);
}

// this function is called once at the start of the game
void	ft_init_func(void *ptr)
{
	(void) ptr;
}

// this function is called every time new data is recieved
void	ft_user_loop(void *ptr)
{
	(void) ptr;
	// your actions go here
	ft_print_all();
}
