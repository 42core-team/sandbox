#include "con_lib.h"
#include "event_handler.h"

#include <time.h>

// Event handlers. Place your code in these functions.
void ft_on_tick(unsigned long tick, void *custom_data);
void ft_on_start(void *custom_data);
void ft_on_exit(void *custom_data);
void ft_on_object_ticked(t_obj *obj, unsigned long tick, void *custom_data);
void ft_on_object_state_change(t_obj *obj, t_obj_state old_state, t_obj_state new_state, void *custom_data);
void ft_on_object_pos_change(t_obj *obj, t_pos old_pos, t_pos new_pos, void *custom_data);
void ft_on_object_balance_change(t_obj *obj, unsigned long old_balance, unsigned long new_balance, void *custom_data);
void ft_on_object_health_change(t_obj *obj, unsigned long old_hp, unsigned long new_hp, void *custom_data);
void ft_on_unit_build(t_obj *unit, t_pos pos, void *custom_data);
void ft_on_unit_transfer_money(t_obj *unit, t_obj *target, unsigned long amount, void *custom_data);
void ft_on_unit_attack(t_obj *unit, t_obj *target, unsigned long damage, void *custom_data);

// save whatever you want in this struct, or in the custom_data field of every t_obj
typedef struct s_custom_data
{
	// int		foo;
}	t_custom_data;

t_pos *find_path(t_pos start, t_pos target, t_obj *unit, int *path_length);

int	main(int argc, char **argv)
{
	// ft_enable_debug();
	ft_init_con("Gridmaster", argc, argv);

	t_event_handler handler = {0};

	handler.on_tick = &ft_on_tick;
	handler.on_start = &ft_on_start;
	handler.on_exit = &ft_on_exit;
	handler.on_object_ticked = &ft_on_object_ticked;
	handler.on_object_state_change = &ft_on_object_state_change;
	handler.on_object_pos_change = &ft_on_object_pos_change;
	handler.on_object_balance_change = &ft_on_object_balance_change;
	handler.on_object_health_change = &ft_on_object_health_change;
	handler.on_unit_build = &ft_on_unit_build;
	handler.on_unit_transfer_money = &ft_on_unit_transfer_money;
	handler.on_unit_attack = &ft_on_unit_attack;

	t_custom_data custom_data;
	// custom_data.foo = 42;

	ft_loop(handler, &custom_data);
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

void ft_on_tick(unsigned long tick, void *custom_data)
{
	(void)tick;
	(void)custom_data;

	if (ft_get_my_core()->s_core.balance >= ft_get_unit_config(nextUnit)->cost)
	{
		ft_create_unit(nextUnit);
		nextUnit++;
		if (nextUnit > 4)
			nextUnit = 0;
	}
}

void ft_on_start(void *custom_data)
{
	(void)custom_data;
}

void ft_on_exit(void *custom_data)
{
	(void)custom_data;
}



void ft_on_object_ticked(t_obj *obj, unsigned long tick, void *custom_data)
{
	(void)obj;
	(void)tick;
	(void)custom_data;

	if (obj->state != STATE_ALIVE)
		return;
	if (obj->type != OBJ_UNIT)
		return;
	if (obj->s_unit.team_id != ft_get_my_core()->s_core.team_id)
		return;

	t_obj *unit = obj;
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

void ft_on_object_state_change(t_obj *obj, t_obj_state old_state, t_obj_state new_state, void *custom_data)
{
	(void)obj;
	(void)old_state;
	(void)new_state;
	(void)custom_data;
}

void ft_on_object_pos_change(t_obj *obj, t_pos old_pos, t_pos new_pos, void *custom_data)
{
	(void)obj;
	(void)old_pos;
	(void)new_pos;
	(void)custom_data;
}

void ft_on_object_balance_change(t_obj *obj, unsigned long old_balance, unsigned long new_balance, void *custom_data)
{
	(void)obj;
	(void)old_balance;
	(void)new_balance;
	(void)custom_data;
}

void ft_on_object_health_change(t_obj *obj, unsigned long old_hp, unsigned long new_hp, void *custom_data)
{
	(void)obj;
	(void)old_hp;
	(void)new_hp;
	(void)custom_data;
}



void ft_on_unit_build(t_obj *unit, t_pos pos, void *custom_data)
{
	(void)unit;
	(void)pos;
	(void)custom_data;
}

void ft_on_unit_transfer_money(t_obj *unit, t_obj *target, unsigned long amount, void *custom_data)
{
	(void)unit;
	(void)target;
	(void)amount;
	(void)custom_data;
}

void ft_on_unit_attack(t_obj *unit, t_obj *target, unsigned long damage, void *custom_data)
{
	(void)unit;
	(void)target;
	(void)damage;
	(void)custom_data;
}
