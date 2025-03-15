#include "con_lib.h"

#include <time.h>

void	ft_user_loop(void *ptr);

t_pos *find_path(t_pos start, t_pos target, t_obj *unit, int *path_length);

int	main(int argc, char **argv)
{
	srand(time(NULL));
	// ft_enable_debug();
	ft_init_con("Gridmaster", argc, argv);
	ft_loop(NULL, &ft_user_loop, NULL, NULL);
	ft_close_con();
	return (0);
}

int nextUnit = 0;

void move_unit_to(t_obj *unit, t_pos target)
{
	t_pos *path;
	int path_length;

	path = find_path(unit->pos, target, unit, &path_length);
	if (path && path_length > 1)
	{
		ft_travel_to_pos(unit, path[1]);
		free (path);
	}
	else
		ft_travel_to_pos(unit, target);
}

void	ft_user_loop(void *ptr)
{
	(void) ptr;

	// ft_print_all();
	ft_print_units();

	if (!ft_get_my_core())
	{
		printf("No core found\n");
		return;
	}
	if (!ft_get_unit_config(nextUnit))
	{
		printf("No unit config found for unit %d\n", nextUnit);
		return;
	}

	if (ft_get_my_core()->s_core.balance >= ft_get_unit_config(nextUnit)->cost)
	{
		ft_create_unit(nextUnit);
		nextUnit++;
		if (nextUnit > 4)
			nextUnit = 0;
	}

	t_obj **units = ft_get_my_units();

	for (int i = 0; units && units[i]; i++)
	{
		t_obj *unit = units[i];

		int typeId = unit->s_unit.type_id;
		if (typeId == UNIT_WARRIOR)
		{
			t_obj * nearestOpponent = ft_get_nearest_opponent_unit(ft_get_my_core());
			if (nearestOpponent)
				move_unit_to(unit, nearestOpponent->pos);
			else
				move_unit_to(unit, ft_get_nearest_opponent_core(unit)->pos);
		}
		else if (typeId == UNIT_MINER)
		{
			t_obj * nearestResource = ft_get_nearest_resource(unit);
			if (nearestResource)
				move_unit_to(unit, nearestResource->pos);
			else
				move_unit_to(unit, ft_get_nearest_opponent_core(unit)->pos);
		}
		else if (typeId == UNIT_CARRIER)
		{
			bool isTouchingCore = ft_distance(unit, ft_get_my_core()) <= 1;
			bool isTouchingUnitWithMoney = false;
			t_obj *closestUnitWithMoney = NULL;

			t_obj **units = game.units;
			double distance = 999999;
			for (int j = 0; units && units[j]; j++)
			{
				if (units[j]->state != STATE_ALIVE)
					continue;
				if (units[j]->s_unit.balance > 0)
				{
					if (ft_distance(unit, units[j]) <= 1)
					{
						isTouchingUnitWithMoney = true;
						closestUnitWithMoney = units[j];
						break;
					}
					if (ft_distance(unit, units[j]) < distance)
					{
						closestUnitWithMoney = units[j];
						distance = ft_distance(unit, units[j]);
					}
					break;
				}
			}
			if (isTouchingCore && unit->s_unit.balance > 0)
				ft_transfer_money(unit, ft_get_my_core(), unit->s_unit.balance);
			else if (isTouchingUnitWithMoney)
				ft_transfer_money(closestUnitWithMoney, unit, closestUnitWithMoney->s_unit.balance);

			if (unit->s_unit.balance <= 0 && closestUnitWithMoney != NULL)
				move_unit_to(unit, closestUnitWithMoney->pos);
			else
				move_unit_to(unit, ft_get_my_core()->pos);
		}
		else if (typeId == UNIT_BUILDER)
		{
			t_pos unitPos = unit->pos;
			t_pos targetPos = {10, 10};
			t_pos buildPos = {11, 10};

			if (unitPos.x == targetPos.x && unitPos.y == targetPos.y)
			{
				ft_build(unit, buildPos);
			}
			else
			{
				ft_travel_to_pos(unit, targetPos);
			}
		}
		else if (typeId == UNIT_ARCHER)
		{
			t_pos pos1 = {0, 10};
			t_pos pos2 = {0, 11};
			t_pos unitPos = unit->pos;

			if (unitPos.x != pos1.x || unitPos.y != pos1.y || unitPos.x != pos2.x || unitPos.y != pos2.y)
				ft_travel_to_pos(unit, pos1);
			if (unitPos.x == pos1.x && unitPos.y == pos1.y)
				ft_travel_to_pos(unit, pos2);
			if (unitPos.x == pos2.x && unitPos.y == pos2.y)
				ft_travel_to_pos(unit, pos1);
		}
	}
}
