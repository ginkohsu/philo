/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinxu <jinxu@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 00:26:57 by jinxu             #+#    #+#             */
/*   Updated: 2025/12/13 19:58:40 by jinxu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"

static bool	check_philosopher_death(t_data *data, int i)
{
	long long	current_time;
	long long	time_since_meal;

	current_time = get_time();
	pthread_mutex_lock(&data->death_mutex);
	time_since_meal = current_time - data->philos[i].last_meal;
	if (time_since_meal >= data->time_to_die)
	{
		data->simulation_end = true;
		pthread_mutex_unlock(&data->death_mutex);
		pthread_mutex_lock(&data->print_mutex);
		printf("%lld %d died\n", current_time - data->start_time,
			data->philos[i].id);
		pthread_mutex_unlock(&data->print_mutex);
		return (true);
	}
	pthread_mutex_unlock(&data->death_mutex);
	return (false);
}

static bool	check_all_philosophers_full(t_data *data)
{
	int	i;
	int	full_count;

	if (data->must_eat_count == -1)
		return (false);
	pthread_mutex_lock(&data->death_mutex);
	full_count = 0;
	i = 0;
	while (i < data->num_philos)
	{
		if (data->philos[i].meals_eaten >= data->must_eat_count)
			full_count++;
		i++;
	}
	if (full_count == data->num_philos)
	{
		data->simulation_end = true;
		pthread_mutex_unlock(&data->death_mutex);
		return (true);
	}
	pthread_mutex_unlock(&data->death_mutex);
	return (false);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (true)
	{
		i = 0;
		while (i < data->num_philos)
		{
			if (check_philosopher_death(data, i))
				return (NULL);
			i++;
		}
		if (check_all_philosophers_full(data))
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}
