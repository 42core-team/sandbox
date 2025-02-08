#include "con_lib.h"

void	ft_user_loop(void *ptr);

int	main(int argc, char **argv)
{
	// ft_enable_debug();
	ft_init_con("Starlord", &argc, argv);
	ft_loop(NULL, &ft_user_loop, NULL, NULL);
	ft_close_con();
	return (0);
}

// this function is called every time new data is recieved
void	ft_user_loop(void *ptr)
{
	(void) ptr;
}
