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
	int	nb_philo;
	int	nb_meal;
	int	nb_meal_max;
	int	time_die;
	int	time_eat;
	int	time_sleep;
}	t_data;

typedef struct s_philo
{
	int				id;
	int				*death;
	int				*end;
	t_data			data;
	pthread_mutex_t	*fork_right;
	pthread_mutex_t	*fork_left;
	pthread_mutex_t	*print_mt;
	pthread_mutex_t	*meal_mt;
	pthread_mutex_t	*death_mt;
}	t_philo;

#endif