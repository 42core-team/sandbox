#include "con_lib.h"

#include <stdio.h>

void	ft_user_loop(void *ptr);

int	main(int argc, char **argv)
{
	// ft_enable_debug();
	ft_init_con("Thanos", &argc, argv);
	ft_loop(NULL, &ft_user_loop, NULL);
	ft_close_con();
	return (0);
}

int nextUnitType = 5;

// this function is called every time new data is recieved
void	ft_user_loop(void *ptr)
{
	(void) ptr;

	ft_print_units();

	t_obj **units = ft_get_my_units();
	t_team *myTeam = ft_get_my_team();
	if (!units || !myTeam)
		return;

	if (ft_get_unit_config(nextUnitType)->cost <= myTeam->balance)
	{
		ft_create_unit(nextUnitType);
		nextUnitType++;
		if (nextUnitType > 5)
			nextUnitType = 1;
	}

	int i = 0;
	while (units && units[i] != 0)
	{
		t_obj *unit = units[i];
		t_obj *nearestResource = ft_get_nearest_resource(unit);
		t_obj *nearestOpponentUnit = ft_get_nearest_opponent_unit(unit);
		t_obj *nearestOpponentCore = ft_get_first_opponent_core();

		switch (units[i]->s_unit.type_id)
		{
			case UNIT_TANK:
				ft_travel_attack(unit, nearestOpponentCore);
				break;
			case UNIT_WORKER:
				ft_travel_attack(unit, nearestResource);
				break;
			case UNIT_HEALER:
				if (nearestOpponentCore)
					ft_travel_attack(unit, ft_get_nearest_team_unit(nearestOpponentCore));
				break;
			default:
				if (nearestOpponentUnit != NULL)
					ft_travel_attack(unit, nearestOpponentUnit);
				else
					ft_travel_attack(unit, nearestOpponentCore);
				break;
		}
		i++;
	}
	free(units);
}
