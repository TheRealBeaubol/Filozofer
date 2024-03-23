/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lboiteux <lboiteux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 13:18:55 by lboiteux          #+#    #+#             */
/*   Updated: 2024/03/23 21:31:44 by lboiteux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	init_forks(t_philo **philo)
{
	T_MUTEX	*forks;
	int		i;

	i = -1;
	forks = malloc((*philo)->data.nb_philo * sizeof(T_MUTEX));
	if (!forks)
		return ;
	memset(forks, 0, (*philo)->data.nb_philo * sizeof(T_MUTEX));
	while (++i < (*philo)->data.nb_philo)
	{
		if (i == (*philo)->data.nb_philo - 1)
		{
			philo[i]->fork_left = &forks[(i + 1) % (*philo)->data.nb_philo];
			philo[i]->fork_right = &forks[i];
		}
		else
		{
			philo[i]->fork_left = &forks[i];
			philo[i]->fork_right = &forks[(i + 1) % (*philo)->data.nb_philo];
		}
	}
	start_simulation(philo);
	free_and_exit_philo(philo, (*philo)->data.nb_philo, forks);
}

void	init_philo(t_philo **philo, t_data data)
{
	int		i;
	int		death;
	T_MUTEX	print_mt;
	T_MUTEX	meal_mt;
	T_MUTEX	death_mt;

	i = -1;
	death = 0;
	memset(&print_mt, 0, sizeof(T_MUTEX));
	memset(&death_mt, 0, sizeof(T_MUTEX));
	memset(&meal_mt, 0, sizeof(T_MUTEX));
	while (++i < data.nb_philo)
	{
		philo[i] = malloc(sizeof(t_philo));
		if (!philo[i])
			return ;
		philo[i]->id = i;
		philo[i]->print_mt = &print_mt;
		philo[i]->death_mt = &death_mt;
		philo[i]->meal_mt = &meal_mt;
		philo[i]->death = &death;
		philo[i]->data = data;
	}
	init_forks(philo);
}

void	free_and_exit_philo(t_philo **philo, int n, T_MUTEX *forks)
{
	int	i;

	i = -1;
	free(forks);
	while (++i < n)
		free(philo[i]);
	free(philo);
}
