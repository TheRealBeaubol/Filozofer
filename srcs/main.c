/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lboiteux <lboiteux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 23:20:49 by lboiteux          #+#    #+#             */
/*   Updated: 2024/03/19 23:20:49 by lboiteux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

long	get_time(void)
{
	struct timeval	t;
	static long		start_time;
	long			time;

	gettimeofday(&t, NULL);
	time = t.tv_sec * 1000 + t.tv_usec / 1000;
	if (start_time == 0)
		start_time = time;
	return (time - start_time);
}

int	meal_check(t_philo **philo)
{
	int	i;

	i = 0;
	pthread_mutex_lock((*philo)->meal_mt);
	while (i < (*philo)->data.nb_philo)
	{
		if ((philo[i])->data.nb_meal < (philo[i])->data.nb_meal_max)
		{
			pthread_mutex_unlock((*philo)->meal_mt);
			return (1);
		}
		// printf("[%d][%d] [%d]\n", (philo[i])->id, (philo[i])->data.nb_meal, (philo[i])->data.nb_meal_max);
		i++;
	}
	pthread_mutex_unlock((*philo)->meal_mt);
	pthread_mutex_lock((*philo)->death_mt);
	*(*philo)->death = 1;
	pthread_mutex_unlock((*philo)->death_mt);
	return (0);
}

void	print_message(t_philo *philo, char *msg)
{
	static int	end;

	end = 0;
	if (*philo->death == 1)
		end = 1;
	if (end == 0)
	{
		pthread_mutex_lock(philo->print_mt);
		printf(msg, get_time(), philo->id + 1);
		pthread_mutex_unlock(philo->print_mt);
	}
}

int	death_check(t_philo *philo)
{
	if (get_time() - philo->data.last_meal >= philo->data.time_die)
	{
		pthread_mutex_lock(philo->death_mt);
		print_message(philo, PHILO_DIED);
		*philo->death = 1;
		pthread_mutex_unlock(philo->death_mt);
	}
	pthread_mutex_lock(philo->death_mt);
	if (*philo->death == 0)
	{
		pthread_mutex_unlock(philo->death_mt);
		return (1);
	}
	pthread_mutex_unlock(philo->death_mt);
	return (0);
}


void	sleep_philo(int type)
{
	long	start_time;

	start_time = get_time();
	while (get_time() - start_time <= type)
		usleep(100);
}

void	fork_lock(t_philo *philo)
{
	pthread_mutex_lock(philo->fork_left);
	print_message(philo, PHILO_FORK);
	pthread_mutex_lock(philo->fork_right);
	print_message(philo, PHILO_FORK);
	print_message(philo, PHILO_EATING);
	philo->data.last_meal = get_time();
	sleep_philo(philo->data.time_eat);
	pthread_mutex_unlock(philo->fork_right);
	pthread_mutex_unlock(philo->fork_left);
}

void	*routine(void *ph)
{
	t_philo	*philo;

	philo = (t_philo *)ph;
	// if (philo->id % 2)
	// 	usleep(100);
	while (1)
	{
		fork_lock(philo);
		pthread_mutex_lock(philo->meal_mt);
		philo->data.nb_meal += 1;
		pthread_mutex_unlock(philo->meal_mt);
		print_message(philo, PHILO_SLEEPING);
		sleep_philo(philo->data.time_sleep);
		print_message(philo, PHILO_THINKING);
		pthread_mutex_lock(philo->death_mt);
		if (*philo->death == 1)
		{
			pthread_mutex_unlock(philo->death_mt);
			break ;
		}
		pthread_mutex_unlock(philo->death_mt);
	}
	return (NULL);
}

int	main(int ac, char **av)
{
	int				i;
	int				death;
	int				nb_philo;
	t_data			data;
	t_philo			**philo;
	pthread_t		*thread;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mt;
	pthread_mutex_t	meal_mt;
	pthread_mutex_t	death_mt;

	if (ac > 6)
		return (0);
	nb_philo = ft_atoi(av[1]);
	data = (t_data){1, nb_philo, 0, 0, 801, 200, 200};
	philo = malloc(nb_philo * sizeof(t_philo *));
	forks = malloc(nb_philo * sizeof(pthread_mutex_t));
	thread = malloc(nb_philo * sizeof(pthread_t));
	memset(philo, 0, nb_philo * sizeof(t_philo *));
	memset(thread, 0, nb_philo * sizeof(pthread_t));
	memset(forks, 0, nb_philo * sizeof(pthread_mutex_t));
	memset(&print_mt, 0, sizeof(pthread_mutex_t));
	memset(&meal_mt, 0, sizeof(pthread_mutex_t));
	memset(&death_mt, 0, sizeof(pthread_mutex_t));
	i = 0;
	death = 0;
	while (i < nb_philo)
	{
		philo[i] = malloc(sizeof(t_philo));
		if (!philo[i])
			return (0);
		philo[i]->id = i;
		philo[i]->death = &death;
		philo[i]->data = data;
		if (i != nb_philo - 1)
		{
			philo[i]->fork_left = &forks[i];
			philo[i]->fork_right = &forks[(i + 1) % nb_philo];
		}
		else
		{
			philo[i]->fork_right = &forks[i];
			philo[i]->fork_left = &forks[(i + 1) % nb_philo];
		}
		philo[i]->meal_mt = &meal_mt;
		philo[i]->print_mt = &print_mt;
		philo[i]->death_mt = &death_mt;
		pthread_create(&(thread[i]), NULL, routine, (void *)philo[i]);
		i++;
	}
	int	died = 0;
	while (1)
	{
		i = -1;
		while (++i != nb_philo)
		{
			if ((!death_check(*philo) || !meal_check(philo)) && !died++)
			{
				pthread_mutex_lock((*philo)->death_mt);
				if (!*(*philo)->death)
					*(*philo)->death = 1;
				pthread_mutex_unlock((*philo)->death_mt);
				break ;
			}
		}
		if (died)
			break ;
		usleep(100);
	}
	i = -1;
	while (++i != nb_philo)
		pthread_join(thread[i], NULL);
	return (1);
}
