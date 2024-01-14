#include "con_lib.h"

void	ft_user_loop(void);

int	main(int argc, char **argv)
{
	ft_enabled_debug();
	ft_init_con(&argc, argv);
	ft_loop(&ft_user_loop);
	ft_close_con();
	return (0);
}

// this function is called every time new data is recieved
void	ft_user_loop(void)
{
	// your actions go here
	ft_print_status(game.status);
	ft_print_teams(game.teams);
	ft_print_cores(game.cores);
	ft_print_resources(game.resources);
	ft_print_units(game.units);
}
