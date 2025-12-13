/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinxu <jinxu@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/12 12:52:33 by jinxu             #+#    #+#             */
/*   Updated: 2025/12/13 23:40:25 by jinxu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"

bool	take_fork(t_philo *philo, pthread_mutex_t *fork)
{
	if (is_simulation_ended(philo->data))
		return (false);
	pthread_mutex_lock(fork);
	if (is_simulation_ended(philo->data))
	{
		pthread_mutex_unlock(fork);
		return (false);
	}
	print_status(philo, "has taken a fork");
	return (true);
}

void	update_meal(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->death_mutex);
	philo->last_meal = get_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->death_mutex);
}

void	release_forks(pthread_mutex_t *first, pthread_mutex_t *second)
{
	pthread_mutex_unlock(second);
	pthread_mutex_unlock(first);
}
