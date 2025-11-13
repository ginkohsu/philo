/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinxu <jinxu@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 12:16:15 by jinxu             #+#    #+#             */
/*   Updated: 2025/11/13 23:59:20 by jinxu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time(void)
{
	struct timeval	tv;
	
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int ft_atoi(const char *str)
{
    long    result;
    int     i;
    
    result = 0;
    i = 0;
    while (str[i] >= '0' && str[i] <= '9')
    {
        result = result * 10 + (str[i] - '0');
        if (result > INT_MAX)
            return (-1);
        i++;
    }
    return ((int)result);
}

void	print_status(t_philo *philo, char *status)
{
	long	timestamp;

	timestamp = get_time() - philo->data->start_time;

	pthread_mutex_lock(&philo->data->print_mutex);
	printf("%ld %d %s\n",timestamp, philo->id, status);
	pthread_mutex_unlock(&philo->data->print_mutex);
}

bool	is_simulation_ended(t_data *data)
{
	bool	ended;

	pthread_mutex_lock(&data->death_mutex);
	ended = data->simulation_end;
	pthread_mutex_unlock(&data->death_mutex);

	return (ended);
}

void	precise_sleep(t_data *data, long milliseconds)
{
	long	start;

	start = get_time();
	while (get_time() - start < milliseconds)
	{
		if (is_simulation_ended(data))
			break;
		usleep(100);
	}
}
