#include "con_lib.h"
#include "event_handler.h"

#include <time.h>
#include <stdio.h>

// Event handlers. Place your code in these functions.
void ft_on_tick(unsigned long tick, void *custom_data);
// void ft_on_start(void *custom_data);
// void ft_on_exit(void *custom_data);
void ft_on_object_ticked(t_obj *obj, unsigned long tick, void *custom_data);
// void ft_on_object_state_change(t_obj *obj, t_obj_state old_state, t_obj_state new_state, void *custom_data);
// void ft_on_object_pos_change(t_obj *obj, t_pos old_pos, t_pos new_pos, void *custom_data);
// void ft_on_object_balance_change(t_obj *obj, unsigned long old_balance, unsigned long new_balance, void *custom_data);
// void ft_on_object_health_change(t_obj *obj, unsigned long old_hp, unsigned long new_hp, void *custom_data);
// void ft_on_unit_build(t_obj *unit, t_pos pos, void *custom_data);
// void ft_on_unit_transfer_money(t_obj *unit, t_obj *target, unsigned long amount, void *custom_data);
// void ft_on_unit_drop_money(t_obj *unit, t_pos pos, unsigned long amount, void *custom_data);
// void ft_on_unit_attack(t_obj *unit, t_obj *target, unsigned long damage, void *custom_data);

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
	srand((unsigned)time(NULL));

	t_event_handler handler = {0};

	handler.on_tick = &ft_on_tick;
	// handler.on_start = &ft_on_start;
	// handler.on_exit = &ft_on_exit;
	handler.on_object_ticked = &ft_on_object_ticked;
	// handler.on_object_state_change = &ft_on_object_state_change;
	// handler.on_object_pos_change = &ft_on_object_pos_change;
	// handler.on_object_balance_change = &ft_on_object_balance_change;
	// handler.on_object_health_change = &ft_on_object_health_change;
	// handler.on_unit_build = &ft_on_unit_build;
	// handler.on_unit_transfer_money = &ft_on_unit_transfer_money;
	// handler.on_unit_drop_money = &ft_on_unit_drop_money;
	// handler.on_unit_attack = &ft_on_unit_attack;

	t_custom_data custom_data;
	// custom_data.foo = 42;

	ft_loop(handler, &custom_data);
	ft_close_con();
	return (0);
}

void move_unit_to(t_obj *unit, t_pos target)
{
	t_pos *path;
	int path_length;

	printf("[PTHF] moving unit %lu from (%d,%d) to (%d,%d)\n",
		unit->id, (int)unit->pos.x, (int)unit->pos.y,
		(int)target.x, (int)target.y);

	path = find_path(unit->pos, target, unit, &path_length);
	if (!path) {
		printf("[PTHF] no path from (%d,%d) to (%d,%d)\n",
				(int)unit->pos.x, (int)unit->pos.y,
				(int)target.x,     (int)target.y);
		ft_travel_to_pos(unit, target);
		return;
	}

	printf("[PTHF] path from (%d,%d) → (%d,%d), length=%d:\n    ",
			(int)unit->pos.x, (int)unit->pos.y,
			(int)target.x,    (int)target.y,
			path_length);
	for (int i = 0; i < path_length; i++) {
		printf("(%d,%d)%s",
				(int)path[i].x, (int)path[i].y,
				(i + 1 < path_length) ? " → " : "\n");
	}

	if (path_length > 1)
	{
		t_pos  next = path[1];
		t_obj *obj  = ft_get_obj_at_pos(next);

		if (obj && obj->type == OBJ_UNIT &&
			obj->s_unit.team_id == unit->s_unit.team_id)
		{
			free(path);
			printf("[PTHF] found friendly unit at (%d,%d), skipping move\n", (int)obj->pos.x, (int)obj->pos.y);
			return;
		}
		if (obj && obj->type == OBJ_CORE &&
			obj->s_core.team_id == unit->s_unit.team_id)
		{
			free(path);
			printf("[PTHF] found friendly core at (%d,%d), skipping move\n", (int)obj->pos.x, (int)obj->pos.y);
			return;
		}

		printf("[PTHF] moving unit to (%d,%d)\n", (int)next.x, (int)next.y);
		ft_move(unit, next);
		ft_attack(unit, next);
	}
	else
	{
		ft_move(unit, target);
	}

	free(path);
}

int nextUnit = 1;

void ft_on_tick(unsigned long tick, void *custom_data)
{
	(void)tick;
	(void)custom_data;

	if (ft_get_my_core()->s_core.balance >= ft_get_unit_config(nextUnit)->cost)
	{
		ft_create_unit(nextUnit);
		nextUnit++;
		if (nextUnit > 2)
			nextUnit = 0;
	}
}

void ft_on_object_ticked(t_obj *unit, unsigned long tick, void *custom_data)
{
	(void)tick;
	(void)custom_data;

	if (unit->state != STATE_ALIVE)
		return;
	if (unit->type != OBJ_UNIT)
		return;
	if (unit->s_unit.team_id != ft_get_my_core()->s_core.team_id)
		return;

	int typeId = unit->s_unit.unit_type;
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
		t_obj * nearestResourceOrMoney = NULL;
		double nearestDistance = 999999;
		for (int i = 0; game.resources && game.resources[i]; i++)
		{
			if (game.resources[i]->state != STATE_ALIVE)
				continue;
			if (game.resources[i]->s_resource_money.balance <= 0)
				continue;

			double distance = ft_distance(unit, game.resources[i]);
			if (distance < nearestDistance)
			{
				nearestDistance = distance;
				nearestResourceOrMoney = game.resources[i];
			}
		}
		for  (int i = 0; game.moneys && game.moneys[i]; i++)
		{
			if (game.moneys[i]->state != STATE_ALIVE)
				continue;
			if (game.moneys[i]->s_resource_money.balance <= 0)
				continue;

			double distance = ft_distance(unit, game.moneys[i]);
			if (distance < nearestDistance)
			{
				nearestDistance = distance;
				nearestResourceOrMoney = game.moneys[i];
			}
		}
		if (nearestResourceOrMoney)
			move_unit_to(unit, nearestResourceOrMoney->pos);
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
}
