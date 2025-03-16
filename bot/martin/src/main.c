#include "con_lib.h"
#include "event_handler.h"

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

int	main(int argc, char **argv)
{
	// ft_enable_debug();
	ft_init_con("The Core Rookies", argc, argv);

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



void ft_on_tick(unsigned long tick, void *custom_data)
{
	(void)tick;
	(void)custom_data;
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
