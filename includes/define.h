/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   define.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lboiteux <lboiteux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 23:34:29 by lboiteux          #+#    #+#             */
/*   Updated: 2024/03/19 23:34:29 by lboiteux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINE_H
# define DEFINE_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <string.h>
# include <sys/time.h>

# define PHILO_FORK		"\033[1;255m ╿ 🕒 %-6d \033[1;255m╿ \
🧙 %d \033[1;38;5;240mhas taken a \033[1;38;5;120mfork\033[0m\n"
# define PHILO_EATING	"\033[1;255m ╿ 🕒 %-6d \033[1;255m╿ \
🧙 %d \033[1;38;5;240mis \033[1;38;5;82meating\033[0m\n"
# define PHILO_SLEEPING	"\033[1;255m ╿ 🕒 %-6d \033[1;255m╿ \
🧙 %d \033[1;38;5;240mis \033[1;38;5;68msleeping\033[0m\n"
# define PHILO_THINKING	"\033[1;255m ╿ 🕒 %-6d \033[1;255m╿ \
🧙 %d \033[1;38;5;240mis \033[1;38;5;50mthinking\033[0m\n"
# define PHILO_DIED	"\033[41m\033[1;255m ╿ 🕒 %-6d \033[1;255m╿ \
🧙 %d \033[1;41mdied\033[0m\n"

# define ERROR_MSG "\033[1;31mWrong arguments ! \033[0;31mArguments must be formated as follows :\n\
\033[1;31m[1] \033[0m\033[1;255m number_of_philosophers \033[0;255m(a number between \033[1;255m0\033[0;255m and \033[1;255m2147483647\033[0;255m)\033[0m\n\
\033[1;31m[2] \033[0m\033[1;255m time_to_die \033[0;255m(a number between \033[1;255m0\033[0;255m and \033[1;255m2147483647\033[0;255m)\033[0m\n\
\033[1;31m[3] \033[0m\033[1;255m time_to_eat \033[0;255m(a number between \033[1;255m0\033[0;255m and \033[1;255m2147483647\033[0;255m)\033[0m\n\
\033[1;31m[4] \033[0m\033[1;255m time_to_sleep \033[0;255m(a number between \033[1;255m0\033[0;255m and \033[1;255m2147483647\033[0;255m)\033[0m\n\
\033[1;31m[5] \033[0m\033[1;255m number_of_times_each_philosopher_must_eat \033[0;255m(a number between \033[1;255m0\033[0;255m and \
\033[1;255m2147483647\033[0;255m)\033[1;31m (optional)\033[0m\n"

# define MUTEX_LOCK         pthread_mutex_lock
# define MUTEX_UNLOCK       pthread_mutex_unlock
# define T_MUTEX            pthread_mutex_t

#endif
