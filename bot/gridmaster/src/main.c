#include "con_lib.h"

void	ft_user_loop(void *ptr);

int	main(int argc, char **argv)
{
	// ft_enable_debug();
	ft_init_con("Gridmaster", argc, argv);
	ft_loop(NULL, &ft_user_loop, NULL, NULL);
	ft_close_con();
	return (0);
}

int nextUnit = 0;

// this function is called every time new data is recieved
void	ft_user_loop(void *ptr)
{
	(void) ptr;

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
		if (nextUnit > 3)
			nextUnit = 0;
	}

	t_obj **units = ft_get_my_units();
	// ft_print_units();
	// ft_print_cores();

	for (int i = 0; units && units[i]; i++)
	{
		int typeId = units[i]->s_unit.type_id;
		if (typeId == UNIT_WARRIOR)
			ft_travel_to_pos(units[i], ft_get_nearest_opponent_core(units[i])->pos);
		else if (typeId == UNIT_MINER)
		{
			bool isTouchingCarrier = false;
			t_obj **units = game.units;
			for (int j = 0; units && units[j]; j++)
			{
				if (units[j]->state != STATE_ALIVE)
					continue;
				if (ft_distance(units[i], units[j]) <= 1)
				{
					isTouchingCarrier = true;
					break;
				}
			}

			if (isTouchingCarrier && units[i]->s_unit.balance > 0)
			{
				ft_transfer_money(units[i], ft_get_my_core(), units[i]->s_unit.balance);
				continue;
			}

			ft_travel_to_pos(units[i], ft_get_nearest_resource(units[i])->pos);
		}
		else if (typeId == UNIT_CARRIER)
		{
			bool isTouchingCore = ft_distance(units[i], ft_get_my_core()) <= 1;
			bool isTouchingUnitWithMoney = false;
			t_obj *unitWithMoney = NULL;
			t_obj **units = game.units;
			for (int j = 0; units && units[j]; j++)
			{
				if (units[j]->state != STATE_ALIVE)
					continue;
				if (units[j]->s_unit.balance > 0 && ft_distance(units[i], units[j]) <= 1)
				{
					isTouchingUnitWithMoney = true;
					unitWithMoney = units[j];
					break;
				}
			}

			if (isTouchingCore && units[i]->s_unit.balance > 0)
			{
				ft_transfer_money(units[i], ft_get_my_core(), units[i]->s_unit.balance);
				continue;
			}
			else if (isTouchingUnitWithMoney)
			{
				ft_transfer_money(unitWithMoney, units[i], unitWithMoney->s_unit.balance);
				continue;
			}

			if (units[i]->s_unit.balance <= 0)
			{
				t_obj *nearestMiner = NULL;
				float nearestDistance = 999999;
				for (int j = 0; units && units[j]; j++)
				{
					if (units[j]->state != STATE_ALIVE)
						continue;
					if (units[j]->s_unit.type_id == UNIT_MINER)
					{
						float distance = ft_distance(units[i], units[j]);
						if (distance < nearestDistance)
						{
							nearestMiner = units[j];
							nearestDistance = distance;
						}
					}
				}
				ft_travel_to_pos(units[i], nearestMiner->pos);
			}
			else
			{
				ft_travel_to_pos(units[i], ft_get_my_core()->pos);
			}
		}
		else if (typeId == UNIT_BUILDER)
		{
			t_pos unitPos = units[i]->pos;
			t_pos targetPos = {10, 10};
			t_pos buildPos = {11, 10};

			if (unitPos.x == targetPos.x && unitPos.y == targetPos.y)
			{
				ft_build(units[i], buildPos);
			}
			else
			{
				ft_travel_to_pos(units[i], targetPos);
			}
		}
	}
}
