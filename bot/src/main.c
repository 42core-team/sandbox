#include "con_lib.h"

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
	// your actions go here
	ft_print_all();

	static int count = 0;
	if (count == 0)
	{
		ft_create(&game.config.unit_configs[1]);
		count++;
	}
	int ind = 0;
	t_core *core;
	while (game.cores[ind].id)
	{
		if (game.cores[ind].team_id != game.team_id)
		{
			core = &game.cores[ind];
			break;
		}
		ind++;
	}

	ind = 0;
	while (game.units[ind].id) {
		// if (game.units[ind].team_id == game.team_id && game.resources[0].id) {
		// 	ft_travel_to_id(game.units[ind].id, game.resources[0].x, game.resources[0].y);
		// 	ft_attack_id(game.units[ind].id, game.resources[0].id);
		// }
		if (game.units[ind].team_id == game.team_id) {
			ft_travel_to_id(game.units[ind].id, core->x, core->y);
			ft_attack_id(game.units[ind].id, core->id);
		}
		ind++;
	}
}
