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

int nextUnitType = 1;

// this function is called every time new data is recieved
void	ft_user_loop(void *ptr)
{
	(void) ptr;

	t_obj **units = ft_get_my_units();
	t_team *myTeam = ft_get_my_team();
	if (!units || !myTeam)
		return;

	if (ft_get_unit_config(nextUnitType)->cost <= myTeam->balance)
	{
		ft_create_unit(nextUnitType);
		nextUnitType++;
		if (nextUnitType > 4)
			nextUnitType = 1;
	}

	int i = 0;
	while (units && units[i] != 0)
	{
		switch (units[i]->s_unit.type_id)
		{
			case UNIT_TANK:
				ft_travel_attack(units[i], ft_get_first_opponent_core());
				break;
			case UNIT_WORKER:
				ft_travel_attack(units[i], ft_get_nearest_resource(units[i]));
				break;
			default:
				if (ft_get_nearest_opponent_unit(units[i]) != NULL)
					ft_travel_attack(units[i], ft_get_nearest_opponent_unit(units[i]));
				else
					ft_travel_attack(units[i], ft_get_first_opponent_core());
				break;
		}
		i++;
	}
	free(units);
}
