/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lboiteux <lboiteux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 23:34:49 by lboiteux          #+#    #+#             */
/*   Updated: 2024/03/19 23:34:49 by lboiteux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

typedef struct s_data
{
	int		nb_meal_max;
	int		nb_philo;
	int		nb_meal;
	long	last_meal;
	int		time_die;
	int		time_eat;
	int		time_sleep;
}	t_data;

typedef struct s_philo
{
	int		id;
	int		*death;
	t_data	data;
	T_MUTEX	*fork_right;
	T_MUTEX	*fork_left;
	T_MUTEX	*print_mt;
	T_MUTEX	*meal_mt;
	T_MUTEX	*death_mt;
}	t_philo;

#endif