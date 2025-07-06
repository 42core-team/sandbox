#include "bot.h"

#include <time.h>
#include <stdio.h>

void ft_on_tick(unsigned long tick);
void ft_on_object_ticked(t_obj *unit, unsigned long tick);

int	main(int argc, char **argv)
{
	return ft_game_start("Gridmaster", argc, argv, ft_on_tick, false);
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
		t_obj *obj  = core_get_obj_from_pos(next);

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

int nextUnit = 0;

void ft_on_tick(unsigned long tick)
{
	(void)tick;

	if (ft_get_core_own() && ft_get_core_own()->s_core.balance >= core_get_unitConfig(nextUnit)->cost)
	{
		ft_create_unit(nextUnit);
		nextUnit++;
		if (nextUnit > 2)
			nextUnit = 0;
	}

	for (int i = 0; game.objects && game.objects[i]; i++)
		ft_on_object_ticked(game.objects[i], tick);
}

void ft_on_object_ticked(t_obj *unit, unsigned long tick)
{
	(void)tick;

	if (unit->state != STATE_ALIVE)
		return;
	if (unit->type != OBJ_UNIT)
		return;
	if (unit->s_unit.team_id != ft_get_core_own()->s_core.team_id)
		return;

	int typeId = unit->s_unit.unit_type;
	if (typeId == UNIT_WARRIOR)
	{
		t_obj * nearestOpponent = ft_get_units_opponent_nearest(ft_get_core_own()->pos);
		if (nearestOpponent)
			move_unit_to(unit, nearestOpponent->pos);
		else
			move_unit_to(unit, ft_get_core_opponent()->pos);
	}
	else if (typeId == UNIT_MINER)
	{
		t_obj * nearestResourceOrMoney = ft_get_resource_money_nearest(unit->pos);
		if (nearestResourceOrMoney)
			move_unit_to(unit, nearestResourceOrMoney->pos);
		else
			move_unit_to(unit, ft_get_core_opponent()->pos);
	}
	else if (typeId == UNIT_CARRIER)
	{
		bool isTouchingCore = core_util_distance(unit->pos, ft_get_core_own()->pos) <= 1;
		bool isTouchingUnitWithMoney = false;
		t_obj *closestUnitWithMoney = NULL;

		t_obj **units = game.objects;
		double distance = 999999;
		for (int j = 0; units && units[j]; j++)
		{
			if (units[j]->state != STATE_ALIVE || units[j]->type != OBJ_UNIT)
				continue;
			if (units[j]->s_unit.balance > 0)
			{
				if (core_util_distance(unit->pos, units[j]->pos) <= 1)
				{
					isTouchingUnitWithMoney = true;
					closestUnitWithMoney = units[j];
					break;
				}
				if (core_util_distance(unit->pos, units[j]->pos) < distance)
				{
					closestUnitWithMoney = units[j];
					distance = core_util_distance(unit->pos, units[j]->pos);
				}
				break;
			}
		}
		if (isTouchingCore && unit->s_unit.balance > 0)
			ft_transfer_money(unit, ft_get_core_own(), unit->s_unit.balance);
		else if (isTouchingUnitWithMoney)
			ft_transfer_money(closestUnitWithMoney, unit, closestUnitWithMoney->s_unit.balance);

		if (unit->s_unit.balance <= 0 && closestUnitWithMoney != NULL)
			move_unit_to(unit, closestUnitWithMoney->pos);
		else
			move_unit_to(unit, ft_get_core_own()->pos);
	}
}
