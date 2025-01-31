#include "con_lib.h"

void	ft_user_loop(void *ptr);

int	main(int argc, char **argv)
{
	// ft_enable_debug();
	ft_init_con("Starlord", &argc, argv);
	ft_loop(NULL, &ft_user_loop, NULL);
	ft_close_con();
	return (0);
}

// this function is called every time new data is recieved
void	ft_user_loop(void *ptr)
{
	(void) ptr;

	if (ft_get_my_team()->balance >= ft_get_unit_config(UNIT_WARRIOR)->cost)
		ft_create_unit(UNIT_WARRIOR);

	t_obj **my_units = ft_get_my_units();

	int ind = 0;
	while (my_units[ind] != NULL)
	{
		ft_travel_attack(my_units[ind], ft_get_first_opponent_core());
		ind++;
	}

	free(my_units);
}
