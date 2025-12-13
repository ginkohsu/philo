/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinxu <jinxu@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 22:33:11 by jinxu             #+#    #+#             */
/*   Updated: 2025/12/13 23:26:13 by jinxu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"

static void	*lone_philosopher(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_status(philo, "has taken a fork");
	while (!is_simulation_ended(philo->data))
		usleep(1000);
	pthread_mutex_unlock(philo->left_fork);
	return (NULL);
}

static void	philosopher_eat(t_philo *philo)
{
	if (!take_fork(philo, philo->left_fork))
		return ;
	if (!take_fork(philo, philo->right_fork))
	{
		pthread_mutex_unlock(philo -> left_fork);
		return ;
	}
	update_meal(philo);
	print_status(philo, "is eating");
	precise_sleep(philo -> data, philo -> data -> time_to_eat);
	release_forks(philo -> left_fork, philo -> right_fork);
}

static void	philosopher_sleep(t_philo *philo)
{
	print_status(philo, "is sleeping");
	precise_sleep(philo->data, philo->data->time_to_sleep);
}

static void	philosopher_think(t_philo *philo)
{
	print_status(philo, "is thinking");
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->data->num_philos == 1)
		return (lone_philosopher(philo));
	if (philo -> id % 2 == 0)
		usleep(1000);
	while (!is_simulation_ended(philo->data))
	{
		philosopher_eat(philo);
		philosopher_sleep(philo);
		philosopher_think(philo);
	}
	return (NULL);
}
