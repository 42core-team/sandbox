#include "con_lib.h"

void	ft_user_loop(void *ptr);

int	main(int argc, char **argv)
{
	// ft_enable_debug();
	ft_init_con("Gridmaster", &argc, argv);
	ft_loop(NULL, &ft_user_loop, NULL, NULL);
	ft_close_con();
	return (0);
}

// this function is called every time new data is recieved
void	ft_user_loop(void *ptr)
{
	(void) ptr;

	ft_create_unit(UNIT_WARRIOR);

	t_obj **units = ft_get_my_units();

	for (int i = 0; units && units[i]; i++)
	{
		ft_travel_to_pos(units[i], ft_get_first_opponent_core()->pos);
	}
}
