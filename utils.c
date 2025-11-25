/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinxu <jinxu@student.hive.fi>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 12:16:15 by jinxu             #+#    #+#             */
/*   Updated: 2025/11/25 14:56:16 by jinxu            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	get_time(void)
{
	struct timeval	tv;
	
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int ft_atoi(const char *nptr)
{
    int sign;
    int nb;

    sign = 1;
    nb = 0;
    while (*nptr == ' ' || *nptr == '\f'
        || *nptr == '\n' || *nptr == '\r' || *nptr == '\t' || *nptr == '\v')
        nptr++;
    if (*nptr == '+' || *nptr == '-')
    {
        if (*nptr == '-')
            sign = -1;
        nptr++;
    }
    while (*nptr >= '0' && *nptr <= '9')
    {
        nb = nb * 10 + (*nptr - '0');
        nptr++;
    }
    return (nb * sign);
}


void	print_status(t_philo *philo, char *status)
{
	long	timestamp;

	timestamp = get_time() - philo->data->start_time;
	pthread_mutex_lock(&philo->data->death_mutex);
	if (philo->data->simulation_end)
	{
		pthread_mutex_unlock(&philo->data->death_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->data->death_mutex);
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
