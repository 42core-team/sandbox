#include "con_lib.h"
#include <stdio.h>

void	ft_user_loop(void);

int	main(int argc, char **argv)
{
	// ft_enable_debug();
	ft_init_con(&argc, argv);
	ft_loop(&ft_user_loop);
	ft_close_con();
	return (0);
}

// this function is called every time new data is recieved
void	ft_user_loop(void)
{
	t_core	*opponent_core;
	t_team	*my_team;
	int		ind;

	ind = 0;
	while (game.cores[ind].id != 0)
	{
		if (game.cores[ind].team_id != game.my_team_id)
			opponent_core = &game.cores[ind];
		ind++;
	}

	ind = 0;
	while (game.teams[ind].id != 0)
	{
		if (game.teams[ind].id == game.my_team_id)
			my_team = &game.teams[ind];
		ind++;
	}

	if (my_team->balance >= game.config.units[0].cost)
		ft_create_type_id(game.config.units[0].type_id);
	ind = 0;
	while (game.units[ind].id != 0)
	{
		if (game.units[ind].team_id == game.my_team_id)
		{
			ft_travel_to(&game.units[ind], opponent_core->x, opponent_core->y);
			ft_attack_core(&game.units[ind], opponent_core);
		}
		ind++;
	}
}
