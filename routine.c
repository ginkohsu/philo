/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinxu <jinxu@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 22:33:11 by jinxu             #+#    #+#             */
/*   Updated: 2025/11/25 15:39:32 by jinxu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"

static void	*lone_philosopher(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_status(philo, "has taken a fork");
	precise_sleep(philo->data, philo->data->time_to_die);
	print_status(philo, "died");
	pthread_mutex_unlock(philo->left_fork);
	return (NULL);
}

static void	philosopher_eat(t_philo *philo)
{
	pthread_mutex_t *first_fork;
	pthread_mutex_t *second_fork;

	if(philo->id % 2 == 0)
	{
		first_fork = philo->right_fork;
		second_fork = philo->left_fork;
	}
	else
	{
		first_fork = philo->left_fork;
		second_fork = philo->right_fork;
	}
	if (is_simulation_ended(philo->data))
		return ;
	pthread_mutex_lock(first_fork);
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(second_fork);
	print_status(philo, "has taken a fork");

	print_status(philo, "is eating");
	pthread_mutex_lock(&philo->data->death_mutex);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->data->death_mutex);
	philo->meals_eaten++;
	precise_sleep(philo->data, philo->data->time_to_eat);
	
	pthread_mutex_unlock(second_fork);
	pthread_mutex_unlock(first_fork);
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

void	*philo_routine(void	*arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;

	if (philo->data->num_philos == 1)
		return (lone_philosopher(philo));
//	wait_for_start(philo->data);
	while (!is_simulation_ended(philo->data))
	{
		philosopher_eat(philo);
		philosopher_sleep(philo);
		philosopher_think(philo);
	}
	return (NULL);
}
