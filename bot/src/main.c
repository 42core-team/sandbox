#include "con_lib.h"

void	ft_user_loop(void);

int	main(int argc, char **argv)
{
	// ft_enable_debug();
	ft_init_con(&argc, argv);
	ft_loop(&ft_user_loop);
	ft_close_con();
	return (0);
}

// this function is called every time new data is recieved
void	ft_user_loop(void)
{
	// your actions go here
	ft_print_teams(game.teams);
	ft_create(2);
}
