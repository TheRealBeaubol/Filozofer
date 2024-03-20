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

typedef struct	s_data
{
	int	nb_philo;
	int	nb_meal;
	int	nb_meal_max;
	int	time_die;
	int	time_eat;
	int	time_sleep;
}	t_data;

typedef struct	s_fork
{
	pthread_mutex_t fork;
	int				used;
}	t_fork;

typedef struct	s_philo
{
	int				id;
	t_data			data;
	t_fork			*fork_right;
	t_fork			*fork_left;
	pthread_mutex_t *print;
}	t_philo;


#endif