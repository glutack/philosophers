/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: irmoreno <irmoreno@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/04 13:45:46 by irmoreno          #+#    #+#             */
/*   Updated: 2023/05/04 13:45:46 by irmoreno         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_philo_usleep(t_program *tools, time_t tsleep)
{
	time_t	twakeup;

	twakeup = ft_get_time() + tsleep;
	while (ft_get_time() < twakeup)
	{
		if (!ft_routine_status(tools))
			break ;
		usleep(100);
	}
}

int	ft_routine_status(t_program *tools)
{
	int	status;

	status = 1;
	pthread_mutex_lock(&tools->routine_status_mutex);
	if (tools->routine_status == 0)
		status = 0;
	pthread_mutex_unlock(&tools->routine_status_mutex);
	return (status);
}

/*	Returns the current time in milliseconds by multiplying the number of
	seconds by 1000 to convert it to milliseconds and adding the number of
	divided by 1000 to convert them to milliseconds.*/
time_t	ft_get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

/*	Checks if all parameters received are int and if there are enough
	philosophers*/
int	ft_check_arg(int argc, char **argv)
{
	int	i;
	int	j;
	int	nb;

	i = 1;
	j = 0;
	while (i < argc)
	{
		if (argv[i] && (argv[i][j] >= '0' && argv[i][j] <= '9'))
		{
			nb = ft_atoi(argv[i]);
			if (i == 1 && nb <= 0)
				return (ft_error("error: no philosophers on the table\n", 0));
			else if (i > 1 && nb == -1)
				return (ft_error("error: arguments must be int\n", 0));
			i++;
		}
		else
			return (ft_error("error: arguments must be int\n", 0));
		j = 0;
	}
	return (1);
}

int	ft_atoi(const char *str)
{
	long				res;
	unsigned int		i;

	res = 0;
	i = 0;
	while (str[i] && (str[i] >= '0' && str[i] <= '9'))
	{
		res = res * 10 + (str[i] - '0');
		i++;
	}
	if (res > 2147483647)
		return (-1);
	return ((int)res);
}
